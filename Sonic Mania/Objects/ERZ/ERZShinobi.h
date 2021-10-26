#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectERZShinobi;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 field_60;
    int32 timer;
    int32 invincibilityTimer;
    int32 field_6C;
    int32 rotStore;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityERZShinobi;

// Object Struct
extern ObjectERZShinobi *ERZShinobi;

// Standard Entity Events
void ERZShinobi_Update(void);
void ERZShinobi_LateUpdate(void);
void ERZShinobi_StaticUpdate(void);
void ERZShinobi_Draw(void);
void ERZShinobi_Create(void* data);
void ERZShinobi_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZShinobi_EditorDraw(void);
void ERZShinobi_EditorLoad(void);
#endif
void ERZShinobi_Serialize(void);

// Extra Entity Functions
void ERZShinobi_CheckHit(void);
void ERZShinobi_HandleTileCollisions(void);
void ERZShinobi_Unknown3(void);

#endif //!OBJ_ERZSHINOBI_H
