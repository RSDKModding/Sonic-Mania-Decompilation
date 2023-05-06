#ifndef OBJ_VANISHPLATFORM_H
#define OBJ_VANISHPLATFORM_H

#include "Game.h"

// Object Class
struct ObjectVanishPlatform {
    RSDK_OBJECT
};

// Entity Class
struct EntityVanishPlatform {
    MANIA_PLATFORM_BASE
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
};

// Object Struct
extern ObjectVanishPlatform *VanishPlatform;

// Standard Entity Events
void VanishPlatform_Update(void);
void VanishPlatform_LateUpdate(void);
void VanishPlatform_StaticUpdate(void);
void VanishPlatform_Draw(void);
void VanishPlatform_Create(void *data);
void VanishPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void VanishPlatform_EditorDraw(void);
void VanishPlatform_EditorLoad(void);
#endif
void VanishPlatform_Serialize(void);

// Extra Entity Functions
void VanishPlatform_State_Appear(void);
void VanishPlatform_State_Disappear(void);

#endif //! OBJ_VANISHPLATFORM_H
