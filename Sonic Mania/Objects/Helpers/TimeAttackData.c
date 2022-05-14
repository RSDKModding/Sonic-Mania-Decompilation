// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackData Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTimeAttackData *TimeAttackData;

static const char *playerNames[] = { "Invalid", "Sonic", "Tails", "Knuckles", "Mighty", "Ray" };
static const char *actNames[]    = { "Act 1", "Act 2", "Act 3", "" };
static const char *zoneNames[]   = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ", "ERZ", "AIZ" };
static const char *modeNames[]   = { "Mania", "Encore" };

static LeaderboardID leaderboardInfo[] = { { 0, 1, 71, "PlusGHZ1SonicMania", "GHZ1_S_MANIA" },
                                           { 1, 2, 72, "PlusGHZ1TailsMania", "GHZ1_T_MANIA" },
                                           { 2, 3, 73, "PlusGHZ1KnucklesMania", "GHZ1_K_MANIA" },
                                           { 3, 4, 74, "PlusGHZ1MightyMania", "GHZ1_M_MANIA" },
                                           { 4, 5, 75, "PlusGHZ1RayMania", "GHZ1_R_MANIA" },
                                           { 5, 6, 76, "PlusGHZ2SonicMania", "GHZ2_S_MANIA" },
                                           { 6, 7, 77, "PlusGHZ2TailsMania", "GHZ2_T_MANIA" },
                                           { 7, 8, 78, "PlusGHZ2KnucklesMania", "GHZ2_K_MANIA" },
                                           { 8, 9, 79, "PlusGHZ2MightyMania", "GHZ2_M_MANIA" },
                                           { 9, 10, 80, "PlusGHZ2RayMania", "GHZ2_R_MANIA" },
                                           { 10, 11, 81, "PlusCPZ1SonicMania", "CPZ1_S_MANIA" },
                                           { 11, 12, 82, "PlusCPZ1TailsMania", "CPZ1_T_MANIA" },
                                           { 12, 13, 83, "PlusCPZ1KnucklesMania", "CPZ1_K_MANIA" },
                                           { 13, 14, 84, "PlusCPZ1MightyMania", "CPZ1_M_MANIA" },
                                           { 14, 15, 85, "PlusCPZ1RayMania", "CPZ1_R_MANIA" },
                                           { 15, 16, 86, "PlusCPZ2SonicMania", "CPZ2_S_MANIA" },
                                           { 16, 17, 87, "PlusCPZ2TailsMania", "CPZ2_T_MANIA" },
                                           { 17, 18, 88, "PlusCPZ2KnucklesMania", "CPZ2_K_MANIA" },
                                           { 18, 19, 89, "PlusCPZ2MightyMania", "CPZ2_M_MANIA" },
                                           { 19, 20, 90, "PlusCPZ2RayMania", "CPZ2_R_MANIA" },
                                           { 20, 21, 91, "PlusSPZ1SonicMania", "SPZ1_S_MANIA" },
                                           { 21, 22, 92, "PlusSPZ1TailsMania", "SPZ1_T_MANIA" },
                                           { 22, 23, 93, "PlusSPZ1KnucklesMania", "SPZ1_K_MANIA" },
                                           { 23, 24, 94, "PlusSPZ1MightyMania", "SPZ1_M_MANIA" },
                                           { 24, 25, 95, "PlusSPZ1RayMania", "SPZ1_R_MANIA" },
                                           { 25, 26, 96, "PlusSPZ2SonicMania", "SPZ2_S_MANIA" },
                                           { 26, 27, 97, "PlusSPZ2TailsMania", "SPZ2_T_MANIA" },
                                           { 27, 28, 98, "PlusSPZ2KnucklesMania", "SPZ2_K_MANIA" },
                                           { 28, 29, 99, "PlusSPZ2MightyMania", "SPZ2_M_MANIA" },
                                           { 29, 30, 100, "PlusSPZ2RayMania", "SPZ2_R_MANIA" },
                                           { 30, 31, 101, "PlusFBZ1SonicMania", "FBZ1_S_MANIA" },
                                           { 31, 32, 102, "PlusFBZ1TailsMania", "FBZ1_T_MANIA" },
                                           { 32, 33, 103, "PlusFBZ1KnucklesMania", "FBZ1_K_MANIA" },
                                           { 33, 34, 104, "PlusFBZ1MightyMania", "FBZ1_M_MANIA" },
                                           { 34, 35, 105, "PlusFBZ1RayMania", "FBZ1_R_MANIA" },
                                           { 35, 36, 106, "PlusFBZ2SonicMania", "FBZ2_S_MANIA" },
                                           { 36, 37, 107, "PlusFBZ2TailsMania", "FBZ2_T_MANIA" },
                                           { 37, 38, 108, "PlusFBZ2KnucklesMania", "FBZ2_K_MANIA" },
                                           { 38, 39, 109, "PlusFBZ2MightyMania", "FBZ2_M_MANIA" },
                                           { 39, 40, 110, "PlusFBZ2RayMania", "FBZ2_R_MANIA" },
                                           { 40, 41, 111, "PlusPGZ1SonicMania", "PGZ1_S_MANIA" },
                                           { 41, 42, 112, "PlusPGZ1TailsMania", "PGZ1_T_MANIA" },
                                           { 42, 43, 113, "PlusPGZ1KnucklesMania", "PGZ1_K_MANIA" },
                                           { 43, 44, 114, "PlusPGZ1MightyMania", "PGZ1_M_MANIA" },
                                           { 44, 45, 115, "PlusPGZ1RayMania", "PGZ1_R_MANIA" },
                                           { 45, 46, 116, "PlusPGZ2SonicMania", "PGZ2_S_MANIA" },
                                           { 46, 47, 117, "PlusPGZ2TailsMania", "PGZ2_T_MANIA" },
                                           { 47, 48, 118, "PlusPGZ2KnucklesMania", "PGZ2_K_MANIA" },
                                           { 48, 49, 119, "PlusPGZ2MightyMania", "PGZ2_M_MANIA" },
                                           { 49, 50, 120, "PlusPGZ2RayMania", "PGZ2_R_MANIA" },
                                           { 50, 51, 121, "PlusSSZ1SonicMania", "SSZ1_S_MANIA" },
                                           { 51, 52, 122, "PlusSSZ1TailsMania", "SSZ1_T_MANIA" },
                                           { 52, 53, 123, "PlusSSZ1KnucklesMania", "SSZ1_K_MANIA" },
                                           { 53, 54, 124, "PlusSSZ1MightyMania", "SSZ1_M_MANIA" },
                                           { 54, 55, 125, "PlusSSZ1RayMania", "SSZ1_R_MANIA" },
                                           { 55, 56, 126, "PlusSSZ2SonicMania", "SSZ2_S_MANIA" },
                                           { 56, 57, 127, "PlusSSZ2TailsMania", "SSZ2_T_MANIA" },
                                           { 57, 58, 128, "PlusSSZ2KnucklesMania", "SSZ2_K_MANIA" },
                                           { 58, 59, 129, "PlusSSZ2MightyMania", "SSZ2_M_MANIA" },
                                           { 59, 60, 130, "PlusSSZ2RayMania", "SSZ2_R_MANIA" },
                                           { 60, 61, 131, "PlusHCZ1SonicMania", "HCZ1_S_MANIA" },
                                           { 61, 62, 132, "PlusHCZ1TailsMania", "HCZ1_T_MANIA" },
                                           { 62, 63, 133, "PlusHCZ1KnucklesMania", "HCZ1_K_MANIA" },
                                           { 63, 64, 134, "PlusHCZ1MightyMania", "HCZ1_M_MANIA" },
                                           { 64, 65, 135, "PlusHCZ1RayMania", "HCZ1_R_MANIA" },
                                           { 65, 66, 136, "PlusHCZ2SonicMania", "HCZ2_S_MANIA" },
                                           { 66, 67, 137, "PlusHCZ2TailsMania", "HCZ2_T_MANIA" },
                                           { 67, 68, 138, "PlusHCZ2KnucklesMania", "HCZ2_K_MANIA" },
                                           { 68, 69, 139, "PlusHCZ2MightyMania", "HCZ2_M_MANIA" },
                                           { 69, 70, 140, "PlusHCZ2RayMania", "HCZ2_R_MANIA" },
                                           { -1, -1, -1, NULL, NULL },
                                           { -1, -1, -1, NULL, NULL },
                                           { 72, 71, 141, "PlusMSZ1KnucklesMania", "MSZ1_K_MANIA" },
                                           { -1, -1, -1, NULL, NULL },
                                           { -1, -1, -1, NULL, NULL },
                                           { 75, 72, 142, "PlusMSZ2SonicMania", "MSZ2_S_MANIA" },
                                           { 76, 73, 143, "PlusMSZ2TailsMania", "MSZ2_T_MANIA" },
                                           { 77, 74, 144, "PlusMSZ2KnucklesMania", "MSZ2_K_MANIA" },
                                           { 78, 75, 145, "PlusMSZ2MightyMania", "MSZ2_M_MANIA" },
                                           { 79, 76, 146, "PlusMSZ2RayMania", "MSZ2_R_MANIA" },
                                           { 80, 77, 147, "PlusOOZ1SonicMania", "OOZ1_S_MANIA" },
                                           { 81, 78, 148, "PlusOOZ1TailsMania", "OOZ1_T_MANIA" },
                                           { 82, 79, 149, "PlusOOZ1KnucklesMania", "OOZ1_K_MANIA" },
                                           { 83, 80, 150, "PlusOOZ1MightyMania", "OOZ1_M_MANIA" },
                                           { 84, 81, 151, "PlusOOZ1RayMania", "OOZ1_R_MANIA" },
                                           { 85, 82, 152, "PlusOOZ2SonicMania", "OOZ2_S_MANIA" },
                                           { 86, 83, 153, "PlusOOZ2TailsMania", "OOZ2_T_MANIA" },
                                           { 87, 84, 154, "PlusOOZ2KnucklesMania", "OOZ2_K_MANIA" },
                                           { 88, 85, 155, "PlusOOZ2MightyMania", "OOZ2_M_MANIA" },
                                           { 89, 86, 156, "PlusOOZ2RayMania", "OOZ2_R_MANIA" },
                                           { 90, 87, 157, "PlusLRZ1SonicMania", "LRZ1_S_MANIA" },
                                           { 91, 88, 158, "PlusLRZ1TailsMania", "LRZ1_T_MANIA" },
                                           { 92, 89, 159, "PlusLRZ1KnucklesMania", "LRZ1_K_MANIA" },
                                           { 93, 90, 160, "PlusLRZ1MightyMania", "LRZ1_M_MANIA" },
                                           { 94, 91, 161, "PlusLRZ1RayMania", "LRZ1_R_MANIA" },
                                           { 95, 92, 162, "PlusLRZ2SonicMania", "LRZ2_S_MANIA" },
                                           { 96, 93, 163, "PlusLRZ2TailsMania", "LRZ2_T_MANIA" },
                                           { 97, 94, 164, "PlusLRZ2KnucklesMania", "LRZ2_K_MANIA" },
                                           { 98, 95, 165, "PlusLRZ2MightyMania", "LRZ2_M_MANIA" },
                                           { 99, 96, 166, "PlusLRZ2RayMania", "LRZ2_R_MANIA" },
                                           { 100, 97, 167, "PlusMMZ1SonicMania", "MMZ1_S_MANIA" },
                                           { 101, 98, 168, "PlusMMZ1TailsMania", "MMZ1_T_MANIA" },
                                           { 102, 99, 169, "PlusMMZ1KnucklesMania", "MMZ1_K_MANIA" },
                                           { 103, 100, 170, "PlusMMZ1MightyMania", "MMZ1_M_MANIA" },
                                           { 104, 101, 171, "PlusMMZ1RayMania", "MMZ1_R_MANIA" },
                                           { 105, 102, 172, "PlusMMZ2SonicMania", "MMZ2_S_MANIA" },
                                           { 106, 103, 173, "PlusMMZ2TailsMania", "MMZ2_T_MANIA" },
                                           { 107, 104, 174, "PlusMMZ2KnucklesMania", "MMZ2_K_MANIA" },
                                           { 108, 105, 175, "PlusMMZ2MightyMania", "MMZ2_M_MANIA" },
                                           { 109, 106, 176, "PlusMMZ2RayMania", "MMZ2_R_MANIA" },
                                           { 110, 107, 177, "PlusTMZ1SonicMania", "TMZ1_S_MANIA" },
                                           { 111, 108, 178, "PlusTMZ1TailsMania", "TMZ1_T_MANIA" },
                                           { 112, 109, 179, "PlusTMZ1KnucklesMania", "TMZ1_K_MANIA" },
                                           { 113, 110, 180, "PlusTMZ1MightyMania", "TMZ1_M_MANIA" },
                                           { 114, 111, 181, "PlusTMZ1RayMania", "TMZ1_R_MANIA" },
                                           { 115, 112, 182, "PlusTMZ2SonicMania", "TMZ2_S_MANIA" },
                                           { 116, 113, 183, "PlusTMZ2TailsMania", "TMZ2_T_MANIA" },
                                           { 117, 114, 184, "PlusTMZ2KnucklesMania", "TMZ2_K_MANIA" },
                                           { 118, 115, 185, "PlusTMZ2MightyMania", "TMZ2_M_MANIA" },
                                           { 119, 116, 186, "PlusTMZ2RayMania", "TMZ2_R_MANIA" },
                                           { 120, 117, 187, "PlusGHZ1SonicEncore", "GHZ1_S_ENCORE" },
                                           { 121, 118, 188, "PlusGHZ1TailsEncore", "GHZ1_T_ENCORE" },
                                           { 122, 119, 189, "PlusGHZ1KnucklesEncore", "GHZ1_K_ENCORE" },
                                           { 123, 120, 190, "PlusGHZ1MightyEncore", "GHZ1_M_ENCORE" },
                                           { 124, 121, 191, "PlusGHZ1RayEncore", "GHZ1_R_ENCORE" },
                                           { 125, 122, 192, "PlusGHZ2SonicEncore", "GHZ2_S_ENCORE" },
                                           { 126, 123, 193, "PlusGHZ2TailsEncore", "GHZ2_T_ENCORE" },
                                           { 127, 124, 194, "PlusGHZ2KnucklesEncore", "GHZ2_K_ENCORE" },
                                           { 128, 125, 195, "PlusGHZ2MightyEncore", "GHZ2_M_ENCORE" },
                                           { 129, 126, 196, "PlusGHZ2RayEncore", "GHZ2_R_ENCORE" },
                                           { 130, 127, 197, "PlusCPZ1SonicEncore", "CPZ1_S_ENCORE" },
                                           { 131, 128, 198, "PlusCPZ1TailsEncore", "CPZ1_T_ENCORE" },
                                           { 132, 129, 199, "PlusCPZ1KnucklesEncore", "CPZ1_K_ENCORE" },
                                           { 133, 130, 200, "PlusCPZ1MightyEncore", "CPZ1_M_ENCORE" },
                                           { 134, 131, 201, "PlusCPZ1RayEncore", "CPZ1_R_ENCORE" },
                                           { 135, 132, 202, "PlusCPZ2SonicEncore", "CPZ2_S_ENCORE" },
                                           { 136, 133, 203, "PlusCPZ2TailsEncore", "CPZ2_T_ENCORE" },
                                           { 137, 134, 204, "PlusCPZ2KnucklesEncore", "CPZ2_K_ENCORE" },
                                           { 138, 135, 205, "PlusCPZ2MightyEncore", "CPZ2_M_ENCORE" },
                                           { 139, 136, 206, "PlusCPZ2RayEncore", "CPZ2_R_ENCORE" },
                                           { 140, 137, 207, "PlusSPZ1SonicEncore", "SPZ1_S_ENCORE" },
                                           { 141, 138, 208, "PlusSPZ1TailsEncore", "SPZ1_T_ENCORE" },
                                           { 142, 139, 209, "PlusSPZ1KnucklesEncore", "SPZ1_K_ENCORE" },
                                           { 143, 140, 210, "PlusSPZ1MightyEncore", "SPZ1_M_ENCORE" },
                                           { 144, 141, 211, "PlusSPZ1RayEncore", "SPZ1_R_ENCORE" },
                                           { 145, 142, 212, "PlusSPZ2SonicEncore", "SPZ2_S_ENCORE" },
                                           { 146, 143, 213, "PlusSPZ2TailsEncore", "SPZ2_T_ENCORE" },
                                           { 147, 144, 214, "PlusSPZ2KnucklesEncore", "SPZ2_K_ENCORE" },
                                           { 148, 145, 215, "PlusSPZ2MightyEncore", "SPZ2_M_ENCORE" },
                                           { 149, 146, 216, "PlusSPZ2RayEncore", "SPZ2_R_ENCORE" },
                                           { 150, 147, 217, "PlusFBZ1SonicEncore", "FBZ1_S_ENCORE" },
                                           { 151, 148, 218, "PlusFBZ1TailsEncore", "FBZ1_T_ENCORE" },
                                           { 152, 149, 219, "PlusFBZ1KnucklesEncore", "FBZ1_K_ENCORE" },
                                           { 153, 150, 220, "PlusFBZ1MightyEncore", "FBZ1_M_ENCORE" },
                                           { 154, 151, 221, "PlusFBZ1RayEncore", "FBZ1_R_ENCORE" },
                                           { 155, 152, 222, "PlusFBZ2SonicEncore", "FBZ2_S_ENCORE" },
                                           { 156, 153, 223, "PlusFBZ2TailsEncore", "FBZ2_T_ENCORE" },
                                           { 157, 154, 224, "PlusFBZ2KnucklesEncore", "FBZ2_K_ENCORE" },
                                           { 158, 155, 225, "PlusFBZ2MightyEncore", "FBZ2_M_ENCORE" },
                                           { 159, 156, 226, "PlusFBZ2RayEncore", "FBZ2_R_ENCORE" },
                                           { 160, 157, 227, "PlusPGZ1SonicEncore", "PGZ1_S_ENCORE" },
                                           { 161, 158, 228, "PlusPGZ1TailsEncore", "PGZ1_T_ENCORE" },
                                           { 162, 159, 229, "PlusPGZ1KnucklesEncore", "PGZ1_K_ENCORE" },
                                           { 163, 160, 230, "PlusPGZ1MightyEncore", "PGZ1_M_ENCORE" },
                                           { 164, 161, 231, "PlusPGZ1RayEncore", "PGZ1_R_ENCORE" },
                                           { 165, 162, 232, "PlusPGZ2SonicEncore", "PGZ2_S_ENCORE" },
                                           { 166, 163, 233, "PlusPGZ2TailsEncore", "PGZ2_T_ENCORE" },
                                           { 167, 164, 234, "PlusPGZ2KnucklesEncore", "PGZ2_K_ENCORE" },
                                           { 168, 165, 235, "PlusPGZ2MightyEncore", "PGZ2_M_ENCORE" },
                                           { 169, 166, 236, "PlusPGZ2RayEncore", "PGZ2_R_ENCORE" },
                                           { 170, 167, 237, "PlusSSZ1SonicEncore", "SSZ1_S_ENCORE" },
                                           { 171, 168, 238, "PlusSSZ1TailsEncore", "SSZ1_T_ENCORE" },
                                           { 172, 169, 239, "PlusSSZ1KnucklesEncore", "SSZ1_K_ENCORE" },
                                           { 173, 170, 240, "PlusSSZ1MightyEncore", "SSZ1_M_ENCORE" },
                                           { 174, 171, 241, "PlusSSZ1RayEncore", "SSZ1_R_ENCORE" },
                                           { 175, 172, 242, "PlusSSZ2SonicEncore", "SSZ2_S_ENCORE" },
                                           { 176, 173, 243, "PlusSSZ2TailsEncore", "SSZ2_T_ENCORE" },
                                           { 177, 174, 244, "PlusSSZ2KnucklesEncore", "SSZ2_K_ENCORE" },
                                           { 178, 175, 245, "PlusSSZ2MightyEncore", "SSZ2_M_ENCORE" },
                                           { 179, 176, 246, "PlusSSZ2RayEncore", "SSZ2_R_ENCORE" },
                                           { 180, 177, 247, "PlusHCZ1SonicEncore", "HCZ1_S_ENCORE" },
                                           { 181, 178, 248, "PlusHCZ1TailsEncore", "HCZ1_T_ENCORE" },
                                           { 182, 179, 249, "PlusHCZ1KnucklesEncore", "HCZ1_K_ENCORE" },
                                           { 183, 180, 250, "PlusHCZ1MightyEncore", "HCZ1_M_ENCORE" },
                                           { 184, 181, 251, "PlusHCZ1RayEncore", "HCZ1_R_ENCORE" },
                                           { 185, 182, 252, "PlusHCZ2SonicEncore", "HCZ2_S_ENCORE" },
                                           { 186, 183, 253, "PlusHCZ2TailsEncore", "HCZ2_T_ENCORE" },
                                           { 187, 184, 254, "PlusHCZ2KnucklesEncore", "HCZ2_K_ENCORE" },
                                           { 188, 185, 255, "PlusHCZ2MightyEncore", "HCZ2_M_ENCORE" },
                                           { 189, 186, 256, "PlusHCZ2RayEncore", "HCZ2_R_ENCORE" },
                                           { 190, 187, 257, "PlusMSZ1SonicEncore", "MSZ1_S_ENCORE" },
                                           { 191, 188, 258, "PlusMSZ1TailsEncore", "MSZ1_T_ENCORE" },
                                           { 192, 189, 259, "PlusMSZ1KnucklesEncore", "MSZ1_K_ENCORE" },
                                           { 193, 190, 260, "PlusMSZ1MightyEncore", "MSZ1_M_ENCORE" },
                                           { 194, 191, 261, "PlusMSZ1RayEncore", "MSZ1_R_ENCORE" },
                                           { 195, 192, 262, "PlusMSZ2SonicEncore", "MSZ2_S_ENCORE" },
                                           { 196, 193, 263, "PlusMSZ2TailsEncore", "MSZ2_T_ENCORE" },
                                           { 197, 194, 264, "PlusMSZ2KnucklesEncore", "MSZ2_K_ENCORE" },
                                           { 198, 195, 265, "PlusMSZ2MightyEncore", "MSZ2_M_ENCORE" },
                                           { 199, 196, 266, "PlusMSZ2RayEncore", "MSZ2_R_ENCORE" },
                                           { 200, 197, 267, "PlusOOZ1SonicEncore", "OOZ1_S_ENCORE" },
                                           { 201, 198, 268, "PlusOOZ1TailsEncore", "OOZ1_T_ENCORE" },
                                           { 202, 199, 269, "PlusOOZ1KnucklesEncore", "OOZ1_K_ENCORE" },
                                           { 203, 200, 270, "PlusOOZ1MightyEncore", "OOZ1_M_ENCORE" },
                                           { 204, 201, 271, "PlusOOZ1RayEncore", "OOZ1_R_ENCORE" },
                                           { 205, 202, 272, "PlusOOZ2SonicEncore", "OOZ2_S_ENCORE" },
                                           { 206, 203, 273, "PlusOOZ2TailsEncore", "OOZ2_T_ENCORE" },
                                           { 207, 204, 274, "PlusOOZ2KnucklesEncore", "OOZ2_K_ENCORE" },
                                           { 208, 205, 275, "PlusOOZ2MightyEncore", "OOZ2_M_ENCORE" },
                                           { 209, 206, 276, "PlusOOZ2RayEncore", "OOZ2_R_ENCORE" },
                                           { 210, 207, 277, "PlusLRZ1SonicEncore", "LRZ1_S_ENCORE" },
                                           { 211, 208, 278, "PlusLRZ1TailsEncore", "LRZ1_T_ENCORE" },
                                           { 212, 209, 279, "PlusLRZ1KnucklesEncore", "LRZ1_K_ENCORE" },
                                           { 213, 210, 280, "PlusLRZ1MightyEncore", "LRZ1_M_ENCORE" },
                                           { 214, 211, 281, "PlusLRZ1RayEncore", "LRZ1_R_ENCORE" },
                                           { 215, 212, 282, "PlusLRZ2SonicEncore", "LRZ2_S_ENCORE" },
                                           { 216, 213, 283, "PlusLRZ2TailsEncore", "LRZ2_T_ENCORE" },
                                           { 217, 214, 284, "PlusLRZ2KnucklesEncore", "LRZ2_K_ENCORE" },
                                           { 218, 215, 285, "PlusLRZ2MightyEncore", "LRZ2_M_ENCORE" },
                                           { 219, 216, 286, "PlusLRZ2RayEncore", "LRZ2_R_ENCORE" },
                                           { 220, 217, 287, "PlusMMZ1SonicEncore", "MMZ1_S_ENCORE" },
                                           { 221, 218, 288, "PlusMMZ1TailsEncore", "MMZ1_T_ENCORE" },
                                           { 222, 219, 289, "PlusMMZ1KnucklesEncore", "MMZ1_K_ENCORE" },
                                           { 223, 220, 290, "PlusMMZ1MightyEncore", "MMZ1_M_ENCORE" },
                                           { 224, 221, 291, "PlusMMZ1RayEncore", "MMZ1_R_ENCORE" },
                                           { 225, 222, 292, "PlusMMZ2SonicEncore", "MMZ2_S_ENCORE" },
                                           { 226, 223, 293, "PlusMMZ2TailsEncore", "MMZ2_T_ENCORE" },
                                           { 227, 224, 294, "PlusMMZ2KnucklesEncore", "MMZ2_K_ENCORE" },
                                           { 228, 225, 295, "PlusMMZ2MightyEncore", "MMZ2_M_ENCORE" },
                                           { 229, 226, 296, "PlusMMZ2RayEncore", "MMZ2_R_ENCORE" },
                                           { 230, 227, 297, "PlusTMZ1SonicEncore", "TMZ1_S_ENCORE" },
                                           { 231, 228, 298, "PlusTMZ1TailsEncore", "TMZ1_T_ENCORE" },
                                           { 232, 229, 299, "PlusTMZ1KnucklesEncore", "TMZ1_K_ENCORE" },
                                           { 233, 230, 300, "PlusTMZ1MightyEncore", "TMZ1_M_ENCORE" },
                                           { 234, 231, 301, "PlusTMZ1RayEncore", "TMZ1_R_ENCORE" },
                                           { 235, 232, 302, "PlusTMZ2SonicEncore", "TMZ2_S_ENCORE" },
                                           { 236, 233, 303, "PlusTMZ2TailsEncore", "TMZ2_T_ENCORE" },
                                           { 237, 234, 304, "PlusTMZ2KnucklesEncore", "TMZ2_K_ENCORE" },
                                           { 238, 235, 305, "PlusTMZ2MightyEncore", "TMZ2_M_ENCORE" },
                                           { 239, 236, 306, "PlusTMZ2RayEncore", "TMZ2_R_ENCORE" } };

void TimeAttackData_Update(void) {}

void TimeAttackData_LateUpdate(void) {}

void TimeAttackData_StaticUpdate(void) {}

void TimeAttackData_Draw(void) {}

void TimeAttackData_Create(void *data) {}

void TimeAttackData_StageLoad(void) {}

#if RETRO_USE_PLUS
void TimeAttackData_TrackActClear(StatInfo *stat, uint8 zone, uint8 act, uint8 characterID, int32 time, int32 rings, int32 score)
{
    stat->statID = 0;
    stat->name   = "ACT_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)zoneNames[zone];
    stat->data[1] = (void *)actNames[act];
    stat->data[2] = (void *)playerNames[characterID];
    stat->data[3] = 0;
    stat->data[4] = intToVoid(time);
    stat->data[5] = intToVoid(rings);
    stat->data[6] = intToVoid(score);
}
void TimeAttackData_TrackTAClear(StatInfo *stat, uint8 zone, uint8 actID, uint8 characterID, int32 gameMode, int32 time)
{
    stat->statID = 1;
    stat->name   = "TA_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)zoneNames[zone];
    stat->data[1] = (void *)actNames[actID];
    stat->data[2] = (void *)playerNames[characterID];
    stat->data[3] = (void *)modeNames[gameMode];
    stat->data[4] = intToVoid(time);
}
void TimeAttackData_TrackEnemyDefeat(StatInfo *stat, uint8 zoneID, uint8 actID, uint8 characterID, bool32 encore, int32 x, int32 y)
{
    stat->statID = 2;
    stat->name   = "ENEMY_DEFEAT";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)zoneNames[zoneID];
    stat->data[1] = (void *)actNames[actID];
    stat->data[2] = (void *)playerNames[characterID];
    stat->data[3] = intToVoid(encore);
    stat->data[4] = intToVoid(x);
    stat->data[5] = intToVoid(y);
}
#endif

void TimeAttackData_Clear(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    param->puyoSelection = PUYO_SELECTION_NONE;
    memset(param->menuTag, 0, sizeof(param->menuTag));
    param->menuSelection    = 0;
    param->startedTAAttempt = false;
    param->inTimeAttack     = false;
    param->zoneID           = 0;
    param->actID            = 0;

#if RETRO_USE_PLUS
    param->timeAttackRank = 0;
    globals->gameMode     = MODE_MANIA;
#else
    param->timeScore  = 0;
    globals->gameMode = MODE_NOSAVE;
#endif

    globals->suppressAutoMusic = false;
    globals->suppressTitlecard = false;
}
int32 TimeAttackData_GetManiaListPos(int32 zoneID, int32 act, int32 characterID)
{
    int32 listPos = 0;
    switch (zoneID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: listPos = act + 2 * zoneID; break;

        case 6: listPos = act + (2 * zoneID + 1); break;

        case 7:
            if (act)
                listPos = 2 * zoneID + 3;
            else
                listPos = (characterID == 3) + 1 + 2 * zoneID;
            break;

        case 8:
        case 9: listPos = act + 2 * (zoneID + 1); break;

        case 10:
        case 11: listPos = act + (2 * zoneID + 3); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);

    return listPos;
}
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 act, int32 characterID)
{
    int32 listPos = 0;
    switch (zoneID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: listPos = act + 2 * zoneID; break;

        case 6:
        case 7:
        case 8:
        case 9: listPos = act + 2 * zoneID + 1; break;

        case 10:
        case 11: listPos = act + 2 * (zoneID + 1); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);
    return listPos;
}

void TimeAttackData_GetUnpackedTime(int32 time, int32 *minsPtr, int32 *secsPtr, int32 *millisecsPtr)
{
    int32 m  = time / 6000;
    int32 s  = time % 6000 / 100;
    int32 ms = time % 100;

    if (minsPtr)
        *minsPtr = m;
    if (secsPtr)
        *secsPtr = s;
    if (millisecsPtr)
        *millisecsPtr = ms;
}

uint16 *TimeAttackData_GetRecordedTime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank)
{
    rank--;
    if (rank >= 3)
        return NULL;

    uint16 *saveRAM = NULL;
    if (globals->saveLoaded == STATUS_OK)
        saveRAM = (uint16 *)&globals->saveRAM[0x800];
    else
        return NULL;

    // 72 = (12 * 6)
    int32 pos = 72 * (characterID - 1) + 6 * zoneID + act * 3 + rank;
    // not sure why theres an offset of 43 uint16's (86 bytes) but whatever ig
    pos += 43;
    return &saveRAM[pos];
}

#if RETRO_USE_PLUS
uint16 TimeAttackData_LoadTimeAttackDB(void (*callback)(bool32 success))
{
    LogHelpers_Print("Loading Time Attack DB");
    globals->taTableLoaded = STATUS_CONTINUE;

    TimeAttackData->loadEntityPtr = SceneInfo->entity;
    TimeAttackData->loadCallback  = callback;
    globals->taTableID            = API.LoadUserDB("TimeAttackDB.bin", TimeAttackData_LoadTimeAttackDB_CB);

    if (globals->taTableID == -1) {
        LogHelpers_Print("Couldn't claim a slot for loading %s", "TimeAttackDB.bin");
        globals->taTableLoaded = STATUS_ERROR;
    }

    return globals->taTableID;
}

void TimeAttackData_LoadTimeAttackDB_CB(int32 statusCode)
{
    if (statusCode == STATUS_OK) {
        globals->taTableLoaded = STATUS_OK;
        API.SetupUserDBRowSorting(globals->taTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->taTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Time Attack DB");
        TimeAttackData_ResetTimeAttackDB();
    }

    // Bug Details:
    // Due to how options work, this is called after the db is loaded, but before the result is assigned to globals->taTableID
    // meaning that globals->taTableID will be 0xFFFF initially, even if the tabel id was loaded and returned successfully
    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->taTableID, globals->taTableLoaded);

    if (TimeAttackData->loadCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->loadEntityPtr)
            SceneInfo->entity = TimeAttackData->loadEntityPtr;
        TimeAttackData->loadCallback(statusCode == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->loadCallback  = NULL;
        TimeAttackData->loadEntityPtr = NULL;
    }
}

void TimeAttackData_ResetTimeAttackDB(void)
{
    uint16 id = API.InitUserDB("TimeAttackDB.bin", DBVAR_UINT8, "zoneID", DBVAR_UINT8, "act", DBVAR_UINT8, "characterID", DBVAR_UINT8, "encore",
                               DBVAR_UINT32, "score", DBVAR_UINT32, "replayID", NULL);
    globals->taTableID = id;
    if (id == (uint16)-1) {
        globals->taTableLoaded = STATUS_ERROR;
    }
    else {
        globals->taTableLoaded = STATUS_OK;
        if (!API_GetNoSave() && globals->saveLoaded == STATUS_OK) {
            TimeAttackData_MigrateLegacySaves();
        }
    }
}

void TimeAttackData_MigrateLegacySaves(void)
{
    if (globals->saveLoaded == STATUS_OK) {
        TimeAttackData->isMigratingData = true;

        LogHelpers_Print("===========================");
        LogHelpers_Print("Migrating Legacy TA Data...");
        LogHelpers_Print("===========================");

        for (int32 zone = 0; zone < 12; ++zone) {
            for (int32 act = 0; act < 2; ++act) {
                for (int32 charID = 0; charID < 3; ++charID) {
                    for (int32 rank = 0; rank < 3; ++rank) {
                        uint16 *records = TimeAttackData_GetRecordedTime(zone, act, charID + 1, rank);
                        if (records && *records) {
                            int32 score = *records;
                            LogHelpers_Print("Import: zone=%d act=%d charID=%d rank=%d -> %d", zone, act, charID + 1, rank, score);
                            TimeAttackData_AddTADBEntry(zone, act, charID + 1, MODE_MANIA, score, NULL);
                        }
                    }
                }
            }
        }

        TimeAttackData->isMigratingData = false;
    }
}

int32 TimeAttackData_AddTimeAttackDBEntry(uint8 zoneID, uint8 act, uint8 characterID, uint8 mode, int32 score)
{
    if (globals->taTableLoaded != STATUS_OK)
        return -1;

    uint16 rowID = API.AddUserDBRow(globals->taTableID);
    int32 encore = mode & 1;

    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "zoneID", &zoneID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "act", &act);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "characterID", &characterID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "encore", &encore);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "score", &score);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "replayID", NULL);

    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, rowID);
    char buf[0x20];
    memset(buf, 0, 0x20 * sizeof(char));
    API.GetUserDBRowCreationTime(globals->taTableID, rowID, buf, 23, "%Y/%m/%d %H:%M:%S");

    LogHelpers_Print("Time Attack DB Added Entry");
    LogHelpers_Print("Created at %s", buf);
    LogHelpers_Print("Row ID: %d", rowID);
    LogHelpers_Print("UUID: %08X", uuid);

    return rowID;
}

int32 TimeAttackData_AddTADBEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score, void (*callback)(bool32 success))
{
    uint16 row  = TimeAttackData_AddTimeAttackDBEntry(zoneID, act, characterID, encore, score);
    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, row);

    TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);

    if (API.GetSortedUserDBRowCount(globals->taTableID) > 3) {
        int32 row = API.GetSortedUserDBRowID(globals->taTableID, 3);
        API.RemoveDBRow(globals->taTableID, row);

        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 rank  = 0;
    int32 rowID = 0;
    for (rank = 0; rank < 3; ++rank) {
        rowID = API.GetSortedUserDBRowID(globals->taTableID, rank);
        if (API.GetUserDBRowUUID(globals->taTableID, rowID) == uuid)
            break;
    }

    if (rank == 3) {
        if (callback)
            callback(false);

        return 0;
    }

    TimeAttackData->uuid         = uuid;
    TimeAttackData->rowID        = rowID;
    TimeAttackData->personalRank = rank + 1;

    if (TimeAttackData->isMigratingData) {
        if (callback)
            callback(true);
    }
    else {
        TimeAttackData_SaveTimeAttackDB(callback);
    }

    return rank + 1;
}

void TimeAttackData_SaveTimeAttackDB(void (*callback)(bool32 success))
{
    if (API.GetNoSave() || globals->taTableID == (uint16)-1 || globals->taTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Time Attack DB");

        TimeAttackData->saveEntityPtr = SceneInfo->entity;
        TimeAttackData->saveCallback  = callback;

        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
}

void TimeAttackData_SaveTimeAttackDB_CB(int32 statusCode)
{
    if (TimeAttackData->saveCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->saveEntityPtr)
            SceneInfo->entity = TimeAttackData->saveEntityPtr;
        TimeAttackData->saveCallback(statusCode == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->saveCallback  = NULL;
        TimeAttackData->saveEntityPtr = NULL;
    }
}

int32 TimeAttackData_GetScore(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank)
{
    if (rank > 3 && rank)
        return 0;

    uint8 rankID = rank - 1;
    if (!TimeAttackData->loaded || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
        || encore != TimeAttackData->encore) {
        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 rowID = API.GetSortedUserDBRowID(globals->taTableID, rankID);
    if (rowID == -1)
        return 0;

    int32 score = 0;
    API.GetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "score", &score);

    return score;
}

int32 TimeAttackData_GetReplayID(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank)
{
    if (rank > 3 && rank)
        return 0;

    uint8 rankID = rank - 1;
    if (!TimeAttackData->loaded || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
        || encore != TimeAttackData->encore) {
        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 row = API.GetSortedUserDBRowID(globals->taTableID, rankID);
    if (row != -1) {
        int32 replayID = 0;
        API.GetUserDBValue(globals->taTableID, row, DBVAR_UINT32, "replayID", &replayID);

        return replayID;
    }

    return 0;
}

void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore)
{
    LogHelpers_Print("ConfigureTableView(%d, %d, %d, %d)", characterID, zoneID, act, encore);

    // setup every sort row ID for every entry
    API.SetupUserDBRowSorting(globals->taTableID);

    // remove any sort row IDs that dont match the following values
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "zoneID", &zoneID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "act", &act);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "characterID", &characterID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "encore", &encore);

    // sort the remaining rows
    API.SortDBRows(globals->taTableID, DBVAR_UINT32, "score", false);

    TimeAttackData->loaded      = true;
    TimeAttackData->zoneID      = zoneID;
    TimeAttackData->act         = act;
    TimeAttackData->characterID = characterID;
    TimeAttackData->encore      = encore & 1;
}

void TimeAttackData_GetLeaderboardRank_CB(bool32 success, int32 rank)
{
    if (success) {
        LogHelpers_Print("Got back leaderboard rank: %d. Not bad!", rank);
        TimeAttackData->leaderboardRank = rank;
    }
}

void TimeAttackData_AddLeaderboardEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore, int32 score)
{
    StatInfo stat;
    TimeAttackData_TrackTAClear(&stat, zoneID, act, characterID, isEncore, score);
    API.TryTrackStat(&stat);

    LeaderboardID *leaderboardInfo = TimeAttackData_GetLeaderboardInfo(zoneID, act, characterID, isEncore);

    API.TrackScore(leaderboardInfo, score, TimeAttackData_GetLeaderboardRank_CB);
}

LeaderboardID *TimeAttackData_GetLeaderboardInfo(uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore)
{
    LeaderboardID *info = NULL;
    if (zoneID < 12 && act < 2 && characterID <= 5) {
        int32 id = 10 * zoneID + 5 * act + (characterID - 1);
        if (isEncore)
            id += 120;

        info = &leaderboardInfo[id];
    }

    return info;
}

#else
void TimeAttackData_SaveTATime(uint8 zone, uint8 act, uint8 characterID, uint8 rank, uint16 score)
{
    rank--;
    if (rank < 3) {
        // playerID * (12 zones)
        // zone * (2 acts)
        // act * (3 ranks)
        uint16 *record = TimeAttackData_GetRecordedTime(zone, act, characterID, 1);

        for (int32 r = 2; r > rank; --r) record[r] = record[r - 1];

        record[rank] = score;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void TimeAttackData_EditorDraw(void) {}

void TimeAttackData_EditorLoad(void) {}
#endif

void TimeAttackData_Serialize(void) {}
