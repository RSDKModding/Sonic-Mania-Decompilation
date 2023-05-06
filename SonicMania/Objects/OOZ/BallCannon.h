#ifndef OBJ_BALLCANNON_H
#define OBJ_BALLCANNON_H

#include "Game.h"

typedef enum {
    BALLCANNON_CANNON,
    BALLCANNON_CORKV,
    BALLCANNON_CORKH,
} BallCannonTypes;

typedef enum {
    // Clockwise by default
    BALLCANNON_DIR_RIGHT_CW,
    BALLCANNON_DIR_DOWN_CW,
    BALLCANNON_DIR_LEFT_CW,
    BALLCANNON_DIR_UP_CW,
    BALLCANNON_DIR_DOWN_CCW,
    BALLCANNON_DIR_LEFT_CCW,
    BALLCANNON_DIR_UP_CCW,
    BALLCANNON_DIR_RIGHT_CCW,
} BallCannonAngles;

// Object Class
struct ObjectBallCannon {
    RSDK_OBJECT
    Hitbox hitboxCannon;
    Hitbox hitboxCorkBlock;
    Hitbox hitboxCorkEntry;
    uint16 aniFrames;
    TABLE(int32 corkDebrisOffset[32], { -0xC0000, -0xC0000, -0x40000, -0xC0000, 0x40000,  -0xC0000, 0xC0000, -0xC0000, -0xC0000, -0x40000, -0x40000,
                                        -0x40000, 0x40000,  -0x40000, 0xC0000,  -0x40000, -0xC0000, 0x40000, -0x40000, 0x40000,  0x40000,  0x40000,
                                        0xC0000,  0x40000,  -0xC0000, 0xC0000,  -0x40000, 0xC0000,  0x40000, 0xC0000,  0xC0000,  0xC0000 });
    TABLE(int32 corkDebrisVelocity[32], { -0x40000, -0x40000, -0x20000, -0x40000, 0x20000,  -0x40000, 0x40000, -0x40000, -0x3C000, -0x20000, -0x1C000,
                                          -0x20000, 0x1C000,  -0x20000, 0x3C000,  -0x20000, -0x38000, 0x20000, -0x18000, 0x20000,  0x18000,  0x20000,
                                          0x38000,  0x20000,  -0x34000, 0x40000,  -0x14000, 0x40000,  0x14000, 0x40000,  0x34000,  0x40000 });
    uint16 sfxLedgeBreak;
    uint16 sfxFire;
};

// Entity Class
struct EntityBallCannon {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    bool32 exit;
    uint8 unused1;
    uint8 activePlayers;
    uint8 playerTimers[PLAYER_COUNT];
    int32 unused2;
    int32 rotationSpeed;
    Animator animator;
};

// Object Struct
extern ObjectBallCannon *BallCannon;

// Standard Entity Events
void BallCannon_Update(void);
void BallCannon_LateUpdate(void);
void BallCannon_StaticUpdate(void);
void BallCannon_Draw(void);
void BallCannon_Create(void *data);
void BallCannon_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BallCannon_EditorDraw(void);
void BallCannon_EditorLoad(void);
#endif
void BallCannon_Serialize(void);

// Extra Entity Functions
void BallCannon_CheckPlayerEntry(void);
void BallCannon_State_Idle(void);
void BallCannon_State_Inserted(void);
void BallCannon_State_Turning(void);
void BallCannon_State_EjectPlayer(void);
void BallCannon_State_CorkBlocked(void);
void BallCannon_State_CorkOpened(void);
void BallCannon_State_CorkDebris(void);

#endif //! OBJ_BALLCANNON_H
