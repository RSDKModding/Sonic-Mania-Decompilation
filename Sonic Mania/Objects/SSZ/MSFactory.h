#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxMachineActivate;
    uint16 sfxHullClose;
} ObjectMSFactory;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    Vector2 drawPos;
    Animator animator;
} EntityMSFactory;

// Object Struct
extern ObjectMSFactory *MSFactory;

// Standard Entity Events
void MSFactory_Update(void);
void MSFactory_LateUpdate(void);
void MSFactory_StaticUpdate(void);
void MSFactory_Draw(void);
void MSFactory_Create(void* data);
void MSFactory_StageLoad(void);
void MSFactory_EditorDraw(void);
void MSFactory_EditorLoad(void);
void MSFactory_Serialize(void);

// Extra Entity Functions
void MSFactory_Unknown1(void);
void MSFactory_Unknown2(void);
void MSFactory_Unknown3(void);
void MSFactory_Unknown4(void);
void MSFactory_Unknown5(void);

#endif //!OBJ_MSFACTORY_H
