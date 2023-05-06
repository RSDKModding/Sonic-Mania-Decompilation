#ifndef OBJ_TVVAN_H
#define OBJ_TVVAN_H

#include "Game.h"

typedef enum {
    TVVAN_VAN_LEFT,
    TVVAN_VAN_RIGHT,
    TVVAN_SATELLITE_NE_SW,
    TVVAN_SATELLITE_NW_SE,
    TVVAN_SATELLITE_NW_SW,
    TVVAN_SATELLITE_NE_SE,
    TVVAN_SATELLITE_NE_NW,
    TVVAN_SATELLITE_SE_SW,
    TVVAN_SATELLITE_SW_N,
    TVVAN_SATELLITE_NW_S,
    TVVAN_SATELLITE_SE_N,
    TVVAN_SATELLITE_NE_S,
    TVVAN_UNUSED1,
    TVVAN_UNUSED2,
    TVVAN_EXITTV,
    TVVAN_RADIO,
} TVVanTypes;

// Object Class
struct ObjectTVVan {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxFloor;
    Hitbox hitboxFront;
    Hitbox hitboxRoof;
    Hitbox hitboxEntry;
    Hitbox hitboxShopWindow;
    uint16 sfxDoor;
    uint16 sfxSatellite1;
    uint16 sfxSatellite2;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityTVVan {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw[DRAWGROUP_COUNT]);
    uint8 type;
    Vector2 vanPos;
    Vector2 movePos;
    int32 moveOffsetY;
    int32 stoodAngle;
    bool32 stood;
    int32 stoodPos;
    int32 timer;
    int32 targetAngles1[2];
    int32 targetAngles2[2];
    EntityTVVan *nextNode;
    EntityPlayer *player;
    Animator satelliteAnimator;
    Animator leftDoorAnimator;
    Animator rightDoorAnimator;
    Animator windowAnimator;
    Animator radioAnimator;
    Animator tvAnimator;
    Animator tvVanAnimator;
    Animator playerAnimator;
    Animator tvSnowAnimator;
};

// Object Struct
extern ObjectTVVan *TVVan;

// Standard Entity Events
void TVVan_Update(void);
void TVVan_LateUpdate(void);
void TVVan_StaticUpdate(void);
void TVVan_Draw(void);
void TVVan_Create(void *data);
void TVVan_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TVVan_EditorDraw(void);
void TVVan_EditorLoad(void);
#endif
void TVVan_Serialize(void);

// Extra Entity Functions
void TVVan_HandleVanTilt(void);

void TVVan_Draw_Van_Low(void);
void TVVan_Draw_Van_High(void);
void TVVan_Draw_Satellite_NE_SW(void);
void TVVan_Draw_Satellite_NW_SE(void);
void TVVan_Draw_Satellite_NW_SW(void);
void TVVan_Draw_Satellite_NE_SE(void);
void TVVan_Draw_Satellite_NE_NW(void);
void TVVan_Draw_Satellite_SE_SW(void);
void TVVan_Draw_Satellite_SW_N(void);
void TVVan_Draw_Satellite_NW_S(void);
void TVVan_Draw_Satellite_SE_N(void);
void TVVan_Draw_Satellite_NE_S(void);
void TVVan_Draw_Radio(void);
void TVVan_Draw_ExitTV_Active(void);
void TVVan_Draw_ExitTV_Destroyed(void);

void TVVan_StateVan_Idle(void);
void TVVan_CheckPlayerCollisions(void);
void TVVan_StateVan_BroadcastRadio(void);
void TVVan_StateVan_FinishedBroadcast(void);

void TVVan_StateRadio_StartBroadcast(void);
void TVVan_StateRadio_HandleMovement(void);
void TVVan_StateRadio_EnterSatellite1(void);
void TVVan_StateRadio_ExitSatellite2(void);

void TVVan_StateTV_Idle(void);
void TVVan_StateTV_Exploding(void);
void TVVan_StateTV_Destroyed(void);

void TVVan_StateRadio_EnterTV(void);

#endif //! OBJ_TVVAN_H
