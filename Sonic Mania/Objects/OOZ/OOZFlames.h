#ifndef OBJ_OOZFLAMES_H
#define OBJ_OOZFLAMES_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectOOZFlames;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 field_60;
    Animator animator;
} EntityOOZFlames;

// Object Struct
extern ObjectOOZFlames *OOZFlames;

// Standard Entity Events
void OOZFlames_Update(void);
void OOZFlames_LateUpdate(void);
void OOZFlames_StaticUpdate(void);
void OOZFlames_Draw(void);
void OOZFlames_Create(void* data);
void OOZFlames_StageLoad(void);
void OOZFlames_EditorDraw(void);
void OOZFlames_EditorLoad(void);
void OOZFlames_Serialize(void);

// Extra Entity Functions
void OOZFlames_Unknown1(void);
void OOZFlames_Unknown2(void);


#endif //!OBJ_OOZFLAMES_H
