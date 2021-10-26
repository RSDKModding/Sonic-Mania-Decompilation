#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    TABLE(int32 array1[8], { 0, 16, 64, 96, 112, 144, 160, 192 });
    TABLE(int32 array2[8], { 0, -8, 32, 64, 64, 64, 64, 64 });
    TABLE(int32 array3[8], { 0, -24, 8, 8, 8, 8, 8, 8 });
    uint16 aniFrames;
} ObjectUncurlPlant;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    Vector2 drawPositions[8];
    Vector2 positions[8];
    int32 segments[8];
    int32 field_100[8];
    int32 field_120;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    Animator animator;
    Animator animators[8];
} EntityUncurlPlant;

// Object Struct
extern ObjectUncurlPlant *UncurlPlant;

// Standard Entity Events
void UncurlPlant_Update(void);
void UncurlPlant_LateUpdate(void);
void UncurlPlant_StaticUpdate(void);
void UncurlPlant_Draw(void);
void UncurlPlant_Create(void* data);
void UncurlPlant_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UncurlPlant_EditorDraw(void);
void UncurlPlant_EditorLoad(void);
#endif
void UncurlPlant_Serialize(void);

// Extra Entity Functions
void UncurlPlant_CalculateDrawPositions(void);
void UncurlPlant_CalculatePositions(void);

#endif //!OBJ_UNCURLPLANT_H
