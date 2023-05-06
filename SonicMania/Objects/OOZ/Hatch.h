#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

#include "Game.h"

typedef enum {
    HATCH_GO_SUBENTRYHATCH,
    HATCH_GO_SUBEXITHATCH_COPYTILES,
    HATCH_GO_SUBENTRYHATCH_SMOGONLY,
    HATCH_GO_SUBEXIT,
    HATCH_GO_SUBEXITHATCH_NOCOPY,
} HatchGoTypes;

// Object Class
struct ObjectHatch {
    RSDK_OBJECT
    Hitbox hitboxSubExit;
    Hitbox hitboxRange;
    Hitbox hitboxL;
    Hitbox hitboxR;
    Hitbox hitboxEntry;
    uint16 aniFrames;
    uint16 sfxHatchOpen;
    uint16 sfxHatchClose;
    uint16 sfxDescend;
    uint16 sfxSurface;
    uint16 sfxGasPop;
};

// Entity Class
struct EntityHatch {
    RSDK_ENTITY
    StateMachine(state);
    uint8 go;
    Vector2 subOff1;
    Vector2 subOff2;
    int32 depth;
    int32 dest;
    Vector2 startPos;
    int32 hScrollPos;
    int32 vScrollPos;
    int32 timer;
    bool32 useMoveLayer;
    EntityPlayer *playerPtr;
    StateMachine(stateStore);
    bool32 interactionStore;
    bool32 tileColStore;
    Animator baseAnimator;
    Animator hatchAnimator;
    Hitbox hitbox;
    Hitbox hitboxWarpDoor;
};

// Object Struct
extern ObjectHatch *Hatch;

// Standard Entity Events
void Hatch_Update(void);
void Hatch_LateUpdate(void);
void Hatch_StaticUpdate(void);
void Hatch_Draw(void);
void Hatch_Create(void *data);
void Hatch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Hatch_EditorDraw(void);
void Hatch_EditorLoad(void);
#endif
void Hatch_Serialize(void);

// Extra Entity Functions
void Hatch_State_SubEntryHatch(void);
void Hatch_State_PlayerEntered(void);
void Hatch_State_Descend(void);
void Hatch_State_MoveToDestPos(void);
void Hatch_State_Surfacing(void);
void Hatch_State_OpenHatchReleasePlayer(void);
void Hatch_State_CloseHatch(void);
void Hatch_State_FadeIn(void);
void Hatch_State_SubExit(void);
void Hatch_State_FadeOut(void);
void Hatch_State_SubExitHatch(void);

#endif //! OBJ_HATCH_H
