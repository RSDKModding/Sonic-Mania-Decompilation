#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "Game.h"

typedef enum { BUTTON_FLOOR, BUTTON_ROOF, BUTTON_RWALL, BUTTON_LWALL } ButtonTypes;

// Object Class
struct ObjectButton {
    RSDK_OBJECT
    uint16 aniFrames;
    bool32 hasEggman;
    bool32 hasPhantomRider;
    Hitbox hitboxV;
    Hitbox hitboxH;
    int32 activatePos;
    int32 buttonOffset;
    int32 hitboxOffset;
    int32 unused1; // set in stageload, but never used. no way to tell what it is
    uint16 sfxButton;
};

// Entity Class
struct EntityButton {
    MANIA_BUTTON_BASE

    Hitbox hitboxButton;
    Animator buttonAnimator;
    Animator baseAnimator;
};

// Object Struct
extern ObjectButton *Button;

// Standard Entity Events
void Button_Update(void);
void Button_LateUpdate(void);
void Button_StaticUpdate(void);
void Button_Draw(void);
void Button_Create(void *data);
void Button_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Button_EditorDraw(void);
void Button_EditorLoad(void);
#endif
void Button_Serialize(void);

// Extra Entity Functions
void Button_CheckEggmanCollisions(void);
void Button_CheckPRiderCollisions(void);

void Button_HandleFloor(void);
void Button_HandleRoof(void);
void Button_HandleRWall(void);
void Button_HandleLWall(void);

#endif //! OBJ_BUTTON_H
