#ifndef __CLUSTERMAP_H_INCLUDED_
#define __CLUSTERMAP_H_INCLUDED_

std::unordered_map<size_t, size_t> FourthRoundGenerator::clusterMap = {
    {0, 0},    {1, 0},    {2, 0},    {35, 0},   {39, 0},   {40, 0},   {41, 0},
    {42, 0},   {43, 0},   {44, 0},   {45, 0},   {46, 0},   {51, 0},   {52, 0},
    {86, 0},   {89, 0},   {90, 0},   {91, 0},   {92, 0},   {93, 0},   {94, 0},
    {95, 0},   {96, 0},   {101, 0},  {136, 0},  {138, 0},  {139, 0},  {140, 0},
    {141, 0},  {142, 0},  {143, 0},  {144, 0},  {145, 0},  {185, 0},  {186, 0},
    {187, 0},  {188, 0},  {189, 0},  {190, 0},  {191, 0},  {192, 0},  {193, 0},
    {229, 0},  {233, 0},  {234, 0},  {235, 0},  {236, 0},  {238, 0},  {239, 0},
    {240, 0},  {276, 0},  {279, 0},  {280, 0},  {281, 0},  {282, 0},  {283, 0},
    {285, 0},  {286, 0},  {321, 0},  {322, 0},  {324, 0},  {325, 0},  {326, 0},
    {327, 0},  {328, 0},  {329, 0},  {331, 0},  {367, 0},  {368, 0},  {369, 0},
    {370, 0},  {371, 0},  {372, 0},  {373, 0},  {374, 0},  {407, 0},  {408, 0},
    {409, 0},  {410, 0},  {411, 0},  {412, 0},  {413, 0},  {414, 0},  {416, 0},
    {417, 0},  {418, 0},  {449, 0},  {450, 0},  {451, 0},  {452, 0},  {453, 0},
    {454, 0},  {455, 0},  {456, 0},  {457, 0},  {460, 0},  {490, 0},  {491, 0},
    {492, 0},  {493, 0},  {494, 0},  {495, 0},  {496, 0},  {497, 0},  {498, 0},
    {499, 0},  {501, 0},  {530, 0},  {531, 0},  {532, 0},  {533, 0},  {534, 0},
    {535, 0},  {536, 0},  {537, 0},  {538, 0},  {539, 0},  {540, 0},  {569, 0},
    {570, 0},  {571, 0},  {572, 0},  {573, 0},  {574, 0},  {575, 0},  {576, 0},
    {578, 0},  {579, 0},  {580, 0},  {607, 0},  {608, 0},  {609, 0},  {610, 0},
    {611, 0},  {612, 0},  {613, 0},  {614, 0},  {615, 0},  {617, 0},  {618, 0},
    {644, 0},  {645, 0},  {646, 0},  {647, 0},  {648, 0},  {649, 0},  {650, 0},
    {651, 0},  {652, 0},  {653, 0},  {655, 0},  {680, 0},  {681, 0},  {682, 0},
    {683, 0},  {684, 0},  {685, 0},  {686, 0},  {687, 0},  {688, 0},  {689, 0},
    {715, 0},  {716, 0},  {717, 0},  {718, 0},  {720, 0},  {721, 0},  {722, 0},
    {749, 0},  {750, 0},  {751, 0},  {752, 0},  {753, 0},  {755, 0},  {756, 0},
    {782, 0},  {783, 0},  {784, 0},  {785, 0},  {786, 0},  {787, 0},  {789, 0},
    {814, 0},  {815, 0},  {816, 0},  {817, 0},  {818, 0},  {819, 0},  {820, 0},
    {846, 0},  {847, 0},  {848, 0},  {850, 0},  {851, 0},  {852, 0},  {875, 0},
    {877, 0},  {878, 0},  {879, 0},  {881, 0},  {882, 0},  {904, 0},  {905, 0},
    {907, 0},  {908, 0},  {909, 0},  {911, 0},  {932, 0},  {933, 0},  {934, 0},
    {936, 0},  {937, 0},  {938, 0},  {830, 1},  {831, 1},  {832, 1},  {857, 1},
    {861, 1},  {862, 1},  {888, 1},  {891, 1},  {918, 1},  {947, 1},  {971, 1},
    {998, 1},  {1024, 1}, {1049, 1}, {1065, 1}, {1069, 1}, {1070, 1}, {1071, 1},
    {1072, 1}, {1088, 1}, {1091, 1}, {1092, 1}, {1093, 1}, {1094, 1}, {1110, 1},
    {1112, 1}, {1113, 1}, {1114, 1}, {1115, 1}, {1131, 1}, {1132, 1}, {1133, 1},
    {1134, 1}, {1135, 1}, {1143, 1}, {1147, 1}, {1148, 1}, {1149, 1}, {1150, 1},
    {1151, 1}, {1152, 1}, {1153, 1}, {1154, 1}, {1162, 1}, {1165, 1}, {1166, 1},
    {1167, 1}, {1168, 1}, {1169, 1}, {1170, 1}, {1171, 1}, {1172, 1}, {1180, 1},
    {1182, 1}, {1183, 1}, {1184, 1}, {1185, 1}, {1186, 1}, {1187, 1}, {1188, 1},
    {1189, 1}, {1197, 1}, {1198, 1}, {1199, 1}, {1200, 1}, {1201, 1}, {1202, 1},
    {1203, 1}, {1204, 1}, {1205, 1}, {1209, 1}, {1213, 1}, {1214, 1}, {1215, 1},
    {1216, 1}, {1217, 1}, {1218, 1}, {1219, 1}, {1220, 1}, {1224, 1}, {1227, 1},
    {1228, 1}, {1229, 1}, {1230, 1}, {1231, 1}, {1232, 1}, {1233, 1}, {1234, 1},
    {1238, 1}, {1240, 1}, {1241, 1}, {1242, 1}, {1243, 1}, {1244, 1}, {1245, 1},
    {1246, 1}, {1247, 1}, {1251, 1}, {1252, 1}, {1253, 1}, {1254, 1}, {1255, 1},
    {1256, 1}, {1257, 1}, {1258, 1}, {1259, 1}, {1263, 1}, {1264, 1}, {1265, 1},
    {1266, 1}, {1267, 1}, {1268, 1}, {1269, 1}, {1270, 1}, {1273, 1}, {1274, 1},
    {1275, 1}, {1276, 1}, {1277, 1}, {1278, 1}, {1279, 1}, {1280, 1}, {1282, 1},
    {1283, 1}, {1284, 1}, {1285, 1}, {1286, 1}, {1287, 1}, {1288, 1}, {1289, 1},
    {1290, 1}, {1291, 1}, {1292, 1}, {1293, 1}, {1294, 1}, {1295, 1}, {1296, 1},
    {1297, 1}, {1301, 1}, {1302, 1}, {1303, 1}, {1304, 1}, {1307, 1}, {1308, 1},
    {1309, 1}, {1310, 1}, {1312, 1}, {1313, 1}, {1314, 1}, {1315, 1}, {1316, 1},
    {1317, 1}, {1318, 1}, {1319, 1}, {3, 2},    {4, 2},    {5, 2},    {6, 2},
    {7, 2},    {8, 2},    {9, 2},    {10, 2},   {11, 2},   {12, 2},   {13, 2},
    {14, 2},   {15, 2},   {16, 2},   {17, 2},   {18, 2},   {19, 2},   {20, 2},
    {21, 2},   {22, 2},   {24, 2},   {25, 2},   {26, 2},   {53, 2},   {54, 2},
    {55, 2},   {56, 2},   {57, 2},   {58, 2},   {59, 2},   {60, 2},   {61, 2},
    {62, 2},   {63, 2},   {64, 2},   {65, 2},   {66, 2},   {67, 2},   {68, 2},
    {69, 2},   {70, 2},   {71, 2},   {72, 2},   {73, 2},   {75, 2},   {76, 2},
    {102, 2},  {103, 2},  {104, 2},  {105, 2},  {106, 2},  {107, 2},  {108, 2},
    {109, 2},  {110, 2},  {111, 2},  {112, 2},  {113, 2},  {114, 2},  {115, 2},
    {116, 2},  {117, 2},  {118, 2},  {119, 2},  {120, 2},  {121, 2},  {122, 2},
    {123, 2},  {125, 2},  {150, 2},  {151, 2},  {152, 2},  {153, 2},  {154, 2},
    {155, 2},  {156, 2},  {157, 2},  {158, 2},  {159, 2},  {160, 2},  {161, 2},
    {162, 2},  {163, 2},  {164, 2},  {165, 2},  {166, 2},  {167, 2},  {168, 2},
    {169, 2},  {170, 2},  {171, 2},  {172, 2},  {201, 2},  {202, 2},  {203, 2},
    {204, 2},  {205, 2},  {206, 2},  {207, 2},  {208, 2},  {209, 2},  {210, 2},
    {211, 2},  {212, 2},  {213, 2},  {214, 2},  {215, 2},  {216, 2},  {218, 2},
    {219, 2},  {220, 2},  {247, 2},  {248, 2},  {249, 2},  {250, 2},  {251, 2},
    {252, 2},  {253, 2},  {254, 2},  {255, 2},  {256, 2},  {257, 2},  {258, 2},
    {259, 2},  {260, 2},  {261, 2},  {262, 2},  {263, 2},  {265, 2},  {266, 2},
    {292, 2},  {293, 2},  {294, 2},  {295, 2},  {296, 2},  {297, 2},  {298, 2},
    {299, 2},  {300, 2},  {301, 2},  {302, 2},  {303, 2},  {304, 2},  {305, 2},
    {306, 2},  {307, 2},  {308, 2},  {309, 2},  {311, 2},  {336, 2},  {337, 2},
    {338, 2},  {339, 2},  {340, 2},  {341, 2},  {342, 2},  {343, 2},  {344, 2},
    {345, 2},  {346, 2},  {347, 2},  {348, 2},  {349, 2},  {350, 2},  {351, 2},
    {352, 2},  {353, 2},  {354, 2},  {383, 2},  {384, 2},  {385, 2},  {386, 2},
    {387, 2},  {388, 2},  {389, 2},  {390, 2},  {392, 2},  {393, 2},  {394, 2},
    {425, 2},  {426, 2},  {427, 2},  {428, 2},  {429, 2},  {430, 2},  {431, 2},
    {432, 2},  {433, 2},  {435, 2},  {436, 2},  {466, 2},  {467, 2},  {468, 2},
    {469, 2},  {470, 2},  {471, 2},  {472, 2},  {473, 2},  {474, 2},  {475, 2},
    {477, 2},  {506, 2},  {507, 2},  {508, 2},  {509, 2},  {510, 2},  {511, 2},
    {512, 2},  {513, 2},  {514, 2},  {515, 2},  {516, 2},  {550, 2},  {551, 2},
    {552, 2},  {554, 2},  {555, 2},  {556, 2},  {587, 2},  {589, 2},  {590, 2},
    {591, 2},  {593, 2},  {594, 2},  {624, 2},  {625, 2},  {627, 2},  {628, 2},
    {629, 2},  {631, 2},  {660, 2},  {661, 2},  {662, 2},  {664, 2},  {665, 2},
    {666, 2},  {225, 3},  {272, 3},  {318, 3},  {363, 3},  {391, 3},  {395, 3},
    {403, 3},  {434, 3},  {438, 3},  {446, 3},  {476, 3},  {480, 3},  {488, 3},
    {517, 3},  {521, 3},  {529, 3},  {549, 3},  {553, 3},  {557, 3},  {561, 3},
    {565, 3},  {588, 3},  {592, 3},  {596, 3},  {600, 3},  {604, 3},  {626, 3},
    {630, 3},  {634, 3},  {638, 3},  {642, 3},  {663, 3},  {667, 3},  {671, 3},
    {675, 3},  {679, 3},  {699, 3},  {700, 3},  {701, 3},  {702, 3},  {703, 3},
    {704, 3},  {705, 3},  {706, 3},  {707, 3},  {708, 3},  {709, 3},  {710, 3},
    {711, 3},  {712, 3},  {713, 3},  {714, 3},  {733, 3},  {734, 3},  {735, 3},
    {736, 3},  {737, 3},  {738, 3},  {739, 3},  {740, 3},  {741, 3},  {742, 3},
    {743, 3},  {744, 3},  {745, 3},  {746, 3},  {747, 3},  {748, 3},  {766, 3},
    {767, 3},  {768, 3},  {769, 3},  {770, 3},  {771, 3},  {772, 3},  {773, 3},
    {774, 3},  {775, 3},  {776, 3},  {777, 3},  {778, 3},  {779, 3},  {780, 3},
    {781, 3},  {798, 3},  {799, 3},  {800, 3},  {801, 3},  {802, 3},  {803, 3},
    {804, 3},  {805, 3},  {806, 3},  {807, 3},  {808, 3},  {809, 3},  {810, 3},
    {811, 3},  {812, 3},  {813, 3},  {833, 3},  {834, 3},  {835, 3},  {836, 3},
    {837, 3},  {838, 3},  {839, 3},  {840, 3},  {842, 3},  {843, 3},  {844, 3},
    {863, 3},  {864, 3},  {865, 3},  {866, 3},  {867, 3},  {868, 3},  {869, 3},
    {870, 3},  {871, 3},  {873, 3},  {874, 3},  {892, 3},  {893, 3},  {894, 3},
    {895, 3},  {896, 3},  {897, 3},  {898, 3},  {899, 3},  {900, 3},  {901, 3},
    {903, 3},  {920, 3},  {921, 3},  {922, 3},  {923, 3},  {924, 3},  {925, 3},
    {926, 3},  {927, 3},  {928, 3},  {929, 3},  {930, 3},  {952, 3},  {953, 3},
    {954, 3},  {956, 3},  {957, 3},  {958, 3},  {977, 3},  {979, 3},  {980, 3},
    {981, 3},  {983, 3},  {984, 3},  {1002, 3}, {1003, 3}, {1005, 3}, {1006, 3},
    {1007, 3}, {1009, 3}, {1026, 3}, {1027, 3}, {1028, 3}, {1030, 3}, {1031, 3},
    {1032, 3}, {23, 4},   {27, 4},   {28, 4},   {29, 4},   {30, 4},   {31, 4},
    {32, 4},   {33, 4},   {34, 4},   {36, 4},   {37, 4},   {38, 4},   {74, 4},
    {77, 4},   {78, 4},   {79, 4},   {80, 4},   {81, 4},   {82, 4},   {83, 4},
    {84, 4},   {85, 4},   {87, 4},   {88, 4},   {124, 4},  {126, 4},  {127, 4},
    {128, 4},  {129, 4},  {130, 4},  {131, 4},  {132, 4},  {133, 4},  {134, 4},
    {135, 4},  {137, 4},  {173, 4},  {174, 4},  {175, 4},  {176, 4},  {177, 4},
    {178, 4},  {179, 4},  {180, 4},  {181, 4},  {182, 4},  {183, 4},  {184, 4},
    {217, 4},  {221, 4},  {222, 4},  {223, 4},  {224, 4},  {226, 4},  {227, 4},
    {228, 4},  {230, 4},  {231, 4},  {232, 4},  {264, 4},  {267, 4},  {268, 4},
    {269, 4},  {270, 4},  {271, 4},  {273, 4},  {274, 4},  {275, 4},  {277, 4},
    {278, 4},  {310, 4},  {312, 4},  {313, 4},  {314, 4},  {315, 4},  {316, 4},
    {317, 4},  {319, 4},  {320, 4},  {323, 4},  {355, 4},  {356, 4},  {357, 4},
    {358, 4},  {359, 4},  {360, 4},  {361, 4},  {362, 4},  {364, 4},  {365, 4},
    {366, 4},  {396, 4},  {397, 4},  {398, 4},  {399, 4},  {400, 4},  {401, 4},
    {402, 4},  {404, 4},  {405, 4},  {406, 4},  {437, 4},  {439, 4},  {440, 4},
    {441, 4},  {442, 4},  {443, 4},  {444, 4},  {445, 4},  {447, 4},  {448, 4},
    {478, 4},  {479, 4},  {481, 4},  {482, 4},  {483, 4},  {484, 4},  {485, 4},
    {486, 4},  {487, 4},  {489, 4},  {518, 4},  {519, 4},  {520, 4},  {522, 4},
    {523, 4},  {524, 4},  {525, 4},  {526, 4},  {527, 4},  {528, 4},  {558, 4},
    {559, 4},  {560, 4},  {562, 4},  {563, 4},  {564, 4},  {566, 4},  {567, 4},
    {568, 4},  {595, 4},  {597, 4},  {598, 4},  {599, 4},  {601, 4},  {602, 4},
    {603, 4},  {605, 4},  {606, 4},  {632, 4},  {633, 4},  {635, 4},  {636, 4},
    {637, 4},  {639, 4},  {640, 4},  {641, 4},  {643, 4},  {668, 4},  {669, 4},
    {670, 4},  {672, 4},  {673, 4},  {674, 4},  {676, 4},  {677, 4},  {678, 4},
    {948, 5},  {949, 5},  {950, 5},  {975, 5},  {976, 5},  {1001, 5}, {1050, 5},
    {1051, 5}, {1052, 5}, {1073, 5}, {1074, 5}, {1095, 5}, {1136, 5}, {1137, 5},
    {1138, 5}, {1155, 5}, {1156, 5}, {1173, 5}, {1206, 5}, {1207, 5}, {1208, 5},
    {1221, 5}, {1222, 5}, {1235, 5}, {1260, 5}, {1261, 5}, {1262, 5}, {1271, 5},
    {1272, 5}, {1281, 5}, {1298, 5}, {1299, 5}, {1300, 5}, {1305, 5}, {1306, 5},
    {1311, 5}, {1320, 5}, {1321, 5}, {1322, 5}, {1323, 5}, {1324, 5}, {1325, 5},
    {47, 6},   {48, 6},   {49, 6},   {50, 6},   {97, 6},   {98, 6},   {99, 6},
    {100, 6},  {146, 6},  {147, 6},  {148, 6},  {149, 6},  {194, 6},  {195, 6},
    {196, 6},  {197, 6},  {198, 6},  {199, 6},  {200, 6},  {237, 6},  {241, 6},
    {242, 6},  {243, 6},  {244, 6},  {245, 6},  {246, 6},  {284, 6},  {287, 6},
    {288, 6},  {289, 6},  {290, 6},  {291, 6},  {330, 6},  {332, 6},  {333, 6},
    {334, 6},  {335, 6},  {375, 6},  {376, 6},  {377, 6},  {378, 6},  {379, 6},
    {380, 6},  {381, 6},  {382, 6},  {415, 6},  {419, 6},  {420, 6},  {421, 6},
    {422, 6},  {423, 6},  {424, 6},  {458, 6},  {459, 6},  {461, 6},  {462, 6},
    {463, 6},  {464, 6},  {465, 6},  {500, 6},  {502, 6},  {503, 6},  {504, 6},
    {505, 6},  {541, 6},  {542, 6},  {543, 6},  {544, 6},  {545, 6},  {546, 6},
    {547, 6},  {548, 6},  {577, 6},  {581, 6},  {582, 6},  {583, 6},  {584, 6},
    {585, 6},  {586, 6},  {616, 6},  {619, 6},  {620, 6},  {621, 6},  {622, 6},
    {623, 6},  {654, 6},  {656, 6},  {657, 6},  {658, 6},  {659, 6},  {690, 6},
    {691, 6},  {692, 6},  {693, 6},  {694, 6},  {695, 6},  {696, 6},  {697, 6},
    {698, 6},  {723, 6},  {724, 6},  {725, 6},  {726, 6},  {727, 6},  {728, 6},
    {729, 6},  {730, 6},  {731, 6},  {732, 6},  {757, 6},  {758, 6},  {759, 6},
    {760, 6},  {761, 6},  {762, 6},  {763, 6},  {764, 6},  {765, 6},  {790, 6},
    {791, 6},  {792, 6},  {793, 6},  {794, 6},  {795, 6},  {796, 6},  {797, 6},
    {822, 6},  {823, 6},  {824, 6},  {825, 6},  {826, 6},  {827, 6},  {828, 6},
    {829, 6},  {853, 6},  {854, 6},  {855, 6},  {856, 6},  {858, 6},  {859, 6},
    {860, 6},  {883, 6},  {884, 6},  {885, 6},  {886, 6},  {887, 6},  {889, 6},
    {890, 6},  {912, 6},  {913, 6},  {914, 6},  {915, 6},  {916, 6},  {917, 6},
    {919, 6},  {940, 6},  {941, 6},  {942, 6},  {943, 6},  {944, 6},  {945, 6},
    {946, 6},  {967, 6},  {968, 6},  {969, 6},  {970, 6},  {972, 6},  {973, 6},
    {974, 6},  {993, 6},  {994, 6},  {995, 6},  {996, 6},  {997, 6},  {999, 6},
    {1000, 6}, {1018, 6}, {1019, 6}, {1020, 6}, {1021, 6}, {1022, 6}, {1023, 6},
    {1025, 6}, {1042, 6}, {1043, 6}, {1044, 6}, {1045, 6}, {1046, 6}, {1047, 6},
    {1048, 6}, {1066, 6}, {1067, 6}, {1068, 6}, {1087, 6}, {1089, 6}, {1090, 6},
    {1108, 6}, {1109, 6}, {1111, 6}, {1128, 6}, {1129, 6}, {1130, 6}, {719, 7},
    {754, 7},  {788, 7},  {821, 7},  {841, 7},  {845, 7},  {849, 7},  {872, 7},
    {876, 7},  {880, 7},  {902, 7},  {906, 7},  {910, 7},  {931, 7},  {935, 7},
    {939, 7},  {951, 7},  {955, 7},  {959, 7},  {960, 7},  {961, 7},  {962, 7},
    {963, 7},  {964, 7},  {965, 7},  {966, 7},  {978, 7},  {982, 7},  {985, 7},
    {986, 7},  {987, 7},  {988, 7},  {989, 7},  {990, 7},  {991, 7},  {992, 7},
    {1004, 7}, {1008, 7}, {1010, 7}, {1011, 7}, {1012, 7}, {1013, 7}, {1014, 7},
    {1015, 7}, {1016, 7}, {1017, 7}, {1029, 7}, {1033, 7}, {1034, 7}, {1035, 7},
    {1036, 7}, {1037, 7}, {1038, 7}, {1039, 7}, {1040, 7}, {1041, 7}, {1053, 7},
    {1054, 7}, {1055, 7}, {1056, 7}, {1057, 7}, {1058, 7}, {1059, 7}, {1060, 7},
    {1061, 7}, {1062, 7}, {1063, 7}, {1064, 7}, {1075, 7}, {1076, 7}, {1077, 7},
    {1078, 7}, {1079, 7}, {1080, 7}, {1081, 7}, {1082, 7}, {1083, 7}, {1084, 7},
    {1085, 7}, {1086, 7}, {1096, 7}, {1097, 7}, {1098, 7}, {1099, 7}, {1100, 7},
    {1101, 7}, {1102, 7}, {1103, 7}, {1104, 7}, {1105, 7}, {1106, 7}, {1107, 7},
    {1116, 7}, {1117, 7}, {1118, 7}, {1119, 7}, {1120, 7}, {1121, 7}, {1122, 7},
    {1123, 7}, {1124, 7}, {1125, 7}, {1126, 7}, {1127, 7}, {1139, 7}, {1140, 7},
    {1141, 7}, {1142, 7}, {1144, 7}, {1145, 7}, {1146, 7}, {1157, 7}, {1158, 7},
    {1159, 7}, {1160, 7}, {1161, 7}, {1163, 7}, {1164, 7}, {1174, 7}, {1175, 7},
    {1176, 7}, {1177, 7}, {1178, 7}, {1179, 7}, {1181, 7}, {1190, 7}, {1191, 7},
    {1192, 7}, {1193, 7}, {1194, 7}, {1195, 7}, {1196, 7}, {1210, 7}, {1211, 7},
    {1212, 7}, {1223, 7}, {1225, 7}, {1226, 7}, {1236, 7}, {1237, 7}, {1239, 7},
    {1248, 7}, {1249, 7}, {1250, 7}};

#endif
