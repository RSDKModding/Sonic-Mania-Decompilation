#ifndef OBJ_MSPANEL_H
#define OBJ_MSPANEL_H

#include "Game.h"

// Object Class
struct ObjectMSPanel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityMSPanel {
    RSDK_ENTITY
    StateMachine(state);
    Animator panelAnimator;
    Animator cablesAnimator;
};

// Object Struct
extern ObjectMSPanel *MSPanel;

// Standard Entity Events
void MSPanel_Update(void);
void MSPanel_LateUpdate(void);
void MSPanel_StaticUpdate(void);
void MSPanel_Draw(void);
void MSPanel_Create(void *data);
void MSPanel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSPanel_EditorDraw(void);
void MSPanel_EditorLoad(void);
#endif
void MSPanel_Serialize(void);

// Extra Entity Functions
void MSPanel_State_SetupPanel(void);
void MSPanel_State_Active(void);
void MSPanel_State_Explode(void);
void MSPanel_State_Rumbling(void);

#endif //! OBJ_MSPANEL_H
