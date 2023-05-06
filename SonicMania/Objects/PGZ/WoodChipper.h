#ifndef OBJ_WOODCHIPPER_H
#define OBJ_WOODCHIPPER_H

#include "Game.h"

// Object Class
struct ObjectWoodChipper {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxStump;
    Hitbox hitboxWood;
    Hitbox hitboxRazor;
    uint16 sfxChipperChips;
    bool32 playingChipSfx;
    uint16 sfxChipperWood;
    bool32 playingWoodSfx;
};

// Entity Class
struct EntityWoodChipper {
    RSDK_ENTITY
    StateMachine(state);
    uint16 size;
    int32 speed;
    int32 timer;
    Vector2 shakeOffsets[3];
    int32 height;
    uint8 activePlayers;
    Animator animator;
    Animator sawAnimator;
};

// Object Struct
extern ObjectWoodChipper *WoodChipper;

// Standard Entity Events
void WoodChipper_Update(void);
void WoodChipper_LateUpdate(void);
void WoodChipper_StaticUpdate(void);
void WoodChipper_Draw(void);
void WoodChipper_Create(void *data);
void WoodChipper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WoodChipper_EditorDraw(void);
void WoodChipper_EditorLoad(void);
#endif
void WoodChipper_Serialize(void);

// Extra Entity Functions
void WoodChipper_HandlePlayerCollisions(void);

void WoodChipper_State_Chipper(void);
void WoodChipper_State_Debris(void);

#endif //! OBJ_WOODCHIPPER_H
