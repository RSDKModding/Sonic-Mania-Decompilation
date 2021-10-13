#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    uint16 sfxAssemble;
    uint16 sfxMagicBox;
    uint16 sfxClack;
    uint16 sfxPowerDown;
    uint16 sfxImpact;
    uint16 sfxPon;
    uint16 sfxDrop;
    uint16 sfxImpact2;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxPoof;
    uint16 sfxTwinkle;
    uint16 sfxHat;
    uint16 sfxHatNode;
    uint16 sfxBleeps;
    uint16 sfxTransform;
    uint16 sfxAppear1;
    uint16 aniFrames;
    uint16 rouguesFrames;
} ObjectHeavyMystic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyMystic;

// Object Struct
extern ObjectHeavyMystic *HeavyMystic;

// Standard Entity Events
void HeavyMystic_Update(void);
void HeavyMystic_LateUpdate(void);
void HeavyMystic_StaticUpdate(void);
void HeavyMystic_Draw(void);
void HeavyMystic_Create(void* data);
void HeavyMystic_StageLoad(void);
void HeavyMystic_EditorDraw(void);
void HeavyMystic_EditorLoad(void);
void HeavyMystic_Serialize(void);

// Extra Entity Functions
void HeavyMystic_Unknown1(int x, int y);
void HeavyMystic_Unknown2(void);

#endif //!OBJ_HEAVYMYSTIC_H
