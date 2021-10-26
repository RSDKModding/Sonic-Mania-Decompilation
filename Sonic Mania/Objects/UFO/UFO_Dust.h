#ifndef OBJ_UFO_DUST_H
#define OBJ_UFO_DUST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    uint16 spriteIndex;
    uint16 field_E;
} ObjectUFO_Dust;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 height;
    int32 field_64;
    int32 worldX;
    int32 worldY;
    Animator animator;
} EntityUFO_Dust;

// Object Struct
extern ObjectUFO_Dust *UFO_Dust;

// Standard Entity Events
void UFO_Dust_Update(void);
void UFO_Dust_LateUpdate(void);
void UFO_Dust_StaticUpdate(void);
void UFO_Dust_Draw(void);
void UFO_Dust_Create(void* data);
void UFO_Dust_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_Dust_EditorDraw(void);
void UFO_Dust_EditorLoad(void);
#endif
void UFO_Dust_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_DUST_H
