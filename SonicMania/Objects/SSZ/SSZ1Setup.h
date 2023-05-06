#ifndef OBJ_SSZ1SETUP_H
#define OBJ_SSZ1SETUP_H

#include "Game.h"

typedef enum { GENERICTRIGGER_SSZ1_DESTROYHOTARUMKII } GenericTriggerTypesSSZ1;

typedef enum {
    SSZ1_DECOR_GOALPOST,
} DecorTypesSSZ1;

// Object Class
struct ObjectSSZ1Setup {
    RSDK_OBJECT
    int32 unused;
    TileLayer *background2;
    EntitySSZ1Outro *outro;
};

// Entity Class
struct EntitySSZ1Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectSSZ1Setup *SSZ1Setup;

// Standard Entity Events
void SSZ1Setup_Update(void);
void SSZ1Setup_LateUpdate(void);
void SSZ1Setup_StaticUpdate(void);
void SSZ1Setup_Draw(void);
void SSZ1Setup_Create(void *data);
void SSZ1Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZ1Setup_EditorDraw(void);
void SSZ1Setup_EditorLoad(void);
#endif
void SSZ1Setup_Serialize(void);

// Extra Entity Functions
void SSZ1Setup_StageFinish_EndAct1(void);

#endif //! OBJ_SSZ1SETUP_H
