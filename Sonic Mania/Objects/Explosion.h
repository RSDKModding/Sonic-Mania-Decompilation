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
    EntityAnimationData data;
    int planeFilter;
} EntityExplosion;

// Object Struct
extern ObjectExplosion *Explosion;

// Standard Entity Events
void Explosion_Update();
void Explosion_LateUpdate();
void Explosion_StaticUpdate();
void Explosion_Draw();
void Explosion_Create(void* data);
void Explosion_StageLoad();
void Explosion_EditorDraw();
void Explosion_EditorLoad();
void Explosion_Serialize();

// Extra Entity Functions


#endif //!OBJ_EXPLOSION_H
