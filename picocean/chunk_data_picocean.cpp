#include "chunk_data_picocean.h"
#include "../engine/render_globals.h"
#include "../engine/chunk_globals.h"
#define LOD0_CHUNK_TRIANGLES_1_1 352
const struct triangle_16 lod0_chunk_triangles_1_1[LOD0_CHUNK_TRIANGLES_1_1] = {
{{ 0, 0, -17067},{ 0, 2048, -17067},{ 9492, 10394, -15768}, 2, 0, 0xa00d, 0xe00a, 0xd007}
, {{ 0, 0, -17067},{ 9492, 10394, -15768},{ 9492, 0, -15768}, 2, 0, 0xa00d, 0xd007, 0xa00d}
, {{ 9492, 0, -15768},{ 9492, 10394, -15768},{ 17540, 11625, -12068}, 2, 0, 0xa00d, 0xd007, 0x9001}
, {{ 9492, 0, -15768},{ 17540, 11625, -12068},{ 17540, 0, -12068}, 2, 0, 0xa00d, 0x9001, 0xa00d}
, {{ 17540, 0, -12068},{ 17540, 11625, -12068},{ 22917, 10005, -6531}, 2, 0, 0xa00d, 0x9001, 0xa004}
, {{ 17540, 0, -12068},{ 22917, 10005, -6531},{ 22917, 0, -6531}, 2, 0, 0xa00d, 0xa004, 0xa00d}
, {{ 22917, 0, -6531},{ 22917, 10005, -6531},{ 24806, 7261, 0}, 2, 0, 0xa00d, 0xa004, 0x9001}
, {{ 22917, 0, -6531},{ 24806, 7261, 0},{ 24806, 0, 0}, 2, 0, 0xa00d, 0x9001, 0xa00d}
, {{ 24806, 0, 0},{ 24806, 7261, 0},{ 22917, 5827, 6531}, 2, 0, 0xa00d, 0x9001, 0x9001}
, {{ 24806, 0, 0},{ 22917, 5827, 6531},{ 22917, 0, 6531}, 2, 0, 0xa00d, 0x9001, 0xa00d}
, {{ 22917, 0, 6531},{ 22917, 5827, 6531},{ 17540, 4656, 12068}, 2, 0, 0xa00d, 0x9001, 0x9001}
, {{ 22917, 0, 6531},{ 17540, 4656, 12068},{ 17540, 0, 12068}, 2, 0, 0xa00d, 0x9001, 0xa00d}
, {{ 17540, 0, 12068},{ 17540, 4656, 12068},{ 9492, 2048, 15768}, 2, 0, 0xa00d, 0x9001, 0x9001}
, {{ 17540, 0, 12068},{ 9492, 2048, 15768},{ 9492, 0, 15768}, 2, 0, 0xa00d, 0x9001, 0xa00d}
, {{ 9492, 0, 15768},{ 9492, 2048, 15768},{ 0, 307, 17067}, 2, 0, 0xa00d, 0x9001, 0xc00d}
, {{ 9492, 0, 15768},{ 0, 307, 17067},{ 0, 0, 17067}, 2, 0, 0xa00d, 0xc00d, 0xd00d}
, {{ -9492, 0, 15768},{ 0, 0, 17067},{ 0, 307, 17067}, 2, 0, 0xb00e, 0xd00d, 0xc00d}
, {{ 0, 307, 17067},{ -5330, 557, 16338},{ -9492, 752, 15768}, 2, 0, 0xc00d, 0xc00f, 0xe007}
, {{ 0, 307, 17067},{ -9492, 752, 15768},{ -9492, 0, 15768}, 2, 0, 0xc00d, 0xe007, 0xb00e}
, {{ -9492, 0, 15768},{ -9492, 752, 15768},{ -17540, 3939, 12068}, 2, 0, 0xb00e, 0xe007, 0xe009}
, {{ -9492, 0, 15768},{ -17540, 3939, 12068},{ -17540, 0, 12068}, 2, 0, 0xb00e, 0xe009, 0xb00e}
, {{ -17540, 0, 12068},{ -17540, 3939, 12068},{ -22917, 5728, 6531}, 2, 0, 0xb00e, 0xe009, 0xe009}
, {{ -17540, 0, 12068},{ -22917, 5728, 6531},{ -22917, 0, 6531}, 2, 0, 0xb00e, 0xe009, 0xb00e}
, {{ -22917, 0, 6531},{ -22917, 5728, 6531},{ -24806, 7288, 0}, 2, 0, 0xb00e, 0xe009, 0xe009}
, {{ -22917, 0, 6531},{ -24806, 7288, 0},{ -24806, 0, 0}, 2, 0, 0xb00e, 0xe009, 0xb00e}
, {{ -24806, 0, 0},{ -24806, 7288, 0},{ -22917, 6036, -6531}, 2, 0, 0xb00e, 0xe009, 0xe109}
, {{ -24806, 0, 0},{ -22917, 6036, -6531},{ -22917, 0, -6531}, 2, 0, 0xb00e, 0xe109, 0xb10e}
, {{ -22917, 0, -6531},{ -22917, 6036, -6531},{ -17540, 3169, -12068}, 2, 0, 0xb10e, 0xe109, 0xf50a}
, {{ -22917, 0, -6531},{ -17540, 3169, -12068},{ -17540, 0, -12068}, 2, 0, 0xb10e, 0xf50a, 0xa00d}
, {{ 17540, 11625, -12068},{ 20307, 6897, -2498},{ 22917, 7169, -1563}, 2, 0, 0x9001, 0xe10a, 0xe009}
, {{ 17540, 11625, -12068},{ 22917, 7169, -1563},{ 22917, 10005, -6531}, 2, 0, 0x9001, 0xe009, 0xa004}
, {{ -9492, 0, -15768},{ -17540, 0, -12068},{ -17540, 3169, -12068}, 2, 0, 0xa00d, 0xa00d, 0xf50a}
, {{ -13686, 2632, -13840},{ -9492, 2048, -15768},{ -9492, 0, -15768}, 2, 0, 0xe009, 0xe009, 0xa00d}
, {{ -17540, 3169, -12068},{ -13686, 2632, -13840},{ -9492, 0, -15768}, 2, 0, 0xf50a, 0xe009, 0xa00d}
, {{ -9492, 0, -15768},{ -9492, 2048, -15768},{ 0, 2048, -17067}, 2, 0, 0xa00d, 0xe009, 0xe00a}
, {{ -9492, 0, -15768},{ 0, 2048, -17067},{ 0, 0, -17067}, 2, 0, 0xa00d, 0xe00a, 0xa00d}
, {{ 17540, 11625, -12068},{ 9492, 10394, -15768},{ 13641, 6298, -1417}, 2, 0, 0x9001, 0xd007, 0xe009}
, {{ 17540, 11625, -12068},{ 13641, 6298, -1417},{ 17540, 6820, -2320}, 2, 0, 0x9001, 0xe009, 0xe009}
, {{ 4398, 9065, -4158},{ 7358, 4471, -2570},{ 9492, 5851, -1425}, 2, 0, 0xd00b, 0xe009, 0xe009}
, {{ 17540, 4656, 12068},{ 13641, 6298, -1417},{ 9492, 5851, -1425}, 2, 0, 0x9001, 0xe009, 0xe009}
, {{ 17540, 4656, 12068},{ 9492, 5851, -1425},{ 9492, 2048, 15768}, 2, 0, 0x9001, 0xe009, 0x9001}
, {{ -3672, 1546, -4363},{ 0, 2048, -17067},{ -9492, 2048, -15768}, 2, 0, 0xe009, 0xe00a, 0xe009}
, {{ -9492, 2048, -15768},{ -9492, 1620, -5370},{ -8420, 1563, -4120}, 2, 0, 0xe009, 0xc00f, 0xc00f}
, {{ -9492, 2048, -15768},{ -8420, 1563, -4120},{ -3672, 1546, -4363}, 2, 0, 0xe009, 0xc00f, 0xe009}
, {{ -9492, 2048, -15768},{ -13686, 2632, -13840},{ -11708, 2568, -8106}, 2, 0, 0xe009, 0xe009, 0xc00e}
, {{ -17540, 3169, -12068},{ -22917, 6036, -6531},{ -22917, 5894, -528}, 2, 0, 0xf50a, 0xe109, 0xd00c}
, {{ -17540, 3169, -12068},{ -22917, 5894, -528},{ -20710, 4677, -1105}, 2, 0, 0xf50a, 0xd00c, 0xc00f}
, {{ -22917, 6036, -6531},{ -24806, 7288, 0},{ -22917, 5894, -528}, 2, 0, 0xe109, 0xe009, 0xd00c}
, {{ -22917, 5728, 6531},{ -17540, 3939, 12068},{ -17540, 3489, -2059}, 2, 0, 0xe009, 0xe009, 0xc00f}
, {{ -22917, 5728, 6531},{ -17540, 3489, -2059},{ -20710, 4677, -1105}, 2, 0, 0xe009, 0xc00f, 0xc00f}
, {{ -17540, 3939, 12068},{ -9492, 752, 15768},{ -9492, 1326, 1797}, 2, 0, 0xe009, 0xe007, 0xc00e}
, {{ -17540, 3939, 12068},{ -9492, 1326, 1797},{ -14104, 3132, 182}, 2, 0, 0xe009, 0xc00e, 0xc00f}
, {{ -9492, 752, 15768},{ -5330, 557, 16338},{ -7127, 1075, 7585}, 2, 0, 0xe007, 0xc00f, 0xc00f}
, {{ 0, 505, 13185},{ 0, 307, 17067},{ 9492, 2048, 15768}, 2, 0, 0xc00f, 0xc00d, 0x9001}
, {{ 9492, 2048, 15768},{ 4963, 2048, 100},{ 2147, 1527, 567}, 2, 0, 0x9001, 0xe10a, 0xe008}
, {{ 2147, 1527, 567},{ 0, 1130, 924},{ 0, 505, 13185}, 2, 0, 0xe008, 0xe006, 0xc00f}
, {{ 9492, 2048, 15768},{ 2147, 1527, 567},{ 0, 505, 13185}, 2, 0, 0x9001, 0xe008, 0xc00f}
, {{ 17540, 4656, 12068},{ 22917, 5827, 6531},{ 20307, 6897, -2498}, 2, 0, 0x9001, 0x9001, 0xe10a}
, {{ 17540, 4656, 12068},{ 20307, 6897, -2498},{ 17540, 6820, -2320}, 2, 0, 0x9001, 0xe10a, 0xe009}
, {{ 22917, 5827, 6531},{ 24806, 7261, 0},{ 22917, 7169, -1563}, 2, 0, 0x9001, 0x9001, 0xe009}
, {{ 22917, 7169, -1563},{ 20307, 6897, -2498},{ 22917, 5827, 6531}, 2, 0, 0xe009, 0xe10a, 0x9001}
, {{ 17540, 6820, -2320},{ 13641, 6298, -1417},{ 17540, 4656, 12068}, 2, 0, 0xe009, 0xe009, 0x9001}
, {{ 9492, 5851, -1425},{ 13641, 6298, -1417},{ 9492, 10394, -15768}, 2, 0, 0xe009, 0xe009, 0xd007}
, {{ -13194, 2918, -2963},{ -17540, 3489, -2059},{ -14104, 3132, 182}, 1, 0, 0xc00f, 0xc00f, 0xc00f}
, {{ -22917, 5894, -528},{ -24806, 7288, 0},{ -22917, 5728, 6531}, 2, 0, 0xd00c, 0xe009, 0xe009}
, {{ -13194, 2918, -2963},{ -9492, 1567, -4066},{ -9492, 1620, -5370}, 1, 0, 0xc00f, 0xc00f, 0xc00f}
, {{ -13194, 2918, -2963},{ -9492, 1620, -5370},{ -11708, 2568, -8106}, 2, 0, 0xc00f, 0xc00f, 0xc00e}
, {{ 0, 1414, -4631},{ 1087, 3566, -4616},{ 0, 1888, -13944}, 2, 0, 0xf002, 0x7009, 0xe008}
, {{ 22917, 7169, -1563},{ 24806, 7261, 0},{ 22917, 10005, -6531}, 2, 0, 0xe009, 0x9001, 0xa004}
, {{ 9492, 5851, -1425},{ 7358, 4471, -2570},{ 4963, 2048, 100}, 2, 0, 0xe009, 0xe009, 0xe10a}
, {{ 9492, 5851, -1425},{ 4963, 2048, 100},{ 9492, 2048, 15768}, 2, 0, 0xe009, 0xe10a, 0x9001}
, {{ -3672, 1546, -4363},{ -8420, 1563, -4120},{ -5156, 1344, 767}, 2, 0, 0xe009, 0xc00f, 0xc00e}
, {{ -20710, 4677, -1105},{ -22917, 5894, -528},{ -22917, 5728, 6531}, 2, 0, 0xc00f, 0xd00c, 0xe009}
, {{ -20710, 4677, -1105},{ -17540, 3489, -2059},{ -17540, 3169, -12068}, 2, 0, 0xc00f, 0xc00f, 0xf50a}
, {{ -3672, 1546, -4363},{ 0, 1414, -4631},{ 0, 1888, -13944}, 2, 0, 0xe009, 0xf002, 0xe008}
, {{ -3672, 1546, -4363},{ 0, 1888, -13944},{ 0, 2048, -17067}, 2, 0, 0xe009, 0xe008, 0xe00a}
, {{ 17540, 6820, -2320},{ 20307, 6897, -2498},{ 17540, 11625, -12068}, 2, 0, 0xe009, 0xe10a, 0x9001}
, {{ -5156, 1344, 767},{ -7127, 1075, 7585},{ -5330, 557, 16338}, 2, 0, 0xc00e, 0xc00f, 0xc00f}
, {{ -5330, 557, 16338},{ 0, 307, 17067},{ 0, 505, 13185}, 2, 0, 0xc00f, 0xc00d, 0xc00f}
, {{ -5330, 557, 16338},{ 0, 505, 13185},{ -5156, 1344, 767}, 2, 0, 0xc00f, 0xc00f, 0xc00e}
, {{ -7127, 1075, 7585},{ -9492, 1326, 1797},{ -9492, 752, 15768}, 2, 0, 0xc00f, 0xc00e, 0xe007}
, {{ -14104, 3132, 182},{ -17540, 3489, -2059},{ -17540, 3939, 12068}, 2, 0, 0xc00f, 0xc00f, 0xe009}
, {{ -14104, 3132, 182},{ -9492, 1326, 1797},{ -9492, 1567, -4066}, 2, 0, 0xc00f, 0xc00e, 0xc00f}
, {{ -14104, 3132, 182},{ -9492, 1567, -4066},{ -13194, 2918, -2963}, 1, 0, 0xc00f, 0xc00f, 0xc00f}
, {{ -11708, 2568, -8106},{ -9492, 1620, -5370},{ -9492, 2048, -15768}, 2, 0, 0xc00e, 0xc00f, 0xe009}
, {{ -7127, 1075, 7585},{ -5156, 1344, 767},{ -8420, 1563, -4120}, 2, 0, 0xc00f, 0xc00e, 0xc00f}
, {{ -8420, 1563, -4120},{ -9492, 1567, -4066},{ -9492, 1326, 1797}, 2, 0, 0xc00f, 0xc00f, 0xc00e}
, {{ -8420, 1563, -4120},{ -9492, 1326, 1797},{ -7127, 1075, 7585}, 2, 0, 0xc00f, 0xc00e, 0xc00f}
, {{ -8420, 1563, -4120},{ -9492, 1620, -5370},{ -9492, 1567, -4066}, 1, 0, 0xc00f, 0xc00f, 0xc00f}
, {{ -13194, 2918, -2963},{ -11708, 2568, -8106},{ -13686, 2632, -13840}, 2, 0, 0xc00f, 0xc00e, 0xe009}
, {{ -13686, 2632, -13840},{ -17540, 3169, -12068},{ -17540, 3489, -2059}, 2, 0, 0xe009, 0xf50a, 0xc00f}
, {{ -13686, 2632, -13840},{ -17540, 3489, -2059},{ -13194, 2918, -2963}, 2, 0, 0xe009, 0xc00f, 0xc00f}
, {{ -3672, 1546, -4363},{ -5156, 1344, 767},{ 0, 505, 13185}, 2, 0, 0xe009, 0xc00e, 0xc00f}
, {{ -3672, 1546, -4363},{ 0, 505, 13185},{ 0, 1130, 924}, 2, 0, 0xe009, 0xc00f, 0xe006}
, {{ 0, 1283, -2072},{ 0, 1414, -4631},{ -3672, 1546, -4363}, 2, 0, 0xe004, 0xf002, 0xe009}
, {{ 0, 1130, 924},{ 0, 1283, -2072},{ -3672, 1546, -4363}, 2, 0, 0xe006, 0xe004, 0xe009}
, {{ -21181, 2675, -2435},{ -21181, 11549, -2435},{ -18073, 11549, -176}, 2, 0, 0xeb0e, 0xe00e, 0xec0e}
, {{ -21181, 2675, -2435},{ -18073, 11549, -176},{ -18073, 2675, -176}, 2, 0, 0xeb0e, 0xec0e, 0xb70b}
, {{ -18073, 2675, -176},{ -18073, 11549, -176},{ -19260, 11549, 3477}, 2, 0, 0xb70b, 0xec0e, 0xec0e}
, {{ -18073, 2675, -176},{ -19260, 11549, 3477},{ -19260, 2675, 3477}, 2, 0, 0xb70b, 0xec0e, 0xa80a}
, {{ -19260, 2675, 3477},{ -19260, 11549, 3477},{ -23102, 11549, 3477}, 2, 0, 0xa80a, 0xec0e, 0xb90c}
, {{ -19260, 2675, 3477},{ -23102, 11549, 3477},{ -23102, 2675, 3477}, 2, 0, 0xa80a, 0xb90c, 0xec0e}
, {{ -23102, 2675, 3477},{ -23102, 11549, 3477},{ -24289, 11549, -176}, 2, 0, 0xec0e, 0xb90c, 0xec0e}
, {{ -23102, 2675, 3477},{ -24289, 11549, -176},{ -24289, 2675, -176}, 2, 0, 0xec0e, 0xec0e, 0xa80a}
, {{ -24289, 2675, -176},{ -24289, 11549, -176},{ -21181, 11549, -2435}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ -24289, 2675, -176},{ -21181, 11549, -2435},{ -21181, 2675, -2435}, 2, 0, 0xa80a, 0xe00e, 0xeb0e}
, {{ -25136, 11668, -452},{ -21181, 11668, -3325},{ -17226, 11668, -452}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ -17226, 11668, -452},{ -18737, 11668, 4197},{ -23625, 11668, 4197}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ -17226, 11668, -452},{ -23625, 11668, 4197},{ -25136, 11668, -452}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ -23625, 11668, 4197},{ -21181, 16298, 832},{ -25136, 11668, -452}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ -17226, 11668, -452},{ -21181, 16298, 832},{ -18737, 11668, 4197}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ -25136, 11668, -452},{ -21181, 16298, 832},{ -21181, 11668, -3325}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ -21181, 11668, -3325},{ -21181, 16298, 832},{ -17226, 11668, -452}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ -18737, 11668, 4197},{ -21181, 16298, 832},{ -23625, 11668, 4197}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 5086, 686, 9574},{ 5086, 9559, 9574},{ 8815, 9559, 10501}, 2, 0, 0xeb0e, 0xe00e, 0xec0e}
, {{ 5086, 686, 9574},{ 8815, 9559, 10501},{ 8815, 686, 10501}, 2, 0, 0xeb0e, 0xec0e, 0xa80a}
, {{ 8815, 686, 10501},{ 8815, 9559, 10501},{ 9086, 9559, 14334}, 2, 0, 0xa80a, 0xec0e, 0xec0e}
, {{ 8815, 686, 10501},{ 9086, 9559, 14334},{ 9086, 686, 14334}, 2, 0, 0xa80a, 0xec0e, 0xa80b}
, {{ 9086, 686, 14334},{ 9086, 9559, 14334},{ 5525, 9559, 15776}, 2, 0, 0xa80b, 0xec0e, 0xc60c}
, {{ 9086, 686, 14334},{ 5525, 9559, 15776},{ 5525, 686, 15776}, 2, 0, 0xa80b, 0xc60c, 0xec0e}
, {{ 5525, 686, 15776},{ 5525, 9559, 15776},{ 3053, 9559, 12834}, 2, 0, 0xec0e, 0xc60c, 0xec0e}
, {{ 5525, 686, 15776},{ 3053, 9559, 12834},{ 3053, 686, 12834}, 2, 0, 0xec0e, 0xec0e, 0xa80a}
, {{ 3053, 686, 12834},{ 3053, 9559, 12834},{ 5086, 9559, 9574}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 3053, 686, 12834},{ 5086, 9559, 9574},{ 5086, 686, 9574}, 2, 0, 0xa80a, 0xe00e, 0xeb0e}
, {{ 2165, 9505, 12897},{ 4752, 9505, 8749},{ 9496, 9505, 9928}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ 9496, 9505, 9928},{ 9841, 9505, 14805},{ 5310, 9505, 16640}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ 9496, 9505, 9928},{ 5310, 9505, 16640},{ 2165, 9505, 12897}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 5310, 9505, 16640},{ 6313, 14134, 12604},{ 2165, 9505, 12897}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ 9496, 9505, 9928},{ 6313, 14134, 12604},{ 9841, 9505, 14805}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 2165, 9505, 12897},{ 6313, 14134, 12604},{ 4752, 9505, 8749}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 4752, 9505, 8749},{ 6313, 14134, 12604},{ 9496, 9505, 9928}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 9841, 9505, 14805},{ 6313, 14134, 12604},{ 5310, 9505, 16640}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 13489, 3160, 7028},{ 13489, 12034, 7028},{ 17016, 12034, 5504}, 2, 0, 0xeb0e, 0xe00e, 0xec0e}
, {{ 13489, 3160, 7028},{ 17016, 12034, 5504},{ 17016, 3160, 5504}, 2, 0, 0xeb0e, 0xec0e, 0xc60c}
, {{ 17016, 3160, 5504},{ 17016, 12034, 5504},{ 19556, 12034, 8387}, 2, 0, 0xc60c, 0xec0e, 0xec0e}
, {{ 17016, 3160, 5504},{ 19556, 12034, 8387},{ 19556, 3160, 8387}, 2, 0, 0xc60c, 0xec0e, 0xa80a}
, {{ 19556, 3160, 8387},{ 19556, 12034, 8387},{ 17599, 12034, 11693}, 2, 0, 0xa80a, 0xec0e, 0xb70b}
, {{ 19556, 3160, 8387},{ 17599, 12034, 11693},{ 17599, 3160, 11693}, 2, 0, 0xa80a, 0xb70b, 0xec0e}
, {{ 17599, 3160, 11693},{ 17599, 12034, 11693},{ 13850, 12034, 10854}, 2, 0, 0xec0e, 0xb70b, 0xec0e}
, {{ 17599, 3160, 11693},{ 13850, 12034, 10854},{ 13850, 3160, 10854}, 2, 0, 0xec0e, 0xec0e, 0xa80a}
, {{ 13850, 3160, 10854},{ 13850, 12034, 10854},{ 13489, 12034, 7028}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 13850, 3160, 10854},{ 13489, 12034, 7028},{ 13489, 3160, 7028}, 2, 0, 0xa80a, 0xe00e, 0xeb0e}
, {{ 13182, 11979, 11442},{ 12723, 11979, 6575},{ 17210, 11979, 4635}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ 17210, 11979, 4635},{ 20442, 11979, 8303},{ 17952, 11979, 12510}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ 17210, 11979, 4635},{ 17952, 11979, 12510},{ 13182, 11979, 11442}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 17952, 11979, 12510},{ 16302, 16609, 8693},{ 13182, 11979, 11442}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ 17210, 11979, 4635},{ 16302, 16609, 8693},{ 20442, 11979, 8303}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 13182, 11979, 11442},{ 16302, 16609, 8693},{ 12723, 11979, 6575}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 12723, 11979, 6575},{ 16302, 16609, 8693},{ 17210, 11979, 4635}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 20442, 11979, 8303},{ 16302, 16609, 8693},{ 17952, 11979, 12510}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 23736, 6162, -2921},{ 23736, 15036, -2921},{ 24447, 15036, 854}, 2, 0, 0xeb0e, 0xe00e, 0xec0e}
, {{ 23736, 6162, -2921},{ 24447, 15036, 854},{ 24447, 6162, 854}, 2, 0, 0xeb0e, 0xec0e, 0xe00e}
, {{ 24447, 6162, 854},{ 24447, 15036, 854},{ 21076, 15036, 2697}, 2, 0, 0xe00e, 0xec0e, 0xec0e}
, {{ 24447, 6162, 854},{ 21076, 15036, 2697},{ 21076, 6162, 2697}, 2, 0, 0xe00e, 0xec0e, 0xa80a}
, {{ 21076, 6162, 2697},{ 21076, 15036, 2697},{ 18281, 15036, 60}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 21076, 6162, 2697},{ 18281, 15036, 60},{ 18281, 6162, 60}, 2, 0, 0xa80a, 0xe00e, 0xec0e}
, {{ 18281, 6162, 60},{ 18281, 15036, 60},{ 19926, 15036, -3412}, 2, 0, 0xec0e, 0xe00e, 0xec0e}
, {{ 18281, 6162, 60},{ 19926, 15036, -3412},{ 19926, 6162, -3412}, 2, 0, 0xec0e, 0xec0e, 0xa80a}
, {{ 19926, 6162, -3412},{ 19926, 15036, -3412},{ 23736, 15036, -2921}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 19926, 6162, -3412},{ 23736, 15036, -2921},{ 23736, 6162, -2921}, 2, 0, 0xa80a, 0xe00e, 0xeb0e}
, {{ 19499, 14981, -4193},{ 24347, 14981, -3568},{ 25252, 14981, 1235}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ 25252, 14981, 1235},{ 20962, 14981, 3580},{ 17407, 14981, 225}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ 25252, 14981, 1235},{ 17407, 14981, 225},{ 19499, 14981, -4193}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 17407, 14981, 225},{ 21493, 19611, -544},{ 19499, 14981, -4193}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ 25252, 14981, 1235},{ 21493, 19611, -544},{ 20962, 14981, 3580}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 19499, 14981, -4193},{ 21493, 19611, -544},{ 24347, 14981, -3568}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 24347, 14981, -3568},{ 21493, 19611, -544},{ 25252, 14981, 1235}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 20962, 14981, 3580},{ 21493, 19611, -544},{ 17407, 14981, 225}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 10084, 7346, -14286},{ 10084, 16220, -14286},{ 13913, 16220, -13969}, 2, 0, 0xeb0e, 0xa80a, 0xec0e}
, {{ 10084, 7346, -14286},{ 13913, 16220, -13969},{ 13913, 7346, -13969}, 2, 0, 0xeb0e, 0xec0e, 0xa80a}
, {{ 13913, 7346, -13969},{ 13913, 16220, -13969},{ 14795, 16220, -10230}, 2, 0, 0xa80a, 0xec0e, 0xec0e}
, {{ 13913, 7346, -13969},{ 14795, 16220, -10230},{ 14795, 7346, -10230}, 2, 0, 0xa80a, 0xec0e, 0xa80a}
, {{ 14795, 7346, -10230},{ 14795, 16220, -10230},{ 11511, 16220, -8235}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 14795, 7346, -10230},{ 11511, 16220, -8235},{ 11511, 7346, -8235}, 2, 0, 0xa80a, 0xe00e, 0xec0e}
, {{ 11511, 7346, -8235},{ 11511, 16220, -8235},{ 8599, 16220, -10743}, 2, 0, 0xec0e, 0xe00e, 0xec0e}
, {{ 11511, 7346, -8235},{ 8599, 16220, -10743},{ 8599, 7346, -10743}, 2, 0, 0xec0e, 0xec0e, 0xc60c}
, {{ 8599, 7346, -10743},{ 8599, 16220, -10743},{ 10084, 16220, -14286}, 2, 0, 0xc60c, 0xec0e, 0xa80a}
, {{ 8599, 7346, -10743},{ 10084, 16220, -14286},{ 10084, 7346, -14286}, 2, 0, 0xc60c, 0xa80a, 0xeb0e}
, {{ 7733, 16165, -10538},{ 9622, 16165, -15047},{ 14494, 16165, -14644}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ 14494, 16165, -14644},{ 15616, 16165, -9886},{ 11438, 16165, -7348}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ 14494, 16165, -14644},{ 11438, 16165, -7348},{ 7733, 16165, -10538}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 11438, 16165, -7348},{ 11781, 20795, -11493},{ 7733, 16165, -10538}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ 14494, 16165, -14644},{ 11781, 20795, -11493},{ 15616, 16165, -9886}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 7733, 16165, -10538},{ 11781, 20795, -11493},{ 9622, 16165, -15047}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 9622, 16165, -15047},{ 11781, 20795, -11493},{ 14494, 16165, -14644}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 15616, 16165, -9886},{ 11781, 20795, -11493},{ 11438, 16165, -7348}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 8143, 2315, -6289},{ 8143, 14896, -6289},{ 12545, 14896, -3080}, 2, 0, 0xeb0e, 0xe00e, 0xec0e}
, {{ 8143, 2315, -6289},{ 12545, 14896, -3080},{ 12545, 2315, -3080}, 2, 0, 0xeb0e, 0xec0e, 0xa80a}
, {{ 12545, 2315, -3080},{ 12545, 14896, -3080},{ 10854, 14896, 2097}, 2, 0, 0xa80a, 0xec0e, 0xec0e}
, {{ 12545, 2315, -3080},{ 10854, 14896, 2097},{ 10854, 2315, 2097}, 2, 0, 0xa80a, 0xec0e, 0xb70b}
, {{ 10854, 2315, 2097},{ 10854, 14896, 2097},{ 5406, 14896, 2089}, 2, 0, 0xb70b, 0xec0e, 0xa80a}
, {{ 10854, 2315, 2097},{ 5406, 14896, 2089},{ 5406, 2315, 2089}, 2, 0, 0xb70b, 0xa80a, 0xec0e}
, {{ 5406, 2315, 2089},{ 5406, 14896, 2089},{ 3731, 14896, -3094}, 2, 0, 0xec0e, 0xa80a, 0xec0e}
, {{ 5406, 2315, 2089},{ 3731, 14896, -3094},{ 3731, 2315, -3094}, 2, 0, 0xec0e, 0xec0e, 0xa80a}
, {{ 3731, 2315, -3094},{ 3731, 14896, -3094},{ 8143, 14896, -6289}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 3731, 2315, -3094},{ 8143, 14896, -6289},{ 8143, 2315, -6289}, 2, 0, 0xa80a, 0xe00e, 0xeb0e}
, {{ 2531, 14545, -3486},{ 8145, 14545, -7551},{ 13746, 14545, -3468}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ 13746, 14545, -3468},{ 11594, 14545, 3119},{ 4663, 14545, 3108}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ 13746, 14545, -3468},{ 4663, 14545, 3108},{ 2531, 14545, -3486}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 4663, 14545, 3108},{ 8136, 21109, -1655},{ 2531, 14545, -3486}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ 13746, 14545, -3468},{ 8136, 21109, -1655},{ 11594, 14545, 3119}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 2531, 14545, -3486},{ 8136, 21109, -1655},{ 8145, 14545, -7551}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 8145, 14545, -7551},{ 8136, 21109, -1655},{ 13746, 14545, -3468}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 11594, 14545, 3119},{ 8136, 21109, -1655},{ 4663, 14545, 3108}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 7827, 2198, 10833},{ 7827, 7260, 10833},{ 13899, 7260, 8388}, 2, 0, 0xa70a, 0xeb0f, 0xea0f}
, {{ 7827, 2198, 10833},{ 13899, 7260, 8388},{ 13899, 2198, 8388}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 15062, 2198, 11278},{ 15062, 7260, 11278},{ 8991, 7260, 13722}, 2, 0, 0xa70a, 0xea0f, 0xea0f}
, {{ 15062, 2198, 11278},{ 8991, 7260, 13722},{ 8991, 2198, 13722}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 13899, 7260, 8388},{ 7827, 7260, 10833},{ 8991, 7260, 13722}, 2, 0, 0xea0f, 0xeb0f, 0xea0f}
, {{ 13899, 7260, 8388},{ 8991, 7260, 13722},{ 15062, 7260, 11278}, 1, 0, 0xea0f, 0xea0f, 0xea0f}
, {{ 16070, 5279, 6569},{ 16070, 9900, 6569},{ 19186, 9900, 813}, 2, 0, 0xa70a, 0xeb0f, 0xea0f}
, {{ 16070, 5279, 6569},{ 19186, 9900, 813},{ 19186, 5279, 813}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 21925, 5279, 2296},{ 21925, 9900, 2296},{ 18809, 9900, 8052}, 2, 0, 0xa70a, 0xea0f, 0xea0f}
, {{ 21925, 5279, 2296},{ 18809, 9900, 8052},{ 18809, 5279, 8052}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 19186, 9900, 813},{ 16070, 9900, 6569},{ 18809, 9900, 8052}, 2, 0, 0xea0f, 0xeb0f, 0xea0f}
, {{ 19186, 9900, 813},{ 18809, 9900, 8052},{ 21925, 9900, 2296}, 1, 0, 0xea0f, 0xea0f, 0xea0f}
, {{ 19909, 6099, -2448},{ 19909, 12769, -2448},{ 12601, 12769, -8435}, 2, 0, 0xa70a, 0xeb0f, 0xea0f}
, {{ 19909, 6099, -2448},{ 12601, 12769, -8435},{ 12601, 6099, -8435}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 14844, 6842, -10414},{ 14844, 13512, -10414},{ 22152, 13512, -4426}, 2, 0, 0xa70a, 0xea0f, 0xea0f}
, {{ 14844, 6842, -10414},{ 22152, 13512, -4426},{ 22152, 6842, -4426}, 2, 0, 0xa70a, 0xea0f, 0xa70a}
, {{ 12601, 12769, -8435},{ 19909, 12769, -2448},{ 22152, 13512, -4426}, 2, 0, 0xea0f, 0xeb0f, 0xea0f}
, {{ 12601, 12769, -8435},{ 22152, 13512, -4426},{ 14844, 13512, -10414}, 1, 0, 0xea0f, 0xea0f, 0xea0f}
, {{ 1750, 1902, -10508},{ 1750, 10775, -10508},{ -31, 10775, -13913}, 2, 0, 0xeb0e, 0xa80a, 0xec0e}
, {{ 1750, 1902, -10508},{ -31, 10775, -13913},{ -31, 1902, -13913}, 2, 0, 0xeb0e, 0xec0e, 0xa80a}
, {{ -31, 1902, -13913},{ -31, 10775, -13913},{ 2656, 10775, -16659}, 2, 0, 0xa80a, 0xec0e, 0xec0e}
, {{ -31, 1902, -13913},{ 2656, 10775, -16659},{ 2656, 1902, -16659}, 2, 0, 0xa80a, 0xec0e, 0xa80a}
, {{ 2656, 1902, -16659},{ 2656, 10775, -16659},{ 6098, 10775, -14952}, 2, 0, 0xa80a, 0xec0e, 0xe00e}
, {{ 2656, 1902, -16659},{ 6098, 10775, -14952},{ 6098, 1902, -14952}, 2, 0, 0xa80a, 0xe00e, 0xec0e}
, {{ 6098, 1902, -14952},{ 6098, 10775, -14952},{ 5538, 10775, -11150}, 2, 0, 0xec0e, 0xe00e, 0xec0e}
, {{ 6098, 1902, -14952},{ 5538, 10775, -11150},{ 5538, 1902, -11150}, 2, 0, 0xec0e, 0xec0e, 0xc60c}
, {{ 5538, 1902, -11150},{ 5538, 10775, -11150},{ 1750, 10775, -10508}, 2, 0, 0xc60c, 0xec0e, 0xa80a}
, {{ 5538, 1902, -11150},{ 1750, 10775, -10508},{ 1750, 1902, -10508}, 2, 0, 0xc60c, 0xa80a, 0xeb0e}
, {{ 6174, 10721, -10528},{ 1354, 10721, -9711},{ -911, 10721, -14043}, 2, 0, 0x5209, 0x5009, 0x520a}
, {{ -911, 10721, -14043},{ 2507, 10721, -17536},{ 6887, 10721, -15364}, 2, 0, 0x520a, 0x5109, 0x5009}
, {{ -911, 10721, -14043},{ 6887, 10721, -15364},{ 6174, 10721, -10528}, 2, 0, 0x520a, 0x5009, 0x5209}
, {{ 6887, 10721, -15364},{ 3202, 15350, -13436},{ 6174, 10721, -10528}, 2, 0, 0x5009, 0x9007, 0x5209}
, {{ -911, 10721, -14043},{ 3202, 15350, -13436},{ 2507, 10721, -17536}, 2, 0, 0x520a, 0x9007, 0x5109}
, {{ 6174, 10721, -10528},{ 3202, 15350, -13436},{ 1354, 10721, -9711}, 2, 0, 0x5209, 0x9007, 0x5009}
, {{ 1354, 10721, -9711},{ 3202, 15350, -13436},{ -911, 10721, -14043}, 2, 0, 0x5009, 0x9007, 0x520a}
, {{ 2507, 10721, -17536},{ 3202, 15350, -13436},{ 6887, 10721, -15364}, 2, 0, 0x5109, 0x9007, 0x5009}
, {{ 1264, 532, 13573},{ 1264, 8222, 13573},{ 1264, 8222, 10603}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 1264, 532, 13573},{ 1264, 8222, 10603},{ 1264, 532, 10603}, 2, 0, 0xa70a, 0xea0e, 0xa80a}
, {{ 1264, 532, 10603},{ 1264, 8222, 10603},{ 4233, 8222, 10603}, 2, 0, 0xa80a, 0xea0e, 0xea0e}
, {{ 1264, 532, 10603},{ 4233, 8222, 10603},{ 4233, 532, 10603}, 2, 0, 0xa80a, 0xea0e, 0xa70a}
, {{ 4233, 532, 10603},{ 4233, 8222, 10603},{ 4233, 8222, 13573}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 4233, 532, 10603},{ 4233, 8222, 13573},{ 4233, 532, 13573}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 4233, 532, 13573},{ 4233, 8222, 13573},{ 1264, 8222, 13573}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 4233, 532, 13573},{ 1264, 8222, 13573},{ 1264, 532, 13573}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 4233, 8222, 10603},{ 1264, 8222, 10603},{ 1264, 8222, 13573}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 4233, 8222, 10603},{ 1264, 8222, 13573},{ 4233, 8222, 13573}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 1264, 532, 4117},{ 1264, 8222, 4117},{ 1264, 8222, 1148}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 1264, 532, 4117},{ 1264, 8222, 1148},{ 1264, 532, 1148}, 2, 0, 0xa70a, 0xea0e, 0xa80a}
, {{ 1264, 532, 1148},{ 1264, 8222, 1148},{ 4233, 8222, 1148}, 2, 0, 0xa80a, 0xea0e, 0xea0e}
, {{ 1264, 532, 1148},{ 4233, 8222, 1148},{ 4233, 532, 1148}, 2, 0, 0xa80a, 0xea0e, 0xa70a}
, {{ 4233, 532, 1148},{ 4233, 8222, 1148},{ 4233, 8222, 4117}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 4233, 532, 1148},{ 4233, 8222, 4117},{ 4233, 532, 4117}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 4233, 532, 4117},{ 4233, 8222, 4117},{ 1264, 8222, 4117}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 4233, 532, 4117},{ 1264, 8222, 4117},{ 1264, 532, 4117}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 4233, 8222, 1148},{ 1264, 8222, 1148},{ 1264, 8222, 4117}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 4233, 8222, 1148},{ 1264, 8222, 4117},{ 4233, 8222, 4117}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 1905, 5183, 11388},{ 1905, 7956, 11388},{ 1905, 7956, 2693}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 1905, 5183, 11388},{ 1905, 7956, 2693},{ 1905, 5183, 2693}, 2, 0, 0xa70a, 0xea0e, 0xa80a}
, {{ 1905, 5183, 2693},{ 1905, 7956, 2693},{ 3833, 7956, 2693}, 2, 0, 0xa80a, 0xea0e, 0xea0e}
, {{ 1905, 5183, 2693},{ 3833, 7956, 2693},{ 3833, 5183, 2693}, 2, 0, 0xa80a, 0xea0e, 0xa70a}
, {{ 3833, 5183, 2693},{ 3833, 7956, 2693},{ 3833, 7956, 11388}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 3833, 5183, 2693},{ 3833, 7956, 11388},{ 3833, 5183, 11388}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 3833, 5183, 11388},{ 3833, 7956, 11388},{ 1905, 7956, 11388}, 2, 0, 0xa70a, 0xea0e, 0xea0e}
, {{ 3833, 5183, 11388},{ 1905, 7956, 11388},{ 1905, 5183, 11388}, 2, 0, 0xa70a, 0xea0e, 0xa70a}
, {{ 3833, 7956, 2693},{ 1905, 7956, 2693},{ 1905, 7956, 11388}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 3833, 7956, 2693},{ 1905, 7956, 11388},{ 3833, 7956, 11388}, 1, 0, 0xea0e, 0xea0e, 0xea0e}
, {{ 0, 1283, -2072},{ 1008, 3164, -2464},{ 1087, 3566, -4616}, 2, 0, 0xe004, 0x7009, 0x7009}
, {{ 0, 1283, -2072},{ 1087, 3566, -4616},{ 0, 1414, -4631}, 2, 0, 0xe004, 0x7009, 0xf002}
, {{ 3896, 8555, -3590},{ 4398, 9065, -4158},{ 3609, 8555, -4581}, 2, 0, 0xa00d, 0xd00b, 0xa00d}
, {{ 9492, 5851, -1425},{ 9492, 10394, -15768},{ 4398, 9065, -4158}, 2, 0, 0xe009, 0xd007, 0xd00b}
, {{ 4963, 2048, 100},{ 7358, 4471, -2570},{ 4398, 9065, -4158}, 2, 0, 0xe10a, 0xe009, 0xd00b}
, {{ 4963, 2048, 100},{ 4398, 9065, -4158},{ 3896, 8555, -3590}, 2, 0, 0xe10a, 0xd00b, 0xa00d}
, {{ 0, 1130, 924},{ 2147, 1527, 567},{ 1008, 3164, -2464}, 2, 0, 0xe006, 0xe008, 0x7009}
, {{ 0, 1130, 924},{ 1008, 3164, -2464},{ 0, 1283, -2072}, 2, 0, 0xe006, 0x7009, 0xe004}
, {{ 9492, 10394, -15768},{ 0, 2048, -17067},{ 3609, 8555, -4581}, 2, 0, 0xd007, 0xe00a, 0xa00d}
, {{ 4398, 9065, -4158},{ 9492, 10394, -15768},{ 3609, 8555, -4581}, 2, 0, 0xd00b, 0xd007, 0xa00d}
, {{ 1087, 3566, -4616},{ 1008, 3164, -2464},{ 3896, 8555, -3590}, 2, 0, 0x7009, 0x7009, 0xa00d}
, {{ 1087, 3566, -4616},{ 3896, 8555, -3590},{ 3609, 8555, -4581}, 2, 0, 0x7009, 0xa00d, 0xa00d}
, {{ 1008, 3164, -2464},{ 2147, 1527, 567},{ 4963, 2048, 100}, 2, 0, 0x7009, 0xe008, 0xe10a}
, {{ 1008, 3164, -2464},{ 4963, 2048, 100},{ 3896, 8555, -3590}, 2, 0, 0x7009, 0xe10a, 0xa00d}
, {{ 0, 1888, -13944},{ 1087, 3566, -4616},{ 3609, 8555, -4581}, 2, 0, 0xe008, 0x7009, 0xa00d}
, {{ 0, 1888, -13944},{ 3609, 8555, -4581},{ 0, 2048, -17067}, 2, 0, 0xe008, 0xa00d, 0xe00a}
, {{ -3574, -130, 24601},{ -3574, 370, 24601},{ -3574, 370, 16812}, 2, 0, 0x3204, 0x6008, 0x7409}
, {{ -3574, -130, 24601},{ -3574, 370, 16812},{ -3574, -130, 16812}, 2, 0, 0x3204, 0x7409, 0x4304}
, {{ -3574, -130, 16812},{ -3574, 370, 16812},{ -869, 370, 16812}, 2, 0, 0x4304, 0x7409, 0x6108}
, {{ -3574, -130, 16812},{ -869, 370, 16812},{ -869, -130, 16812}, 2, 0, 0x4304, 0x6108, 0x6606}
, {{ -869, -130, 16812},{ -869, 370, 16812},{ -869, 370, 24601}, 2, 0, 0x6606, 0x6108, 0x6008}
, {{ -869, -130, 16812},{ -869, 370, 24601},{ -869, -130, 24601}, 2, 0, 0x6606, 0x6008, 0x2003}
, {{ -869, -130, 24601},{ -869, 370, 24601},{ -3574, 370, 24601}, 2, 0, 0x2003, 0x6008, 0x6008}
, {{ -869, -130, 24601},{ -3574, 370, 24601},{ -3574, -130, 24601}, 2, 0, 0x2003, 0x6008, 0x3204}
, {{ -3574, -130, 16812},{ -869, -130, 16812},{ -869, -130, 24601}, 2, 0, 0x4304, 0x6606, 0x2003}
, {{ -3574, -130, 16812},{ -869, -130, 24601},{ -3574, -130, 24601}, 2, 0, 0x4304, 0x2003, 0x3204}
, {{ -869, 370, 16812},{ -3574, 370, 16812},{ -3574, 370, 24601}, 2, 0, 0x6108, 0x7409, 0x6008}
, {{ -869, 370, 16812},{ -3574, 370, 24601},{ -869, 370, 24601}, 2, 0, 0x6108, 0x6008, 0x6008}
, {{ -8258, -75, 20097},{ -8258, 316, 20097},{ -2996, 316, 19692}, 2, 0, 0x3204, 0x6008, 0x7409}
, {{ -8258, -75, 20097},{ -2996, 316, 19692},{ -2996, -75, 19692}, 2, 0, 0x3204, 0x7409, 0x4304}
, {{ -2996, -75, 19692},{ -2996, 316, 19692},{ -2878, 316, 21804}, 2, 0, 0x4304, 0x7409, 0x6108}
, {{ -2996, -75, 19692},{ -2878, 316, 21804},{ -2878, -75, 21804}, 2, 0, 0x4304, 0x6108, 0x6606}
, {{ -2878, -75, 21804},{ -2878, 316, 21804},{ -8141, 316, 22208}, 2, 0, 0x6606, 0x6108, 0x6008}
, {{ -2878, -75, 21804},{ -8141, 316, 22208},{ -8141, -75, 22208}, 2, 0, 0x6606, 0x6008, 0x2003}
, {{ -8141, -75, 22208},{ -8141, 316, 22208},{ -8258, 316, 20097}, 2, 0, 0x2003, 0x6008, 0x6008}
, {{ -8141, -75, 22208},{ -8258, 316, 20097},{ -8258, -75, 20097}, 2, 0, 0x2003, 0x6008, 0x3204}
, {{ -2996, -75, 19692},{ -2878, -75, 21804},{ -8141, -75, 22208}, 2, 0, 0x4304, 0x6606, 0x2003}
, {{ -2996, -75, 19692},{ -8141, -75, 22208},{ -8258, -75, 20097}, 2, 0, 0x4304, 0x2003, 0x3204}
, {{ -2878, 316, 21804},{ -2996, 316, 19692},{ -8258, 316, 20097}, 2, 0, 0x6108, 0x7409, 0x6008}
, {{ -2878, 316, 21804},{ -8258, 316, 20097},{ -8141, 316, 22208}, 2, 0, 0x6108, 0x6008, 0x6008}
, {{ -1615, 9637, 3773},{ -3194, 12381, 3350},{ -4349, 9637, 4505}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -3616, 9637, 1771},{ -3194, 12381, 3350},{ -1615, 9637, 3773}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -4349, 9637, 4505},{ -3194, 12381, 3350},{ -3616, 9637, 1771}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -2377, 9637, 4765},{ -3194, 12381, 3350},{ -4827, 9637, 3350}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -2377, 9637, 1935},{ -3194, 12381, 3350},{ -2377, 9637, 4765}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -4827, 9637, 3350},{ -3194, 12381, 3350},{ -2377, 9637, 1935}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -5451, 3516, 1092},{ -3194, 8877, 3350},{ -110, 3516, 2523}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -1294, 6840, 4807},{ -3194, 10861, 3350},{ -5406, 6840, 4266}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -2881, 6840, 976},{ -3194, 10861, 3350},{ -1294, 6840, 4807}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -3915, 827, 2628},{ -3194, 12390, 3350},{ -2208, 827, 3086}, 2, 0, 0x4007, 0x3004, 0x4007}
, {{ -5406, 6840, 4266},{ -3194, 10861, 3350},{ -2881, 6840, 976}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -2881, 6840, 5724},{ -3194, 10861, 3350},{ -5406, 6840, 2433}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -1294, 6840, 1892},{ -3194, 10861, 3350},{ -2881, 6840, 5724}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -5406, 6840, 2433},{ -3194, 10861, 3350},{ -1294, 6840, 1892}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -1597, 3516, 6115},{ -3194, 8877, 3350},{ -6386, 3516, 3350}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -1597, 3516, 585},{ -3194, 8877, 3350},{ -1597, 3516, 6115}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -6386, 3516, 3350},{ -3194, 8877, 3350},{ -1597, 3516, 585}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -4020, 3516, 6434},{ -3194, 8877, 3350},{ -5451, 3516, 1092}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -110, 3516, 2523},{ -3194, 8877, 3350},{ -4020, 3516, 6434}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -3458, 827, 4335},{ -3194, 12390, 3350},{ -3915, 827, 2628}, 2, 0, 0x4007, 0x3004, 0x4007}
, {{ -2208, 827, 3086},{ -3194, 12390, 3350},{ -3458, 827, 4335}, 2, 0, 0x4007, 0x3004, 0x4007}
, {{ -16107, 9460, -3422},{ -16944, 11338, -2679},{ -16719, 9460, -1584}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -18005, 9460, -3032},{ -16944, 11338, -2679},{ -16107, 9460, -3422}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -16719, 9460, -1584},{ -16944, 11338, -2679},{ -18005, 9460, -3032}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -15827, 9460, -2613},{ -16944, 11338, -2679},{ -17559, 9460, -1745}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -17444, 9460, -3679},{ -16944, 11338, -2679},{ -15827, 9460, -2613}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -17559, 9460, -1745},{ -16944, 11338, -2679},{ -17444, 9460, -3679}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -19085, 5270, -2240},{ -16944, 8940, -2679},{ -16254, 5270, -4753}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -15395, 7546, -3215},{ -16944, 10298, -2679},{ -17254, 7546, -1070}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -18183, 7546, -3753},{ -16944, 10298, -2679},{ -15395, 7546, -3215}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -17628, 3429, -2539},{ -16944, 11345, -2679},{ -16723, 3429, -3342}, 2, 0, 0x4007, 0x3004, 0x4007}
, {{ -17254, 7546, -1070},{ -16944, 10298, -2679},{ -18183, 7546, -3753}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -15469, 7546, -1963},{ -16944, 10298, -2679},{ -18301, 7546, -1760}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -17061, 7546, -4314},{ -16944, 10298, -2679},{ -15469, 7546, -1963}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -18301, 7546, -1760},{ -16944, 10298, -2679},{ -17061, 7546, -4314}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -14762, 5270, -2549},{ -16944, 8940, -2679},{ -18147, 5270, -855}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -17922, 5270, -4633},{ -16944, 8940, -2679},{ -14762, 5270, -2549}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -18147, 5270, -855},{ -16944, 8940, -2679},{ -17922, 5270, -4633}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -15493, 5270, -1045},{ -16944, 8940, -2679},{ -19085, 5270, -2240}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -16254, 5270, -4753},{ -16944, 8940, -2679},{ -15493, 5270, -1045}, 2, 0, 0xa001, 0x4000, 0xa001}
, {{ -16480, 3429, -2157},{ -16944, 11345, -2679},{ -17628, 3429, -2539}, 2, 0, 0x4007, 0x3004, 0x4007}
, {{ -16723, 3429, -3342},{ -16944, 11345, -2679},{ -16480, 3429, -2157}, 2, 0, 0x4007, 0x3004, 0x4007}
};
const struct chunk lod0_chunks[WORLD_SIZE_X][WORLD_SIZE_Y] = {
{{0, 0, NULL}, 
{0, 0, NULL}, 
{0, 0, NULL}
},{{0, 0, NULL}, 
{LOD0_CHUNK_TRIANGLES_1_1, 0, lod0_chunk_triangles_1_1}, 
{0, 0, NULL}
},{{0, 0, NULL}, 
{0, 0, NULL}, 
{0, 0, NULL}
}};
const struct chunk lod1_chunks[WORLD_SIZE_X][WORLD_SIZE_Y] = {
{{0, 0, NULL}, 
{0, 0, NULL}, 
{0, 0, NULL}
},{{0, 0, NULL}, 
{0, 0, NULL}, 
{0, 0, NULL}
},{{0, 0, NULL}, 
{0, 0, NULL}, 
{0, 0, NULL}
}};
struct texture chunk_texture_list[0] = {};
const struct chunk_lighting chunk_lights[3][3] = {{{0, NULL},{0, NULL},{0, NULL}},
{{0, NULL},{0, NULL},{0, NULL}},
{{0, NULL},{0, NULL},{0, NULL}}};