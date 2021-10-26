#ifndef OBJ_BRIDGE_H
#define OBJ_BRIDGE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectBridge;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 length;
    bool32 burnable;
    uint8 field_60;
    uint8 activePlayerCount;
    uint8 timer;
    int32 stoodPos;
    int32 field_68;
    int32 field_6C;
    Entity *playerPtr;
    int32 startPos;
    int32 endPos;
    Animator animator;
    int32 dword94;
} EntityBridge;

// Object Struct
extern ObjectBridge *Bridge;

// Standard Entity Events
void Bridge_Update(void);
void Bridge_LateUpdate(void);
void Bridge_StaticUpdate(void);
void Bridge_Draw(void);
void Bridge_Create(void* data);
void Bridge_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Bridge_EditorDraw(void);
void Bridge_EditorLoad(void);
#endif
void Bridge_Serialize(void);

// Extra Entity Functions
void Bridge_Burn(int32 offset);


#endif //!OBJ_BRIDGE_H
