#ifndef OBJ_PLATFORMCONTROL_H
#define OBJ_PLATFORMCONTROL_H

#include "Game.h"
#include "Button.h"
#include "Platform.h"
#include "PlatformNode.h"

typedef enum {
    PLATFORMCONTROL_CIRCUIT,
    PLATFORMCONTROL_REVERSE,
    PLATFORMCONTROL_TELEPORT,
    PLATFORMCONTROL_STOP,
} PlatformControlTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectPlatformControl;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 nodeCount;
    int32 childCount;
    int32 speed;
    bool32 setActive;
    uint8 type;
    bool32 isActive;
    int32 buttonTag;
    EntityButton *taggedButton;
} EntityPlatformControl;

// Object Struct
extern ObjectPlatformControl *PlatformControl;

// Standard Entity Events
void PlatformControl_Update(void);
void PlatformControl_LateUpdate(void);
void PlatformControl_StaticUpdate(void);
void PlatformControl_Draw(void);
void PlatformControl_Create(void *data);
void PlatformControl_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlatformControl_EditorDraw(void);
void PlatformControl_EditorLoad(void);
#endif
void PlatformControl_Serialize(void);

// Extra Entity Functions
void PlatformControl_ManagePlatformVelocity(EntityPlatform *platform, EntityPlatformNode *node);

#endif //! OBJ_PLATFORMCONTROL_H
