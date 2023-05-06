#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "Game.h"

typedef enum {
    MSZSPOTLIGHT_RED,
    MSZSPOTLIGHT_GREEN,
    MSZSPOTLIGHT_BLUE,
} MSZSpotlightColors;

// Object Class
struct ObjectMSZSpotlight {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityMSZSpotlight {
    RSDK_ENTITY
    StateMachine(state);
    uint8 color;
    int32 timer;
    int32 angleOffset;
    Vector2 startPos;
    Animator animatorSpotlight;
};

// Object Struct
extern ObjectMSZSpotlight *MSZSpotlight;

// Standard Entity Events
void MSZSpotlight_Update(void);
void MSZSpotlight_LateUpdate(void);
void MSZSpotlight_StaticUpdate(void);
void MSZSpotlight_Draw(void);
void MSZSpotlight_Create(void *data);
void MSZSpotlight_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void);
void MSZSpotlight_EditorLoad(void);
#endif
void MSZSpotlight_Serialize(void);

// Extra Entity Functions
void MSZSpotlight_State_Appear(void);
void MSZSpotlight_State_Circling(void);
void MSZSpotlight_State_Idle(void);
void MSZSpotlight_State_MoveToBox(void);
void MSZSpotlight_State_Disappear(void);

#endif //! OBJ_MSZSPOTLIGHT_H
