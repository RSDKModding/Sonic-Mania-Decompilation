#ifndef OBJ_UIMEDALLIONPANEL_H
#define OBJ_UIMEDALLIONPANEL_H

#include "Game.h"

// Object Class
struct ObjectUIMedallionPanel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIMedallionPanel {
    RSDK_ENTITY
    Animator animator;
};

// Object Struct
extern ObjectUIMedallionPanel *UIMedallionPanel;

// Standard Entity Events
void UIMedallionPanel_Update(void);
void UIMedallionPanel_LateUpdate(void);
void UIMedallionPanel_StaticUpdate(void);
void UIMedallionPanel_Draw(void);
void UIMedallionPanel_Create(void *data);
void UIMedallionPanel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIMedallionPanel_EditorDraw(void);
void UIMedallionPanel_EditorLoad(void);
#endif
void UIMedallionPanel_Serialize(void);

// Extra Entity Functions
void UIMedallionPanel_DrawPanel(void);

#endif //! OBJ_UIMEDALLIONPANEL_H
