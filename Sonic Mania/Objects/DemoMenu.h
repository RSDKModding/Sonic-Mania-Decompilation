#ifndef OBJ_DEMOMENU_H
#define OBJ_DEMOMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectDemoMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
    void (*state)(void);
    int timer;
    int selectedZone;
    Vector2 drawPosA;
    Vector2 drawPosB;
    Vector2 drawPosC;
    float vDelta;
    AnimationData zoneSelData;
    AnimationData barBlackData;
    AnimationData barFlashingData;
    AnimationData GHZData;
    AnimationData SPZData;
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
void DemoMenu_DrawStagePreview(Vector2 *pos, AnimationData *data, int zoneID);

void DemoMenu_State_Appear(void);
void DemoMenu_State_Selection(void);
void DemoMenu_State_Load(void);
void DemoMenu_State_Disappear(void);

#endif //!OBJ_DEMOMENU_H
