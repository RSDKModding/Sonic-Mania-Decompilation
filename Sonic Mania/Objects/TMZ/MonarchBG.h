#ifndef OBJ_MONARCHBG_H
#define OBJ_MONARCHBG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 meshIndex;
    uint16 sceneIndex;
} ObjectMonarchBG;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    Animator animatorTop;
    Animator animatorBottom;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
} EntityMonarchBG;

// Object Struct
extern ObjectMonarchBG *MonarchBG;

// Standard Entity Events
void MonarchBG_Update(void);
void MonarchBG_LateUpdate(void);
void MonarchBG_StaticUpdate(void);
void MonarchBG_Draw(void);
void MonarchBG_Create(void* data);
void MonarchBG_StageLoad(void);
void MonarchBG_EditorDraw(void);
void MonarchBG_EditorLoad(void);
void MonarchBG_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MONARCHBG_H
