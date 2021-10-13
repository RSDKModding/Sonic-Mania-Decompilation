#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectBSS_HUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animData1;
    Animator animData2;
    Animator numbersData;
} EntityBSS_HUD;

// Object Struct
extern ObjectBSS_HUD *BSS_HUD;

// Standard Entity Events
void BSS_HUD_Update(void);
void BSS_HUD_LateUpdate(void);
void BSS_HUD_StaticUpdate(void);
void BSS_HUD_Draw(void);
void BSS_HUD_Create(void* data);
void BSS_HUD_StageLoad(void);
void BSS_HUD_EditorDraw(void);
void BSS_HUD_EditorLoad(void);
void BSS_HUD_Serialize(void);

// Extra Entity Functions
void BSS_HUD_DrawNumbers(int32 value, Vector2 *drawPos);

#endif //!OBJ_BSS_HUD_H
