#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "Game.h"

typedef enum {
    WEATHERMOBILE_EGGMAN,
    WEATHERMOBILE_LIGHTS,
    WEATHERMOBILE_WIND_MANAGER,
    WEATHERMOBILE_WIND,
    WEATHERMOBILE_RAIN_MANAGER,
    WEATHERMOBILE_RAIN,
    WEATHERMOBILE_CLOUD,
} WeatherMobileTypes;

// Object Class
struct ObjectWeatherMobile {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0, 0x28000,
                                  -0x20000, 4, 0, -0x10000, -0x10000, 5, 0, 0x10000,  -0x10000, 6, 0, -0x8000, -0x10000 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 currentAttack;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxCloud;
    uint16 sfxLightning;
    uint16 sfxSun;
    uint16 sfxWind;
    bool32 playingWindSfx;
    int32 windSfxChannel;
    int32 windSfxTimer;
    uint16 sfxRain;
    bool32 playingRainSfx;
    uint16 aniFrames;
    uint16 eggmanFrames;
};

// Entity Class
struct EntityWeatherMobile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 lightsTimer;
    Vector2 origin;
    EntityWeatherMobile *parent;
    EntityWeatherTV *weatherTV;
    Animator mainAnimator;
    Animator seatAnimator;
    Animator eggmanAnimator;
    Animator eggMobileAnimator;
    Animator decorAnimator;
    Animator radarDishLAnimator;
    Animator radarDishRAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectWeatherMobile *WeatherMobile;

// Standard Entity Events
void WeatherMobile_Update(void);
void WeatherMobile_LateUpdate(void);
void WeatherMobile_StaticUpdate(void);
void WeatherMobile_Draw(void);
void WeatherMobile_Create(void *data);
void WeatherMobile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WeatherMobile_EditorDraw(void);
void WeatherMobile_EditorLoad(void);
#endif
void WeatherMobile_Serialize(void);

// Extra Entity Functions
void WeatherMobile_HandleAnimations(void);
void WeatherMobile_CheckPlayerCollisions(void);
void WeatherMobile_Hit(void);
void WeatherMobile_Explode(void);
void WeatherMobile_HandleDefeat(void);

void WeatherMobile_Draw_Eggman(void);
void WeatherMobile_Draw_Lights(void);
void WeatherMobile_Draw_Lightning(void);

void WeatherMobile_State_SetupArena(void);
void WeatherMobile_State_StartBoss(void);
void WeatherMobile_State_EnterEggman(void);
void WeatherMobile_State_HandleMovement(void);
void WeatherMobile_State_WindAttack(void);
void WeatherMobile_State_SunAttack(void);

void WeatherMobile_StateLights_EnterLights(void);
void WeatherMobile_StateLights_Shine(void);
void WeatherMobile_StateLights_DimAndDissapear(void);

void WeatherMobile_StateWindManager_SetupWind(void);
void WeatherMobile_StateWindManager_StrongWind(void);
void WeatherMobile_StateWindManager_FinishWind(void);

void WeatherMobile_State_Wind(void);

void WeatherMobile_State_RainManager(void);

void WeatherMobile_State_RainDrop(void);
void WeatherMobile_State_RainDropSplash(void);

void WeatherMobile_StateCloud_Idle(void);
void WeatherMobile_StateCloud_ShowTell(void);
void WeatherMobile_StateCloud_Strike(void);
void WeatherMobile_StateCloud_FadeAway(void);

void WeatherMobile_State_Defeated(void);
void WeatherMobile_State_DefeatFall(void);
void WeatherMobile_State_FleeRise(void);
void WeatherMobile_State_FleeAdjust(void);
void WeatherMobile_State_Flee(void);

#endif //! OBJ_WEATHERMOBILE_H
