#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBlade;
    uint16 sfxHit;
} ObjectPhantomShinobi;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 field_6C;
    int32 spearOffset;
    int32 rotStore;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    bool32 prevOnGround;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityPhantomShinobi;

// Object Struct
extern ObjectPhantomShinobi *PhantomShinobi;

// Standard Entity Events
void PhantomShinobi_Update(void);
void PhantomShinobi_LateUpdate(void);
void PhantomShinobi_StaticUpdate(void);
void PhantomShinobi_Draw(void);
void PhantomShinobi_Create(void* data);
void PhantomShinobi_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomShinobi_EditorDraw(void);
void PhantomShinobi_EditorLoad(void);
#endif
void PhantomShinobi_Serialize(void);

// Extra Entity Functions
void PhantomShinobi_CheckPlayerCollisions(void);
void PhantomShinobi_HandleTileCollisions(void);
void PhantomShinobi_ResetStates(void);

void PhantomShinobi_State_Unknown1(void);
void PhantomShinobi_State_Unknown2(void);
void PhantomShinobi_State_Unknown3(void);
void PhantomShinobi_State_Unknown4(void);
void PhantomShinobi_State_Unknown5(void);
void PhantomShinobi_State_Unknown6(void);
void PhantomShinobi_State_Unknown7(void);
void PhantomShinobi_State_Unknown8(void);

bool32 PhantomShinobi_BladeCheckCB(void);

#endif //!OBJ_PHANTOMSHINOBI_H
