#ifndef OBJ_UFO_HUD_H
#define OBJ_UFO_HUD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectUFO_HUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    bool32 showRingCount;
    int32 timer;
    uint8 palID;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityUFO_HUD;

// Object Struct
extern ObjectUFO_HUD *UFO_HUD;

// Standard Entity Events
void UFO_HUD_Update(void);
void UFO_HUD_LateUpdate(void);
void UFO_HUD_StaticUpdate(void);
void UFO_HUD_Draw(void);
void UFO_HUD_Create(void* data);
void UFO_HUD_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_HUD_EditorDraw(void);
void UFO_HUD_EditorLoad(void);
#endif
void UFO_HUD_Serialize(void);

// Extra Entity Functions
void UFO_HUD_CheckLevelUp(void);
void UFO_HUD_LevelUpMach(void);
void UFO_HUD_DrawNumbers(Vector2 *drawPos, int32 value);

#endif //!OBJ_UFO_HUD_H
