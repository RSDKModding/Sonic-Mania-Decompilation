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
void UIMedallionPanel_Update(void);
void UIMedallionPanel_LateUpdate(void);
void UIMedallionPanel_StaticUpdate(void);
void UIMedallionPanel_Draw(void);
void UIMedallionPanel_Create(void* data);
void UIMedallionPanel_StageLoad(void);
void UIMedallionPanel_EditorDraw(void);
void UIMedallionPanel_EditorLoad(void);
void UIMedallionPanel_Serialize(void);

// Extra Entity Functions
void UIMedallionPanel_DrawPanel(void);


#endif //!OBJ_UIMEDALLIONPANEL_H
