#ifndef OBJ_MMZLIGHTNING_H
#define OBJ_MMZLIGHTNING_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectMMZLightning;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int field_60;
    int field_64;
    Animator animator;
} EntityMMZLightning;

// Object Struct
extern ObjectMMZLightning *MMZLightning;

// Standard Entity Events
void MMZLightning_Update(void);
void MMZLightning_LateUpdate(void);
void MMZLightning_StaticUpdate(void);
void MMZLightning_Draw(void);
void MMZLightning_Create(void* data);
void MMZLightning_StageLoad(void);
void MMZLightning_EditorDraw(void);
void MMZLightning_EditorLoad(void);
void MMZLightning_Serialize(void);

// Extra Entity Functions
void MMZLightning_Unknown1(void);
void MMZLightning_Unknown2(void);
void MMZLightning_Unknown3(void);
void MMZLightning_Unknown4(void);
void MMZLightning_Unknown5(void);
void MMZLightning_Unknown6(void);
void MMZLightning_Unknown7(void);

#endif

#endif //!OBJ_MMZLIGHTNING_H
