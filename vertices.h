#ifndef VERTICES_H
#define VERTICES_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

// Create a player polygon (initial position)
    Polygon collisionx;
    collisionx.vertices = {
        Vectorx2f(2790, 1840), // Initial position of player
        Vectorx2f(2790, 1910),
        Vectorx2f(2880, 1910),
        Vectorx2f(2880, 1840)
    };

	Polygon attackx;
	attackx.vertices = {
		Vectorx2f(2749, 1945), //attack range polygon
		Vectorx2f(2749, 1805),
		Vectorx2f(2919, 1805),
		Vectorx2f(2919, 1945)
	};

	Polygon barrelx;
	barrelx.vertices = {
		Vectorx2f(1705, 1090), //attack range polygon
		Vectorx2f(1805, 1090),
		Vectorx2f(1805, 1010),
		Vectorx2f(1705, 1010)
	};

	//Felstrond collision barriers

	Polygon fboundry1;
	fboundry1.vertices = {
		
		Vectorx2f(9050, -1414),
		Vectorx2f(8808, -1041),
		Vectorx2f(6695, -2250),
		Vectorx2f(6880, -2988)
		
	};

	Polygon fboundry2;
	fboundry2.vertices = {
		
		Vectorx2f(8955, -935),
		Vectorx2f(9095, -320),
		Vectorx2f(9625, -240),
		Vectorx2f(9625, -1350),
		Vectorx2f(9145, -1305)
	};

	Polygon fboundry3;
	fboundry3.vertices = {
		
		Vectorx2f(9640, -465),
		Vectorx2f(9640, 40),
		Vectorx2f(9550, 40),
		Vectorx2f(9550, -465)
	};

	Polygon fboundry4;
	fboundry4.vertices = {
		
		Vectorx2f(9620, 75),
		Vectorx2f(9340, 75),
		Vectorx2f(9340, 200),
		Vectorx2f(9620, 200)
	};

	Polygon fboundry5;
	fboundry5.vertices = {
		
		Vectorx2f(9225, 190),
		Vectorx2f(9225, 610),
		Vectorx2f(9640, 1375),
		Vectorx2f(9945, 1375),
		Vectorx2f(9945, 485)
		
	};

	Polygon fboundry6;
	fboundry6.vertices = {
		
		Vectorx2f(9720, 1425),
		Vectorx2f(3235, 4535)
		
	};

	Polygon fboundry7;
	fboundry7.vertices = {
		
		Vectorx2f(3235, 4480),
		Vectorx2f(510, 3115)
		
	};

	Polygon fboundry8;
	fboundry8.vertices = {
		
		Vectorx2f(610, 3070),
		Vectorx2f(1055, 2245)
		
	};

	Polygon fboundry9;
	fboundry9.vertices = {
		
		Vectorx2f(935, 2030),
		Vectorx2f(765, 1500)
		
	};

	Polygon fboundry10;
	fboundry10.vertices = {
		
		Vectorx2f(1410, -500),
		Vectorx2f(1405, -1095)
		
	};

	Polygon fboundry11;
	fboundry11.vertices = {
		
		Vectorx2f(1405, -1065),
		Vectorx2f(940, -1550)
		
	};

	Polygon fboundry12;
	fboundry12.vertices = {
		
		Vectorx2f(940, -1510),
		Vectorx2f(5690, -2790
)
		
	};

	Polygon fboundry13;
	fboundry13.vertices = {
		
		Vectorx2f(5690, -2830),
		Vectorx2f(6655, -2515)
		
	};

	Polygon fboundry14;
	fboundry14.vertices = {
		
		Vectorx2f(1035, 2180),
		Vectorx2f(865, 1500)
		
	};

	Polygon fboundry15;
	fboundry15.vertices = {
		
		Vectorx2f(860, 1490),
		Vectorx2f(1390, -440)
		
	};

	Polygon fhouseb1;
	fhouseb1.vertices = {
		Vectorx2f(3100, 3985),
		Vectorx2f(2440, 3585)
		
	};

	Polygon fhouseb2;
	fhouseb2.vertices = {
		Vectorx2f(2440, 3585),
		Vectorx2f(4205, 1670)
	};

	Polygon fhouseb3;
	fhouseb3.vertices = {
		Vectorx2f(4205, 1670),
		Vectorx2f(4575, 2505)
		
	};

	Polygon fhouseb4;
	fhouseb4.vertices = {
		Vectorx2f(4575, 2540),
		Vectorx2f(3100, 4025)
		
	};

	//Fjelenvar collision barriers

	Polygon fjelhouse1;
	fjelhouse1.vertices = {
		Vectorx2f(-30, 3775),
		Vectorx2f(1360, 1340)
	};

	Polygon fjelhouse2;
	fjelhouse2.vertices = {
		Vectorx2f(1360, 1340),
		Vectorx2f(-25, 200)
	};

	Polygon westbarrier;
	westbarrier.vertices = {
		Vectorx2f(-30, 6525),
		Vectorx2f(-30, -4370)
	};

	Polygon northwestbarrier1;
	northwestbarrier1.vertices = {
		Vectorx2f(-30, -2350),
		Vectorx2f(425, -3015)
	};

	Polygon tavernbarrier;
	tavernbarrier.vertices = {
		Vectorx2f(425, -3015),
		Vectorx2f(1635, -3365)
	};

	Polygon northwestbarrier2;
	northwestbarrier2.vertices = {
		Vectorx2f(1635, -3365),
		Vectorx2f(2205, -3630)
	};

	Polygon northbarrier1;
	northbarrier1.vertices = {
		Vectorx2f(2205, -3630),
		Vectorx2f(2455, -3750)
	};

	Polygon northbarrier2;
	northbarrier2.vertices = {
		Vectorx2f(2455, -3750),
		Vectorx2f(2950, -3795)
	};

	Polygon northbarrier3;
	northbarrier3.vertices = {
		Vectorx2f(2950, -3795),
		Vectorx2f(3040, -3830)
	};

	Polygon northbarrier4;
	northbarrier4.vertices = {
		Vectorx2f(3040, -3830),
		Vectorx2f(3220, -3810)
	};

	Polygon northbarrier5;
	northbarrier5.vertices = {
		Vectorx2f(3220, -3810),
		Vectorx2f(4065, -3940)
	};

	Polygon northbarrier6;
	northbarrier6.vertices = {
		Vectorx2f(4065, -3940),
		Vectorx2f(5120, -3830)
	};

	Polygon northbarrier7;
	northbarrier7.vertices = {
		Vectorx2f(5120, -3830),
		Vectorx2f(5265, -3855)
	};

	Polygon northbarrier8;
	northbarrier8.vertices = {
		Vectorx2f(5265, -3855),
		Vectorx2f(5625, -3810)
	};

	Polygon northbarrier9;
	northbarrier9.vertices = {
		Vectorx2f(5625, -3810),
		Vectorx2f(5825, -3845)
	};
	
	Polygon northbarrier10;
	northbarrier10.vertices = {
		Vectorx2f(5825, -3845),
		Vectorx2f(5975, -3980)
	};

	Polygon northbarrier11;
	northbarrier11.vertices = {
		Vectorx2f(5975, -3980),
		Vectorx2f(6995, -4035)
	};

	Polygon northbarrier12;
	northbarrier12.vertices = {
		Vectorx2f(6995, -4035),
		Vectorx2f(7665, -3410)
	};

	Polygon northbarrier13;
	northbarrier13.vertices = {
		Vectorx2f(8035, -3540),
		Vectorx2f(8495, -3540)
	};

	Polygon blacksmithshouse1;
	blacksmithshouse1.vertices = {
		Vectorx2f(7665, -3410),
		Vectorx2f(8130, -2620)
	};

	Polygon blacksmithshouse2;
	blacksmithshouse2.vertices = {
		Vectorx2f(8130, -2620),
		Vectorx2f(8405, -2695)
	};

	Polygon blacksmithshouse3;
	blacksmithshouse3.vertices = {
		Vectorx2f(8405, -2695),
		Vectorx2f(7890, -3565)
	};

	Polygon blacksmithshouse4;
	blacksmithshouse4.vertices = {
		Vectorx2f(7890, -3565),
		Vectorx2f(8035, -3540)
	};

	Polygon cliffside1;
	cliffside1.vertices = {
		Vectorx2f(8395, -3640),
		Vectorx2f(8535, -2720)
	};

	Polygon cliffside2;
	cliffside2.vertices = {
		Vectorx2f(8535, -2720),
		Vectorx2f(8470, -2405)
	};

	Polygon cliffside3;
	cliffside3.vertices = {
		Vectorx2f(8470, -2405),
		Vectorx2f(7885, -1795)
	};

	Polygon cliffside4;
	cliffside4.vertices = {
		Vectorx2f(7885, -1795),
		Vectorx2f(6975, -1125)
	};

	Polygon cliffside5;
	cliffside5.vertices = {
		Vectorx2f(6975, -1125),
		Vectorx2f(6850, -1085)
	};

	Polygon cliffside6;
	cliffside6.vertices = {
		Vectorx2f(6850, -1085),
		Vectorx2f(6215, -215)
	};

	Polygon cliffside7;
	cliffside7.vertices = {
		Vectorx2f(6215, -215),
		Vectorx2f(5735, 415)
	};

	Polygon cliffside8;
	cliffside8.vertices = {
		Vectorx2f(5220, 1355),
		Vectorx2f(4870, 1705)
	};

	Polygon cliffside9;
	cliffside9.vertices = {
		Vectorx2f(4870, 1705),
		Vectorx2f(4685, 1680)
	};

	Polygon cliffside10;
	cliffside10.vertices = {
		Vectorx2f(4685, 1680),
		Vectorx2f(4465, 2055)
	};

	Polygon cliffside11;
	cliffside11.vertices = {
		Vectorx2f(4465, 2055),
		Vectorx2f(4330, 2180)
	};

	Polygon cliffside12;
	cliffside12.vertices = {
		Vectorx2f(4330, 2180),
		Vectorx2f(4110, 2310)
	};

	Polygon cliffside13;
	cliffside13.vertices = {
		Vectorx2f(4110, 2310),
		Vectorx2f(3460, 3030)
	};

	Polygon cliffside14;
	cliffside14.vertices = {
		Vectorx2f(3460, 3030),
		Vectorx2f(3190, 3215)
	};

	Polygon cliffside15;
	cliffside15.vertices = {
		Vectorx2f(3190, 3215),
		Vectorx2f(3000, 3255)
	};

	Polygon cliffside16;
	cliffside16.vertices = {
		Vectorx2f(3000, 3255),
		Vectorx2f(2695, 3580)
	};

	Polygon cliffside17;
	cliffside17.vertices = {
		Vectorx2f(2695, 3580),
		Vectorx2f(2520, 3635)
	};

	Polygon cliffside18;
	cliffside18.vertices = {
		Vectorx2f(2520, 3635),
		Vectorx2f(2495, 3715)
	};

	Polygon cliffside19;
	cliffside19.vertices = {
		Vectorx2f(2495, 3715),
		Vectorx2f(2360, 3795)
	};

	Polygon cliffside20;
	cliffside20.vertices = {
		Vectorx2f(2360, 3795),
		Vectorx2f(1890, 3810)
	};

	Polygon cliffside21;
	cliffside20.vertices = {
		Vectorx2f(1890, 3810),
		Vectorx2f(1660, 3915)
	};

	Polygon cliffside22;
	cliffside22.vertices = {
		Vectorx2f(1660, 3915),
		Vectorx2f(305, 5120)
	};

	Polygon cliffside23;
	cliffside23.vertices = {
		Vectorx2f(305, 5120),
		Vectorx2f(-15, 5255)
	};

	Polygon cliffside24;
	cliffside24.vertices = {
		Vectorx2f(2310, 3780),
		Vectorx2f(1895, 3775)
	};

	Polygon lowercliffs1;
	lowercliffs1.vertices = {
		Vectorx2f(10315, 120),
		Vectorx2f(10010, 160)
	};

	Polygon lowercliffs2;
	lowercliffs2.vertices = {
		Vectorx2f(10010, 160),
		Vectorx2f(9910, 240)
	};

	Polygon lowercliffs3;
	lowercliffs3.vertices = {
		Vectorx2f(9910, 240),
		Vectorx2f(9890, 350)
	};

	Polygon lowercliffs4;
	lowercliffs4.vertices = {
		Vectorx2f(9890, 350),
		Vectorx2f(9290, 1045)
	};

	Polygon lowercliffs5;
	lowercliffs5.vertices = {
		Vectorx2f(9290, 1045),
		Vectorx2f(8875, 1350)
	};

	Polygon lowercliffs6;
	lowercliffs6.vertices = {
		Vectorx2f(8875, 1350),
		Vectorx2f(8830, 1335)
	};

	Polygon lowercliffs7;
	lowercliffs7.vertices = {
		Vectorx2f(8830, 1335),
		Vectorx2f(8415, 1675)
	};

	Polygon lowercliffs8;
	lowercliffs8.vertices = {
		Vectorx2f(8415, 1675),
		Vectorx2f(8445, 1770)
	};

	Polygon lowercliffs9;
	lowercliffs9.vertices = {
		Vectorx2f(8445, 1770),
		Vectorx2f(8140, 2015)
	};

	Polygon lowercliffs10;
	lowercliffs10.vertices = {
		Vectorx2f(8140, 2015),
		Vectorx2f(7910, 2115)
	};

	Polygon lowercliffs11;
	lowercliffs11.vertices = {
		Vectorx2f(7065, 2920),
		Vectorx2f(6905, 3035)
	};

	Polygon lowercliffs12;
	lowercliffs12.vertices = {
		Vectorx2f(6905, 3035),
		Vectorx2f(6630, 3470)
	};

	Polygon lowercliffs13;
	lowercliffs13.vertices = {
		Vectorx2f(6630, 3470),
		Vectorx2f(6535, 3435)
	};

	Polygon lowercliffs14;
	lowercliffs14.vertices = {
		Vectorx2f(6535, 3435),
		Vectorx2f(6385, 3965)
	};

	Polygon lowercliffs15;
	lowercliffs15.vertices = {
		Vectorx2f(6385, 3965),
		Vectorx2f(6430, 4090)
	};

	Polygon lowercliffs16;
	lowercliffs16.vertices = {
		Vectorx2f(6430, 4090),
		Vectorx2f(6230, 4360)
	};

	Polygon lowercliffs17;
	lowercliffs17.vertices = {
		Vectorx2f(6230, 4360),
		Vectorx2f(5635, 5540)
	};

	Polygon lowercliffs18;
	lowercliffs18.vertices = {
		Vectorx2f(5635, 5540),
		Vectorx2f(6025, 6005)
	};

	Polygon lowercliffs19;
	lowercliffs19.vertices = {
		Vectorx2f(6025, 6005),
		Vectorx2f(8645, 5865)
	};

	Polygon lowercliffs20;
	lowercliffs20.vertices = {
		Vectorx2f(8645, 5865),
		Vectorx2f(9140, 5695)
	};

	Polygon lowercliffs21;
	lowercliffs21.vertices = {
		Vectorx2f(9140, 5695),
		Vectorx2f(10205, 5990)
	};

	Polygon lowercliffs22;
	lowercliffs22.vertices = {
		Vectorx2f(10205, 5990),
		Vectorx2f(10665, 6020)
	};

	Polygon west1;
	west1.vertices = {
		Vectorx2f(10670, 6015),
		Vectorx2f(10670, 3895)
	};

	Polygon west2;
	west2.vertices = {
		Vectorx2f(10670, 3895),
		Vectorx2f(9860, 3820)
	};

	Polygon west3;
	west3.vertices = {
		Vectorx2f(9860, 3820),
		Vectorx2f(9725, 3385)
	};

	Polygon west4;
	west4.vertices = {
		Vectorx2f(9725, 3385),
		Vectorx2f(9860, 2620)
	};

	Polygon west5;
	west5.vertices = {
		Vectorx2f(9860, 2620),
		Vectorx2f(9960, 2305)
	};

	Polygon west6;
	west6.vertices = {
		Vectorx2f(9960, 2305),
		Vectorx2f(10290, 1950)
	};

	Polygon west7;
	west7.vertices = {
		Vectorx2f(10290, 1950),
		Vectorx2f(9455, 1075)
	};

	Polygon west8;
	west8.vertices = {
		Vectorx2f(9455, 1075),
		Vectorx2f(9750, 780)
	};

	Polygon west9;
	west9.vertices = {
		Vectorx2f(9750, 780),
		Vectorx2f(10245, 1275)
	};

	Polygon west10;
	west10.vertices = {
		Vectorx2f(10245, 1275),
		Vectorx2f(10245, 120)
	};

	Polygon bridge1;
	bridge1.vertices = {
		Vectorx2f(7100, 2900),
		Vectorx2f(6705, 2255)
	};

	Polygon bridge2;
	bridge2.vertices = {
		Vectorx2f(6705, 2255),
		Vectorx2f(6345, 1910)
	};

	Polygon bridge3;
	bridge3.vertices = {
		Vectorx2f(6345, 1910),
		Vectorx2f(5605, 1395)
	};

	Polygon bridge4;
	bridge4.vertices = {
		Vectorx2f(5605, 1395),
		Vectorx2f(5220, 1335)
	};

	Polygon bridge5;
	bridge5.vertices = {
		Vectorx2f(5655, 430),
		Vectorx2f(5855, 385)
	};

	Polygon bridge6;
	bridge6.vertices = {
		Vectorx2f(5855, 385),
		Vectorx2f(5975, 395)
	};

	Polygon bridge7;
	bridge7.vertices = {
		Vectorx2f(5975, 395),
		Vectorx2f(6350, 535)
	};

	Polygon bridge8;
	bridge8.vertices = {
		Vectorx2f(6350, 535),
		Vectorx2f(6795, 850)
	};

	Polygon bridge9;
	bridge9.vertices = {
		Vectorx2f(6795, 850),
		Vectorx2f(7280, 1310)
	};

	Polygon bridge10;
	bridge10.vertices = {
		Vectorx2f(7280, 1310),
		Vectorx2f(7665, 1760)
	};

	Polygon bridge11;
	bridge11.vertices = {
		Vectorx2f(7665, 1760),
		Vectorx2f(7975, 2240)
	};

	Polygon tree1;
	tree1.vertices = {
		Vectorx2f(6520, 5655),
		Vectorx2f(6520, 5955)
	};

	Polygon tree2;
	tree2.vertices = {
		Vectorx2f(6520, 5655),
		Vectorx2f(5685, 5655)
	};

	

	

	//wilderness1 barriers

	//tree1

	Polygon w1barrier1;
	w1barrier1.vertices = {
		Vectorx2f(1700, -1500),
		Vectorx2f(2065, -1300
)
	};

	Polygon w1barrier2;
	w1barrier2.vertices = {
		Vectorx2f(2065, -1300),
		Vectorx2f(2465, -1300)
	};

	Polygon w1barrier3;
	w1barrier3.vertices = {
		Vectorx2f(2465, -1300),
		Vectorx2f(2920, -1500)
	};

	Polygon w1barrier4;
	w1barrier4.vertices = {
		Vectorx2f(2920, -1500),
		Vectorx2f(1700, -1500)
	};

	//tree2

	Polygon w1barrier5;
	w1barrier5.vertices = {
		Vectorx2f(6885, -3385),
		Vectorx2f(7715, -3320)
	};

	Polygon w1barrier6;
	w1barrier6.vertices = {
		Vectorx2f(7715, -3320),
		Vectorx2f(7870, -3420)
	};

	Polygon w1barrier7;
	w1barrier7.vertices = {
		Vectorx2f(7870, -3420),
		Vectorx2f(7735, -3635)
	};

	Polygon w1barrier8;
	w1barrier8.vertices = {
		Vectorx2f(7735, -3635),
		Vectorx2f(6885, -3385)
	};

	//tree3

	Polygon w1barrier9;
	w1barrier9.vertices = {
		Vectorx2f(9775, -1595),
		Vectorx2f(9920, -1330)
	};

	Polygon w1barrier10;
	w1barrier10.vertices = {
		Vectorx2f(9920, -1330),
		Vectorx2f(10455, -1090)
	};

	Polygon w1barrier11;
	w1barrier11.vertices = {
		Vectorx2f(10455, -1090),
		Vectorx2f(10965, -1185)
	};

	Polygon w1barrier12;
	w1barrier12.vertices = {
		Vectorx2f(10965, -1185),
		Vectorx2f(10965, -1580)
	};

	Polygon w1barrier13;
	w1barrier13.vertices = {
		Vectorx2f(10965, 1580),
		Vectorx2f(9775, -1595)
	};

	//tree4

	Polygon w1barrier14;
	w1barrier14.vertices = {
		Vectorx2f(10315, 2685),
		Vectorx2f(10685, 2830)
	};

	Polygon w1barrier15;
	w1barrier15.vertices = {
		Vectorx2f(10685, 2830),
		Vectorx2f(10970, 2755)
	};

	Polygon w1barrier16;
	w1barrier16.vertices = {
		Vectorx2f(10970, 2755),
		Vectorx2f(10315, 2685)
	};

	//cliffside

	Polygon w1barrier17;
	w1barrier17.vertices = {
		Vectorx2f(2685, 665),
		Vectorx2f(3500, 890)
	};

	Polygon w1barrier18;
	w1barrier18.vertices = {
		Vectorx2f(3500, 890),
		Vectorx2f(3755, 845)
	};

	Polygon w1barrier19;
	w1barrier19.vertices = {
		Vectorx2f(3755, 845),
		Vectorx2f(4020, 645)
	};

	Polygon w1barrier20;
	w1barrier20.vertices = {
		Vectorx2f(4020, 645),
		Vectorx2f(4185, 420)
	};

	Polygon w1barrier21;
	w1barrier21.vertices = {
		Vectorx2f(4185, 420),
		Vectorx2f(5155, -335
)
	};

	Polygon w1barrier22;
	w1barrier22.vertices = {
		Vectorx2f(5605, -335),
		Vectorx2f(5345, -705)
	};

	Polygon w1barrier23;
	w1barrier23.vertices = {
		Vectorx2f(5345, -705),
		Vectorx2f(6230, -1030)
	};

	Polygon w1barrier24;
	w1barrier24.vertices = {
		Vectorx2f(6230, -1030),
		Vectorx2f(7180, -1275)
	};

	Polygon w1barrier25;
	w1barrier25.vertices = {
		Vectorx2f(7180, -1275),
		Vectorx2f(8825, -2475)
	};

	Polygon w1barrier26;
	w1barrier26.vertices = {
		Vectorx2f(8825, -2475),
		Vectorx2f(8410, -2665)
	};

	Polygon w1barrier27;
	w1barrier27.vertices = {
		Vectorx2f(8410, -2665),
		Vectorx2f(7380, -2100)
	};

	Polygon w1barrier28;
	w1barrier28.vertices = {
		Vectorx2f(7380, -2100),
		Vectorx2f(5320, -1715)
	};

	Polygon w1barrier29;
	w1barrier29.vertices = {
		Vectorx2f(5320, -1715),
		Vectorx2f(4810, -1255)
	};

	Polygon w1barrier30;
	w1barrier30.vertices = {
		Vectorx2f(4810, -1255),
		Vectorx2f(2765, 465)
	};

	Polygon w1barrier31;
	w1barrier31.vertices = {
		Vectorx2f(2765, 465),
		Vectorx2f(2685, 665)
	};

	//map bounds

	Polygon w1barrier32;
	w1barrier32.vertices = {
		Vectorx2f(-30, -4270),
		Vectorx2f(-30, 6730)
	};

	Polygon w1barrier33;
	w1barrier33.vertices = {
		Vectorx2f(-30, 6730),
		Vectorx2f(10965, 6730)
	};

	Polygon w1barrier34;
	w1barrier34.vertices = {
		Vectorx2f(10965, 6730),
		Vectorx2f(10965, -4270)
	};

	Polygon w1barrier35;
	w1barrier35.vertices = {
		Vectorx2f(10965, -4270),
		Vectorx2f(-30, -4270)
	};

	Polygon fjeltavernwall1;
	fjeltavernwall1.vertices = {
		Vectorx2f(300, 2415),
		Vectorx2f(2565, 1630)
	};

	// Fjelenvar collision vertices

	// North barrier

	Polygon fjelnorthbarrier;
	fjelnorthbarrier.vertices = {
		Vectorx2f(0, -4200),
		Vectorx2f(5000, -4200)
	};

	Polygon fjelwestbarrier;
	fjelwestbarrier.vertices = {
		Vectorx2f(0, -4200),
		Vectorx2f(0, 800)
	};

	Polygon fjeleastbarrier;
	fjeleastbarrier.vertices = {
		Vectorx2f(5000, -4200),
		Vectorx2f(5000, 800)
	};

	Polygon fjelsouthbarrier;
	fjelsouthbarrier.vertices = {
		Vectorx2f(0, 700),
		Vectorx2f(5000, 700)
	};

	Polygon fjelhousebarrier1;
	fjelhousebarrier1.vertices = {
		Vectorx2f(1007, 900),
		Vectorx2f(0, 325)
	};

	Polygon fjelhousebarrier2;
	fjelhousebarrier2.vertices = {
		Vectorx2f(0, -1889),
		Vectorx2f(899, -2273)
	};

	Polygon fjelhousebarrier3;
	fjelhousebarrier3.vertices = {
		Vectorx2f(899, -2273),
		Vectorx2f(899, -2607)
	};

	Polygon fjelcliffbarrier1;
	fjelcliffbarrier1.vertices = {
		Vectorx2f(899, -2607),
		Vectorx2f(1912, -2873)
	};

	Polygon fjelcliffbarrier2;
	fjelcliffbarrier2.vertices = {
		Vectorx2f(1912, -2873),
		Vectorx2f(2189, -3070)
	};

	Polygon fjelcliffbarrier3;
	fjelcliffbarrier3.vertices = {
		Vectorx2f(2189, -3070),
		Vectorx2f(2762, -3196)
	};

	Polygon fjelcliffbarrier4;
	fjelcliffbarrier4.vertices = {
		Vectorx2f(2762, -3196),
		Vectorx2f(3055, -3196)
	};

	Polygon fjelcliffbarrier5;
	fjelcliffbarrier5.vertices = {
		Vectorx2f(3055, -3196),
		Vectorx2f(3236, -3162)
	};

	Polygon fjelcliffbarrier6;
	fjelcliffbarrier6.vertices = {
		Vectorx2f(3236, -3162),
		Vectorx2f(3736, -3021)
	};

	Polygon fjelcliffbarrier7;
	fjelcliffbarrier7.vertices = {
		Vectorx2f(3736, -3021),
		Vectorx2f(4026, -2844)
	};

	Polygon fjelcliffbarrier8;
	fjelcliffbarrier8.vertices = {
		Vectorx2f(4026, -2844),
		Vectorx2f(4328, -2425)
	};

	Polygon fjelhouse2barrier1;
	fjelhouse2barrier1.vertices = {
		Vectorx2f(4328, -2425),
		Vectorx2f(4328, -2105)
	};

	Polygon fjelhouse2barrier2;
	fjelhouse2barrier2.vertices = {
		Vectorx2f(4328, -2105),
		Vectorx2f(5000, -1795)
	};

	Polygon fjelcliff2barrier1;
	fjelcliff2barrier1.vertices = {
		Vectorx2f(5000, -790),
		Vectorx2f(4173, -784)
	};

	Polygon fjelcliff2barrier2;
	fjelcliff2barrier2.vertices = {
		Vectorx2f(4173, -784),
		Vectorx2f(3959, -420)
	};

	Polygon fjelcliff2barrier3;
	fjelcliff2barrier3.vertices = {
		Vectorx2f(3959, -420),
		Vectorx2f(3551, -182)
	};

	Polygon fjelcliff2barrier4;
	fjelcliff2barrier4.vertices = {
		Vectorx2f(3551, -182),
		Vectorx2f(2846, -95)
	};

	Polygon fjelcliff2barrier5;
	fjelcliff2barrier5.vertices = {
		Vectorx2f(2846, -95),
		Vectorx2f(1965, -311)
	};

	Polygon fjelcliff2barrier6;
	fjelcliff2barrier6.vertices = {
		Vectorx2f(1965, -311),
		Vectorx2f(1875, -139)
	};

	Polygon fjelcliff2barrier7;
	fjelcliff2barrier7.vertices = {
		Vectorx2f(1875, -139),
		Vectorx2f(2091, 96)
	};

	Polygon fjelcliff2barrier8;
	fjelcliff2barrier8.vertices = {
		Vectorx2f(2091, 96),
		Vectorx2f(3062, 329)
	};

	Polygon fjelcliff2barrier9;
	fjelcliff2barrier9.vertices = {
		Vectorx2f(3062, 329),
		Vectorx2f(3789, 209)
	};

	Polygon fjelcliff2barrier10;
	fjelcliff2barrier10.vertices = {
		Vectorx2f(3789, 209),
		Vectorx2f(5000, 189)
	};

	

#endif // VERTICES_H
