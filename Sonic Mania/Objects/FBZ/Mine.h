#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxExplosion;
} ObjectMine;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 timer;
    Animator animator;
} EntityMine;

// Object Struct
extern ObjectMine *Mine;

// Standard Entity Events
void Mine_Update(void);
void Mine_LateUpdate(void);
void Mine_StaticUpdate(void);
void Mine_Draw(void);
void Mine_Create(void* data);
void Mine_StageLoad(void);
void Mine_EditorDraw(void);
void Mine_EditorLoad(void);
void Mine_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void Mine_CheckMightyHit(void);
#endif

#endif //!OBJ_MINE_H
