#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "Game.h"

typedef enum { GENERICTRIGGER_PSZ2_PETALSINACTIVE, GENERICTRIGGER_PSZ2_PETALSACTIVE } GenericTriggerTypesPSZ2;

// Object Class
struct ObjectPSZ2Setup {
    RSDK_OBJECT
    TABLE(int32 petalAniDurationTable[8], { 9, 7, 6, 7, 9, 7, 6, 7 });
    int32 petalAniDuration;
    int32 petalAniFrame;
    bool32 petalBehaviourActive;
    int16 petalTimer;
    uint16 aniTiles1;
    uint16 aniTiles2;
};

// Entity Class
struct EntityPSZ2Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectPSZ2Setup *PSZ2Setup;

// Standard Entity Events
void PSZ2Setup_Update(void);
void PSZ2Setup_LateUpdate(void);
void PSZ2Setup_StaticUpdate(void);
void PSZ2Setup_Draw(void);
void PSZ2Setup_Create(void *data);
void PSZ2Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZ2Setup_EditorDraw(void);
void PSZ2Setup_EditorLoad(void);
#endif
void PSZ2Setup_Serialize(void);

// Extra Entity Functions
void PSZ2Setup_Trigger_ActivatePetalBehaviour(void);
void PSZ2Setup_Trigger_DeactivatePetalBehaviour(void);
void PSZ2Setup_ActTransitionLoad(void);
void PSZ2Setup_StageFinish_EndAct2(void);

#endif //! OBJ_PSZ2SETUP_H
