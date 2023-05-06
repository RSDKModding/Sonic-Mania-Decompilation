#ifndef OBJ_EGGJANKENPART_H
#define OBJ_EGGJANKENPART_H

#include "Game.h"

typedef enum {
    EGGJANKENPART_FACEPLATE,
    EGGJANKENPART_FACEPLATE_TL,
    EGGJANKENPART_FACEPLATE_TR,
    EGGJANKENPART_FACEPLATE_BL,
    EGGJANKENPART_FACEPLATE_BR,
    EGGJANKENPART_SIDE_L,
    EGGJANKENPART_SIDE_R,
    EGGJANKENPART_BALL_TL,
    EGGJANKENPART_BALL_TR,
    EGGJANKENPART_BALL_BL,
    EGGJANKENPART_BALL_BR,
    EGGJANKENPART_BUTTON,
} EggJankenPart_Parts;

// Object Class
struct ObjectEggJankenPart {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityEggJankenPart {
    RSDK_ENTITY
    Animator animator;
};

// Object Entity
extern ObjectEggJankenPart *EggJankenPart;

// Standard Entity Events
void EggJankenPart_Update(void);
void EggJankenPart_LateUpdate(void);
void EggJankenPart_StaticUpdate(void);
void EggJankenPart_Draw(void);
void EggJankenPart_Create(void *data);
void EggJankenPart_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggJankenPart_EditorDraw(void);
void EggJankenPart_EditorLoad(void);
#endif
void EggJankenPart_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_EGGJANKENPART_H
