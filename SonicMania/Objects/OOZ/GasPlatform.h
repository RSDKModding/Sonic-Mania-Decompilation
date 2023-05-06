#ifndef OBJ_GASPLATFORM_H
#define OBJ_GASPLATFORM_H

#include "Game.h"

typedef enum {
    GASPLATFORM_INTERVAL,
    GASPLATFORM_CONTACT,
    GASPLATFORM_BOSS,
} GasPlatformTypes;

// Object Class
struct ObjectGasPlatform {
    RSDK_OBJECT
    Hitbox hitboxGas;
    Vector2 range;
    uint16 sfxGasPop;
    uint16 sfxSpring;
};

// Entity Class
struct EntityGasPlatform {
    MANIA_PLATFORM_BASE
    uint16 interval;
    uint16 intervalOffset;
    Animator gasAnimator;
};

// Object Struct
extern ObjectGasPlatform *GasPlatform;

// Standard Entity Events
void GasPlatform_Update(void);
void GasPlatform_LateUpdate(void);
void GasPlatform_StaticUpdate(void);
void GasPlatform_Draw(void);
void GasPlatform_Create(void *data);
void GasPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GasPlatform_EditorDraw(void);
void GasPlatform_EditorLoad(void);
#endif
void GasPlatform_Serialize(void);

// Extra Entity Functions
void GasPlatform_PopPlatform(void);
void GasPlatform_State_Popped(void);
void GasPlatform_State_SpringCooldown(void);
void GasPlatform_State_Shaking(void);

#endif //! OBJ_GASPLATFORM_H
