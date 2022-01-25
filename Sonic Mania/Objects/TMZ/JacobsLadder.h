#ifndef OBJ_JACOBSLADDER_H
#define OBJ_JACOBSLADDER_H

#include "SonicMania.h"

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
    int32 field_6C;
    Vector2 field_70;
    int32 field_78;
    int32 field_7C;
    bool32 field_80;
    int32 field_84;
    uint8 activePlayers;
    uint8 playerTimers[4];
    Vector2 playerPositions[4];
    Hitbox hitbox;
    Animator animator2;
    Animator animator;
};

// Object Struct
extern ObjectJacobsLadder *JacobsLadder;

// Standard Entity Events
void JacobsLadder_Update(void);
void JacobsLadder_LateUpdate(void);
void JacobsLadder_StaticUpdate(void);
void JacobsLadder_Draw(void);
void JacobsLadder_Create(void* data);
void JacobsLadder_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void JacobsLadder_EditorDraw(void);
void JacobsLadder_EditorLoad(void);
#endif
void JacobsLadder_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JACOBSLADDER_H
