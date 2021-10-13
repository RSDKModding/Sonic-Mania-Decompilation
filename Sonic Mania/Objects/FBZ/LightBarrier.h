#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectLightBarrier;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int size;
    bool32 enabled;
    int timer;
    int sprY;
    int alphaTimer;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
} EntityLightBarrier;

// Object Struct
extern ObjectLightBarrier *LightBarrier;

// Standard Entity Events
void LightBarrier_Update(void);
void LightBarrier_LateUpdate(void);
void LightBarrier_StaticUpdate(void);
void LightBarrier_Draw(void);
void LightBarrier_Create(void* data);
void LightBarrier_StageLoad(void);
void LightBarrier_EditorDraw(void);
void LightBarrier_EditorLoad(void);
void LightBarrier_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LIGHTBARRIER_H
