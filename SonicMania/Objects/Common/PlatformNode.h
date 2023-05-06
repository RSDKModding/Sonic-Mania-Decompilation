#ifndef OBJ_PLATFORMNODE_H
#define OBJ_PLATFORMNODE_H

#include "Game.h"

// Object Class
struct ObjectPlatformNode {
    RSDK_OBJECT
    uint16 aniFrames; // used only in-editor
};

// Entity Class
struct EntityPlatformNode {
    RSDK_ENTITY
    int32 nodeFlag;
    Animator animator;
};

// Object Struct
extern ObjectPlatformNode *PlatformNode;

// Standard Entity Events
void PlatformNode_Update(void);
void PlatformNode_LateUpdate(void);
void PlatformNode_StaticUpdate(void);
void PlatformNode_Draw(void);
void PlatformNode_Create(void *data);
void PlatformNode_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlatformNode_EditorDraw(void);
void PlatformNode_EditorLoad(void);
#endif
void PlatformNode_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PLATFORMNODE_H
