#ifndef OBJ_DEMOMENU_H
#define OBJ_DEMOMENU_H

#include "Game.h"

// Object Class
struct ObjectDemoMenu {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityDemoMenu {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 selectedZone;
    Vector2 zoneSelPos;
    Vector2 ghzPos;
    Vector2 spzPos;
    float vDelta;
    Animator zoneSelAnimator;
    Animator blackBarAnimator;
    Animator flashBarAnimator;
    Animator ghzAnimator;
    Animator spzAnimator;
    SpriteFrame *ghzFrame;
    SpriteFrame *spzFrame;
};

// Object Entity
extern ObjectDemoMenu *DemoMenu;

// Standard Entity Events
void DemoMenu_Update(void);
void DemoMenu_LateUpdate(void);
void DemoMenu_StaticUpdate(void);
void DemoMenu_Draw(void);
void DemoMenu_Create(void *data);
void DemoMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DemoMenu_EditorDraw(void);
void DemoMenu_EditorLoad(void);
#endif
void DemoMenu_Serialize(void);

// Extra Entity Functions
void DemoMenu_DrawStagePreview(Vector2 *pos, Animator *animator, int32 zoneID);

void DemoMenu_State_Appear(void);
void DemoMenu_State_Selection(void);
void DemoMenu_State_Load(void);
void DemoMenu_State_Disappear(void);

#endif //! OBJ_DEMOMENU_H
