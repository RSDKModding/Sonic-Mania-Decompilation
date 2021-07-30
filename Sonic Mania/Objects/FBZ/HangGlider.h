#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectHangGlider;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    Entity *playerPtr;
    Animator animator1;
    Animator animator2;
    Animator animator4;
    Animator animator3;
} EntityHangGlider;

// Object Struct
extern ObjectHangGlider *HangGlider;

// Standard Entity Events
void HangGlider_Update(void);
void HangGlider_LateUpdate(void);
void HangGlider_StaticUpdate(void);
void HangGlider_Draw(void);
void HangGlider_Create(void* data);
void HangGlider_StageLoad(void);
void HangGlider_EditorDraw(void);
void HangGlider_EditorLoad(void);
void HangGlider_Serialize(void);

// Extra Entity Functions
void HangGlider_Unknown1(void);
void HangGlider_Unknown2(void);


#endif //!OBJ_HANGGLIDER_H
