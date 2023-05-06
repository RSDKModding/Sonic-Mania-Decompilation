#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

#include "Game.h"

typedef enum {
    FBZMISSILE_LAUNCHER_V,
    FBZMISSILE_LAUNCHER_H,
    FBZMISSILE_HULL,
    FBZMISSILE_VERTICAL,
    FBZMISSILE_HORIZONTAL,
} FBZMissileTypes;

// Object Class
struct ObjectFBZMissile {
    RSDK_OBJECT
    Hitbox hitboxMissileV;
    Hitbox hitboxMissileH;
    Hitbox hitboxHull;
    TABLE(int32 velocities[8], { 0x10000, 0xE000, 0x12000, 0x14000, 0x10000, 0xE000, 0xC000, 0xE000 });
    int32 velocityID;
    uint16 aniFrames;
    uint16 sfxPush;
    uint16 sfxPush2;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityFBZMissile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    FBZMissileTypes type;
    uint8 interval;
    uint8 intervalOffset;
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectFBZMissile *FBZMissile;

// Standard Entity Events
void FBZMissile_Update(void);
void FBZMissile_LateUpdate(void);
void FBZMissile_StaticUpdate(void);
void FBZMissile_Draw(void);
void FBZMissile_Create(void *data);
void FBZMissile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZMissile_EditorDraw(void);
void FBZMissile_EditorLoad(void);
#endif
void FBZMissile_Serialize(void);

// Extra Entity Functions
void FBZMissile_StateLauncherV_Delay(void);
void FBZMissile_StateLauncherV_Launch(void);

void FBZMissile_StateLauncherH_Fire(void);
void FBZMissile_StateLauncherH_Prepare(void);

void FBZMissile_StateVertical_Rise(void);
void FBZMissile_StateVertical_Fall(void);
void FBZMissile_StateHorizontal_Move(void);

void FBZMissile_State_Hull(void);

#endif //! OBJ_FBZMISSILE_H
