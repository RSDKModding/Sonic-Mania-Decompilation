#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort sfxChain;
} ObjectMagPlatform;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    int length;
} EntityMagPlatform;

// Object Struct
extern ObjectMagPlatform *MagPlatform;

// Standard Entity Events
void MagPlatform_Update(void);
void MagPlatform_LateUpdate(void);
void MagPlatform_StaticUpdate(void);
void MagPlatform_Draw(void);
void MagPlatform_Create(void* data);
void MagPlatform_StageLoad(void);
void MagPlatform_EditorDraw(void);
void MagPlatform_EditorLoad(void);
void MagPlatform_Serialize(void);

// Extra Entity Functions
void MagPlatform_Unknown1(void);
void MagPlatform_Unknown2(void);
void MagPlatform_Unknown3(void);
void MagPlatform_Unknown4(void);

#endif //!OBJ_MAGPLATFORM_H
