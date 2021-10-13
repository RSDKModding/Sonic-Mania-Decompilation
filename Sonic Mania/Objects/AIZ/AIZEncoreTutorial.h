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
    int32 timer2;
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
void AIZEncoreTutorial_EditorDraw(void);
void AIZEncoreTutorial_EditorLoad(void);
void AIZEncoreTutorial_Serialize(void);

// Extra Entity Functions
void AIZEncoreTutorial_Unknown1(void);
void AIZEncoreTutorial_Unknown2(void);
void AIZEncoreTutorial_Unknown3(void);
void AIZEncoreTutorial_Unknown4(void);
void AIZEncoreTutorial_Unknown5(void);

#endif

#endif //!OBJ_AIZENCORETUTORIAL_H
