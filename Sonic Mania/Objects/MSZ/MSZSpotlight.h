#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "SonicMania.h"

typedef enum {
    MSZSPOTLIGHT_RED,
    MSZSPOTLIGHT_GREEN,
    MSZSPOTLIGHT_BLUE,
}MSZSpotlightColours;

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
    int32 moveFlag;
    Vector2 startPos;
    Animator animator;
};

// Object Struct
extern ObjectMSZSpotlight *MSZSpotlight;

// Standard Entity Events
void MSZSpotlight_Update(void);
void MSZSpotlight_LateUpdate(void);
void MSZSpotlight_StaticUpdate(void);
void MSZSpotlight_Draw(void);
void MSZSpotlight_Create(void* data);
void MSZSpotlight_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void);
void MSZSpotlight_EditorLoad(void);
#endif
void MSZSpotlight_Serialize(void);

// Extra Entity Functions
void MSZSpotlight_Unknown1(void);
void MSZSpotlight_Unknown2(void);
void MSZSpotlight_Unknown3(void);
void MSZSpotlight_Unknown4(void);
void MSZSpotlight_Unknown5(void);

#endif //!OBJ_MSZSPOTLIGHT_H
