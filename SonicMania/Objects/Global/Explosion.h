#ifndef OBJ_EXPLOSION_H
#define OBJ_EXPLOSION_H

#include "Game.h"

typedef enum {
    EXPLOSION_ITEMBOX,
    EXPLOSION_ENEMY,
    EXPLOSION_BOSS,
    EXPLOSION_BOSSPUFF,
} ExplosionTypes;

// Object Class
struct ObjectExplosion {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxDestroy;
};

// Entity Class
struct EntityExplosion {
    RSDK_ENTITY
    Animator animator;
    int32 planeFilter;
};

// Object Struct
extern ObjectExplosion *Explosion;

// Standard Entity Events
void Explosion_Update(void);
void Explosion_LateUpdate(void);
void Explosion_StaticUpdate(void);
void Explosion_Draw(void);
void Explosion_Create(void *data);
void Explosion_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Explosion_EditorDraw(void);
void Explosion_EditorLoad(void);
#endif
void Explosion_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_EXPLOSION_H
