#ifndef OBJ_EXPLOSION_H
#define OBJ_EXPLOSION_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_Destroy;
} ObjectExplosion;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    int planeFilter;
} EntityExplosion;

// Object Struct
extern ObjectExplosion *Explosion;

// Standard Entity Events
void Explosion_Update(void);
void Explosion_LateUpdate(void);
void Explosion_StaticUpdate(void);
void Explosion_Draw(void);
void Explosion_Create(void* data);
void Explosion_StageLoad(void);
void Explosion_EditorDraw(void);
void Explosion_EditorLoad(void);
void Explosion_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EXPLOSION_H
