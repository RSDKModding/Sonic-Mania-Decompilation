#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

#include "Game.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxPlayer;
    Vector2 sfxRange;
    uint8 magnetSfxTimer;
    bool32 playingMagnetSfx;
    uint16 sfxMagnet;
} ObjectElectroMagnet;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    int32 height;
    int32 shieldRange;
    int32 timer;
    bool32 invisible;
    Hitbox hitboxMagnetRange;
    Hitbox hitboxShieldRange;
} EntityElectroMagnet;

// Object Struct
extern ObjectElectroMagnet *ElectroMagnet;

// Standard Entity Events
void ElectroMagnet_Update(void);
void ElectroMagnet_LateUpdate(void);
void ElectroMagnet_StaticUpdate(void);
void ElectroMagnet_Draw(void);
void ElectroMagnet_Create(void *data);
void ElectroMagnet_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ElectroMagnet_EditorDraw(void);
void ElectroMagnet_EditorLoad(void);
#endif
void ElectroMagnet_Serialize(void);

// Extra Entity Functions
void ElectroMagnet_State_AwaitInterval(void);
void ElectroMagnet_State_MagnetActive(void);

#endif //! OBJ_ELECTROMAGNET_H
