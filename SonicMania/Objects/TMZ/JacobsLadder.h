#ifndef OBJ_JACOBSLADDER_H
#define OBJ_JACOBSLADDER_H

#include "Game.h"

// Object Class
struct ObjectJacobsLadder {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxLadder;
};

// Entity Class
struct EntityJacobsLadder {
    RSDK_ENTITY
    int32 height;
    int32 interval;
    int32 intervalOffset;
    int32 duration;
    bool32 flip;
    int32 unused1;
    Vector2 startPos;
    int32 unused2;
    int32 electricPos;
    bool32 showElectricity;
    bool32 electricFadeOut;
    uint8 activePlayers;
    uint8 playerTimers[PLAYER_COUNT];
    Vector2 playerPositions[PLAYER_COUNT];
    Hitbox hitbox;
    Animator sideAnimator;
    Animator electricAnimator;
};

// Object Struct
extern ObjectJacobsLadder *JacobsLadder;

// Standard Entity Events
void JacobsLadder_Update(void);
void JacobsLadder_LateUpdate(void);
void JacobsLadder_StaticUpdate(void);
void JacobsLadder_Draw(void);
void JacobsLadder_Create(void *data);
void JacobsLadder_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void JacobsLadder_EditorDraw(void);
void JacobsLadder_EditorLoad(void);
#endif
void JacobsLadder_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_JACOBSLADDER_H
