#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectERZShinobi;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int field_60;
    int timer;
    int invincibilityTimer;
    int field_6C;
    int rotStore;
    int field_74;
    int field_78;
    int field_7C;
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
void ERZShinobi_EditorDraw(void);
void ERZShinobi_EditorLoad(void);
void ERZShinobi_Serialize(void);

// Extra Entity Functions
void ERZShinobi_CheckHit(void);
void ERZShinobi_HandleTileCollisions(void);
void ERZShinobi_Unknown3(void);

#endif //!OBJ_ERZSHINOBI_H
