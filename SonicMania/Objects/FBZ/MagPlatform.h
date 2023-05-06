#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "Game.h"

// Object Class
struct ObjectMagPlatform {
    RSDK_OBJECT
    uint16 sfxChain;
};

// Entity Class
struct EntityMagPlatform {
    MANIA_PLATFORM_BASE

    int32 length;
};

// Object Struct
extern ObjectMagPlatform *MagPlatform;

// Standard Entity Events
void MagPlatform_Update(void);
void MagPlatform_LateUpdate(void);
void MagPlatform_StaticUpdate(void);
void MagPlatform_Draw(void);
void MagPlatform_Create(void *data);
void MagPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MagPlatform_EditorDraw(void);
void MagPlatform_EditorLoad(void);
#endif
void MagPlatform_Serialize(void);

// Extra Entity Functions
void MagPlatform_Collide_SolidAllHazardBottom(void);
void MagPlatform_State_Idle(void);
void MagPlatform_State_Rise(void);
void MagPlatform_State_Fall(void);

#endif //! OBJ_MAGPLATFORM_H
