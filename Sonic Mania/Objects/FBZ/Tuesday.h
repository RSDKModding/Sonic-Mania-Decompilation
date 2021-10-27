#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxElecCharge;
    uint16 sfxZap;
    uint16 sfxBossHit;
    uint16 sfxExplosion;
    uint16 sfxDrop;
} ObjectTuesday;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 links;
    uint8 invincibleTimer;
    uint8 health;
    Vector2 drawPos;
    Entity *linkPtrs[8];
    Entity *field_88;
    uint8 field_8C;
    uint8 field_8D;
    uint8 field_8E;
    uint8 field_8F;
    uint16 timer;
    uint16 field_92;
    uint16 field_94;
    uint16 field_96;
    Animator animator1;
    Animator animator2;
} EntityTuesday;

// Object Struct
extern ObjectTuesday *Tuesday;

// Standard Entity Events
void Tuesday_Update(void);
void Tuesday_LateUpdate(void);
void Tuesday_StaticUpdate(void);
void Tuesday_Draw(void);
void Tuesday_Create(void* data);
void Tuesday_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Tuesday_EditorDraw(void);
void Tuesday_EditorLoad(void);
#endif
void Tuesday_Serialize(void);

// Extra Entity Functions
void Tuesday_DrawElectricity(void);
void Tuesday_Unknown2(void);
void Tuesday_Unknown3(void);
void Tuesday_State_Destroyed(void);
void Tuesday_State_Debris(void);

#endif //!OBJ_TUESDAY_H
