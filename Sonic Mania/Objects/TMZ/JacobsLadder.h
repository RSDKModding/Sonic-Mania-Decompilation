#ifndef OBJ_JACOBSLADDER_H
#define OBJ_JACOBSLADDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxLadder;
} ObjectJacobsLadder;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int height;
    int interval;
    int intervalOffset;
    int duration;
    bool32 flip;
    int field_6C;
    Vector2 field_70;
    int field_78;
    int field_7C;
    bool32 field_80;
    int field_84;
    uint8 activePlayers;
    uint8 playerTimers[4];
    Vector2 playerPositions[4];
    Hitbox hitbox;
    Animator animator2;
    Animator animator;
} EntityJacobsLadder;

// Object Struct
extern ObjectJacobsLadder *JacobsLadder;

// Standard Entity Events
void JacobsLadder_Update(void);
void JacobsLadder_LateUpdate(void);
void JacobsLadder_StaticUpdate(void);
void JacobsLadder_Draw(void);
void JacobsLadder_Create(void* data);
void JacobsLadder_StageLoad(void);
void JacobsLadder_EditorDraw(void);
void JacobsLadder_EditorLoad(void);
void JacobsLadder_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JACOBSLADDER_H
