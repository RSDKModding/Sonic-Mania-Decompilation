#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

#include "Game.h"

typedef enum {
    BUTTONDOOR_UP,
    BUTTONDOOR_LEFT,
    BUTTONDOOR_DOWN,
    BUTTONDOOR_RIGHT,
} ButtonDoorOrientations;

// Object Class
struct ObjectButtonDoor {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxOpen;
};

// Entity Class
struct EntityButtonDoor {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Vector2 size;
    Vector2 segmentSize;
    int32 movePos;
    int32 buttonTag;
    int32 length;
    EntityButton *taggedButton;
    int32 duration;
    uint8 orientation;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectButtonDoor *ButtonDoor;

// Standard Entity Events
void ButtonDoor_Update(void);
void ButtonDoor_LateUpdate(void);
void ButtonDoor_StaticUpdate(void);
void ButtonDoor_Draw(void);
void ButtonDoor_Create(void *data);
void ButtonDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ButtonDoor_EditorDraw(void);
void ButtonDoor_EditorLoad(void);
#endif
void ButtonDoor_Serialize(void);

// Extra Entity Functions
void ButtonDoor_SetupSize(void);
void ButtonDoor_SetupTagLink(void);
void ButtonDoor_DrawSprites(void);

#endif //! OBJ_BUTTONDOOR_H
