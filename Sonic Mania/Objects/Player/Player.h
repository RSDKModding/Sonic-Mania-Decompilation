#ifndef OBJ_PLAYER_H
#define OBJ_PLAYER_H

#include "SonicMania.h"

typedef enum {
    ANI_IDLE,
    ANI_BORED1,
    ANI_BORED2,
    ANI_LOOKUP,
    ANI_CROUCH,
    ANI_WALK,
    ANI_AIRWALK,
    ANI_JOG,
    ANI_RUN,
    ANI_DASH,
    ANI_JUMP,
    ANI_SPRINGTWIRL,
    ANI_SPRINGDIAGONAL,
    ANI_SKID,
    ANI_SKIDTURN,
    ANI_SPINDASH,
    ANI_DROPDASH,
    ANI_PUSH,
    ANI_HURT,
    ANI_DIE,
    ANI_DROWN,
    ANI_BALANCE1,
    ANI_BALANCE2,
    ANI_SPRINGCS,
    ANI_STANDCS,
    ANI_FAN,
    ANI_VICTORY,
    ANI_OUTTAHERE,
    ANI_HANG,
    ANI_HANGMOVE,
    ANI_POLESWINGH,
    ANI_POLESWINGV,
    ANI_SHAFTSWING,
    ANI_TURNTABLE,
    ANI_TWISTER,
    ANI_SPIRALRUN,
    ANI_STICK,
    ANI_PULLEYHOLD,
    ANI_SHIMMYIDLE,
    ANI_SHIMMYMOVE,
    ANI_BREATHE,
    ANI_BUBBLE,
    ANI_RIDE,
    ANI_CLING,
    ANI_BUNGEE,
    ANI_TWISTRUN,
#if RETRO_USE_PLUS
    ANI_FLUME,
#endif
    ANI_TRANSFORM,
    ANI_FLY,
    ANI_FLYTIRED,
    ANI_FLYLIFT,
    ANI_FLYLIFTTIRED,
    ANI_SWIM,
    ANI_SWIMTIRED,
    ANI_SWIMLIFT,
} PLAYER_ANI;

typedef enum {
    CHAR_NONE,
    CHAR_SONIC,
    CHAR_TAILS,
    CHAR_SONIC_TAILS,
    CHAR_KNUX,
    CHAR_SONIC_KNUX,
    CHAR_TAILS_KNUX,
    CHAR_SONIC_TAILS_KNUX,
} CharacterIDS;

typedef enum {
    SHIELD_NONE,
    SHIELD_BLUE,
    SHIELD_BUBBLE,
    SHIELD_FIRE,
    SHIELD_LIGHTNING,
} ShieldTypes;

typedef enum {
    HURTFLAG_NONE,
    HURTFLAG_HURT,
    HURTFLAG_DIE,
    HURTFLAG_DROWN,
} HurtFlags;

typedef enum {
    SUPERSTATE_NONE,
    SUPERSTATE_FADEIN,
    SUPERSTATE_SUPER,
    SUPERSTATE_FADEOUT,
    SUPERSTATE_DONE,
} SuperStates;

// Object Class
#if RETRO_USE_PLUS
typedef struct {
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
    TABLE(colour superPalette_Sonic[18], { 0x000080, 0x0038C0, 0x0068F0, 0x1888F0, 0x30A0F0, 0x68D0F0, 0xF0C001, 0xF0D028, 0xF0E040, 0xF0E860,
                                           0xF0E898, 0xF0E8D0, 0xF0D898, 0xF0E0B0, 0xF0E8C0, 0xF0F0D8, 0xF0F0F0, 0xF0F0F8 });
    TABLE(colour superPalette_Tails[18], { 0x800801, 0xB01801, 0xD05001, 0xE07808, 0xE89008, 0xF0A801, 0xF03830, 0xF06848, 0xF09860, 0xF0B868,
                                           0xF0C870, 0xF0D870, 0xF03830, 0xF06848, 0xF09860, 0xF0B868, 0xF0C870, 0xF0D870 });
    TABLE(colour superPalette_Knux[18], { 0x580818, 0x980130, 0xD00840, 0xE82858, 0xF06080, 0xF08088, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0,
                                          0xF0B0C8, 0xF0C0C8, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8, 0xF0C0C8 });
    TABLE(colour superPalette_Mighty[18], { 0x501010, 0x882020, 0xA83030, 0xC84040, 0xE06868, 0xF09098, 0x701010, 0xD84040, 0xF05858, 0xF07878,
                                            0xF0B8B8, 0xF0E0E8, 0x701010, 0xD84040, 0xF05858, 0xF07878, 0xF0B8B8, 0xF0E0E8 });
    TABLE(colour superPalette_Ray[18], { 0xA06800, 0xB88810, 0xD0A810, 0xE0C020, 0xE8D038, 0xF0E078, 0xE0A801, 0xF0C820, 0xF0E820, 0xF0F040, 0xF0F068,
                                         0xF0F0B8, 0xE0A801, 0xF0C820, 0xF0E820, 0xF0F040, 0xF0F068, 0xF0F0B8 });
    TABLE(colour superPalette_Sonic_HCZ[18], { 0x200888, 0x3020C8, 0x3840F0, 0x4070F0, 0x4098F0, 0x40C0F0, 0x88C880, 0x68E090, 0x50F098, 0x68F0C0,
                                               0x78F0C8, 0xA0F0D8, 0x60E898, 0x48F0A0, 0x58F0B0, 0x68F0C0, 0x90F0D0, 0xA0F0D8 });
    TABLE(colour superPalette_Tails_HCZ[18], { 0x880808, 0xA03810, 0xA05848, 0xB07058, 0xC08068, 0xC89078, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893,
                                               0xE6B09D, 0xEABAA7, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893, 0xE6B09D, 0xEABAA7 });
    TABLE(colour superPalette_Knux_HCZ[18], { 0x181050, 0x301090, 0x5018A8, 0x8828C0, 0xA048C0, 0xB868C8, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF,
                                              0xD086EB, 0xDE9CED, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF, 0xD086EB, 0xDE9CED });
    TABLE(colour superPalette_Mighty_HCZ[18], { 0x381058, 0x502098, 0x7028B0, 0x8048C8, 0x7868C8, 0x8098D0, 0x401078, 0x9038C0, 0x9068C0, 0x9890E0,
                                                0xA8C0D8, 0xC0E8F0, 0x401078, 0x9038C0, 0x9068C0, 0x9890E0, 0xA8C0D8, 0xC0E8F0 });
    TABLE(colour superPalette_Ray_HCZ[18], { 0x406090, 0x488890, 0x68A880, 0x70C080, 0x68D080, 0x50E888, 0x80B088, 0x78D090, 0x68F080, 0x50F098,
                                             0x90F0C0, 0xA8F0E0, 0x80B088, 0x78D090, 0x68F080, 0x50F098, 0x90F0C0, 0xA8F0E0 });
    TABLE(colour superPalette_Sonic_CPZ[18], { 0x4000D8, 0x5800E0, 0x6810E0, 0x8020E0, 0xA020E0, 0xC040E0, 0xE04880, 0xE06890, 0xE078A8, 0xE078D8,
                                               0xE080E0, 0xE080E0, 0xE080B0, 0xE080B0, 0xE080C0, 0xE080C0, 0xE080E0, 0xE080E0 });
    TABLE(colour superPalette_Tails_CPZ[18], { 0xC80180, 0xD00178, 0xE00180, 0xE81088, 0xE83098, 0xE84898, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0,
                                               0xF098F0, 0xF0A0F0, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0, 0xF098F0, 0xF0A0F0 });
    TABLE(colour superPalette_Knux_CPZ[18], { 0xA00180, 0xB00178, 0xC00190, 0xD001B0, 0xE001E0, 0xE820E8, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0,
                                              0xF0A8F0, 0xF0B0F0, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0, 0xF0A8F0, 0xF0B0F0 });
    TABLE(colour superPalette_Mighty_CPZ[18], { 0xA00180, 0xD80188, 0xE001A0, 0xE001B0, 0xE001D8, 0xE001E0, 0xB80180, 0xE001A8, 0xE001C8, 0xE001E0,
                                                0xE040E0, 0xE078E0, 0xB80180, 0xE001A8, 0xE001C8, 0xE001E0, 0xE040E0, 0xE078E0 });
    TABLE(colour superPalette_Ray_CPZ[18], { 0xE00180, 0xE00190, 0xE02898, 0xE048A8, 0xE060B8, 0xE078E0, 0xE02880, 0xE05888, 0xE08088, 0xE080A8,
                                             0xE080D8, 0xE080E0, 0xE02880, 0xE05888, 0xE08088, 0xE080A8, 0xE080D8, 0xE080E0 });
    bool32 cantSwap;
    int32 playerCount;
    uint16 upState;
    uint16 downState;
    uint16 leftState;
    uint16 rightState;
    uint16 jumpPressState;
    uint16 jumpHoldState;
    int32 flyCarryPosA;
    int32 flyCarryPosB;
    Vector2 flyCarryPositions[0x10];
    Vector2 curFlyCarryPos;
    int32 P2JumpActionDelay;
    int32 jumpInDelay;
    int32 p2InputDelay;
    bool32 disableP2KeyCheck;
    int32 rings;
    STATIC(int32 ringExtraLife, 100);
    int32 powerups;
    STATIC(int32 savedLives, 3);
    int32 savedScore;
    STATIC(int32 savedScore1UP, 50000);
    uint16 sonicSpriteIndex;
    uint16 superSpriteIndex;
    uint16 tailsSpriteIndex;
    uint16 tailsTailsSpriteIndex;
    uint16 knuxSpriteIndex;
    uint16 mightySpriteIndex;
    uint16 raySpriteIndex;
    uint16 sfx_Jump;
    uint16 sfx_Roll;
    uint16 sfx_Charge;
    uint16 sfx_Release;
    uint16 sfx_PeelCharge;
    uint16 sfx_PeelRelease;
    uint16 sfx_Dropdash;
    uint16 sfx_LoseRings;
    uint16 sfx_Hurt;
    uint16 sfx_PimPom;
    uint16 sfx_Skidding;
    uint16 sfx_Grab;
    uint16 sfx_Flying;
    bool32 playingFlySFX;
    uint16 sfx_Tired;
    uint16 field_A36;
    bool32 playingTiredSFX;
    uint16 sfx_Land;
    uint16 sfx_Slide;
    uint16 sfx_Outtahere;
    uint16 sfx_Transform2;
    uint16 sfx_Swap;
    uint16 sfx_SwapFail;
    uint16 sfx_MightyDeflect;
    uint16 sfx_MightyDrill;
    uint16 sfx_MightyLand;
    uint16 sfx_MightyUnspin;
    int32 raySwoopTimer;
    int32 rayDiveTimer;
    bool32 gotHit[4];
    StateMachine(configureGhostCB);
    bool32 (*canSuperCB)(bool32 isHUD);
    int32 superDashCooldown;
} ObjectPlayer;
#else
typedef struct {
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
    int32 flyCarryPosA;
    int32 flyCarryPosB;
    Vector2 flyCarryPositions[16];
    Vector2 curFlyCarryPos;
    int32 P2JumpActionDelay;
    int32 jumpInDelay;
    int32 p2InputDelay;
    uint8 value17;
    int32 rings;
    STATIC(int32 ringExtraLife, 100);
    int32 powerups;
    STATIC(int32 savedLives, 3);
    int32 savedScore;
    STATIC(int32 savedScore1UP, 50000);
    uint16 sonicSpriteIndex;
    uint16 superSpriteIndex;
    uint16 tailsTailsSpriteIndex;
    uint16 tailsSpriteIndex;
    uint16 knuxSpriteIndex;
    TABLE(colour superPalette_Sonic[18], { 0x000080, 0x0038C0, 0x0068F0, 0x1888F0, 0x30A0F0, 0x68D0F0, 0xF0C001, 0xF0D028, 0xF0E040, 0xF0E860,
                                           0xF0E898, 0xF0E8D0, 0xF0D898, 0xF0E0B0, 0xF0E8C0, 0xF0F0D8, 0xF0F0F0, 0xF0F0F8 });
    TABLE(colour superPalette_Tails[18], { 0x800801, 0xB01801, 0xD05001, 0xE07808, 0xE89008, 0xF0A801, 0xF03830, 0xF06848, 0xF09860, 0xF0B868,
                                           0xF0C870, 0xF0D870, 0xF03830, 0xF06848, 0xF09860, 0xF0B868, 0xF0C870, 0xF0D870 });
    TABLE(colour superPalette_Knux[18], { 0x580818, 0x980130, 0xD00840, 0xE82858, 0xF06080, 0xF08088, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0,
                                          0xF0B0C8, 0xF0C0C8, 0xF05878, 0xF06090, 0xF080A0, 0xF098B0, 0xF0B0C8, 0xF0C0C8 });
    TABLE(colour superPalette_Sonic_HCZ[18], { 0x200888, 0x3020C8, 0x3840F0, 0x4070F0, 0x4098F0, 0x40C0F0, 0x88C880, 0x68E090, 0x50F098, 0x68F0C0,
                                               0x78F0C8, 0xA0F0D8, 0x60E898, 0x48F0A0, 0x58F0B0, 0x68F0C0, 0x90F0D0, 0xA0F0D8 });
    TABLE(colour superPalette_Tails_HCZ[18], { 0x880808, 0xA03810, 0xA05848, 0xB07058, 0xC08068, 0xC89078, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893,
                                               0xE6B09D, 0xEABAA7, 0xCC6161, 0xDC8462, 0xD5978A, 0xDEA893, 0xE6B09D, 0xEABAA7 });
    TABLE(colour superPalette_Knux_HCZ[18], { 0x181050, 0x301090, 0x5018A8, 0x8828C0, 0xA048C0, 0xB868C8, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF,
                                              0xD086EB, 0xDE9CED, 0x746DA3, 0x7F65D0, 0x9768E0, 0xC070EF, 0xD086EB, 0xDE9CED });
    TABLE(colour superPalette_Sonic_CPZ[18], { 0x4000D8, 0x5800E0, 0x6810E0, 0x8020E0, 0xA020E0, 0xC040E0, 0xE04880, 0xE06890, 0xE078A8, 0xE078D8,
                                               0xE080E0, 0xE080E0, 0xE080B0, 0xE080B0, 0xE080C0, 0xE080C0, 0xE080E0, 0xE080E0 });
    TABLE(colour superPalette_Tails_CPZ[18], { 0xC80180, 0xD00178, 0xE00180, 0xE81088, 0xE83098, 0xE84898, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0,
                                               0xF098F0, 0xF0A0F0, 0xF078F0, 0xF078F0, 0xF080F0, 0xF088F0, 0xF098F0, 0xF0A0F0 });
    TABLE(colour superPalette_Knux_CPZ[18], { 0xA00180, 0xB00178, 0xC00190, 0xD001B0, 0xE001E0, 0xE820E8, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0,
                                              0xF0A8F0, 0xF0B0F0, 0xF078D8, 0xF078E8, 0xF088F0, 0xF098F0, 0xF0A8F0, 0xF0B0F0 });
    uint16 sfx_Jump;
    uint16 sfx_Roll;
    uint16 sfx_Charge;
    uint16 sfx_Release;
    uint16 sfx_PeelCharge;
    uint16 sfx_PeelRelease;
    uint16 sfx_Dropdash;
    uint16 sfx_LoseRings;
    uint16 sfx_Hurt;
    uint16 field_66E;
    uint16 sfx_Skidding;
    uint16 sfx_Grab;
    uint16 sfx_Flying;
    uint16 field_676;
    bool32 playingFlySFX;
    uint16 sfx_Tired;
    uint16 field_67E;
    bool32 playingTiredSFX;
    uint16 sfx_Land;
    uint16 sfx_Slide;
    uint16 sfx_Outtahere;
    uint16 sfx_Transform2;
    bool32 gotHit[4];
} ObjectPlayer;
#endif

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(nextAirState);
    StateMachine(nextGroundState);
    EntityCamera *camera;
    Animator playerAnimator;
    Animator tailAnimator;
    int32 maxWalkSpeed;
    int32 maxJogSpeed;
    int32 maxRunSpeed;
    int32 field_A4;
    int32 tailRotation;
    int32 tailDirection;
    uint16 spriteIndex;
    uint16 tailSpriteIndex;
    uint16 storedAnim;
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
    int32 hyperRing;
    int32 timer;
    int32 outtaHereTimer;
    int32 abilityTimer;
    int32 spindashCharge;
    int32 abilityValue;
    int32 airTimer;
    int32 invincibleTimer;
    int32 speedShoesTimer;
    int32 blinkTimer;
    int32 scrollDelay;
    int32 skidding;
    int32 pushing;
    int32 underwater;      // 0 = not in water, 1 = in palette water, else water entityID
    bool32 groundedStore;  // stored grounded state
    bool32 invertGravity;  // FBZ magnet flip
    bool32 isChibi;        // MMZ Chibi form flag
    bool32 forceTransform; // force transform flag
    int32 superState;
    int32 superSecondTimer;
    int32 superBlendAmount;
    int32 superBlendState;
    bool32 sidekick;
    int32 scoreBonus;
    int32 cameraOffset;
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
    Vector2 moveOffset;
    StateMachine(stateInputReplay);
    StateMachine(stateInput);
    int32 controllerID;
    int32 controlLock;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    int32 jumpAbility;
    int32 jumpAbilityTimer;
    StateMachine(movesetState);
    StateMachine(peeloutState);
    int32 flyCarryTimer;
    Vector2 sidekickPos;
    Vector2 leaderPos;
    uint8 hurtFlag;
    bool32 forceJumpIn;
#if RETRO_USE_PLUS
    bool32 isGhost;
#endif
    int32 abilityValues[8];
    void* abilityPtrs[8];
#if RETRO_USE_PLUS
    int32 uncurlTimer;
#endif
#if RETRO_USE_TOUCH_CONTROLS
    int32 touchJump;
#endif
} EntityPlayer;

// Object Struct
extern ObjectPlayer *Player;

// Standard Entity Events
void Player_Update(void);
void Player_LateUpdate(void);
void Player_StaticUpdate(void);
void Player_Draw(void);
void Player_Create(void *data);
void Player_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Player_EditorDraw(void);
void Player_EditorLoad(void);
#endif
void Player_Serialize(void);

// Extra Entity Functions
void Player_LoadSprites(void);
void Player_LoadSpritesVS(void);
void Player_SaveValues(void);
void Player_GiveScore(EntityPlayer *player, int32 score);
void Player_GiveRings(int32 amount, EntityPlayer *player, bool32 playSFX);
void Player_GiveLife(EntityPlayer *entity);
void Player_ApplyShieldEffect(EntityPlayer *player);
void Player_ChangeCharacter(EntityPlayer *entity, int32 character);
bool32 Player_CheckGoSuper(EntityPlayer *player, uint8 emeraldflags);
void Player_BlendSuperSonicColours(int32 bankID);
void Player_BlendSuperTailsColours(int32 bankID);
void Player_BlendSuperKnuxColours(int32 bankID);
#if RETRO_USE_PLUS
void Player_BlendSuperMightyColours(int32 bankID);
void Player_BlendSuperRayColours(int32 bankID);
#endif
void Player_HandleSuperForm(void);
bool32 Player_CheckKeyPress(void);
void Player_LoseRings(EntityPlayer *player, int32 rings, uint8 cPlane);
void Player_LoseHyperRings(EntityPlayer *player, int32 rings, uint8 cPlane);
//returns the pointer to the nearest player to the current entity on the x axis only
EntityPlayer *Player_GetNearestPlayerX(void);
// returns the pointer to the nearest player to the current entity on both the x & y axis'
EntityPlayer *Player_GetNearestPlayer(void);
#if RETRO_USE_PLUS
void Player_RemoveEncoreLeader(void);
#endif
void Player_ResetBoundaries(EntityPlayer *player);
void Player_State_TransportTube(void);
void Player_HandleDeath(EntityPlayer *player);
void Player_ResetState(EntityPlayer *player);

// Collision
//returns a pointer to the currently active player hitbox
Hitbox *Player_GetHitbox(EntityPlayer *player);
//performs a non-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionTouch(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
// performs an all-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionBox(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
// performs a top-solid box collision with the player, this handles setting the appropriate collision-related player variabes automatically
bool32 Player_CheckCollisionPlatform(EntityPlayer *player, void *entity, Hitbox *entityHitbox);

//Hits the player if not invulnerable, returns true if player was hit
bool32 Player_CheckHit(EntityPlayer *player, void *entity);
// the same as above, though the knockback dir is gotten from the player dir rather the direction of the distance
bool32 Player_CheckHitFlip(EntityPlayer *player);
// Hits the player if not invulnerable and the player's shield doesn't match shield, returns true if player was hit
bool32 Player_CheckElementalHit(EntityPlayer *player, void *entity, int shield);
//returns true if the player is in an "attacking" state
bool32 Player_CheckAttacking(EntityPlayer *player, void *e);
//checks if the player collided with an entity, this collision differs from the touch one above since it uses hammerdrop & instashield if appropriate
bool32 Player_CheckBadnikTouch(EntityPlayer *player, void *entity, Hitbox *entityHitbox);
//checks if the player is attacking the badnik, returns true if the player attacked the badnik, otherwise the player is hit and returns false
bool32 Player_CheckBadnikBreak(void *entity, EntityPlayer *player, bool32 destroy);
//similar to checkBadnikTouch, this time for bosses, handles rebounds and stuff properly, does NOT check for hammerdrop/instashield hitboxes. returns true if player hit the boss, otherwise the player is hit and returns false
bool32 Player_CheckBossHit(EntityPlayer *player, void *entity);
//similar to checkHit, but for projectiles, handles the rebound effect when using shields or crouching as mighty, returns true if deflected, otherwise the player is hit and returns false
bool32 Player_CheckProjectileHit(EntityPlayer *player, void *projectile);
//idk yet, needs more research
bool32 Player_CheckHit2(EntityPlayer *player, void *entity, bool32 flag);

// State helpers
void Player_ChangePhysicsState(EntityPlayer *entity);
void Player_HandleGroundMovement(void);
void Player_HandleGroundRotation(void);
void Player_HandleAirMovement(void);
void Player_HandleAirFriction(void);
void Player_StartJump(EntityPlayer *entity);
void Player_StartRoll(void);
bool32 Player_SwapMainPlayer(bool32 flag);
void Player_StartPeelout(void);
void Player_HandleRollDeceleration(void);
void Player_Hit(EntityPlayer *player);
bool32 Player_CheckValidState(EntityPlayer *player);
void Player_CheckStartFlyCarry(EntityPlayer *player);
void Player_P2JumpBackIn(void);
void Player_ForceSuperTransform(void);

// States
void Player_State_None(void);
void Player_State_Ground(void);
void Player_State_Air(void);
void Player_State_Roll(void);
void Player_State_ForceRoll_Ground(void);
void Player_State_ForceRoll_Air(void);
void Player_State_LookUp(void);
void Player_State_Crouch(void);
void Player_State_Spindash(void);
void Player_State_Peelout(void);
void Player_State_OuttaHere(void);
void Player_State_Transform(void);
void Player_State_Hit(void);
void Player_State_Die(void);
void Player_State_Drown(void);
void Player_State_DropDash(void);
void Player_State_BubbleBounce(void);
void Player_State_TailsFlight(void);
void Player_State_FlyCarried(void);
void Player_State_KnuxGlideLeft(void);
void Player_State_KnuxGlideRight(void);
void Player_State_KnuxGlideDrop(void);
void Player_State_GlideSlide(void);
void Player_State_KnuxWallClimb(void);
void Player_State_KnuxLedgePullUp(void);
#if RETRO_USE_PLUS
void Player_State_MightyHammerDrop(void);
void Player_State_MightyUnspin(void);
void Player_SpawnMightyHammerdropDust(int32 speed, Hitbox *hitbox);
bool32 Player_CheckMightyUnspin(int32 bounceDistance, EntityPlayer *player, bool32 checkHammerDrop, int32 *uncurlTimer);
void Player_State_RayGlide(void);
#endif
void Player_State_FlyIn(void);
void Player_State_JumpIn(void);
void Player_State_StartJumpIn(void);
void Player_EndFlyJumpIn(EntityPlayer *thisEntity, EntityPlayer *player);
void Player_State_EncoreRespawn(void);
void Player_State_Victory(void);
void Player_State_Bubble(void);
void Player_State_WaterSlide(void);

void Player_SonicJumpAbility(void);
void Player_TailsJumpAbility(void);
void Player_KnuxJumpAbility(void);
#if RETRO_USE_PLUS
void Player_MightyJumpAbility(void);
void Player_RayJumpAbility(void);

bool32 Player_CheckRayDiving(void);
bool32 Player_CheckRaySwooping(void);
void Player_UpdateRayDiveSFX(int32 sfxID);
void Player_UpdateRaySwoopSFX(int32 sfxID);
#endif

// Input States
void Player_ProcessP1Input(void);
void Player_ProcessP2InputLag(void);
void Player_ProcessP2Input_AI(void);
void Player_ProcessP2Input_None(void);
void Player_ProcessP2Input_Unknown(void);
void Player_ProcessP2Input_Player(void);

#endif //! OBJ_PLAYER_H
