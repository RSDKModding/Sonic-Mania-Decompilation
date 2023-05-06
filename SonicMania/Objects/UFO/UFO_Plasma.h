#ifndef OBJ_UFO_PLASMA_H
#define OBJ_UFO_PLASMA_H

#include "Game.h"

// Object Class
struct ObjectUFO_Plasma {
    RSDK_OBJECT
    bool32 forceEnabled; // unused, this was prolly copied from OOZ/Smog lol
    ScanlineInfo *scanlines;
    ScanlineInfo scanlineList[0x100];
    uint16 aniFrames;
    int32 starPostID; // unused, this was prolly copied from OOZ/Smog lol
};

// Entity Class
struct EntityUFO_Plasma {
    RSDK_ENTITY
    StateMachine(state); // unused
    Animator animator;   // unused
    int32 unused1;       // unused
    int32 unused2;       // unused
};

// Object Struct
extern ObjectUFO_Plasma *UFO_Plasma;

// Standard Entity Events
void UFO_Plasma_Update(void);
void UFO_Plasma_LateUpdate(void);
void UFO_Plasma_StaticUpdate(void);
void UFO_Plasma_Draw(void);
void UFO_Plasma_Create(void *data);
void UFO_Plasma_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Plasma_EditorDraw(void);
void UFO_Plasma_EditorLoad(void);
#endif
void UFO_Plasma_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_PLASMA_H
