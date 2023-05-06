#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

#include "Game.h"

typedef enum {
    TWISTINGDOOR_V_SHORT,
    TWISTINGDOOR_H_SHORT,
    TWISTINGDOOR_H_LONG,
    TWISTINGDOOR_V_LONG,
} TwistingDoorTypes;

// Object Class
struct ObjectTwistingDoor {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxOpen;
};

// Entity Class
struct EntityTwistingDoor {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 buttonTag;
    bool32 hasTension;
    uint8 autoOpen;
    uint8 collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;

    bool32 close;
    EntityButton *taggedButton;
};

// Object Struct
extern ObjectTwistingDoor *TwistingDoor;

// Standard Entity Events
void TwistingDoor_Update(void);
void TwistingDoor_LateUpdate(void);
void TwistingDoor_StaticUpdate(void);
void TwistingDoor_Draw(void);
void TwistingDoor_Create(void *data);
void TwistingDoor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TwistingDoor_EditorDraw(void);
void TwistingDoor_EditorLoad(void);
#endif
void TwistingDoor_Serialize(void);

// Extra Entity Functions
void TwistingDoor_State_CheckOpen(void);
void TwistingDoor_State_Opening(void);
void TwistingDoor_State_Opened(void);
void TwistingDoor_State_Closing(void);

#endif //! OBJ_TWISTINGDOOR_H
