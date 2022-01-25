#ifndef OBJ_GASPLATFORM_H
#define OBJ_GASPLATFORM_H

#include "SonicMania.h"

// Object Class
struct ObjectGasPlatform {
    RSDK_OBJECT
    Hitbox hitbox;
    Vector2 range;
    uint16 sfxGasPop;
    uint16 sfxSpring;
};

// Entity Class
struct EntityGasPlatform {
    MANIA_PLATFORM_BASE
    uint16 interval;
    uint16 intervalOffset;
    Animator animator2;
};

// Object Struct
extern ObjectGasPlatform *GasPlatform;

// Standard Entity Events
void GasPlatform_Update(void);
void GasPlatform_LateUpdate(void);
void GasPlatform_StaticUpdate(void);
void GasPlatform_Draw(void);
void GasPlatform_Create(void* data);
void GasPlatform_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GasPlatform_EditorDraw(void);
void GasPlatform_EditorLoad(void);
#endif
void GasPlatform_Serialize(void);

// Extra Entity Functions
void GasPlatform_Unknown1(void);
void GasPlatform_Unknown2(void);
void GasPlatform_Unknown3(void);
void GasPlatform_Unknown4(void);


#endif //!OBJ_GASPLATFORM_H
