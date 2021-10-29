#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 activePlayers;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxRush;
} ObjectCurrent;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    Vector2 size;
    uint8 strength;
    Hitbox hitbox;
    uint8 activePlayers;
    Vector2 playerPositions[4];
    int32 buttonTag;
    EntityButton *taggedButton;
    bool32 activated;
    int32 planeFilter;
    bool32 waterOnly;
    bool32 fbzAchievement;
    Animator animator;
} EntityCurrent;

// Object Struct
extern ObjectCurrent *Current;

// Standard Entity Events
void Current_Update(void);
void Current_LateUpdate(void);
void Current_StaticUpdate(void);
void Current_Draw(void);
void Current_Create(void* data);
void Current_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Current_EditorDraw(void);
void Current_EditorLoad(void);
#endif
void Current_Serialize(void);

// Extra Entity Functions
void Current_GetTaggedButton(void);
Vector2 Current_Unknown2(uint8 a1);
Vector2 Current_Unknown3(uint8 a1);

void Current_State_Type0(void);
void Current_State_Type1(void);
void Current_State_Type2(void);
void Current_State_Type3(void);
void Current_State_Type4(void);
void Current_State_Type5(void);
void Current_State_Type6(void);
void Current_State_Type7(void);
void Current_State_LaundoMobile(void);

void Current_Player_State_Type0(void);
void Current_Player_State_Type1(void);
void Current_Player_State_Type2(void);
void Current_Player_State_Type3(void);

#endif //!OBJ_CURRENT_H
