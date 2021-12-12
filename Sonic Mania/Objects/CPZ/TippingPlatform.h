#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTippingPlatform;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    uint8 interval;
    uint8 intervalOffset;
    uint8 duration;
    uint8 bossID;
    EntitySyringe *syringe;
} EntityTippingPlatform;

// Object Struct
extern ObjectTippingPlatform *TippingPlatform;

// Standard Entity Events
void TippingPlatform_Update(void);
void TippingPlatform_LateUpdate(void);
void TippingPlatform_StaticUpdate(void);
void TippingPlatform_Draw(void);
void TippingPlatform_Create(void* data);
void TippingPlatform_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TippingPlatform_EditorDraw(void);
void TippingPlatform_EditorLoad(void);
#endif
void TippingPlatform_Serialize(void);

// Extra Entity Functions
void TippingPlatform_State_Tipping_Boss(void);
void TippingPlatform_State_RestorePlatform(void);
void TippingPlatform_State_Tipping(void);
void TippingPlatform_State_Restore(void);
void TippingPlatform_State_Tipping_Delay(void);

#endif //!OBJ_TIPPINGPLATFORM_H
