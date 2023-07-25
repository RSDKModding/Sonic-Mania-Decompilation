#ifndef OBJ_PLAYER_H
#define OBJ_PLAYER_H

#include "Game.h"

// Helper Constants
#define PLAYER_PALETTE_INDEX_SONIC_OLD (2) // sonic's original palette indices, here for legacy reasons. Only exists in a few places in the final game.

#define PLAYER_PALETTE_INDEX_SONIC     (64)
#define PLAYER_PALETTE_INDEX_TAILS     (70)
#define PLAYER_PALETTE_INDEX_KNUX      (80)
#if MANIA_USE_PLUS
#define PLAYER_PALETTE_INDEX_MIGHTY (96)
#define PLAYER_PALETTE_INDEX_RAY    (113)
#endif

#define PLAYER_PRIMARY_COLOR_COUNT (6)

// Helper Enums
typedef enum {
    ANI_IDLE,
    ANI_BORED_1,
    ANI_BORED_2,
    ANI_LOOK_UP,
    ANI_CROUCH,
    ANI_WALK,
    ANI_AIR_WALK,
    ANI_JOG,
    ANI_RUN,
    ANI_DASH,
    ANI_JUMP,
    ANI_SPRING_TWIRL,
    ANI_SPRING_DIAGONAL,
    ANI_SKID,
    ANI_SKID_TURN,
    ANI_SPINDASH,
    ANI_ABILITY_0,
    ANI_PUSH,
    ANI_HURT,
    ANI_DIE,
    ANI_DROWN,
    ANI_BALANCE_1,
    ANI_BALANCE_2,
    ANI_SPRING_CS,
    ANI_STAND_CS,
    ANI_FAN,
    ANI_VICTORY,
    ANI_OUTTA_HERE,
    ANI_HANG,
    ANI_HANG_MOVE,
    ANI_POLE_SWING_H,
    ANI_POLE_SWING_V,
    ANI_SHAFT_SWING,
    ANI_TURNTABLE,
    ANI_TWISTER,
    ANI_SPIRAL_RUN,
    ANI_STICK,
    ANI_PULLEY_HOLD,
    ANI_SHIMMY_IDLE,
    ANI_SHIMMY_MOVE,
    ANI_BREATHE,
    ANI_BUBBLE,
    ANI_RIDE,
    ANI_CLING,
    ANI_BUNGEE,
    ANI_TWIST_RUN,
#if MANIA_USE_PLUS
    ANI_FLUME,
#endif
    ANI_TRANSFORM,
    ANI_ABILITY_1,
    ANI_ABILITY_2,
    ANI_ABILITY_3,
    ANI_ABILITY_4,
    ANI_ABILITY_5,
    ANI_ABILITY_6,
    ANI_ABILITY_7,

    // Sonic Ability Anim Aliases
    ANI_DROPDASH = ANI_ABILITY_0,
    ANI_PEELOUT  = ANI_ABILITY_1,

    // Tails Ability Anim Aliases
    ANI_FLY            = ANI_ABILITY_1,
    ANI_FLY_TIRED      = ANI_ABILITY_2,
    ANI_FLY_LIFT       = ANI_ABILITY_3,
    ANI_FLY_LIFT_TIRED = ANI_ABILITY_4,
    ANI_SWIM           = ANI_ABILITY_5,
    ANI_SWIM_TIRED     = ANI_ABILITY_6,
    ANI_SWIM_LIFT      = ANI_ABILITY_7,

    // Knux Ability Anim Aliases
    ANI_LEDGE_PULL_UP = ANI_ABILITY_0,
    ANI_GLIDE         = ANI_ABILITY_1,
    ANI_GLIDE_DROP    = ANI_ABILITY_2,
    ANI_GLIDE_LAND    = ANI_ABILITY_3,
    ANI_GLIDE_SLIDE   = ANI_ABILITY_4,
    ANI_CLIMB_IDLE    = ANI_ABILITY_5,
    ANI_CLIMB_UP      = ANI_ABILITY_6,
    ANI_CLIMB_DOWN    = ANI_ABILITY_7,

#if MANIA_USE_PLUS
    // Mighty Ability Anim Aliases
    ANI_HAMMERDROP = ANI_ABILITY_0,
    ANI_UNSPIN     = ANI_ABILITY_1,

    // Ray Ability Anim Aliases
    ANI_HANG2    = ANI_ABILITY_0,
    ANI_FLY_UP   = ANI_ABILITY_1,
    ANI_FLY_DOWN = ANI_ABILITY_2,
#endif
} PlayerAnimationIDs;

// for "characterID" in editor
typedef enum {
    PLAYER_CHAR_NONE,
    PLAYER_CHAR_SONIC,
    PLAYER_CHAR_TAILS,
    PLAYER_CHAR_SONIC_TAILS,
    PLAYER_CHAR_KNUX,
    PLAYER_CHAR_SONIC_KNUX,
    PLAYER_CHAR_TAILS_KNUX,
    PLAYER_CHAR_SONIC_TAILS_KNUX,
} PlayerCharacterIDS;

typedef enum {
    SHIELD_NONE,
    SHIELD_BLUE,
    SHIELD_BUBBLE,
    SHIELD_FIRE,
    SHIELD_LIGHTNING,
} ShieldTypes;

typedef enum {
    PLAYER_DEATH_NONE,
    PLAYER_DEATH_DIE_USESFX,
    PLAYER_DEATH_DIE_NOSFX,
    PLAYER_DEATH_DROWN,
} DeathTypes;

typedef enum {
    PLAYER_HURT_NONE,
    PLAYER_HURT_HASSHIELD,
    PLAYER_HURT_RINGLOSS,
    PLAYER_HURT_DIE,
} HurtTypes;

typedef enum {
    SUPERSTATE_NONE,
    SUPERSTATE_FADEIN,
    SUPERSTATE_SUPER,
    SUPERSTATE_FADEOUT,
    SUPERSTATE_DONE,
} SuperStates;

// Object Class
#if MANIA_USE_PLUS
struct ObjectPlayer {
    RSDK_OBJECT
    TABLE(int32 sonicPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(int32 tailsPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(int32 knuxPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x60000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x30000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x60000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x30000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x60000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x30000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x60000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x8000, 0x300, 0x30000, -0x20000 });
    TABLE(int32 mightyPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(int32 rayPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(color superPalette_Sonic[18], { 0x000080, 0x0038C0, 0x0068F0, 0x1888F0, 0x30A0F0, 0x68D0F0, 0xF0C001, 0xF0D028, 0xF0E040, 0xF0E860,
                                          0xF0E898, 0xF0E8D0, 0xF0D898, 0xF0E0B0, 0xF0E8C0, 0xF0F0D8, 0xF0F0F0, 0xF0F0F8 });
    TABLE(color superPalette_Tails[18], { 0x800801, 0xB01801, 0xD05001, 0xE07808, 0xE89008, 0xF0A801, 0xF03830, 0xF06848, 0xF09860, 0xF0B868,
                                          0xF0C870, 0xF0D870, 0xF03830, 0xF06848, 0xF09860, 0xF0B868, 0xF0C870, 0xF0D870 });
    TABLE(color superPalette_Knux[18], { 0x580818, 0x980130, 0xD00840, 0xE82858, 0xF06080, 0xF08088, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8,
                                         0xF0C0C8, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8, 0xF0C0C8 });
    TABLE(color superPalette_Mighty[18], { 0x501010, 0x882020, 0xA83030, 0xC84040, 0xE06868, 0xF09098, 0x701010, 0xD84040, 0xF05858, 0xF07878,
                                           0xF0B8B8, 0xF0E0E8, 0x701010, 0xD84040, 0xF05858, 0xF07878, 0xF0B8B8, 0xF0E0E8 });
    TABLE(color superPalette_Ray[18], { 0xA06800, 0xB88810, 0xD0A810, 0xE0C020, 0xE8D038, 0xF0E078, 0xE0A801, 0xF0C820, 0xF0E820, 0xF0F040, 0xF0F068,
                                        0xF0F0B8, 0xE0A801, 0xF0C820, 0xF0E820, 0xF0F040, 0xF0F068, 0xF0F0B8 });
    TABLE(color superPalette_Sonic_HCZ[18], { 0x200888, 0x3020C8, 0x3840F0, 0x4070F0, 0x4098F0, 0x40C0F0, 0x88C880, 0x68E090, 0x50F098, 0x68F0C0,
                                              0x78F0C8, 0xA0F0D8, 0x60E898, 0x48F0A0, 0x58F0B0, 0x68F0C0, 0x90F0D0, 0xA0F0D8 });
    TABLE(color superPalette_Tails_HCZ[18], { 0x880808, 0xA03810, 0xA05848, 0xB07058, 0xC08068, 0xC89078, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893,
                                              0xE6B09D, 0xEABAA7, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893, 0xE6B09D, 0xEABAA7 });
    TABLE(color superPalette_Knux_HCZ[18], { 0x181050, 0x301090, 0x5018A8, 0x8828C0, 0xA048C0, 0xB868C8, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF,
                                             0xD086EB, 0xDE9CED, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF, 0xD086EB, 0xDE9CED });
    TABLE(color superPalette_Mighty_HCZ[18], { 0x381058, 0x502098, 0x7028B0, 0x8048C8, 0x7868C8, 0x8098D0, 0x401078, 0x9038C0, 0x9068C0, 0x9890E0,
                                               0xA8C0D8, 0xC0E8F0, 0x401078, 0x9038C0, 0x9068C0, 0x9890E0, 0xA8C0D8, 0xC0E8F0 });
    TABLE(color superPalette_Ray_HCZ[18], { 0x406090, 0x488890, 0x68A880, 0x70C080, 0x68D080, 0x50E888, 0x80B088, 0x78D090, 0x68F080, 0x50F098,
                                            0x90F0C0, 0xA8F0E0, 0x80B088, 0x78D090, 0x68F080, 0x50F098, 0x90F0C0, 0xA8F0E0 });
    TABLE(color superPalette_Sonic_CPZ[18], { 0x4000D8, 0x5800E0, 0x6810E0, 0x8020E0, 0xA020E0, 0xC040E0, 0xE04880, 0xE06890, 0xE078A8, 0xE078D8,
                                              0xE080E0, 0xE080E0, 0xE080B0, 0xE080B0, 0xE080C0, 0xE080C0, 0xE080E0, 0xE080E0 });
    TABLE(color superPalette_Tails_CPZ[18], { 0xC80180, 0xD00178, 0xE00180, 0xE81088, 0xE83098, 0xE84898, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0,
                                              0xF098F0, 0xF0A0F0, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0, 0xF098F0, 0xF0A0F0 });
    TABLE(color superPalette_Knux_CPZ[18], { 0xA00180, 0xB00178, 0xC00190, 0xD001B0, 0xE001E0, 0xE820E8, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0,
                                             0xF0A8F0, 0xF0B0F0, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0, 0xF0A8F0, 0xF0B0F0 });
    TABLE(color superPalette_Mighty_CPZ[18], { 0xA00180, 0xD80188, 0xE001A0, 0xE001B0, 0xE001D8, 0xE001E0, 0xB80180, 0xE001A8, 0xE001C8, 0xE001E0,
                                               0xE040E0, 0xE078E0, 0xB80180, 0xE001A8, 0xE001C8, 0xE001E0, 0xE040E0, 0xE078E0 });
    TABLE(color superPalette_Ray_CPZ[18], { 0xE00180, 0xE00190, 0xE02898, 0xE048A8, 0xE060B8, 0xE078E0, 0xE02880, 0xE05888, 0xE08088, 0xE080A8,
                                            0xE080D8, 0xE080E0, 0xE02880, 0xE05888, 0xE08088, 0xE080A8, 0xE080D8, 0xE080E0 });
    bool32 cantSwap;
    int32 playerCount;
    uint16 upState;
    uint16 downState;
    uint16 leftState;
    uint16 rightState;
    uint16 jumpPressState;
    uint16 jumpHoldState;
    int32 nextLeaderPosID;
    int32 lastLeaderPosID;
    Vector2 leaderPositionBuffer[16];
    Vector2 targetLeaderPosition;
    int32 autoJumpTimer;
    int32 respawnTimer;
    int32 aiInputSwapTimer;
    bool32 disableP2KeyCheck;
    int32 rings;
    STATIC(int32 ringExtraLife, 100);
    int32 powerups;
    STATIC(int32 savedLives, 3);
    int32 savedScore;
    STATIC(int32 savedScore1UP, 50000);
    uint16 sonicFrames;
    uint16 superFrames;
    uint16 tailsFrames;
    uint16 tailsTailsFrames;
    uint16 knuxFrames;
    uint16 mightyFrames;
    uint16 rayFrames;
    uint16 sfxJump;
    uint16 sfxRoll;
    uint16 sfxCharge;
    uint16 sfxRelease;
    uint16 sfxPeelCharge;
    uint16 sfxPeelRelease;
    uint16 sfxDropdash;
    uint16 sfxLoseRings;
    uint16 sfxHurt;
    uint16 sfxPimPom;
    uint16 sfxSkidding;
    uint16 sfxGrab;
    uint16 sfxFlying;
    bool32 playingFlySfx;
    uint16 sfxTired;
    bool32 playingTiredSfx;
    uint16 sfxLand;
    uint16 sfxSlide;
    uint16 sfxOuttahere;
    uint16 sfxTransform2;
    uint16 sfxSwap;
    uint16 sfxSwapFail;
    uint16 sfxMightyDeflect;
    uint16 sfxMightyDrill;
    uint16 sfxMightyLand;
    uint16 sfxMightyUnspin;
    int32 raySwoopTimer;
    int32 rayDiveTimer;
    bool32 gotHit[PLAYER_COUNT];
    StateMachine(configureGhostCB);
    bool32 (*canSuperCB)(bool32 isHUD);
    int32 superDashCooldown;
};
#else
struct ObjectPlayer {
    RSDK_OBJECT
    int32 playerCount;
    TABLE(int32 sonicPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(int32 tailsPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x68000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x38000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x80000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x68000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x80000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x38000, -0x20000 });
    TABLE(int32 knuxPhysicsTable[64],
          { 0x60000, 0xC00,  0x1800, 0x600,  0x8000,  0x600, 0x60000, -0x40000, 0x30000, 0x600,  0xC00,  0x300, 0x4000, 0x300, 0x30000, -0x20000,
            0xA0000, 0x3000, 0x6000, 0x1800, 0x10000, 0x600, 0x60000, -0x40000, 0x50000, 0x1800, 0x3000, 0xC00, 0x8000, 0x300, 0x30000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x60000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x4000, 0x300, 0x30000, -0x20000,
            0xC0000, 0x1800, 0x3000, 0xC00,  0x8000,  0x600, 0x60000, -0x40000, 0x60000, 0xC00,  0x1800, 0x600, 0x8000, 0x300, 0x30000, -0x20000 });
    uint16 upState;
    uint16 downState;
    uint16 leftState;
    uint16 rightState;
    uint16 jumpPressState;
    uint16 jumpHoldState;
    int32 nextLeaderPosID;
    int32 lastLeaderPosID;
    Vector2 leaderPositionBuffer[16];
    Vector2 targetLeaderPosition;
    int32 autoJumpTimer;
    int32 respawnTimer;
    int32 aiInputSwapTimer;
#if GAME_VERSION != VER_100 // may not work with building static var files, but who's building 1.00 static var files anyways lol
    bool32 disableP2KeyCheck;
#endif
    int32 rings;
    STATIC(int32 ringExtraLife, 100);
    int32 powerups;
    STATIC(int32 savedLives, 3);
    int32 savedScore;
    STATIC(int32 savedScore1UP, 50000);
    uint16 sonicFrames;
    uint16 superFrames;
    uint16 tailsTailsFrames;
    uint16 tailsFrames;
    uint16 knuxFrames;
    TABLE(color superPalette_Sonic[18], { 0x000080, 0x0038C0, 0x0068F0, 0x1888F0, 0x30A0F0, 0x68D0F0, 0xF0C001, 0xF0D028, 0xF0E040, 0xF0E860,
                                          0xF0E898, 0xF0E8D0, 0xF0D898, 0xF0E0B0, 0xF0E8C0, 0xF0F0D8, 0xF0F0F0, 0xF0F0F8 });
    TABLE(color superPalette_Tails[18], { 0x800801, 0xB01801, 0xD05001, 0xE07808, 0xE89008, 0xF0A801, 0xF03830, 0xF06848, 0xF09860, 0xF0B868,
                                          0xF0C870, 0xF0D870, 0xF03830, 0xF06848, 0xF09860, 0xF0B868, 0xF0C870, 0xF0D870 });
    TABLE(color superPalette_Knux[18], { 0x580818, 0x980130, 0xD00840, 0xE82858, 0xF06080, 0xF08088, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8,
                                         0xF0C0C8, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8, 0xF0C0C8 });
    TABLE(color superPalette_Sonic_HCZ[18], { 0x200888, 0x3020C8, 0x3840F0, 0x4070F0, 0x4098F0, 0x40C0F0, 0x88C880, 0x68E090, 0x50F098, 0x68F0C0,
                                              0x78F0C8, 0xA0F0D8, 0x60E898, 0x48F0A0, 0x58F0B0, 0x68F0C0, 0x90F0D0, 0xA0F0D8 });
    TABLE(color superPalette_Tails_HCZ[18], { 0x880808, 0xA03810, 0xA05848, 0xB07058, 0xC08068, 0xC89078, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893,
                                              0xE6B09D, 0xEABAA7, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893, 0xE6B09D, 0xEABAA7 });
    TABLE(color superPalette_Knux_HCZ[18], { 0x181050, 0x301090, 0x5018A8, 0x8828C0, 0xA048C0, 0xB868C8, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF,
                                             0xD086EB, 0xDE9CED, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF, 0xD086EB, 0xDE9CED });
    TABLE(color superPalette_Sonic_CPZ[18], { 0x4000D8, 0x5800E0, 0x6810E0, 0x8020E0, 0xA020E0, 0xC040E0, 0xE04880, 0xE06890, 0xE078A8, 0xE078D8,
                                              0xE080E0, 0xE080E0, 0xE080B0, 0xE080B0, 0xE080C0, 0xE080C0, 0xE080E0, 0xE080E0 });
    TABLE(color superPalette_Tails_CPZ[18], { 0xC80180, 0xD00178, 0xE00180, 0xE81088, 0xE83098, 0xE84898, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0,
                                              0xF098F0, 0xF0A0F0, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0, 0xF098F0, 0xF0A0F0 });
    TABLE(color superPalette_Knux_CPZ[18], { 0xA00180, 0xB00178, 0xC00190, 0xD001B0, 0xE001E0, 0xE820E8, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0,
                                             0xF0A8F0, 0xF0B0F0, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0, 0xF0A8F0, 0xF0B0F0 });
    uint16 sfxJump;
    uint16 sfxRoll;
    uint16 sfxCharge;
    uint16 sfxRelease;
    uint16 sfxPeelCharge;
    uint16 sfxPeelRelease;
    uint16 sfxDropdash;
    uint16 sfxLoseRings;
    uint16 sfxHurt;
    uint16 unused; // this matches up perfectly with the position of "sfxPimPom" in plus, and it cant be padding so :eye:
    uint16 sfxSkidding;
    uint16 sfxGrab;
    uint16 sfxFlying;
    bool32 playingFlySfx;
    uint16 sfxTired;
    bool32 playingTiredSfx;
    uint16 sfxLand;
    uint16 sfxSlide;
    uint16 sfxOuttahere;
    uint16 sfxTransform2;
    bool32 gotHit[PLAYER_COUNT];
};
#endif

// Entity Class
struct EntityPlayer {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(nextAirState);
    StateMachine(nextGroundState);
    EntityCamera *camera;
    Animator animator;
    Animator tailAnimator;
    int32 minJogVelocity;
    int32 minRunVelocity;
    int32 minDashVelocity;
    int32 unused; // the only used variable in the player struct, I cant find a ref to it anywhere so...
    int32 tailRotation;
    int32 tailDirection;
    uint16 aniFrames;
    uint16 tailFrames;
    uint16 animationReserve; // what anim to return to after SpringTwirl/SpringDiagonal has finished and the player is falling downwards
    uint16 playerID;
    Hitbox *outerbox;
    Hitbox *innerbox;
    int32 characterID;
    int32 rings;
    int32 ringExtraLife;
    int32 shield;
    int32 lives;
    int32 score;
    int32 score1UP;
    bool32 hyperRing;
    int32 timer;
    int32 outtaHereTimer;
    int32 abilityTimer;
    int32 spindashCharge;
    int32 abilityValue;
    int32 drownTimer;
    int32 invincibleTimer;
    int32 speedShoesTimer;
    int32 blinkTimer;
    int32 scrollDelay;
    int32 skidding;
    int32 pushing;
    int32 underwater;     // 0 = not in water, 1 = in palette water, else water entityID
    bool32 groundedStore; // prev frame's onGround value
    bool32 invertGravity;
    bool32 isChibi;
#if GAME_VERSION != VER_100
    bool32 isTransforming;
#endif
    int32 superState;
    int32 superRingLossTimer;
    int32 superBlendAmount;
    int32 superBlendState;
    bool32 sidekick;
    int32 scoreBonus;
    int32 jumpOffset;
    int32 collisionFlagH;
    int32 collisionFlagV;
    int32 topSpeed;
    int32 acceleration;
    int32 deceleration;
    int32 airAcceleration;
    int32 airDeceleration;
    int32 skidSpeed;
    int32 rollingFriction;
    int32 rollingDeceleration;
    int32 gravityStrength;
    int32 abilitySpeed;
    int32 jumpStrength;
    int32 jumpCap;
    int32 flailing;
    int32 sensorX[5];
    int32 sensorY;
    Vector2 moveLayerPosition;
#if MANIA_USE_PLUS
    StateMachine(stateInputReplay);
#endif
    StateMachine(stateInput);
    int32 controllerID;
    int32 controlLock;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    bool32 applyJumpCap;
    int32 jumpAbilityState;
    StateMachine(stateAbility);
    StateMachine(statePeelout);
    int32 flyCarryTimer;
    Vector2 flyCarrySidekickPos;
    Vector2 flyCarryLeaderPos;
    uint8 deathType;
    bool32 forceRespawn;
#if MANIA_USE_PLUS
    bool32 isGhost;
#endif
    int32 abilityValues[8];
    void *abilityPtrs[8];
#if MANIA_USE_PLUS
    int32 uncurlTimer;
#endif
};

// Object Struct
extern ObjectPlayer *Player;

// Standard Entity Events
void Player_Update(void);
void Player_LateUpdate(void);
void Player_StaticUpdate(void);
void Player_Draw(void);
void Player_Create(void *data);
void Player_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Player_EditorDraw(void);
void Player_EditorLoad(void);
#endif
void Player_Serialize(void);

// Extra Entity Functions
void Player_LoadSprites(void);
void Player_LoadSpritesVS(void);
void Player_SaveValues(void);
void Player_GiveScore(EntityPlayer *player, int32 score);
void Player_GiveRings(EntityPlayer *player, int32 amount, bool32 playSfx);
void Player_GiveLife(EntityPlayer *entity);
void Player_ApplyShield(EntityPlayer *player);
void Player_ChangeCharacter(EntityPlayer *entity, int32 character);
bool32 Player_TryTransform(EntityPlayer *player, uint8 emeraldMasks);
void Player_BlendSuperSonicColors(int32 bankID);
void Player_BlendSuperTailsColors(int32 bankID);
void Player_BlendSuperKnuxColors(int32 bankID);
#if MANIA_USE_PLUS
void Player_BlendSuperMightyColors(int32 bankID);
void Player_BlendSuperRayColors(int32 bankID);
#endif
void Player_HandleSuperForm(void);
bool32 Player_CheckP2KeyPress(void);
// returns the pointer to the nearest player to the current entity on the x axis only
EntityPlayer *Player_GetNearestPlayerX(void);
// returns the pointer to the nearest player to the current entity on both the x & y axis
EntityPlayer *Player_GetNearestPlayer(void);
#if MANIA_USE_PLUS
void Player_RemoveEncoreLeader(void);
#endif
void Player_ResetBoundaries(EntityPlayer *player);
void Player_HandleDeath(EntityPlayer *player);
void Player_HandleQuickRespawn(EntityPlayer *player);

// Collision
// returns a pointer to the currently active player hitbox (outer box)
Hitbox *Player_GetHitbox(EntityPlayer *player);
// returns a pointer to the alternate player hitbox (inner box)
Hitbox *Player_GetAltHitbox(EntityPlayer *player);
// performs a non-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionTouch(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
// performs an all-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionBox(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
// performs a top-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionPlatform(EntityPlayer *player, void *entity, Hitbox *entityHitbox);

// Hits the player if not invulnerable, returns true if player was hit
bool32 Player_Hurt(EntityPlayer *player, void *entity);
// the same as above, though the knockback dir is gotten from the player dir rather the direction of the distance
bool32 Player_HurtFlip(EntityPlayer *player);
// Hits the player if not invulnerable and the player's shield doesn't match shield, returns true if player was hit
bool32 Player_ElementHurt(EntityPlayer *player, void *entity, int32 shield);
// returns true if the player is in an "attacking" state
bool32 Player_CheckAttacking(EntityPlayer *player, void *e);
// returns true if the player is in an "attacking" state (but omits invincibility timer as being an "attacking" state)
bool32 Player_CheckAttackingNoInvTimer(EntityPlayer *player, void *e);
// checks if the player collided with an entity, this collision differs from the touch one above since it uses hammerdrop & instashield if appropriate
bool32 Player_CheckBadnikTouch(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
// checks if the player is attacking the badnik, returns true if the player attacked the badnik, otherwise the player is hit and returns false
bool32 Player_CheckBadnikBreak(EntityPlayer *player, void *entity, bool32 destroy);
// similar to checkBadnikTouch, this time for bosses, handles rebounds and stuff properly, does NOT check for hammerdrop/instashield hitboxes. returns
// true if player hit the boss, otherwise the player is hit and returns false
bool32 Player_CheckBossHit(EntityPlayer *player, void *entity);
// similar to checkHit, but for projectiles, handles the rebound effect when using shields or crouching as mighty, returns true if deflected,
// otherwise the player is hit and returns false
bool32 Player_ProjectileHurt(EntityPlayer *player, void *projectile);
#if MANIA_USE_PLUS
// similar to checkHit, but for objects that should bounce off mighty's shell, returns true if deflected, otherwise the player is hit and returns
// false
bool32 Player_CheckMightyShellHit(EntityPlayer *player, void *e, int32 velX, int32 velY);
#endif
// Similar to "Player_CheckBadnikBreak" but omits the score/explosion & animal stuff. Does not destroy 'entity'
bool32 Player_CheckItemBreak(EntityPlayer *player, void *entity, bool32 hitIfNotAttacking);

// State helpers
void Player_UpdatePhysicsState(EntityPlayer *entity);
void Player_HandleIdleAnimation(void);
void Player_HandleGroundAnimation(void);
void Player_HandleGroundMovement(void);
void Player_HandleGroundRotation(void);
void Player_HandleAirRotation(void);
void Player_HandleAirMovement(void);
void Player_HandleAirFriction(void);
void Player_Action_Jump(EntityPlayer *entity);
void Player_Action_Roll(void);
void Player_Action_Spindash(void);
void Player_Action_Peelout(void);
#if MANIA_USE_PLUS
bool32 Player_SwapMainPlayer(bool32 forceSwap);
#endif
void Player_HandleRollDeceleration(void);
void Player_Hit(EntityPlayer *player);
bool32 Player_CheckValidState(EntityPlayer *player);
void Player_HandleFlyCarry(EntityPlayer *leader);
void Player_HandleSidekickRespawn(void);
void Player_State_StartSuper(void);

// States
void Player_State_Static(void);
void Player_State_Ground(void);
void Player_State_Air(void);
void Player_State_Roll(void);
void Player_State_TubeRoll(void);
void Player_State_TubeAirRoll(void);
void Player_State_LookUp(void);
void Player_State_Crouch(void);
void Player_State_Spindash(void);
void Player_State_Peelout(void);
void Player_State_OuttaHere(void);
void Player_State_Transform(void);
void Player_State_Hurt(void);
void Player_State_Death(void);
void Player_State_Drown(void);
void Player_State_DropDash(void);
void Player_State_BubbleBounce(void);
void Player_State_TailsFlight(void);
void Player_State_FlyCarried(void);
void Player_State_KnuxGlideLeft(void);
void Player_State_KnuxGlideRight(void);
void Player_State_KnuxGlideDrop(void);
void Player_State_KnuxGlideSlide(void);
void Player_State_KnuxWallClimb(void);
void Player_State_KnuxLedgePullUp(void);
#if MANIA_USE_PLUS
void Player_State_MightyHammerDrop(void);
void Player_State_MightyUnspin(void);
void Player_SpawnMightyHammerdropDust(int32 speed, Hitbox *hitbox);
bool32 Player_CheckMightyUnspin(EntityPlayer *player, int32 bounceDistance, bool32 checkHammerDrop, int32 *uncurlTimer);
void Player_State_RayGlide(void);
#endif
void Player_State_FlyToPlayer(void);
void Player_State_ReturnToPlayer(void);
void Player_State_HoldRespawn(void);
void Player_FinishedReturnToPlayer(EntityPlayer *player, EntityPlayer *leader);
void Player_State_EncoreRespawn(void);
void Player_State_Victory(void);
void Player_State_Bubble(void);
void Player_State_WaterSlide(void);
void Player_State_TransportTube(void);

// Gravity States
void Player_Gravity_True(void);
void Player_Gravity_False(void);

// Jump Ability States
void Player_JumpAbility_Sonic(void);
void Player_JumpAbility_Tails(void);
void Player_JumpAbility_Knux(void);
#if MANIA_USE_PLUS
void Player_JumpAbility_Mighty(void);
void Player_JumpAbility_Ray(void);

bool32 Player_SfxCheck_RayDive(void);
bool32 Player_SfxCheck_RaySwoop(void);
void Player_SfxUpdate_RayDive(int32 sfxID);
void Player_SfxUpdate_RaySwoop(int32 sfxID);
#endif

// Input States
void Player_Input_P1(void);
void Player_Input_P2_Delay(void);
void Player_Input_P2_AI(void);
void Player_Input_AI_SpindashPt1(void);
void Player_Input_AI_SpindashPt2(void);
void Player_Input_P2_Player(void);

#endif //! OBJ_PLAYER_H
