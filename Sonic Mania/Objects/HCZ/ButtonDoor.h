#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxOpen;
} ObjectButtonDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Vector2 size;
    Vector2 size2;
    int32 movePos;
    int32 buttonTag;
    int32 length;
    EntityButton *taggedButton;
    int32 duration;
    uint8 orientation;
    Hitbox hitbox;
    Animator animator;
} EntityButtonDoor;

// Object Struct
extern ObjectButtonDoor *ButtonDoor;

// Standard Entity Events
void ButtonDoor_Update(void);
void ButtonDoor_LateUpdate(void);
void ButtonDoor_StaticUpdate(void);
void ButtonDoor_Draw(void);
void ButtonDoor_Create(void* data);
void ButtonDoor_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ButtonDoor_EditorDraw(void);
void ButtonDoor_EditorLoad(void);
#endif
void ButtonDoor_Serialize(void);

// Extra Entity Functions
void ButtonDoor_SetupSize(void);
void ButtonDoor_SetupButtonTag(void);
void ButtonDoor_DrawSprites(void);

#endif //!OBJ_BUTTONDOOR_H
