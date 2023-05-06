#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

#include "Game.h"

typedef enum {
    DIRECTORCHAIR_CLOSED,
    DIRECTORCHAIR_STRETCHED,
} DirectorChairTypes;

// Object Class
struct ObjectDirectorChair {
    RSDK_OBJECT
    uint8 extendCount;
    uint8 retractCount;
    uint16 aniFrames;
    Hitbox hitboxChair;
    Hitbox hitboxL;
    Hitbox hitboxR;
    uint16 sfxUnravel;
    uint16 sfxExtend;
    uint16 sfxRetract;
};

// Entity Class
struct EntityDirectorChair {
    // Platform "Inherit"
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 speed;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 tileOrigin;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 size;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;

    Animator scissorAnimator;
    int32 extendVel;
    int32 minRetract;
    int32 initExtendVel;
};

// Object Struct
extern ObjectDirectorChair *DirectorChair;

// Standard Entity Events
void DirectorChair_Update(void);
void DirectorChair_LateUpdate(void);
void DirectorChair_StaticUpdate(void);
void DirectorChair_Draw(void);
void DirectorChair_Create(void *data);
void DirectorChair_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DirectorChair_EditorDraw(void);
void DirectorChair_EditorLoad(void);
#endif
void DirectorChair_Serialize(void);

// Extra Entity Functions
void DirectorChair_Collide_Chair(void);

void DirectorChair_State_Idle(void);
void DirectorChair_State_StartExtend(void);
void DirectorChair_State_Extend(void);
void DirectorChair_State_StartRetract(void);
void DirectorChair_State_Retract(void);

#endif //! OBJ_DIRECTORCHAIR_H
