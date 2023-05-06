#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "Game.h"

typedef enum {
    TUESDAY_GONDOLA,
    TUESDAY_NODE,
} TuesdayTypes;

// Object Class
struct ObjectTuesday {
    RSDK_OBJECT
    Hitbox hitboxNode;
    Hitbox hitboxGondola;
    uint16 aniFrames;
    uint16 sfxElecCharge;
    uint16 sfxZap;
    uint16 sfxBossHit;
    uint16 sfxExplosion;
    uint16 sfxDrop;
};

// Entity Class
struct EntityTuesday {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 links;
    uint8 invincibleTimer;
    uint8 health;
    Vector2 drawPos;
    EntityTuesday *linkNodes[8];
    EntityTuesday *parent;
    uint8 linkMask;
    uint8 nextShockFlags;
    uint8 shockFlags;
    uint8 shockType;
    uint16 timer;
    uint16 shockTimer;
    uint16 linkCount;
    Animator nodeAnimator;
    Animator gondolaAnimator;
};

// Object Struct
extern ObjectTuesday *Tuesday;

// Standard Entity Events
void Tuesday_Update(void);
void Tuesday_LateUpdate(void);
void Tuesday_StaticUpdate(void);
void Tuesday_Draw(void);
void Tuesday_Create(void *data);
void Tuesday_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Tuesday_EditorDraw(void);
void Tuesday_EditorLoad(void);
#endif
void Tuesday_Serialize(void);

// Extra Entity Functions
void Tuesday_Hit(void);
void Tuesday_Explode(void);

void Tuesday_DrawElectricity(void);
void Tuesday_State_Controller(void);
void Tuesday_State_Node(void);
void Tuesday_State_Destroyed(void);
void Tuesday_State_Debris(void);

#endif //! OBJ_TUESDAY_H
