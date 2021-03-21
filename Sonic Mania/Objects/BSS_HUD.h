#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectBSS_HUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    AnimationData animData1;
    AnimationData animData2;
    AnimationData numbersData;
} EntityBSS_HUD;

// Object Struct
extern ObjectBSS_HUD *BSS_HUD;

// Standard Entity Events
void BSS_HUD_Update();
void BSS_HUD_LateUpdate();
void BSS_HUD_StaticUpdate();
void BSS_HUD_Draw();
void BSS_HUD_Create(void* data);
void BSS_HUD_StageLoad();
void BSS_HUD_EditorDraw();
void BSS_HUD_EditorLoad();
void BSS_HUD_Serialize();

// Extra Entity Functions
void BSS_HUD_DrawNumbers(int value, Vector2 *drawPos);

#endif //!OBJ_BSS_HUD_H
