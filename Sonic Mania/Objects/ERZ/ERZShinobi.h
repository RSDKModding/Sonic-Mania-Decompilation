#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "SonicMania.h"

// Object Class
struct ObjectERZShinobi {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityERZShinobi {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 spearOffset;
    int32 rotStore;
    int32 unused;
    int32 numBounces;
    bool32 prevOnGround;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

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
void ERZShinobi_CheckPlayerCollisions(void);
void ERZShinobi_HandleTileCollisions(void);
void ERZShinobi_Unknown3(void);

#endif //!OBJ_ERZSHINOBI_H
