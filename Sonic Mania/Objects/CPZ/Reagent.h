#ifndef OBJ_REAGENT_H
#define OBJ_REAGENT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
    ushort sfxLand;
} ObjectReagent;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int timer;
    int startX;
    int field_68;
    int r;
    int g;
    int b;
    Animator animator;
} EntityReagent;

// Object Struct
extern ObjectReagent *Reagent;

// Standard Entity Events
void Reagent_Update(void);
void Reagent_LateUpdate(void);
void Reagent_StaticUpdate(void);
void Reagent_Draw(void);
void Reagent_Create(void* data);
void Reagent_StageLoad(void);
void Reagent_EditorDraw(void);
void Reagent_EditorLoad(void);
void Reagent_Serialize(void);

// Extra Entity Functions
void Reagent_Unknown1(void);
void Reagent_Unknown2(void);

#endif //!OBJ_REAGENT_H
