#ifndef OBJ_PLATFORMNODE_H
#define OBJ_PLATFORMNODE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectPlatformNode;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int nodeFlag;
    Animator animator;
} EntityPlatformNode;

// Object Struct
extern ObjectPlatformNode *PlatformNode;

// Standard Entity Events
void PlatformNode_Update(void);
void PlatformNode_LateUpdate(void);
void PlatformNode_StaticUpdate(void);
void PlatformNode_Draw(void);
void PlatformNode_Create(void* data);
void PlatformNode_StageLoad(void);
void PlatformNode_EditorDraw(void);
void PlatformNode_EditorLoad(void);
void PlatformNode_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PLATFORMNODE_H
