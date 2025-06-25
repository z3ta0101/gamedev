#ifndef WORLDWORLDTIME_HH
#define WORLDWORLDTIME_HH

#include <SFML/Graphics.hpp>
#include <string>
#include <random>
#include <array>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "feedback.h"

class WorldTime {
private:
    int64_t gameDays_;          // Days since Jan 1, 914
    float gameSeconds_;         // Seconds within current day
    double totalAdventureSeconds_; // Total game seconds since start
    const int birthYear_;       // Player birth year (897), immutable
    int birthMonth_;            // 0-based (0 = January)
    int birthDay_;              // 1-based
    int birthHour_;             // 0-23
    int birthMin_;              // 0-59
    int birthSec_;              // 0-59
    bool isPaused_;             // Pause state
    int playerAge_;             // Player age
    bool isFading_;             // Fade effect active
    float fadeTime_;            // Current fade time
    const float fadeDuration_;  // Total fade duration (seconds)
    sf::RectangleShape fadeShape_; // Black rectangle for fade effect
    Feedback& feedback_;         // Reference to Feedback object

    // Norwegian days
    static constexpr std::array<const char*, 7> norwegianDays_ = {
        "lørdag", "søndag", "mandag", "tirsdag", "onsdag", "torsdag", "fredag"
    };

    // Norwegian months
    static constexpr std::array<const char*, 12> norwegianMonths_ = {
        "januar", "februar", "mars", "april", "mai", "juni",
        "juli", "august", "september", "oktober", "november", "desember"
    };

    // Days per month (non-leap year)
    static constexpr std::array<int, 12> daysInMonth_ = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    struct DateTime {
        int year, month, day, hour, min, sec, wday;
    };

    DateTime computeDateTime(int64_t days, float seconds) const {
        // Convert seconds to time
        int totalSeconds = static_cast<int>(seconds);
        int hour = totalSeconds / 3600;
        totalSeconds %= 3600;
        int min = totalSeconds % 3600 / 60;
        int sec = totalSeconds % 60;

        // Base date: Jan 1, 914
        int year = 914;
        int64_t daysRemaining = days;
        int month = 0; // 0-based
        int day = 1;   // 1-based

        // Adjust years
        while (daysRemaining != 0) {
            bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            int daysInYear = isLeapYear ? 366 : 365;
            if (daysRemaining >= daysInYear) {
                daysRemaining -= daysInYear;
                year++;
            } else if (daysRemaining < 0) {
                year--;
                daysRemaining += daysInYear;
            } else {
                break;
            }
        }

        // Adjust months and days
        bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        while (daysRemaining != 0) {
            int daysThisMonth = (month == 1 && isLeapYear) ? 29 : daysInMonth_[month];
            if (daysRemaining >= daysThisMonth) {
                daysRemaining -= daysThisMonth;
                month++;
                if (month == 12) {
                    month = 0;
                    year++;
                    isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
                }
            } else if (daysRemaining < 0) {
                month--;
                if (month < 0) {
                    month = 11;
                    year--;
                    isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
                }
                daysRemaining += (month == 1 && isLeapYear) ? 29 : daysInMonth_[month];
            } else {
                break;
            }
        }
        day += daysRemaining;

        // Compute weekday (Zeller’s congruence for Gregorian)
        int y = year;
        int m = month + 1;
        if (m < 3) {
            m += 12;
            y--;
        }
        int k = y / 400 - y / 100 + y / 4 + y + (13 * (m + 1)) / 5 + day;
        int wday = (k % 7 + 7) % 7; // 0 = Sunday

        return {year, month, day, hour, min, sec, wday};
    }

    std::string getTimeOfDay(int hour) const {
        if (hour >= 0 && hour < 4) return "Midnatt";
        if (hour >= 4 && hour < 12) return "Morgen";
        if (hour >= 12 && hour < 18) return "Ettermiddag";
        return "Kveld";
    }

    std::string formatTime(int64_t days, float seconds, const std::string& format = "%d %B %Y %H:%M:%S") const {
        auto dt = computeDateTime(days, seconds);
        std::stringstream ss;
        ss << norwegianDays_[dt.wday] << ", ";
        if (format == "%d %B %Y %H:%M:%S") {
            ss << std::setw(2) << std::setfill('0') << dt.day << " ";
            ss << norwegianMonths_[dt.month] << " ";
            ss << dt.year << " ";
            ss << std::setw(2) << std::setfill('0') << dt.hour << ":";
            ss << std::setw(2) << std::setfill('0') << dt.min << ":";
            ss << std::setw(2) << std::setfill('0') << dt.sec;
        } else {
            ss << dt.year << "-" << std::setw(2) << std::setfill('0') << (dt.month + 1)
               << "-" << std::setw(2) << std::setfill('0') << dt.day;
        }
        return ss.str();
    }

    std::string formatAdventureTime() const {
        int64_t totalSeconds = static_cast<int64_t>(totalAdventureSeconds_);
        int64_t days = totalSeconds / 86400;
        totalSeconds %= 86400;
        int64_t hours = totalSeconds / 3600;
        totalSeconds %= 3600;
        int64_t mins = totalSeconds / 60;
        int64_t secs = totalSeconds % 60;
        std::stringstream ss;
        ss << "Eventyrtid: " << days << " dager, " << hours << " timer, "
           << mins << " minutter, " << secs << " sekunder";
        return ss.str();
    }

    int64_t getDaysUntilBirthday() const {
        auto current = computeDateTime(gameDays_, gameSeconds_);
        int currentYear = current.year;
        int targetMonth = birthMonth_;
        int targetDay = birthDay_;

        // If birthday has passed this year, target next year
        if (current.month > birthMonth_ || (current.month == birthMonth_ && current.day >= birthDay_)) {
            currentYear++;
        }

        // Calculate days to target date
        int64_t days = 0;
        int year = current.year;
        int month = current.month;
        int day = current.day;

        // Move to end of current month
        bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        int daysInCurrentMonth = (month == 1 && isLeapYear) ? 29 : daysInMonth_[month];
        days += daysInCurrentMonth - day + 1; // Include current day

        // Move to target year
        month = (month + 1) % 12;
        if (month == 0) {
            year++;
            isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        }
        while (year < currentYear) {
            days += isLeapYear ? 366 : 365;
            year++;
            isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        }

        // Move to target month
        while (month != targetMonth) {
            days += (month == 1 && isLeapYear) ? 29 : daysInMonth_[month];
            month = (month + 1) % 12;
            if (month == 0) {
                year++;
                isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            }
        }

        // Move to target day
        days += targetDay; // Include target day

        return days;
    }

    void accelerateTimeByYears(int years) {
        int64_t daysToAdd = 0;
        for (int y = 0; y < years; ++y) {
            int year = 914 + y;
            bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            daysToAdd += isLeapYear ? 366 : 365;
        }
        gameDays_ += daysToAdd;
        double secondsToAdd = static_cast<double>(daysToAdd) * 86400.0;
        totalAdventureSeconds_ += secondsToAdd;

        // Update age
        auto dt = computeDateTime(gameDays_, gameSeconds_);
        playerAge_ = dt.year - birthYear_;

        // Send to feedback
        feedback_.addMessage("Tiden akselerert med 100 år!");
    }

    void accelerateTimeByHours(int hours, int& playerHealth, int maxPlayerHealth) {
        double secondsToAdd = static_cast<double>(hours) * 3600.0;
        gameSeconds_ += secondsToAdd;
        totalAdventureSeconds_ += secondsToAdd;

        // Handle day rollover
        while (gameSeconds_ >= 86400.0f) {
            gameSeconds_ -= 86400.0f;
            gameDays_++;
        }
        if (gameSeconds_ < 0.0f) gameSeconds_ = 0.0f;

        // Heal 1 HP per 8 hours if not at max
        int hpHealed = 0;
        if (playerHealth < maxPlayerHealth) {
            hpHealed = hours / 8;
            playerHealth = std::min(playerHealth + hpHealed, maxPlayerHealth);
        }
        // Send heal message to feedback
        if (hpHealed > 0) {
            feedback_.addMessage("Helbredet " + std::to_string(hpHealed) + " HP, Ny HP: " + std::to_string(playerHealth));
        } else if (hpHealed == 0 && hours >= 8) {
            feedback_.addMessage("Helse allerede på maksimum: " + std::to_string(playerHealth));
        }

        // Update age
        auto dt = computeDateTime(gameDays_, gameSeconds_);
        playerAge_ = dt.year - birthYear_;
    }

public:
    WorldTime(Feedback& feedback)
        : gameDays_(0), gameSeconds_(0.0f), totalAdventureSeconds_(0.0),
          birthYear_(897), isPaused_(false), playerAge_(17),
          isFading_(false), fadeTime_(0.0f), fadeDuration_(1.0f),
          fadeShape_(sf::Vector2f(1400, 1400)), feedback_(feedback)
    {
        // Initialize game time: Jan 1, 914
        gameDays_ = 0;
        gameSeconds_ = 0.0f;

        // Initialize fade shape
        fadeShape_.setFillColor(sf::Color(0, 0, 0, 0)); // Transparent

        // Generate random birth date in 897 (Jan 1–Dec 31)
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> monthDist(0, 11);
        birthMonth_ = monthDist(gen);
        bool isLeapYear = false; // 897 is not a leap year
        int maxDays = (birthMonth_ == 1) ? 28 : daysInMonth_[birthMonth_];
        std::uniform_int_distribution<> dayDist(1, maxDays);
        birthDay_ = dayDist(gen);
        std::uniform_int_distribution<> hourDist(0, 23);
        std::uniform_int_distribution<> minSecDist(0, 59);
        birthHour_ = hourDist(gen);
        birthMin_ = minSecDist(gen);
        birthSec_ = minSecDist(gen);
    }

    void setPaused(bool paused) {
        isPaused_ = paused;
    }

    void update(float deltaTime, int& playerHealth) {
        if (!isPaused_) {
            // Advance time at 12x speed
            float secondsAdvanced = deltaTime * 12.0f;
            gameSeconds_ += secondsAdvanced;
            totalAdventureSeconds_ += secondsAdvanced;
            while (gameSeconds_ >= 86400.0f) {
                gameSeconds_ -= 86400.0f;
                gameDays_++;
            }
            // Prevent floating-point drift
            if (gameSeconds_ < 0.0f) gameSeconds_ = 0.0f;
        }

        // Update fade effect
        if (isFading_) {
            fadeTime_ += deltaTime;
            float t = fadeTime_ / fadeDuration_;
            if (t <= 0.5f) {
                // Fade in (0 to 255)
                int alpha = static_cast<int>(t * 2.0f * 255.0f);
                fadeShape_.setFillColor(sf::Color(0, 0, 0, alpha));
                // Debug fade
                std::cout << "Fading in, alpha: " << alpha << std::endl;
            } else if (t <= 1.0f) {
                // Fade out (255 to 0)
                int alpha = static_cast<int>((1.0f - (t - 0.5f) * 2.0f) * 255.0f);
                fadeShape_.setFillColor(sf::Color(0, 0, 0, alpha));
                // Debug fade
                std::cout << "Fading out, alpha: " << alpha << std::endl;
            } else {
                // Fade complete
                isFading_ = false;
                fadeTime_ = 0.0f;
                fadeShape_.setFillColor(sf::Color(0, 0, 0, 0));
                std::cout << "Fade complete" << std::endl;
            }
        }

        // Update age
        auto dt = computeDateTime(gameDays_, gameSeconds_);
        playerAge_ = dt.year - birthYear_;

        // Output when 'T' is pressed
        static bool wasTPressed = false;
        bool isTPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::T);
        if (isTPressed && !wasTPressed) {
            std::stringstream ss;
            ss << "WorldTime.h Versjon: FINAL-FIX-V23-2025-06-18\n";
            ss << "Spilletid: " << getCurrentTime() << " (" << getTimeOfDay(dt.hour) << ")\n";
            ss << "Fødselsdato: " << getBirthTime() << "\n";
            ss << "Spiller Alder: " << getPlayerAge() << "\n";
            ss << "Dager til bursdag: " << getDaysUntilBirthday() << "\n";
            ss << getAdventureTime() << "\n";
            ss << "Spiller Helse: " << playerHealth;
            feedback_.addMessage(ss.str());
        }
        wasTPressed = isTPressed;

        // Accelerate time by 100 years when 'Y' is pressed
        static bool wasYPressed = false;
        bool isYPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Y);
        if (isYPressed && !wasYPressed) {
            accelerateTimeByYears(100);
        }
        wasYPressed = isYPressed;
    }

    void draw(sf::RenderWindow& window) const {
        if (isFading_ || fadeShape_.getFillColor().a > 0) {
            // Save current view
            sf::View currentView = window.getView();
            // Set default view (window coordinates)
            sf::View defaultView(sf::FloatRect(0, 0, 1400, 1400));
            window.setView(defaultView);
            // Debug view
            std::cout << "Drawing fade, alpha: " << (int)fadeShape_.getFillColor().a
                      << ", view: (" << defaultView.getCenter().x << ", " << defaultView.getCenter().y
                      << "), size: (" << defaultView.getSize().x << ", " << defaultView.getSize().y << ")" << std::endl;
            // Draw fade
            window.draw(fadeShape_);
            // Restore game view
            window.setView(currentView);
        }
    }

    void triggerRest(int& playerHealth, int maxPlayerHealth = 10) {
        if (!isFading_) {
            isFading_ = true;
            fadeTime_ = 0.0f;
            int hpMissing = maxPlayerHealth - playerHealth;
            if (hpMissing > 0) {
                int hoursNeeded = hpMissing * 8; // 1 HP per 8 hours
                accelerateTimeByHours(hoursNeeded, playerHealth, maxPlayerHealth);
                feedback_.addMessage("Du har hvilt i " + std::to_string(hoursNeeded) + " timer for å bli fullt helbredet");
            } else {
                feedback_.addMessage("Helse allerede på maksimum: " + std::to_string(playerHealth));
            }
        }
    }

    std::string getCurrentTime(const std::string& format = "%d %B %Y %H:%M:%S") const {
        return formatTime(gameDays_, gameSeconds_, format);
    }

    std::string getBirthTime(const std::string& format = "%d %B %Y %H:%M:%S") const {
        // Compute days from birth date to Jan 1, 914
        int64_t daysToBirth = 0;
        // Years from 898 to 913 (full years)
        for (int y = 898; y <= 913; ++y) {
            bool isLeapYear = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
            daysToBirth += isLeapYear ? 366 : 365;
        }
        // Months from Jan to birthMonth_ in 897
        for (int m = 0; m < birthMonth_; ++m) {
            bool isLeapYear = false; // 897 is not a leap year
            daysToBirth += (m == 1 && isLeapYear) ? 29 : daysInMonth_[m];
        }
        // Days in birth month (up to birthDay_)
        daysToBirth += (birthDay_ - 1);
        // Make negative since 897 is before 914
        daysToBirth = -daysToBirth;
        float seconds = birthHour_ * 3600.0f + birthMin_ * 60.0f + birthSec_;
        return formatTime(daysToBirth, seconds, format);
    }

    int getPlayerAge() const {
        return playerAge_;
    }

    std::string getAdventureTime() const {
        return formatAdventureTime();
    }
};

#endif // WORLDWORLDTIME_HH
