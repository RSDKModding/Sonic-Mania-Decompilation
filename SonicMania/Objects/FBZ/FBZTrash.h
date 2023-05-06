#ifndef OBJ_FBZTRASH_H
#define OBJ_FBZTRASH_H

#include "Game.h"

typedef enum {
    FBZTRASH_TRASH,
    FBZTRASH_UNUSED,
    FBZTRASH_ORB,
    FBZTRASH_ORBINAUT,
} FBZTrashTypes;

// Object Class
struct ObjectFBZTrash {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityFBZTrash {
    RSDK_ENTITY
    StateMachine(state);
    FBZTrashTypes type;
    int32 frameID;
    int32 rumbleMove;
    int32 timer;
    int32 radius;
    Vector2 targetPos;
    Vector2 startPos;
    EntityFBZTrash *parent;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectFBZTrash *FBZTrash;

// Standard Entity Events
void FBZTrash_Update(void);
void FBZTrash_LateUpdate(void);
void FBZTrash_StaticUpdate(void);
void FBZTrash_Draw(void);
void FBZTrash_Create(void *data);
void FBZTrash_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZTrash_EditorDraw(void);
void FBZTrash_EditorLoad(void);
#endif
void FBZTrash_Serialize(void);

// Extra Entity Functions
void FBZTrash_SummonOrbinautOrbs(EntityFBZTrash *trashPtr, int32 angle);
void FBZTrash_SummonOrbinaut(int32 x, int32 y);
void FBZTrash_State_LooseTrash(void);
void FBZTrash_State_ReactMagnet(void);
void FBZTrash_State_MoveToTarget(void);
void FBZTrash_State_OrbinautOrb(void);
void FBZTrash_State_OrbinautMove(void);

#endif //! OBJ_FBZTRASH_H
