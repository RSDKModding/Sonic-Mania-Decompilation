#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

#include "Game.h"

// Object Class
struct ObjectLightBarrier {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityLightBarrier {
    RSDK_ENTITY
    int32 size;
    bool32 enabled;
    int32 timer;
    int32 sprY;
    int32 barrierAlpha;
    Hitbox hitboxBarrier;
    Animator emitterAnimator;
    Animator barrierAnimator;
};

// Object Struct
extern ObjectLightBarrier *LightBarrier;

// Standard Entity Events
void LightBarrier_Update(void);
void LightBarrier_LateUpdate(void);
void LightBarrier_StaticUpdate(void);
void LightBarrier_Draw(void);
void LightBarrier_Create(void *data);
void LightBarrier_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LightBarrier_EditorDraw(void);
void LightBarrier_EditorLoad(void);
#endif
void LightBarrier_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_LIGHTBARRIER_H
