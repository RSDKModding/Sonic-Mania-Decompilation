#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "SonicMania.h"

// Object Class
struct ObjectPhantomHand {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[41], { 10,       0, 0, -0x20000, -0x10000, 1, 0, -0x10000, -0x10000, 2, 0, 0x10000, -0x10000, 3, 0, 0x20000,
                                  -0x10000, 4, 0, -0x20000, -0x8000,  5, 0, -0x10000, -0x8000,  6, 0, 0x10000, -0x8000,  7, 0, 0x20000,
                                  0x8000,   8, 0, 0x20000,  -0x8000,  9, 0, 0x20000,  -0x4000 });
    bool32 grabbedPlayers[4];
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityPhantomHand {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer2;
    int32 timer;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    Entity* parent;
    int32 field_78;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectPhantomHand *PhantomHand;

// Standard Entity Events
void PhantomHand_Update(void);
void PhantomHand_LateUpdate(void);
void PhantomHand_StaticUpdate(void);
void PhantomHand_Draw(void);
void PhantomHand_Create(void* data);
void PhantomHand_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomHand_EditorDraw(void);
void PhantomHand_EditorLoad(void);
#endif
void PhantomHand_Serialize(void);

// Extra Entity Functions
void PhantomHand_CheckPlayerGrab(int playerX, int playerY);

void PhantomHand_State_Summon(void);
void PhantomHand_State_Unknown1(void);
void PhantomHand_State_Unknown2(void);
void PhantomHand_State_Unknown3(void);
void PhantomHand_State_Unknown5(void);
void PhantomHand_State_Unknown6(void);
void PhantomHand_State_Unknown7(void);
void PhantomHand_State_Unknown8(void);
void PhantomHand_State_Unknown9(void);
void PhantomHand_State_Unknown4(void);

#endif //!OBJ_PHANTOMHAND_H
