#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "Game.h"

// Object Class
struct ObjectMine {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxMine;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityMine {
    RSDK_ENTITY
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectMine *Mine;

// Standard Entity Events
void Mine_Update(void);
void Mine_LateUpdate(void);
void Mine_StaticUpdate(void);
void Mine_Draw(void);
void Mine_Create(void *data);
void Mine_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Mine_EditorDraw(void);
void Mine_EditorLoad(void);
#endif
void Mine_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void Mine_CheckMightyHit(void);
#endif

#endif //! OBJ_MINE_H
