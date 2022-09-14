// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackData Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTimeAttackData *TimeAttackData;

#if MANIA_USE_PLUS
static const char *playerNames[] = { "Invalid", "Sonic", "Tails", "Knuckles", "Mighty", "Ray" };
static const char *actNames[]    = { "Act 1", "Act 2", "Act 3", "" };
static const char *zoneNames[]   = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ", "ERZ", "AIZ" };
static const char *modeNames[]   = { "Mania", "Encore" };

// clang-format off
static LeaderboardID leaderboardInfo[] = {
    { .idPS4 = 0,   .idUnknown = 1,   .idSwitch = 71,  .idXbox = "PlusGHZ1SonicMania",      .idPC = "GHZ1_S_MANIA" },
    { .idPS4 = 1,   .idUnknown = 2,   .idSwitch = 72,  .idXbox = "PlusGHZ1TailsMania",      .idPC = "GHZ1_T_MANIA" },
    { .idPS4 = 2,   .idUnknown = 3,   .idSwitch = 73,  .idXbox = "PlusGHZ1KnucklesMania",   .idPC = "GHZ1_K_MANIA" },
    { .idPS4 = 3,   .idUnknown = 4,   .idSwitch = 74,  .idXbox = "PlusGHZ1MightyMania",     .idPC = "GHZ1_M_MANIA" },
    { .idPS4 = 4,   .idUnknown = 5,   .idSwitch = 75,  .idXbox = "PlusGHZ1RayMania",        .idPC = "GHZ1_R_MANIA" },
    { .idPS4 = 5,   .idUnknown = 6,   .idSwitch = 76,  .idXbox = "PlusGHZ2SonicMania",      .idPC = "GHZ2_S_MANIA" },
    { .idPS4 = 6,   .idUnknown = 7,   .idSwitch = 77,  .idXbox = "PlusGHZ2TailsMania",      .idPC = "GHZ2_T_MANIA" },
    { .idPS4 = 7,   .idUnknown = 8,   .idSwitch = 78,  .idXbox = "PlusGHZ2KnucklesMania",   .idPC = "GHZ2_K_MANIA" },
    { .idPS4 = 8,   .idUnknown = 9,   .idSwitch = 79,  .idXbox = "PlusGHZ2MightyMania",     .idPC = "GHZ2_M_MANIA" },
    { .idPS4 = 9,   .idUnknown = 10,  .idSwitch = 80,  .idXbox = "PlusGHZ2RayMania",        .idPC = "GHZ2_R_MANIA" },
    { .idPS4 = 10,  .idUnknown = 11,  .idSwitch = 81,  .idXbox = "PlusCPZ1SonicMania",      .idPC = "CPZ1_S_MANIA" },
    { .idPS4 = 11,  .idUnknown = 12,  .idSwitch = 82,  .idXbox = "PlusCPZ1TailsMania",      .idPC = "CPZ1_T_MANIA" },
    { .idPS4 = 12,  .idUnknown = 13,  .idSwitch = 83,  .idXbox = "PlusCPZ1KnucklesMania",   .idPC = "CPZ1_K_MANIA" },
    { .idPS4 = 13,  .idUnknown = 14,  .idSwitch = 84,  .idXbox = "PlusCPZ1MightyMania",     .idPC = "CPZ1_M_MANIA" },
    { .idPS4 = 14,  .idUnknown = 15,  .idSwitch = 85,  .idXbox = "PlusCPZ1RayMania",        .idPC = "CPZ1_R_MANIA" },
    { .idPS4 = 15,  .idUnknown = 16,  .idSwitch = 86,  .idXbox = "PlusCPZ2SonicMania",      .idPC = "CPZ2_S_MANIA" },
    { .idPS4 = 16,  .idUnknown = 17,  .idSwitch = 87,  .idXbox = "PlusCPZ2TailsMania",      .idPC = "CPZ2_T_MANIA" },
    { .idPS4 = 17,  .idUnknown = 18,  .idSwitch = 88,  .idXbox = "PlusCPZ2KnucklesMania",   .idPC = "CPZ2_K_MANIA" },
    { .idPS4 = 18,  .idUnknown = 19,  .idSwitch = 89,  .idXbox = "PlusCPZ2MightyMania",     .idPC = "CPZ2_M_MANIA" },
    { .idPS4 = 19,  .idUnknown = 20,  .idSwitch = 90,  .idXbox = "PlusCPZ2RayMania",        .idPC = "CPZ2_R_MANIA" },
    { .idPS4 = 20,  .idUnknown = 21,  .idSwitch = 91,  .idXbox = "PlusSPZ1SonicMania",      .idPC = "SPZ1_S_MANIA" },
    { .idPS4 = 21,  .idUnknown = 22,  .idSwitch = 92,  .idXbox = "PlusSPZ1TailsMania",      .idPC = "SPZ1_T_MANIA" },
    { .idPS4 = 22,  .idUnknown = 23,  .idSwitch = 93,  .idXbox = "PlusSPZ1KnucklesMania",   .idPC = "SPZ1_K_MANIA" },
    { .idPS4 = 23,  .idUnknown = 24,  .idSwitch = 94,  .idXbox = "PlusSPZ1MightyMania",     .idPC = "SPZ1_M_MANIA" },
    { .idPS4 = 24,  .idUnknown = 25,  .idSwitch = 95,  .idXbox = "PlusSPZ1RayMania",        .idPC = "SPZ1_R_MANIA" },
    { .idPS4 = 25,  .idUnknown = 26,  .idSwitch = 96,  .idXbox = "PlusSPZ2SonicMania",      .idPC = "SPZ2_S_MANIA" },
    { .idPS4 = 26,  .idUnknown = 27,  .idSwitch = 97,  .idXbox = "PlusSPZ2TailsMania",      .idPC = "SPZ2_T_MANIA" },
    { .idPS4 = 27,  .idUnknown = 28,  .idSwitch = 98,  .idXbox = "PlusSPZ2KnucklesMania",   .idPC = "SPZ2_K_MANIA" },
    { .idPS4 = 28,  .idUnknown = 29,  .idSwitch = 99,  .idXbox = "PlusSPZ2MightyMania",     .idPC = "SPZ2_M_MANIA" },
    { .idPS4 = 29,  .idUnknown = 30,  .idSwitch = 100, .idXbox = "PlusSPZ2RayMania",        .idPC = "SPZ2_R_MANIA" },
    { .idPS4 = 30,  .idUnknown = 31,  .idSwitch = 101, .idXbox = "PlusFBZ1SonicMania",      .idPC = "FBZ1_S_MANIA" },
    { .idPS4 = 31,  .idUnknown = 32,  .idSwitch = 102, .idXbox = "PlusFBZ1TailsMania",      .idPC = "FBZ1_T_MANIA" },
    { .idPS4 = 32,  .idUnknown = 33,  .idSwitch = 103, .idXbox = "PlusFBZ1KnucklesMania",   .idPC = "FBZ1_K_MANIA" },
    { .idPS4 = 33,  .idUnknown = 34,  .idSwitch = 104, .idXbox = "PlusFBZ1MightyMania",     .idPC = "FBZ1_M_MANIA" },
    { .idPS4 = 34,  .idUnknown = 35,  .idSwitch = 105, .idXbox = "PlusFBZ1RayMania",        .idPC = "FBZ1_R_MANIA" },
    { .idPS4 = 35,  .idUnknown = 36,  .idSwitch = 106, .idXbox = "PlusFBZ2SonicMania",      .idPC = "FBZ2_S_MANIA" },
    { .idPS4 = 36,  .idUnknown = 37,  .idSwitch = 107, .idXbox = "PlusFBZ2TailsMania",      .idPC = "FBZ2_T_MANIA" },
    { .idPS4 = 37,  .idUnknown = 38,  .idSwitch = 108, .idXbox = "PlusFBZ2KnucklesMania",   .idPC = "FBZ2_K_MANIA" },
    { .idPS4 = 38,  .idUnknown = 39,  .idSwitch = 109, .idXbox = "PlusFBZ2MightyMania",     .idPC = "FBZ2_M_MANIA" },
    { .idPS4 = 39,  .idUnknown = 40,  .idSwitch = 110, .idXbox = "PlusFBZ2RayMania",        .idPC = "FBZ2_R_MANIA" },
    { .idPS4 = 40,  .idUnknown = 41,  .idSwitch = 111, .idXbox = "PlusPGZ1SonicMania",      .idPC = "PGZ1_S_MANIA" },
    { .idPS4 = 41,  .idUnknown = 42,  .idSwitch = 112, .idXbox = "PlusPGZ1TailsMania",      .idPC = "PGZ1_T_MANIA" },
    { .idPS4 = 42,  .idUnknown = 43,  .idSwitch = 113, .idXbox = "PlusPGZ1KnucklesMania",   .idPC = "PGZ1_K_MANIA" },
    { .idPS4 = 43,  .idUnknown = 44,  .idSwitch = 114, .idXbox = "PlusPGZ1MightyMania",     .idPC = "PGZ1_M_MANIA" },
    { .idPS4 = 44,  .idUnknown = 45,  .idSwitch = 115, .idXbox = "PlusPGZ1RayMania",        .idPC = "PGZ1_R_MANIA" },
    { .idPS4 = 45,  .idUnknown = 46,  .idSwitch = 116, .idXbox = "PlusPGZ2SonicMania",      .idPC = "PGZ2_S_MANIA" },
    { .idPS4 = 46,  .idUnknown = 47,  .idSwitch = 117, .idXbox = "PlusPGZ2TailsMania",      .idPC = "PGZ2_T_MANIA" },
    { .idPS4 = 47,  .idUnknown = 48,  .idSwitch = 118, .idXbox = "PlusPGZ2KnucklesMania",   .idPC = "PGZ2_K_MANIA" },
    { .idPS4 = 48,  .idUnknown = 49,  .idSwitch = 119, .idXbox = "PlusPGZ2MightyMania",     .idPC = "PGZ2_M_MANIA" },
    { .idPS4 = 49,  .idUnknown = 50,  .idSwitch = 120, .idXbox = "PlusPGZ2RayMania",        .idPC = "PGZ2_R_MANIA" },
    { .idPS4 = 50,  .idUnknown = 51,  .idSwitch = 121, .idXbox = "PlusSSZ1SonicMania",      .idPC = "SSZ1_S_MANIA" },
    { .idPS4 = 51,  .idUnknown = 52,  .idSwitch = 122, .idXbox = "PlusSSZ1TailsMania",      .idPC = "SSZ1_T_MANIA" },
    { .idPS4 = 52,  .idUnknown = 53,  .idSwitch = 123, .idXbox = "PlusSSZ1KnucklesMania",   .idPC = "SSZ1_K_MANIA" },
    { .idPS4 = 53,  .idUnknown = 54,  .idSwitch = 124, .idXbox = "PlusSSZ1MightyMania",     .idPC = "SSZ1_M_MANIA" },
    { .idPS4 = 54,  .idUnknown = 55,  .idSwitch = 125, .idXbox = "PlusSSZ1RayMania",        .idPC = "SSZ1_R_MANIA" },
    { .idPS4 = 55,  .idUnknown = 56,  .idSwitch = 126, .idXbox = "PlusSSZ2SonicMania",      .idPC = "SSZ2_S_MANIA" },
    { .idPS4 = 56,  .idUnknown = 57,  .idSwitch = 127, .idXbox = "PlusSSZ2TailsMania",      .idPC = "SSZ2_T_MANIA" },
    { .idPS4 = 57,  .idUnknown = 58,  .idSwitch = 128, .idXbox = "PlusSSZ2KnucklesMania",   .idPC = "SSZ2_K_MANIA" },
    { .idPS4 = 58,  .idUnknown = 59,  .idSwitch = 129, .idXbox = "PlusSSZ2MightyMania",     .idPC = "SSZ2_M_MANIA" },
    { .idPS4 = 59,  .idUnknown = 60,  .idSwitch = 130, .idXbox = "PlusSSZ2RayMania",        .idPC = "SSZ2_R_MANIA" },
    { .idPS4 = 60,  .idUnknown = 61,  .idSwitch = 131, .idXbox = "PlusHCZ1SonicMania",      .idPC = "HCZ1_S_MANIA" },
    { .idPS4 = 61,  .idUnknown = 62,  .idSwitch = 132, .idXbox = "PlusHCZ1TailsMania",      .idPC = "HCZ1_T_MANIA" },
    { .idPS4 = 62,  .idUnknown = 63,  .idSwitch = 133, .idXbox = "PlusHCZ1KnucklesMania",   .idPC = "HCZ1_K_MANIA" },
    { .idPS4 = 63,  .idUnknown = 64,  .idSwitch = 134, .idXbox = "PlusHCZ1MightyMania",     .idPC = "HCZ1_M_MANIA" },
    { .idPS4 = 64,  .idUnknown = 65,  .idSwitch = 135, .idXbox = "PlusHCZ1RayMania",        .idPC = "HCZ1_R_MANIA" },
    { .idPS4 = 65,  .idUnknown = 66,  .idSwitch = 136, .idXbox = "PlusHCZ2SonicMania",      .idPC = "HCZ2_S_MANIA" },
    { .idPS4 = 66,  .idUnknown = 67,  .idSwitch = 137, .idXbox = "PlusHCZ2TailsMania",      .idPC = "HCZ2_T_MANIA" },
    { .idPS4 = 67,  .idUnknown = 68,  .idSwitch = 138, .idXbox = "PlusHCZ2KnucklesMania",   .idPC = "HCZ2_K_MANIA" },
    { .idPS4 = 68,  .idUnknown = 69,  .idSwitch = 139, .idXbox = "PlusHCZ2MightyMania",     .idPC = "HCZ2_M_MANIA" },
    { .idPS4 = 69,  .idUnknown = 70,  .idSwitch = 140, .idXbox = "PlusHCZ2RayMania",        .idPC = "HCZ2_R_MANIA" },
    { .idPS4 = -1,  .idUnknown = -1,  .idSwitch = -1,  .idXbox = NULL,                      .idPC = NULL },
    { .idPS4 = -1,  .idUnknown = -1,  .idSwitch = -1,  .idXbox = NULL,                      .idPC = NULL },
    { .idPS4 = 72,  .idUnknown = 71,  .idSwitch = 141, .idXbox = "PlusMSZ1KnucklesMania",   .idPC = "MSZ1_K_MANIA" },
    { .idPS4 = -1,  .idUnknown = -1,  .idSwitch = -1,  .idXbox = NULL,                      .idPC = NULL },
    { .idPS4 = -1,  .idUnknown = -1,  .idSwitch = -1,  .idXbox = NULL,                      .idPC = NULL },
    { .idPS4 = 75,  .idUnknown = 72,  .idSwitch = 142, .idXbox = "PlusMSZ2SonicMania",      .idPC = "MSZ2_S_MANIA" },
    { .idPS4 = 76,  .idUnknown = 73,  .idSwitch = 143, .idXbox = "PlusMSZ2TailsMania",      .idPC = "MSZ2_T_MANIA" },
    { .idPS4 = 77,  .idUnknown = 74,  .idSwitch = 144, .idXbox = "PlusMSZ2KnucklesMania",   .idPC = "MSZ2_K_MANIA" },
    { .idPS4 = 78,  .idUnknown = 75,  .idSwitch = 145, .idXbox = "PlusMSZ2MightyMania",     .idPC = "MSZ2_M_MANIA" },
    { .idPS4 = 79,  .idUnknown = 76,  .idSwitch = 146, .idXbox = "PlusMSZ2RayMania",        .idPC = "MSZ2_R_MANIA" },
    { .idPS4 = 80,  .idUnknown = 77,  .idSwitch = 147, .idXbox = "PlusOOZ1SonicMania",      .idPC = "OOZ1_S_MANIA" },
    { .idPS4 = 81,  .idUnknown = 78,  .idSwitch = 148, .idXbox = "PlusOOZ1TailsMania",      .idPC = "OOZ1_T_MANIA" },
    { .idPS4 = 82,  .idUnknown = 79,  .idSwitch = 149, .idXbox = "PlusOOZ1KnucklesMania",   .idPC = "OOZ1_K_MANIA" },
    { .idPS4 = 83,  .idUnknown = 80,  .idSwitch = 150, .idXbox = "PlusOOZ1MightyMania",     .idPC = "OOZ1_M_MANIA" },
    { .idPS4 = 84,  .idUnknown = 81,  .idSwitch = 151, .idXbox = "PlusOOZ1RayMania",        .idPC = "OOZ1_R_MANIA" },
    { .idPS4 = 85,  .idUnknown = 82,  .idSwitch = 152, .idXbox = "PlusOOZ2SonicMania",      .idPC = "OOZ2_S_MANIA" },
    { .idPS4 = 86,  .idUnknown = 83,  .idSwitch = 153, .idXbox = "PlusOOZ2TailsMania",      .idPC = "OOZ2_T_MANIA" },
    { .idPS4 = 87,  .idUnknown = 84,  .idSwitch = 154, .idXbox = "PlusOOZ2KnucklesMania",   .idPC = "OOZ2_K_MANIA" },
    { .idPS4 = 88,  .idUnknown = 85,  .idSwitch = 155, .idXbox = "PlusOOZ2MightyMania",     .idPC = "OOZ2_M_MANIA" },
    { .idPS4 = 89,  .idUnknown = 86,  .idSwitch = 156, .idXbox = "PlusOOZ2RayMania",        .idPC = "OOZ2_R_MANIA" },
    { .idPS4 = 90,  .idUnknown = 87,  .idSwitch = 157, .idXbox = "PlusLRZ1SonicMania",      .idPC = "LRZ1_S_MANIA" },
    { .idPS4 = 91,  .idUnknown = 88,  .idSwitch = 158, .idXbox = "PlusLRZ1TailsMania",      .idPC = "LRZ1_T_MANIA" },
    { .idPS4 = 92,  .idUnknown = 89,  .idSwitch = 159, .idXbox = "PlusLRZ1KnucklesMania",   .idPC = "LRZ1_K_MANIA" },
    { .idPS4 = 93,  .idUnknown = 90,  .idSwitch = 160, .idXbox = "PlusLRZ1MightyMania",     .idPC = "LRZ1_M_MANIA" },
    { .idPS4 = 94,  .idUnknown = 91,  .idSwitch = 161, .idXbox = "PlusLRZ1RayMania",        .idPC = "LRZ1_R_MANIA" },
    { .idPS4 = 95,  .idUnknown = 92,  .idSwitch = 162, .idXbox = "PlusLRZ2SonicMania",      .idPC = "LRZ2_S_MANIA" },
    { .idPS4 = 96,  .idUnknown = 93,  .idSwitch = 163, .idXbox = "PlusLRZ2TailsMania",      .idPC = "LRZ2_T_MANIA" },
    { .idPS4 = 97,  .idUnknown = 94,  .idSwitch = 164, .idXbox = "PlusLRZ2KnucklesMania",   .idPC = "LRZ2_K_MANIA" },
    { .idPS4 = 98,  .idUnknown = 95,  .idSwitch = 165, .idXbox = "PlusLRZ2MightyMania",     .idPC = "LRZ2_M_MANIA" },
    { .idPS4 = 99,  .idUnknown = 96,  .idSwitch = 166, .idXbox = "PlusLRZ2RayMania",        .idPC = "LRZ2_R_MANIA" },
    { .idPS4 = 100, .idUnknown = 97,  .idSwitch = 167, .idXbox = "PlusMMZ1SonicMania",      .idPC = "MMZ1_S_MANIA" },
    { .idPS4 = 101, .idUnknown = 98,  .idSwitch = 168, .idXbox = "PlusMMZ1TailsMania",      .idPC = "MMZ1_T_MANIA" },
    { .idPS4 = 102, .idUnknown = 99,  .idSwitch = 169, .idXbox = "PlusMMZ1KnucklesMania",   .idPC = "MMZ1_K_MANIA" },
    { .idPS4 = 103, .idUnknown = 100, .idSwitch = 170, .idXbox = "PlusMMZ1MightyMania",     .idPC = "MMZ1_M_MANIA" },
    { .idPS4 = 104, .idUnknown = 101, .idSwitch = 171, .idXbox = "PlusMMZ1RayMania",        .idPC = "MMZ1_R_MANIA" },
    { .idPS4 = 105, .idUnknown = 102, .idSwitch = 172, .idXbox = "PlusMMZ2SonicMania",      .idPC = "MMZ2_S_MANIA" },
    { .idPS4 = 106, .idUnknown = 103, .idSwitch = 173, .idXbox = "PlusMMZ2TailsMania",      .idPC = "MMZ2_T_MANIA" },
    { .idPS4 = 107, .idUnknown = 104, .idSwitch = 174, .idXbox = "PlusMMZ2KnucklesMania",   .idPC = "MMZ2_K_MANIA" },
    { .idPS4 = 108, .idUnknown = 105, .idSwitch = 175, .idXbox = "PlusMMZ2MightyMania",     .idPC = "MMZ2_M_MANIA" },
    { .idPS4 = 109, .idUnknown = 106, .idSwitch = 176, .idXbox = "PlusMMZ2RayMania",        .idPC = "MMZ2_R_MANIA" },
    { .idPS4 = 110, .idUnknown = 107, .idSwitch = 177, .idXbox = "PlusTMZ1SonicMania",      .idPC = "TMZ1_S_MANIA" },
    { .idPS4 = 111, .idUnknown = 108, .idSwitch = 178, .idXbox = "PlusTMZ1TailsMania",      .idPC = "TMZ1_T_MANIA" },
    { .idPS4 = 112, .idUnknown = 109, .idSwitch = 179, .idXbox = "PlusTMZ1KnucklesMania",   .idPC = "TMZ1_K_MANIA" },
    { .idPS4 = 113, .idUnknown = 110, .idSwitch = 180, .idXbox = "PlusTMZ1MightyMania",     .idPC = "TMZ1_M_MANIA" },
    { .idPS4 = 114, .idUnknown = 111, .idSwitch = 181, .idXbox = "PlusTMZ1RayMania",        .idPC = "TMZ1_R_MANIA" },
    { .idPS4 = 115, .idUnknown = 112, .idSwitch = 182, .idXbox = "PlusTMZ2SonicMania",      .idPC = "TMZ2_S_MANIA" },
    { .idPS4 = 116, .idUnknown = 113, .idSwitch = 183, .idXbox = "PlusTMZ2TailsMania",      .idPC = "TMZ2_T_MANIA" },
    { .idPS4 = 117, .idUnknown = 114, .idSwitch = 184, .idXbox = "PlusTMZ2KnucklesMania",   .idPC = "TMZ2_K_MANIA" },
    { .idPS4 = 118, .idUnknown = 115, .idSwitch = 185, .idXbox = "PlusTMZ2MightyMania",     .idPC = "TMZ2_M_MANIA" },
    { .idPS4 = 119, .idUnknown = 116, .idSwitch = 186, .idXbox = "PlusTMZ2RayMania",        .idPC = "TMZ2_R_MANIA" },

    { .idPS4 = 120, .idUnknown = 117, .idSwitch = 187, .idXbox = "PlusGHZ1SonicEncore",     .idPC = "GHZ1_S_ENCORE" },
    { .idPS4 = 121, .idUnknown = 118, .idSwitch = 188, .idXbox = "PlusGHZ1TailsEncore",     .idPC = "GHZ1_T_ENCORE" },
    { .idPS4 = 122, .idUnknown = 119, .idSwitch = 189, .idXbox = "PlusGHZ1KnucklesEncore",  .idPC = "GHZ1_K_ENCORE" },
    { .idPS4 = 123, .idUnknown = 120, .idSwitch = 190, .idXbox = "PlusGHZ1MightyEncore",    .idPC = "GHZ1_M_ENCORE" },
    { .idPS4 = 124, .idUnknown = 121, .idSwitch = 191, .idXbox = "PlusGHZ1RayEncore",       .idPC = "GHZ1_R_ENCORE" },
    { .idPS4 = 125, .idUnknown = 122, .idSwitch = 192, .idXbox = "PlusGHZ2SonicEncore",     .idPC = "GHZ2_S_ENCORE" },
    { .idPS4 = 126, .idUnknown = 123, .idSwitch = 193, .idXbox = "PlusGHZ2TailsEncore",     .idPC = "GHZ2_T_ENCORE" },
    { .idPS4 = 127, .idUnknown = 124, .idSwitch = 194, .idXbox = "PlusGHZ2KnucklesEncore",  .idPC = "GHZ2_K_ENCORE" },
    { .idPS4 = 128, .idUnknown = 125, .idSwitch = 195, .idXbox = "PlusGHZ2MightyEncore",    .idPC = "GHZ2_M_ENCORE" },
    { .idPS4 = 129, .idUnknown = 126, .idSwitch = 196, .idXbox = "PlusGHZ2RayEncore",       .idPC = "GHZ2_R_ENCORE" },
    { .idPS4 = 130, .idUnknown = 127, .idSwitch = 197, .idXbox = "PlusCPZ1SonicEncore",     .idPC = "CPZ1_S_ENCORE" },
    { .idPS4 = 131, .idUnknown = 128, .idSwitch = 198, .idXbox = "PlusCPZ1TailsEncore",     .idPC = "CPZ1_T_ENCORE" },
    { .idPS4 = 132, .idUnknown = 129, .idSwitch = 199, .idXbox = "PlusCPZ1KnucklesEncore",  .idPC = "CPZ1_K_ENCORE" },
    { .idPS4 = 133, .idUnknown = 130, .idSwitch = 200, .idXbox = "PlusCPZ1MightyEncore",    .idPC = "CPZ1_M_ENCORE" },
    { .idPS4 = 134, .idUnknown = 131, .idSwitch = 201, .idXbox = "PlusCPZ1RayEncore",       .idPC = "CPZ1_R_ENCORE" },
    { .idPS4 = 135, .idUnknown = 132, .idSwitch = 202, .idXbox = "PlusCPZ2SonicEncore",     .idPC = "CPZ2_S_ENCORE" },
    { .idPS4 = 136, .idUnknown = 133, .idSwitch = 203, .idXbox = "PlusCPZ2TailsEncore",     .idPC = "CPZ2_T_ENCORE" },
    { .idPS4 = 137, .idUnknown = 134, .idSwitch = 204, .idXbox = "PlusCPZ2KnucklesEncore",  .idPC = "CPZ2_K_ENCORE" },
    { .idPS4 = 138, .idUnknown = 135, .idSwitch = 205, .idXbox = "PlusCPZ2MightyEncore",    .idPC = "CPZ2_M_ENCORE" },
    { .idPS4 = 139, .idUnknown = 136, .idSwitch = 206, .idXbox = "PlusCPZ2RayEncore",       .idPC = "CPZ2_R_ENCORE" },
    { .idPS4 = 140, .idUnknown = 137, .idSwitch = 207, .idXbox = "PlusSPZ1SonicEncore",     .idPC = "SPZ1_S_ENCORE" },
    { .idPS4 = 141, .idUnknown = 138, .idSwitch = 208, .idXbox = "PlusSPZ1TailsEncore",     .idPC = "SPZ1_T_ENCORE" },
    { .idPS4 = 142, .idUnknown = 139, .idSwitch = 209, .idXbox = "PlusSPZ1KnucklesEncore",  .idPC = "SPZ1_K_ENCORE" },
    { .idPS4 = 143, .idUnknown = 140, .idSwitch = 210, .idXbox = "PlusSPZ1MightyEncore",    .idPC = "SPZ1_M_ENCORE" },
    { .idPS4 = 144, .idUnknown = 141, .idSwitch = 211, .idXbox = "PlusSPZ1RayEncore",       .idPC = "SPZ1_R_ENCORE" },
    { .idPS4 = 145, .idUnknown = 142, .idSwitch = 212, .idXbox = "PlusSPZ2SonicEncore",     .idPC = "SPZ2_S_ENCORE" },
    { .idPS4 = 146, .idUnknown = 143, .idSwitch = 213, .idXbox = "PlusSPZ2TailsEncore",     .idPC = "SPZ2_T_ENCORE" },
    { .idPS4 = 147, .idUnknown = 144, .idSwitch = 214, .idXbox = "PlusSPZ2KnucklesEncore",  .idPC = "SPZ2_K_ENCORE" },
    { .idPS4 = 148, .idUnknown = 145, .idSwitch = 215, .idXbox = "PlusSPZ2MightyEncore",    .idPC = "SPZ2_M_ENCORE" },
    { .idPS4 = 149, .idUnknown = 146, .idSwitch = 216, .idXbox = "PlusSPZ2RayEncore",       .idPC = "SPZ2_R_ENCORE" },
    { .idPS4 = 150, .idUnknown = 147, .idSwitch = 217, .idXbox = "PlusFBZ1SonicEncore",     .idPC = "FBZ1_S_ENCORE" },
    { .idPS4 = 151, .idUnknown = 148, .idSwitch = 218, .idXbox = "PlusFBZ1TailsEncore",     .idPC = "FBZ1_T_ENCORE" },
    { .idPS4 = 152, .idUnknown = 149, .idSwitch = 219, .idXbox = "PlusFBZ1KnucklesEncore",  .idPC = "FBZ1_K_ENCORE" },
    { .idPS4 = 153, .idUnknown = 150, .idSwitch = 220, .idXbox = "PlusFBZ1MightyEncore",    .idPC = "FBZ1_M_ENCORE" },
    { .idPS4 = 154, .idUnknown = 151, .idSwitch = 221, .idXbox = "PlusFBZ1RayEncore",       .idPC = "FBZ1_R_ENCORE" },
    { .idPS4 = 155, .idUnknown = 152, .idSwitch = 222, .idXbox = "PlusFBZ2SonicEncore",     .idPC = "FBZ2_S_ENCORE" },
    { .idPS4 = 156, .idUnknown = 153, .idSwitch = 223, .idXbox = "PlusFBZ2TailsEncore",     .idPC = "FBZ2_T_ENCORE" },
    { .idPS4 = 157, .idUnknown = 154, .idSwitch = 224, .idXbox = "PlusFBZ2KnucklesEncore",  .idPC = "FBZ2_K_ENCORE" },
    { .idPS4 = 158, .idUnknown = 155, .idSwitch = 225, .idXbox = "PlusFBZ2MightyEncore",    .idPC = "FBZ2_M_ENCORE" },
    { .idPS4 = 159, .idUnknown = 156, .idSwitch = 226, .idXbox = "PlusFBZ2RayEncore",       .idPC = "FBZ2_R_ENCORE" },
    { .idPS4 = 160, .idUnknown = 157, .idSwitch = 227, .idXbox = "PlusPGZ1SonicEncore",     .idPC = "PGZ1_S_ENCORE" },
    { .idPS4 = 161, .idUnknown = 158, .idSwitch = 228, .idXbox = "PlusPGZ1TailsEncore",     .idPC = "PGZ1_T_ENCORE" },
    { .idPS4 = 162, .idUnknown = 159, .idSwitch = 229, .idXbox = "PlusPGZ1KnucklesEncore",  .idPC = "PGZ1_K_ENCORE" },
    { .idPS4 = 163, .idUnknown = 160, .idSwitch = 230, .idXbox = "PlusPGZ1MightyEncore",    .idPC = "PGZ1_M_ENCORE" },
    { .idPS4 = 164, .idUnknown = 161, .idSwitch = 231, .idXbox = "PlusPGZ1RayEncore",       .idPC = "PGZ1_R_ENCORE" },
    { .idPS4 = 165, .idUnknown = 162, .idSwitch = 232, .idXbox = "PlusPGZ2SonicEncore",     .idPC = "PGZ2_S_ENCORE" },
    { .idPS4 = 166, .idUnknown = 163, .idSwitch = 233, .idXbox = "PlusPGZ2TailsEncore",     .idPC = "PGZ2_T_ENCORE" },
    { .idPS4 = 167, .idUnknown = 164, .idSwitch = 234, .idXbox = "PlusPGZ2KnucklesEncore",  .idPC = "PGZ2_K_ENCORE" },
    { .idPS4 = 168, .idUnknown = 165, .idSwitch = 235, .idXbox = "PlusPGZ2MightyEncore",    .idPC = "PGZ2_M_ENCORE" },
    { .idPS4 = 169, .idUnknown = 166, .idSwitch = 236, .idXbox = "PlusPGZ2RayEncore",       .idPC = "PGZ2_R_ENCORE" },
    { .idPS4 = 170, .idUnknown = 167, .idSwitch = 237, .idXbox = "PlusSSZ1SonicEncore",     .idPC = "SSZ1_S_ENCORE" },
    { .idPS4 = 171, .idUnknown = 168, .idSwitch = 238, .idXbox = "PlusSSZ1TailsEncore",     .idPC = "SSZ1_T_ENCORE" },
    { .idPS4 = 172, .idUnknown = 169, .idSwitch = 239, .idXbox = "PlusSSZ1KnucklesEncore",  .idPC = "SSZ1_K_ENCORE" },
    { .idPS4 = 173, .idUnknown = 170, .idSwitch = 240, .idXbox = "PlusSSZ1MightyEncore",    .idPC = "SSZ1_M_ENCORE" },
    { .idPS4 = 174, .idUnknown = 171, .idSwitch = 241, .idXbox = "PlusSSZ1RayEncore",       .idPC = "SSZ1_R_ENCORE" },
    { .idPS4 = 175, .idUnknown = 172, .idSwitch = 242, .idXbox = "PlusSSZ2SonicEncore",     .idPC = "SSZ2_S_ENCORE" },
    { .idPS4 = 176, .idUnknown = 173, .idSwitch = 243, .idXbox = "PlusSSZ2TailsEncore",     .idPC = "SSZ2_T_ENCORE" },
    { .idPS4 = 177, .idUnknown = 174, .idSwitch = 244, .idXbox = "PlusSSZ2KnucklesEncore",  .idPC = "SSZ2_K_ENCORE" },
    { .idPS4 = 178, .idUnknown = 175, .idSwitch = 245, .idXbox = "PlusSSZ2MightyEncore",    .idPC = "SSZ2_M_ENCORE" },
    { .idPS4 = 179, .idUnknown = 176, .idSwitch = 246, .idXbox = "PlusSSZ2RayEncore",       .idPC = "SSZ2_R_ENCORE" },
    { .idPS4 = 180, .idUnknown = 177, .idSwitch = 247, .idXbox = "PlusHCZ1SonicEncore",     .idPC = "HCZ1_S_ENCORE" },
    { .idPS4 = 181, .idUnknown = 178, .idSwitch = 248, .idXbox = "PlusHCZ1TailsEncore",     .idPC = "HCZ1_T_ENCORE" },
    { .idPS4 = 182, .idUnknown = 179, .idSwitch = 249, .idXbox = "PlusHCZ1KnucklesEncore",  .idPC = "HCZ1_K_ENCORE" },
    { .idPS4 = 183, .idUnknown = 180, .idSwitch = 250, .idXbox = "PlusHCZ1MightyEncore",    .idPC = "HCZ1_M_ENCORE" },
    { .idPS4 = 184, .idUnknown = 181, .idSwitch = 251, .idXbox = "PlusHCZ1RayEncore",       .idPC = "HCZ1_R_ENCORE" },
    { .idPS4 = 185, .idUnknown = 182, .idSwitch = 252, .idXbox = "PlusHCZ2SonicEncore",     .idPC = "HCZ2_S_ENCORE" },
    { .idPS4 = 186, .idUnknown = 183, .idSwitch = 253, .idXbox = "PlusHCZ2TailsEncore",     .idPC = "HCZ2_T_ENCORE" },
    { .idPS4 = 187, .idUnknown = 184, .idSwitch = 254, .idXbox = "PlusHCZ2KnucklesEncore",  .idPC = "HCZ2_K_ENCORE" },
    { .idPS4 = 188, .idUnknown = 185, .idSwitch = 255, .idXbox = "PlusHCZ2MightyEncore",    .idPC = "HCZ2_M_ENCORE" },
    { .idPS4 = 189, .idUnknown = 186, .idSwitch = 256, .idXbox = "PlusHCZ2RayEncore",       .idPC = "HCZ2_R_ENCORE" },
    { .idPS4 = 190, .idUnknown = 187, .idSwitch = 257, .idXbox = "PlusMSZ1SonicEncore",     .idPC = "MSZ1_S_ENCORE" },
    { .idPS4 = 191, .idUnknown = 188, .idSwitch = 258, .idXbox = "PlusMSZ1TailsEncore",     .idPC = "MSZ1_T_ENCORE" },
    { .idPS4 = 192, .idUnknown = 189, .idSwitch = 259, .idXbox = "PlusMSZ1KnucklesEncore",  .idPC = "MSZ1_K_ENCORE" },
    { .idPS4 = 193, .idUnknown = 190, .idSwitch = 260, .idXbox = "PlusMSZ1MightyEncore",    .idPC = "MSZ1_M_ENCORE" },
    { .idPS4 = 194, .idUnknown = 191, .idSwitch = 261, .idXbox = "PlusMSZ1RayEncore",       .idPC = "MSZ1_R_ENCORE" },
    { .idPS4 = 195, .idUnknown = 192, .idSwitch = 262, .idXbox = "PlusMSZ2SonicEncore",     .idPC = "MSZ2_S_ENCORE" },
    { .idPS4 = 196, .idUnknown = 193, .idSwitch = 263, .idXbox = "PlusMSZ2TailsEncore",     .idPC = "MSZ2_T_ENCORE" },
    { .idPS4 = 197, .idUnknown = 194, .idSwitch = 264, .idXbox = "PlusMSZ2KnucklesEncore",  .idPC = "MSZ2_K_ENCORE" },
    { .idPS4 = 198, .idUnknown = 195, .idSwitch = 265, .idXbox = "PlusMSZ2MightyEncore",    .idPC = "MSZ2_M_ENCORE" },
    { .idPS4 = 199, .idUnknown = 196, .idSwitch = 266, .idXbox = "PlusMSZ2RayEncore",       .idPC = "MSZ2_R_ENCORE" },
    { .idPS4 = 200, .idUnknown = 197, .idSwitch = 267, .idXbox = "PlusOOZ1SonicEncore",     .idPC = "OOZ1_S_ENCORE" },
    { .idPS4 = 201, .idUnknown = 198, .idSwitch = 268, .idXbox = "PlusOOZ1TailsEncore",     .idPC = "OOZ1_T_ENCORE" },
    { .idPS4 = 202, .idUnknown = 199, .idSwitch = 269, .idXbox = "PlusOOZ1KnucklesEncore",  .idPC = "OOZ1_K_ENCORE" },
    { .idPS4 = 203, .idUnknown = 200, .idSwitch = 270, .idXbox = "PlusOOZ1MightyEncore",    .idPC = "OOZ1_M_ENCORE" },
    { .idPS4 = 204, .idUnknown = 201, .idSwitch = 271, .idXbox = "PlusOOZ1RayEncore",       .idPC = "OOZ1_R_ENCORE" },
    { .idPS4 = 205, .idUnknown = 202, .idSwitch = 272, .idXbox = "PlusOOZ2SonicEncore",     .idPC = "OOZ2_S_ENCORE" },
    { .idPS4 = 206, .idUnknown = 203, .idSwitch = 273, .idXbox = "PlusOOZ2TailsEncore",     .idPC = "OOZ2_T_ENCORE" },
    { .idPS4 = 207, .idUnknown = 204, .idSwitch = 274, .idXbox = "PlusOOZ2KnucklesEncore",  .idPC = "OOZ2_K_ENCORE" },
    { .idPS4 = 208, .idUnknown = 205, .idSwitch = 275, .idXbox = "PlusOOZ2MightyEncore",    .idPC = "OOZ2_M_ENCORE" },
    { .idPS4 = 209, .idUnknown = 206, .idSwitch = 276, .idXbox = "PlusOOZ2RayEncore",       .idPC = "OOZ2_R_ENCORE" },
    { .idPS4 = 210, .idUnknown = 207, .idSwitch = 277, .idXbox = "PlusLRZ1SonicEncore",     .idPC = "LRZ1_S_ENCORE" },
    { .idPS4 = 211, .idUnknown = 208, .idSwitch = 278, .idXbox = "PlusLRZ1TailsEncore",     .idPC = "LRZ1_T_ENCORE" },
    { .idPS4 = 212, .idUnknown = 209, .idSwitch = 279, .idXbox = "PlusLRZ1KnucklesEncore",  .idPC = "LRZ1_K_ENCORE" },
    { .idPS4 = 213, .idUnknown = 210, .idSwitch = 280, .idXbox = "PlusLRZ1MightyEncore",    .idPC = "LRZ1_M_ENCORE" },
    { .idPS4 = 214, .idUnknown = 211, .idSwitch = 281, .idXbox = "PlusLRZ1RayEncore",       .idPC = "LRZ1_R_ENCORE" },
    { .idPS4 = 215, .idUnknown = 212, .idSwitch = 282, .idXbox = "PlusLRZ2SonicEncore",     .idPC = "LRZ2_S_ENCORE" },
    { .idPS4 = 216, .idUnknown = 213, .idSwitch = 283, .idXbox = "PlusLRZ2TailsEncore",     .idPC = "LRZ2_T_ENCORE" },
    { .idPS4 = 217, .idUnknown = 214, .idSwitch = 284, .idXbox = "PlusLRZ2KnucklesEncore",  .idPC = "LRZ2_K_ENCORE" },
    { .idPS4 = 218, .idUnknown = 215, .idSwitch = 285, .idXbox = "PlusLRZ2MightyEncore",    .idPC = "LRZ2_M_ENCORE" },
    { .idPS4 = 219, .idUnknown = 216, .idSwitch = 286, .idXbox = "PlusLRZ2RayEncore",       .idPC = "LRZ2_R_ENCORE" },
    { .idPS4 = 220, .idUnknown = 217, .idSwitch = 287, .idXbox = "PlusMMZ1SonicEncore",     .idPC = "MMZ1_S_ENCORE" },
    { .idPS4 = 221, .idUnknown = 218, .idSwitch = 288, .idXbox = "PlusMMZ1TailsEncore",     .idPC = "MMZ1_T_ENCORE" },
    { .idPS4 = 222, .idUnknown = 219, .idSwitch = 289, .idXbox = "PlusMMZ1KnucklesEncore",  .idPC = "MMZ1_K_ENCORE" },
    { .idPS4 = 223, .idUnknown = 220, .idSwitch = 290, .idXbox = "PlusMMZ1MightyEncore",    .idPC = "MMZ1_M_ENCORE" },
    { .idPS4 = 224, .idUnknown = 221, .idSwitch = 291, .idXbox = "PlusMMZ1RayEncore",       .idPC = "MMZ1_R_ENCORE" },
    { .idPS4 = 225, .idUnknown = 222, .idSwitch = 292, .idXbox = "PlusMMZ2SonicEncore",     .idPC = "MMZ2_S_ENCORE" },
    { .idPS4 = 226, .idUnknown = 223, .idSwitch = 293, .idXbox = "PlusMMZ2TailsEncore",     .idPC = "MMZ2_T_ENCORE" },
    { .idPS4 = 227, .idUnknown = 224, .idSwitch = 294, .idXbox = "PlusMMZ2KnucklesEncore",  .idPC = "MMZ2_K_ENCORE" },
    { .idPS4 = 228, .idUnknown = 225, .idSwitch = 295, .idXbox = "PlusMMZ2MightyEncore",    .idPC = "MMZ2_M_ENCORE" },
    { .idPS4 = 229, .idUnknown = 226, .idSwitch = 296, .idXbox = "PlusMMZ2RayEncore",       .idPC = "MMZ2_R_ENCORE" },
    { .idPS4 = 230, .idUnknown = 227, .idSwitch = 297, .idXbox = "PlusTMZ1SonicEncore",     .idPC = "TMZ1_S_ENCORE" },
    { .idPS4 = 231, .idUnknown = 228, .idSwitch = 298, .idXbox = "PlusTMZ1TailsEncore",     .idPC = "TMZ1_T_ENCORE" },
    { .idPS4 = 232, .idUnknown = 229, .idSwitch = 299, .idXbox = "PlusTMZ1KnucklesEncore",  .idPC = "TMZ1_K_ENCORE" },
    { .idPS4 = 233, .idUnknown = 230, .idSwitch = 300, .idXbox = "PlusTMZ1MightyEncore",    .idPC = "TMZ1_M_ENCORE" },
    { .idPS4 = 234, .idUnknown = 231, .idSwitch = 301, .idXbox = "PlusTMZ1RayEncore",       .idPC = "TMZ1_R_ENCORE" },
    { .idPS4 = 235, .idUnknown = 232, .idSwitch = 302, .idXbox = "PlusTMZ2SonicEncore",     .idPC = "TMZ2_S_ENCORE" },
    { .idPS4 = 236, .idUnknown = 233, .idSwitch = 303, .idXbox = "PlusTMZ2TailsEncore",     .idPC = "TMZ2_T_ENCORE" },
    { .idPS4 = 237, .idUnknown = 234, .idSwitch = 304, .idXbox = "PlusTMZ2KnucklesEncore",  .idPC = "TMZ2_K_ENCORE" },
    { .idPS4 = 238, .idUnknown = 235, .idSwitch = 305, .idXbox = "PlusTMZ2MightyEncore",    .idPC = "TMZ2_M_ENCORE" },
    { .idPS4 = 239, .idUnknown = 236, .idSwitch = 306, .idXbox = "PlusTMZ2RayEncore",       .idPC = "TMZ2_R_ENCORE" },
};
// clang-format on
#endif

void TimeAttackData_Update(void) {}

void TimeAttackData_LateUpdate(void) {}

void TimeAttackData_StaticUpdate(void) {}

void TimeAttackData_Draw(void) {}

void TimeAttackData_Create(void *data) {}

void TimeAttackData_StageLoad(void) {}

#if MANIA_USE_PLUS
void TimeAttackData_TrackActClear(StatInfo *stat, uint8 zone, uint8 act, uint8 characterID, int32 time, int32 rings, int32 score)
{
    stat->statID = 0;
    stat->name   = "ACT_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)zoneNames[zone];
    stat->data[1] = (void *)actNames[act];
    stat->data[2] = (void *)playerNames[characterID];
    stat->data[3] = 0;
    stat->data[4] = INT_TO_VOID(time);
    stat->data[5] = INT_TO_VOID(rings);
    stat->data[6] = INT_TO_VOID(score);
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
    stat->data[4] = INT_TO_VOID(time);
}
void TimeAttackData_TrackEnemyDefeat(StatInfo *stat, uint8 zoneID, uint8 actID, uint8 characterID, bool32 encore, int32 x, int32 y)
{
    stat->statID = 2;
    stat->name   = "ENEMY_DEFEAT";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)zoneNames[zoneID];
    stat->data[1] = (void *)actNames[actID];
    stat->data[2] = (void *)playerNames[characterID];
    stat->data[3] = INT_TO_VOID(encore);
    stat->data[4] = INT_TO_VOID(x);
    stat->data[5] = INT_TO_VOID(y);
}
#endif

TimeAttackRAM *TimeAttackData_GetTimeAttackRAM(void) { return (TimeAttackRAM *)&globals->saveRAM[0x800]; }

void TimeAttackData_Clear(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    param->puyoSelection = PUYO_SELECTION_NONE;
    memset(param->menuTag, 0, sizeof(param->menuTag));
    param->menuSelection    = 0;
    param->startedTAAttempt = false;
    param->inTimeAttack     = false;
    param->zoneID           = 0;
    param->actID            = 0;

#if MANIA_USE_PLUS
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
        case ZONE_GHZ:
        case ZONE_CPZ:
        case ZONE_SPZ:
        case ZONE_FBZ:
        case ZONE_PGZ:
        case ZONE_SSZ: listPos = act + 2 * zoneID; break;

        case ZONE_HCZ: listPos = act + (2 * zoneID + 1); break;

        case ZONE_MSZ:
            if (act != ACT_1)
                listPos = 2 * zoneID + 3;
            else
                listPos = (characterID == CHAR_KNUX) + 1 + 2 * zoneID;
            break;

        case ZONE_OOZ:
        case ZONE_LRZ: listPos = act + 2 * (zoneID + 1); break;

        case ZONE_MMZ:
        case ZONE_TMZ: listPos = act + (2 * zoneID + 3); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);

    return listPos;
}
#if MANIA_USE_PLUS
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 act, int32 characterID)
{
    int32 listPos = 0;
    switch (zoneID) {
        case ZONE_GHZ:
        case ZONE_CPZ:
        case ZONE_SPZ:
        case ZONE_FBZ:
        case ZONE_PGZ:
        case ZONE_SSZ: listPos = act + 2 * zoneID; break;

        case ZONE_HCZ:
        case ZONE_MSZ:
        case ZONE_OOZ:
        case ZONE_LRZ: listPos = act + 2 * zoneID + 1; break;

        case ZONE_MMZ:
        case ZONE_TMZ: listPos = act + 2 * (zoneID + 1); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);
    return listPos;
}
#endif

uint32 TimeAttackData_GetPackedTime(int32 minutes, int32 seconds, int32 milliseconds) { return 6000 * minutes + 100 * seconds + milliseconds; }

void TimeAttackData_GetUnpackedTime(int32 time, int32 *minutes, int32 *seconds, int32 *milliseconds)
{
    if (minutes)
        *minutes = time / 6000;

    if (seconds)
        *seconds = time % 6000 / 100;

    if (milliseconds)
        *milliseconds = time % 100;
}

uint16 *TimeAttackData_GetRecordedTime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank)
{
    rank--;
    if (rank >= 3)
        return NULL;

    TimeAttackRAM *recordsRAM = NULL;
    if (globals->saveLoaded == STATUS_OK)
        recordsRAM = TimeAttackData_GetTimeAttackRAM();
    else
        return NULL;

    return &recordsRAM->records[characterID - 1][zoneID][act][rank];
}

#if MANIA_USE_PLUS
void TimeAttackData_CreateDB(void)
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

uint16 TimeAttackData_LoadDB(void (*callback)(bool32 success))
{
    LogHelpers_Print("Loading Time Attack DB");
    globals->taTableLoaded = STATUS_CONTINUE;

    TimeAttackData->loadEntityPtr = SceneInfo->entity;
    TimeAttackData->loadCallback  = callback;
    globals->taTableID            = API.LoadUserDB("TimeAttackDB.bin", TimeAttackData_LoadDBCallback);

    if (globals->taTableID == -1) {
        LogHelpers_Print("Couldn't claim a slot for loading %s", "TimeAttackDB.bin");
        globals->taTableLoaded = STATUS_ERROR;
    }

    return globals->taTableID;
}

void TimeAttackData_SaveDB(void (*callback)(bool32 success))
{
    if (API.GetNoSave() || globals->taTableID == (uint16)-1 || globals->taTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Time Attack DB");

        TimeAttackData->saveEntityPtr = SceneInfo->entity;
        TimeAttackData->saveCallback  = callback;

        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveDBCallback);
    }
}

void TimeAttackData_LoadDBCallback(int32 status)
{
    if (status == STATUS_OK) {
        globals->taTableLoaded = STATUS_OK;
        API.SetupUserDBRowSorting(globals->taTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->taTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Time Attack DB");
        TimeAttackData_CreateDB();
    }

    // Bug Details:
    // Due to how options work, this is called after the db is loaded, but before the result is assigned to globals->taTableID
    // meaning that globals->taTableID will be 0xFFFF initially, even if the tabel id was loaded and returned successfully
    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->taTableID, globals->taTableLoaded);

    if (TimeAttackData->loadCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->loadEntityPtr)
            SceneInfo->entity = TimeAttackData->loadEntityPtr;
        TimeAttackData->loadCallback(status == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->loadCallback  = NULL;
        TimeAttackData->loadEntityPtr = NULL;
    }
}

void TimeAttackData_SaveDBCallback(int32 status)
{
    if (TimeAttackData->saveCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->saveEntityPtr)
            SceneInfo->entity = TimeAttackData->saveEntityPtr;
        TimeAttackData->saveCallback(status == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->saveCallback  = NULL;
        TimeAttackData->saveEntityPtr = NULL;
    }
}

void TimeAttackData_MigrateLegacySaves(void)
{
    if (globals->saveLoaded == STATUS_OK) {
        TimeAttackData->isMigratingData = true;

        LogHelpers_Print("===========================");
        LogHelpers_Print("Migrating Legacy TA Data...");
        LogHelpers_Print("===========================");

        for (int32 zone = ZONE_GHZ; zone <= ZONE_TMZ; ++zone) {
            for (int32 act = ACT_1; act <= ACT_2; ++act) {
                for (int32 charID = CHAR_SONIC; charID <= CHAR_KNUX; ++charID) {
                    for (int32 rank = 0; rank < 3; ++rank) {
                        uint16 *records = TimeAttackData_GetRecordedTime(zone, act, charID, rank + 1);
                        if (records && *records) {
                            int32 score = *records;
                            LogHelpers_Print("Import: zone=%d act=%d charID=%d rank=%d -> %d", zone, act, charID, rank, score);
                            TimeAttackData_AddRecord(zone, act, charID, MODE_MANIA, score, NULL);
                        }
                    }
                }
            }
        }

        TimeAttackData->isMigratingData = false;
    }
}

int32 TimeAttackData_AddDBRow(uint8 zoneID, uint8 act, uint8 characterID, uint8 mode, int32 score)
{
    if (globals->taTableLoaded != STATUS_OK)
        return -1;

    uint16 rowID    = API.AddUserDBRow(globals->taTableID);
    uint8 encore    = mode & 1;
    uint32 replayID = 0;

    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "zoneID", &zoneID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "act", &act);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "characterID", &characterID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "encore", &encore);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "score", &score);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "replayID", &replayID);

    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, rowID);
    char buf[0x20];
    memset(buf, 0, sizeof(buf));
    API.GetUserDBRowCreationTime(globals->taTableID, rowID, buf, sizeof(buf) - 1, "%Y/%m/%d %H:%M:%S");

    LogHelpers_Print("Time Attack DB Added Entry");
    LogHelpers_Print("Created at %s", buf);
    LogHelpers_Print("Row ID: %d", rowID);
    LogHelpers_Print("UUID: %08X", uuid);

    return rowID;
}

int32 TimeAttackData_AddRecord(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score, void (*callback)(bool32 success))
{
    uint16 row  = TimeAttackData_AddDBRow(zoneID, act, characterID, encore, score);
    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, row);

    TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);

    if (API.GetSortedUserDBRowCount(globals->taTableID) > 3) {
        API.RemoveDBRow(globals->taTableID, API.GetSortedUserDBRowID(globals->taTableID, 3));

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
        TimeAttackData_SaveDB(callback);
    }

    return rank + 1;
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

void TimeAttackData_Leaderboard_GetRank(bool32 success, int32 rank)
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

    LeaderboardID *curLeaderboard = TimeAttackData_GetLeaderboardInfo(zoneID, act, characterID, isEncore);

    API.TrackScore(curLeaderboard, score, TimeAttackData_Leaderboard_GetRank);
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
void TimeAttackData_AddRecord(uint8 zone, uint8 act, uint8 characterID, uint8 rank, uint16 score)
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
