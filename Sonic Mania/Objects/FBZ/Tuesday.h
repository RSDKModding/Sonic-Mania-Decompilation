#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    ushort aniFrames;
    ushort sfxElecCharge;
    ushort sfxZap;
    ushort sfxBossHit;
    ushort sfxExplosion;
    ushort sfxDrop;
} ObjectTuesday;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    byte links;
    byte invincibleTimer;
    byte health;
    Vector2 drawPos;
    Entity *linkPtrs[8];
    Entity *field_88;
    byte field_8C;
    byte field_8D;
    byte field_8E;
    byte field_8F;
    ushort timer;
    ushort field_92;
    ushort field_94;
    ushort field_96;
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
void Tuesday_EditorDraw(void);
void Tuesday_EditorLoad(void);
void Tuesday_Serialize(void);

// Extra Entity Functions
void Tuesday_DrawElectricity(void);
void Tuesday_Unknown2(void);
void Tuesday_Unknown3(void);
void Tuesday_State_Destroyed(void);
void Tuesday_State_Debris(void);

#endif //!OBJ_TUESDAY_H
