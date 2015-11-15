#define DICTIONARY_NAME "cgendict.txt"

#define DIC_MIN 1
#define DIC_MAX 48

#define CR_SIZE 2

// this is the index to seek inside the dictionary file, to find a section
// the dictionary is sorted according to alphabets, then within each alphabet
// it is sorted according to the word length in the dictionary
// e.g. if maximum word length in the current dictionary is 28, min is 2,
// index 0 points to length 2, and index 26 points to length 28
int hashoff[26][48] = {
{0,3,103,868,2368,6008,11888,20582,32512,46405,58693,68482,75524,80279,83479,85808,87230,88199,88639,89143,89385,89592,89736,0,89786,0,89840,89898,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{89928,89931,89987,90387,92079,95915,103483,114805,127905,140698,151462,159002,163860,166800,168720,169995,170625,170891,171091,171280,171346,171415,171487,0,0,171512,0,171540,0,171570,171634,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{171667,171670,171742,172187,173879,178030,186126,199554,217524,236345,254657,271310,284190,293325,299101,302824,305380,307261,308481,309153,309637,309798,310086,310261,310339,310447,0,0,0,0,310531,0,310564,310599,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{310635,310638,310714,311134,312526,315347,320571,329193,340393,352966,364846,374232,381470,386135,388871,391115,392087,392809,393329,393602,393778,393962,394058,0,394083,394191,394247,0,0,0,0,394276,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{394310,394313,394401,394846,395722,397339,400499,405980,413530,422231,429803,436082,441052,444067,446195,447657,448809,449569,450109,450466,450752,451005,451221,451396,451552,451660,451716,451745,0,451835,0,451867,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{451901,451904,451948,452303,453425,456106,460586,467588,477538,487163,495443,501605,505231,507766,509798,510750,511182,511410,511730,511877,512009,512078,512126,512176,512228,512255,0,512311,512341,512372,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{512404,512407,512447,512797,514009,516487,520935,527118,533668,540598,546538,551010,553530,555390,556302,556914,557562,557942,558122,558374,558506,558667,0,558715,558741,0,558768,558797,558857,0,0,0,0,558888,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{558924,558927,558979,559284,560436,562830,566846,572975,580755,588983,596507,602110,606534,609054,610542,611307,611847,612227,612507,612675,612763,612901,0,612949,0,0,613001,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{613030,613033,613109,613534,614170,615122,617226,620646,625706,632823,641031,649013,655971,661161,663897,666209,667541,668434,669054,669348,669612,669796,669940,670065,670143,0,670224,670253,670283,670314,0,670346,0,0,670380,0,0,0,0,0,0,0,0,0,0,0,0,670417},
{670467,670470,670498,670698,671238,672141,673669,675802,677542,679203,680451,681244,681860,682205,682493,682697,682769,682902,683002,683023,683045,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{683068,683071,683115,683275,683917,684918,686398,688198,689998,691637,692525,693292,693950,694430,694622,694826,694952,695047,695067,695088,0,695132,0,0,0,695204,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{695232,695235,695291,695611,696973,699535,703551,708861,715381,721431,726567,730207,732657,734142,735022,735498,735822,736126,736366,736471,736603,736672,736696,0,0,0,0,0,0,0,736771,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{736804,736807,736883,737263,738625,741523,747107,755432,765392,776392,786280,794184,800470,804895,807695,809412,810762,811522,812022,812463,812683,812844,812892,813017,813121,0,813175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{813204,813207,813275,813615,814413,815757,817773,820860,824440,828224,831548,834421,836843,838733,839965,840594,841350,841597,841897,841981,842025,842048,842096,0,0,0,0,0,0,0,842121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{842154,842157,842241,842656,843418,844475,846595,850357,855367,860999,865619,869415,872005,873715,875107,875736,876060,876269,876429,876618,876706,876729,0,876777,0,0,876803,876832,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{876862,876865,876933,877393,879067,882623,889367,900482,913512,929154,944310,956543,966343,972763,976875,979782,981474,982462,982942,983425,983579,983694,983862,983937,984093,984120,0,0,0,0,984148,0,0,0,0,0,0,0,0,0,0,0,0,0,984181,0,0,0},
{984228,984231,984259,984299,984377,984741,985237,986011,986991,987948,988872,989665,990421,991066,991466,991619,991691,991748,991808,991829,991851,991874,991898,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{991948,991951,991991,992336,993608,996009,1001545,1010185,1020585,1030969,1040497,1048401,1054645,1058995,1061571,1062948,1063794,1064326,1064626,1064878,1064966,1065081,1065129,0,1065154,1065208,1065236,0,0,0,0,0,0,0,0,0,0,0,1065265,0,0,0,0,0,0,0,0,0},
{1065306,1065309,1065385,1065945,1068237,1074803,1086203,1103888,1127758,1151925,1173153,1188740,1200836,1209491,1214947,1218007,1219771,1220816,1221576,1222017,1222259,1222489,1222609,1222709,1222761,1222815,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1222899,1222902,1222966,1223421,1224999,1228296,1234192,1243372,1253862,1263630,1272450,1279925,1285483,1289398,1291574,1292866,1293694,1294416,1294796,1295027,1295203,1295272,1295296,1295421,0,0,0,0,0,0,0,0,0,0,0,0,0,1295551,0,0,0,0,0,0,0,0,0,0},
{1295591,1295594,1295634,1295794,1296124,1296824,1298752,1301218,1305018,1309880,1315028,1319136,1322790,1325235,1327123,1328092,1328524,1328809,1328989,1329115,1329203,0,0,1329249,0,1329327,0,0,0,0,1329355,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1329388,1329391,1329443,1329648,1330146,1331273,1333169,1335437,1338717,1342127,1344875,1347033,1348573,1349698,1350658,1351202,1351508,1351698,1351858,1351984,1352028,1352074,1352098,0,1352123,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1352204,1352207,1352255,1352475,1353465,1355264,1358216,1362923,1368453,1373755,1378507,1381874,1384030,1385155,1385987,1386395,1386737,1386908,1387008,1387050,0,0,0,1387072,0,0,0,0,1387098,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1387129,1387132,1387168,1387198,1387258,1387356,1387476,1387602,1387792,1387913,1388045,1388201,1388243,1388288,1388304,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1388338,1388341,1388373,1388503,1388911,1389289,1389841,1390327,1390797,1391281,1391749,1392087,1392199,1392319,1392335,1392352,1392424,0,0,1392443,0,0,1392487,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1392512,1392515,1392531,1392626,1392830,1393103,1393519,1394086,1394516,1394989,1395277,1395485,1395597,1395717,1395765,1395782,1395800,0,0,1395819,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

// this is the number of words in the particular alphabets
// with a particular length
int hashcount[26][48] = {
{1,25,153,250,520,735,966,1193,1263,1024,753,503,317,200,137,79,51,22,24,11,9,6,2,0,2,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,14,80,282,548,946,1258,1310,1163,897,580,347,196,120,75,35,14,10,9,3,3,3,1,0,0,1,0,1,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,18,89,282,593,1012,1492,1797,1711,1526,1281,920,609,361,219,142,99,61,32,22,7,12,7,3,4,3,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,19,84,232,403,653,958,1120,1143,990,722,517,311,171,132,54,38,26,13,8,8,4,1,0,4,2,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,22,89,146,231,395,609,755,791,631,483,355,201,133,86,64,40,27,17,13,11,9,7,6,4,2,1,3,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,11,71,187,383,560,778,995,875,690,474,259,169,127,56,24,12,16,7,6,3,2,2,2,1,2,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,10,70,202,354,556,687,655,630,495,344,180,124,57,36,36,20,9,12,6,7,2,0,1,1,0,1,2,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,13,61,192,342,502,681,778,748,627,431,316,168,93,45,30,20,14,8,4,6,2,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,19,85,106,136,263,380,506,647,684,614,497,346,171,136,74,47,31,14,12,8,6,5,3,3,0,1,1,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,7,40,90,129,191,237,174,151,104,61,44,23,18,12,4,7,5,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,11,32,107,143,185,200,180,149,74,59,47,32,12,12,7,5,1,1,2,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,14,64,227,366,502,590,652,550,428,280,175,99,55,28,18,16,12,5,6,3,1,3,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,19,76,227,414,698,925,996,1000,824,608,449,295,175,101,75,40,25,21,10,7,2,5,4,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,17,68,133,192,252,343,358,344,277,221,173,126,77,37,42,13,15,4,2,1,2,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,21,83,127,151,265,418,501,512,385,292,185,114,87,37,18,11,8,9,4,1,2,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,17,92,279,508,843,1235,1303,1422,1263,941,700,428,257,171,94,52,24,23,7,5,7,3,6,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
{1,7,8,13,52,62,86,98,87,77,61,54,43,25,9,4,3,3,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,10,69,212,343,692,960,1040,944,794,608,446,290,161,81,47,28,15,12,4,5,2,1,0,2,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{1,19,112,382,938,1425,1965,2387,2197,1769,1199,864,577,341,180,98,55,38,21,11,10,5,4,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,16,91,263,471,737,1020,1049,888,735,575,397,261,136,76,46,38,19,11,8,3,1,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
{1,10,32,55,100,241,274,380,442,429,316,261,163,118,57,24,15,9,6,4,2,0,0,3,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,13,41,83,161,237,252,328,310,229,166,110,75,60,32,17,10,8,6,2,2,1,1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,12,44,165,257,369,523,553,482,396,259,154,75,52,24,19,9,5,2,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,9,6,10,14,15,14,19,11,11,12,3,3,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,8,26,68,54,69,54,47,44,39,26,8,8,1,1,4,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,4,19,34,39,52,63,43,43,24,16,8,8,3,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
