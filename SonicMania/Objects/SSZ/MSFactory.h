#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

#include "Game.h"

// Object Class
struct ObjectMSFactory {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxMachineActivate;
    uint16 sfxHullClose;
};

// Entity Class
struct EntityMSFactory {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 drawPos;
    Animator animator;
};

// Object Struct
extern ObjectMSFactory *MSFactory;

// Standard Entity Events
void MSFactory_Update(void);
void MSFactory_LateUpdate(void);
void MSFactory_StaticUpdate(void);
void MSFactory_Draw(void);
void MSFactory_Create(void *data);
void MSFactory_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSFactory_EditorDraw(void);
void MSFactory_EditorLoad(void);
#endif
void MSFactory_Serialize(void);

// Extra Entity Functions
void MSFactory_State_SetupFactory(void);
void MSFactory_State_SetupMetalSonic(void);
void MSFactory_State_OpeningDoor(void);
void MSFactory_State_CreateSilverSonic(void);
void MSFactory_State_CloseDoor(void);

#endif //! OBJ_MSFACTORY_H
