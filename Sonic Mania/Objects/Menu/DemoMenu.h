#ifndef OBJ_DEMOMENU_H
#define OBJ_DEMOMENU_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectDemoMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
    StateMachine(state);
    int timer;
    int selectedZone;
    Vector2 drawPosA;
    Vector2 drawPosB;
    Vector2 drawPosC;
    float vDelta;
    Animator zoneSelData;
    Animator barBlackData;
    Animator barFlashingData;
    Animator GHZData;
    Animator SPZData;
    SpriteFrame *ghzFrame;
    SpriteFrame *spzFrame;
} EntityDemoMenu;

// Object Entity
extern ObjectDemoMenu *DemoMenu;

// Standard Entity Events
void DemoMenu_Update(void);
void DemoMenu_LateUpdate(void);
void DemoMenu_StaticUpdate(void);
void DemoMenu_Draw(void);
void DemoMenu_Create(void* data);
void DemoMenu_StageLoad(void);
void DemoMenu_EditorDraw(void);
void DemoMenu_EditorLoad(void);
void DemoMenu_Serialize(void);

// Extra Entity Functions
void DemoMenu_DrawStagePreview(Vector2 *pos, Animator *animator, int zoneID);

void DemoMenu_State_Appear(void);
void DemoMenu_State_Selection(void);
void DemoMenu_State_Load(void);
void DemoMenu_State_Disappear(void);

#endif //!OBJ_DEMOMENU_H
