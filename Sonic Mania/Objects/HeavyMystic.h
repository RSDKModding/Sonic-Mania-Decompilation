#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    ushort sfxAssemble;
    ushort sfxMagicBox;
    ushort sfxClack;
    ushort sfxPowerDown;
    ushort sfxImpact;
    ushort sfxPon;
    ushort sfxDrop;
    ushort sfxImpact2;
    ushort sfxHit;
    ushort sfxExplosion;
    ushort sfxPoof;
    ushort sfxTwinkle;
    ushort sfxHat;
    ushort sfxHatNode;
    ushort sfxBleeps;
    ushort sfxTransform;
    ushort sfxAppear1;
    ushort aniFrames;
    ushort rouguesFrames;
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


#endif //!OBJ_HEAVYMYSTIC_H
