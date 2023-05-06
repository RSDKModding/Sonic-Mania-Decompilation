#ifndef OBJ_MONARCHBG_H
#define OBJ_MONARCHBG_H

#include "Game.h"

// Object Class
struct ObjectMonarchBG {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 meshIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityMonarchBG {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    Animator animatorTop;
    Animator animatorBottom;
    Matrix unusedMatrix1;
    Matrix unusedMatrix2;
    Matrix unusedMatrix3;
    Matrix unusedMatrix4;
};

// Object Struct
extern ObjectMonarchBG *MonarchBG;

// Standard Entity Events
void MonarchBG_Update(void);
void MonarchBG_LateUpdate(void);
void MonarchBG_StaticUpdate(void);
void MonarchBG_Draw(void);
void MonarchBG_Create(void *data);
void MonarchBG_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MonarchBG_EditorDraw(void);
void MonarchBG_EditorLoad(void);
#endif
void MonarchBG_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_MONARCHBG_H
