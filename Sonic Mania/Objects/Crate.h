#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxExplosion2;
} ObjectCrate;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 amplitude;
    int speed;
    int hasTension;
    sbyte frameID;
    byte collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int collapseDelay;
    int stoodAngle;
    byte stoodPlayers;
    byte pushPlayersL;
    byte pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int childCount;
    bool32 ignoreItemBox;
} EntityCrate;

// Object Struct
extern ObjectCrate *Crate;

// Standard Entity Events
void Crate_Update(void);
void Crate_LateUpdate(void);
void Crate_StaticUpdate(void);
void Crate_Draw(void);
void Crate_Create(void* data);
void Crate_StageLoad(void);
void Crate_EditorDraw(void);
void Crate_EditorLoad(void);
void Crate_Serialize(void);

// Extra Entity Functions
void Crate_Unknown1(void *e);
void Crate_Unknown2(void *e, int offset);
bool32 Crate_Unknown3(void);
void Crate_Unknown4(void);
void Crate_Unknown5(void);
void Crate_Unknown6(void);
void Crate_Unknown7(void);

#endif //!OBJ_CRATE_H
