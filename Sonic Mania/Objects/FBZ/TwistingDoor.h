#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxOpen;
} ObjectTwistingDoor;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 amplitude;
    int buttonTag;
    bool32 hasTension;
    uint8 autoOpen;
    uint8 collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int collapseDelay;
    int stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int childCount;
    bool32 close;
    EntityButton *buttonPtr;
} EntityTwistingDoor;

// Object Struct
extern ObjectTwistingDoor *TwistingDoor;

// Standard Entity Events
void TwistingDoor_Update(void);
void TwistingDoor_LateUpdate(void);
void TwistingDoor_StaticUpdate(void);
void TwistingDoor_Draw(void);
void TwistingDoor_Create(void* data);
void TwistingDoor_StageLoad(void);
void TwistingDoor_EditorDraw(void);
void TwistingDoor_EditorLoad(void);
void TwistingDoor_Serialize(void);

// Extra Entity Functions
void TwistingDoor_Unknown1(void);
void TwistingDoor_Unknown2(void);
void TwistingDoor_Unknown3(void);
void TwistingDoor_Unknown4(void);

#endif //!OBJ_TWISTINGDOOR_H
