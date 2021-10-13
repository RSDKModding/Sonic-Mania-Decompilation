#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT 
    int32 field_4;
    int32 field_8;
    uint16 field_C;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxRumble;
    uint16 sfxJump2;
    uint16 sfxSpecialRing;
    uint16 sfxMSElecPulse;
    uint16 sfxMSBall;
    uint16 sfxMSFireball;
    uint16 sfxBeep3;
    uint16 sfxBeep4;
    uint16 sfxRockemSockem;
    uint16 sfxMSShoot;
    uint16 sfxMSChargeFire;
    uint16 sfxMSTransform;
    uint16 sfxTransform2;
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
} ObjectMetalSonic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 posUnknown;
    int32 timer;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    Animator animator;
    Animator animator2;
} EntityMetalSonic;

// Object Struct
extern ObjectMetalSonic *MetalSonic;

// Standard Entity Events
void MetalSonic_Update(void);
void MetalSonic_LateUpdate(void);
void MetalSonic_StaticUpdate(void);
void MetalSonic_Draw(void);
void MetalSonic_Create(void* data);
void MetalSonic_StageLoad(void);
void MetalSonic_EditorDraw(void);
void MetalSonic_EditorLoad(void);
void MetalSonic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_METALSONIC_H
