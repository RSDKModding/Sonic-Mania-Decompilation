#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectHangConveyor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 length;
    uint8 activePlayers1;
    uint8 activePlayers2;
    uint8 activePlayers3;
    uint8 activePlayers4;
    Vector2 field_64[4];
    int32 field_84[4];
    int32 field_94[4];
    Vector2 field_A4[4];
    int32 field_C4[4];
    Vector2 startPos;
    int32 field_DC;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityHangConveyor;

// Object Struct
extern ObjectHangConveyor *HangConveyor;

// Standard Entity Events
void HangConveyor_Update(void);
void HangConveyor_LateUpdate(void);
void HangConveyor_StaticUpdate(void);
void HangConveyor_Draw(void);
void HangConveyor_Create(void* data);
void HangConveyor_StageLoad(void);
void HangConveyor_EditorDraw(void);
void HangConveyor_EditorLoad(void);
void HangConveyor_Serialize(void);

// Extra Entity Functions
void HangConveyor_DrawSprites(void);

void HangConveyor_Unknown1(void);
void HangConveyor_Unknown2(void);


#endif //!OBJ_HANGCONVEYOR_H
