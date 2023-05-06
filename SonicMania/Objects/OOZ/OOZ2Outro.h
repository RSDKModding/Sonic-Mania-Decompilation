#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectOOZ2Outro {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGlug;
    uint16 sfxSubLaunch;
};

// Entity Class
struct EntityOOZ2Outro {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    Vector2 moveOffset;
    Vector2 scrollOffset;
    int32 timer;
    Animator animator; // unused
    TileLayer *moveLayer;
    EntityEggPrison *prisonPtr;
    Vector2 prisonPos;
};

// Object Struct
extern ObjectOOZ2Outro *OOZ2Outro;

// Standard Entity Events
void OOZ2Outro_Update(void);
void OOZ2Outro_LateUpdate(void);
void OOZ2Outro_StaticUpdate(void);
void OOZ2Outro_Draw(void);
void OOZ2Outro_Create(void *data);
void OOZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OOZ2Outro_EditorDraw(void);
void OOZ2Outro_EditorLoad(void);
#endif
void OOZ2Outro_Serialize(void);

// Extra Entity Functions
void OOZ2Outro_StageFinish_EndAct2(void);
void OOZ2Outro_State_SubFloat(void);
void OOZ2Outro_CheckSkip(void);
void OOZ2Outro_State_BoardSub(void);
void OOZ2Outro_State_SubActivate(void);
void OOZ2Outro_State_SubLaunch(void);

#endif

#endif //! OBJ_OOZ2OUTRO_H
