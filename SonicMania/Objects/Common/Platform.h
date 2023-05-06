#ifndef OBJ_PLATFORM_H
#define OBJ_PLATFORM_H

#include "Game.h"

typedef enum {
    PLATFORM_FIXED,
    PLATFORM_FALL,
    PLATFORM_LINEAR,
    PLATFORM_CIRCULAR,
    PLATFORM_SWING,
    PLATFORM_PATH,
    PLATFORM_PUSH,
    PLATFORM_TRACK,
    PLATFORM_REACT,
    PLATFORM_HOVER_REACT,
    PLATFORM_DOORSLIDE,
    PLATFORM_PATH_REACT,
    PLATFORM_SWING_REACT,
    PLATFORM_TRACK_REACT,
    PLATFORM_CLACKER,
    PLATFORM_CHILD,
    PLATFORM_DIPROCK,
} PlatformTypes;

typedef enum {
    PLATFORM_C_PLATFORM,
    PLATFORM_C_SOLID,
    PLATFORM_C_TILED,
    PLATFORM_C_HURT,
    PLATFORM_C_NONE,
    PLATFORM_C_SOLID_HURT_SIDES,
    PLATFORM_C_SOLID_HURT_BOTTOM,
    PLATFORM_C_SOLID_HURT_TOP,
    PLATFORM_C_SOLID_HOLD,
    PLATFORM_C_SOLID_STICKY,
    PLATFORM_C_STICKY_TOP,
    PLATFORM_C_STICKY_LEFT,
    PLATFORM_C_STICKY_RIGHT,
    PLATFORM_C_STICKY_BOTTOM,
    PLATFORM_C_SOLID_BARREL,
    PLATFORM_C_SOLID_NOCRUSH,
} PlatformCollisionTypes;

// Object Class
struct ObjectPlatform {
    RSDK_OBJECT
    uint16 aniFrames;
    Vector2 stoodPos[PLAYER_COUNT];
    uint16 sfxClacker;
    uint16 sfxClang;
    uint16 sfxPush;
    uint16 sfxClack;
    bool32 playingPushSfx;
    bool32 useClack;
};

// Entity Class
struct EntityPlatform {
    MANIA_PLATFORM_BASE
};

// Object Struct
extern ObjectPlatform *Platform;

// Standard Entity Events
void Platform_Update(void);
void Platform_LateUpdate(void);
void Platform_StaticUpdate(void);
void Platform_Draw(void);
void Platform_Create(void *data);
void Platform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Platform_EditorDraw(void);
void Platform_EditorLoad(void);
#endif
void Platform_Serialize(void);

// Extra Entity Functions

// States
void Platform_State_Fixed(void);
void Platform_State_Fall(void);
void Platform_State_Falling(void);
void Platform_State_Falling2(void);
void Platform_State_Hold(void);
void Platform_State_Linear(void);
void Platform_State_Circular(void);
void Platform_State_Swing(void);
void Platform_State_Clacker(void);
void Platform_State_Push_Init(void);
void Platform_State_Push(void);
void Platform_State_Push_SlideOffL(void);
void Platform_State_Push_SlideOffR(void);
void Platform_State_Push_Fall(void);
void Platform_State_Path(void);
void Platform_State_PathStop(void);
void Platform_State_Track(void);
void Platform_State_React(void);
void Platform_State_Hover_React(void);
void Platform_State_ReactMove(void);
void Platform_State_ReactSlow(void);
void Platform_State_Hover(void);
void Platform_State_ReactWait(void);
void Platform_State_DoorSlide(void);
void Platform_State_PathReact(void);
void Platform_State_Child(void);
void Platform_State_SwingReact(void);
void Platform_State_Swing2(void);
void Platform_State_SwingWait(void);
void Platform_State_SwingReturn(void);
void Platform_State_TrackReact(void);
void Platform_State_Track2(void);
void Platform_State_TrackWait(void);
void Platform_State_TrackReturn(void);
void Platform_State_DipRock(void);

// Collision States
void Platform_Collision_Platform(void);
void Platform_Collision_Solid(void);
void Platform_Collision_Solid_NoCrush(void);
void Platform_Collision_Solid_Hurt_Sides(void);
void Platform_Collision_Solid_Hurt_Bottom(void);
void Platform_Collision_Solid_Hurt_Top(void);
void Platform_Collision_Solid_Hold(void);
void Platform_Collision_Solid_Barrel(void);
void Platform_Collision_Sticky(void);
void Platform_Collision_Tiles(void);
void Platform_Collision_Hurt(void);
void Platform_Collision_None(void);

// Collision Helpers
void Platform_HandleStood(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers);
void Platform_HandleStood_Tiles(EntityPlatform *self, EntityPlayer *player, int32 playerID);
void Platform_HandleStood_Hold(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers);
void Platform_HandleStood_Barrel(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers);
void Platform_HandleStood_Sticky(EntityPlatform *self, EntityPlayer *player, int32 playerID, uint8 cSide);

#endif //! OBJ_PLATFORM_H
