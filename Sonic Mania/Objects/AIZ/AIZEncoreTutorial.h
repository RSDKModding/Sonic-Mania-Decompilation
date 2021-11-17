#ifndef OBJ_AIZENCORETUTORIAL_H
#define OBJ_AIZENCORETUTORIAL_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 cutsceneFrames;
    uint16 dustFrames;
} ObjectAIZEncoreTutorial;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 swapCount;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityAIZEncoreTutorial;

// Object Struct
extern ObjectAIZEncoreTutorial *AIZEncoreTutorial;

// Standard Entity Events
void AIZEncoreTutorial_Update(void);
void AIZEncoreTutorial_LateUpdate(void);
void AIZEncoreTutorial_StaticUpdate(void);
void AIZEncoreTutorial_Draw(void);
void AIZEncoreTutorial_Create(void* data);
void AIZEncoreTutorial_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void AIZEncoreTutorial_EditorDraw(void);
void AIZEncoreTutorial_EditorLoad(void);
#endif
void AIZEncoreTutorial_Serialize(void);

// Extra Entity Functions
void AIZEncoreTutorial_State_ShowTutBubble(void);
void AIZEncoreTutorial_State_EnterTutorial(void);
void AIZEncoreTutorial_State_ShowSwapTutorial(void);
void AIZEncoreTutorial_State_ExitTutorial(void);
void AIZEncoreTutorial_State_ReturnToCutscene(void);

#endif

#endif //!OBJ_AIZENCORETUTORIAL_H
