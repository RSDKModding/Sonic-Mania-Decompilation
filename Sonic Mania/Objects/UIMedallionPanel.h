#ifndef OBJ_UIMEDALLIONPANEL_H
#define OBJ_UIMEDALLIONPANEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIMedallionPanel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    AnimationData data;
} EntityUIMedallionPanel;

// Object Struct
extern ObjectUIMedallionPanel *UIMedallionPanel;

// Standard Entity Events
void UIMedallionPanel_Update();
void UIMedallionPanel_LateUpdate();
void UIMedallionPanel_StaticUpdate();
void UIMedallionPanel_Draw();
void UIMedallionPanel_Create(void* data);
void UIMedallionPanel_StageLoad();
void UIMedallionPanel_EditorDraw();
void UIMedallionPanel_EditorLoad();
void UIMedallionPanel_Serialize();

// Extra Entity Functions
void UIMedallionPanel_DrawPanel();


#endif //!OBJ_UIMEDALLIONPANEL_H
