#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    int playerTimers[PLAYER_MAX];
    ushort sfxFreeze;
    ushort sfxLedgeBreak;
    ushort sfxWindowShatter;
    ushort sfxStruggle;
} ObjectIce;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    byte size;
    byte type;
    byte subType;
    byte subFlip;
    int bottomSmash;
    int knuxSmash;
    byte timer;
    byte animaionID;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Entity *playerPtr;
    Vector2 playerPos;
    Vector2 dwordDC;
    int dwordE4;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
} EntityIce;

// Object Struct
extern ObjectIce *Ice;

// Standard Entity Events
void Ice_Update(void);
void Ice_LateUpdate(void);
void Ice_StaticUpdate(void);
void Ice_Draw(void);
void Ice_Create(void *data);
void Ice_StageLoad(void);
void Ice_EditorDraw(void);
void Ice_EditorLoad(void);
void Ice_Serialize(void);

// Extra Entity Functions
void Ice_ZoneCB(void);

void Ice_Unknown2(Entity *p);
bool32 Ice_Unknown3(void);
bool32 Ice_Unknown4(void);
void Ice_State_FrozenPlayer(void);
void Ice_Unknown6(int a1, int a2, int a3, int velX, int velY, int a6);
void Ice_Unknown7(int velX, Entity *p, int velY);
void Ice_Unknown8(Entity *p);
Entity *Ice_Shatter(EntityIce *ice, int velX, int velY);
void Ice_TimeOverCB(void);

void Ice_Unknown11(void);
void Ice_Unknown12(void);
void Ice_Unknown13(void);
void Ice_Unknown14(void);
void Ice_Unknown15(void);
void Ice_Unknown16(void);
void Ice_Unknown17(void);

void Ice_StateDraw_Unknown1(void);
void Ice_StateDraw_Unknown2(void);
void Ice_StateDraw_Unknown3(void);
void Ice_StateDraw_Unknown4(void);

#endif //! OBJ_ICE_H
