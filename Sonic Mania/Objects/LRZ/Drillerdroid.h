#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    int32 field_2C;
    int32 field_30;
    int32 field_34;
    int32 field_38;
    int32 field_3C;
    int32 field_40;
    int32 field_44;
    int32 field_48;
    int32 field_4C;
    int32 field_50;
    int32 field_54;
    int32 field_58;
    uint8 field_5C[4];
    uint8 field_60;
    uint8 field_61;
    uint8 field_62;
    uint8 field_63;
    uint8 field_64;
    uint8 field_65;
    uint8 field_66;
    uint8 field_67;
    int32 field_68;
    uint8 field_6C;
    uint8 field_6D;
    uint8 field_6E;
    uint8 field_6F;
    int32 field_70;
    Entity* field_74;
    uint16 aniFrames;
    uint16 ticFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxImpact;
    uint16 sfxJump;
    uint16 sfxTargeting;
    uint16 sfxSizzle;
    uint16 sfxDrop;
    uint16 sfxFail;
    uint16 sfxClang;
} ObjectDrillerdroid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDrillerdroid;

// Object Struct
extern ObjectDrillerdroid *Drillerdroid;

// Standard Entity Events
void Drillerdroid_Update(void);
void Drillerdroid_LateUpdate(void);
void Drillerdroid_StaticUpdate(void);
void Drillerdroid_Draw(void);
void Drillerdroid_Create(void* data);
void Drillerdroid_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void);
void Drillerdroid_EditorLoad(void);
#endif
void Drillerdroid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DRILLERDROID_H
