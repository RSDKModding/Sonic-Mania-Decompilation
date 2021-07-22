#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT 
    int field_4;
    int field_8;
    ushort field_C;
    ushort sfxHit;
    ushort sfxExplosion2;
    ushort sfxExplosion3;
    ushort sfxRumble;
    ushort sfxJump2;
    ushort sfxSpecialRing;
    ushort sfxMSElecPulse;
    ushort sfxMSBall;
    ushort sfxMSFireball;
    ushort sfxBeep3;
    ushort sfxBeep4;
    ushort sfxRockemSockem;
    ushort sfxMSShoot;
    ushort sfxMSChargeFire;
    ushort sfxMSTransform;
    ushort sfxTransform2;
    Hitbox hitbox1;
    Hitbox hitbox2;
    ushort aniFrames;
} ObjectMetalSonic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 posUnknown;
    int timer;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
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
