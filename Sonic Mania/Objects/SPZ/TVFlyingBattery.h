#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlyover;
    bool32 isPlayingFlyover;
    int32 flyoverChannel;
    Entity *weatherTV;
} ObjectTVFlyingBattery;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    Vector2 field_5C;
    int32 field_64;
    int32 oscillation;
    int32 field_6C;
    int32 field_70;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityTVFlyingBattery;

// Object Struct
extern ObjectTVFlyingBattery *TVFlyingBattery;

// Standard Entity Events
void TVFlyingBattery_Update(void);
void TVFlyingBattery_LateUpdate(void);
void TVFlyingBattery_StaticUpdate(void);
void TVFlyingBattery_Draw(void);
void TVFlyingBattery_Create(void* data);
void TVFlyingBattery_StageLoad(void);
void TVFlyingBattery_EditorDraw(void);
void TVFlyingBattery_EditorLoad(void);
void TVFlyingBattery_Serialize(void);

// Extra Entity Functions
void TVFlyingBattery_DrawSection(Vector2 drawPos, bool32 flag);
void TVFlyingBattery_DrawSprites(void);

#endif //!OBJ_TVFLYINGBATTERY_H
