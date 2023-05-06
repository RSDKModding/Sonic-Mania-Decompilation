#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "Game.h"

// Object Class
struct ObjectWeatherTV {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 prevCardAngle;
    uint16 sfxCardAppear;
    uint16 sfxCardFlip;
    uint16 sfxCardSelected;
    uint16 sfxBuzz;
    bool32 playingTheBuzz;
    bool32 useHighLayer;
    uint8 attackList;
};

// Entity Class
struct EntityWeatherTV {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 scanlineAlpha;
    int32 logoAlpha;
    int32 rectAlpha;
    Vector2 tvActivateSize;
    Vector2 logoPos;
    Vector2 weatherIconPos;
    Vector2 cluckoidPos;
    Vector2 cluckoidVelocity;
    Vector2 micPos;
    Vector2 rainPos;
    int32 micVel;
    int32 weatherIconVel;
    int32 rainVel;
    int32 rainAlpha;
    int32 cardSpinSpeed;
    int32 cardSpinCount;
    int32 bgCloudPos;
    int32 temperature;
    Vector2 liveScale;
    EntityWeatherMobile *eggman;
    Animator rainAnimator;
    Animator bgAnimator;
    Animator tvAnimator;
    Animator logosAnimator;
    Animator unusedAnimator;
    Animator microphoneAnimator;
    Animator cluckoidAnimator;
    Animator weatherIconBGAnimator;
    Animator weatherIconAnimator;
    Animator numbersAnimator;
};

// Object Struct
extern ObjectWeatherTV *WeatherTV;

// Standard Entity Events
void WeatherTV_Update(void);
void WeatherTV_LateUpdate(void);
void WeatherTV_StaticUpdate(void);
void WeatherTV_Draw(void);
void WeatherTV_Create(void *data);
void WeatherTV_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WeatherTV_EditorDraw(void);
void WeatherTV_EditorLoad(void);
#endif
void WeatherTV_Serialize(void);

// Extra Entity Functions
void WeatherTV_ShutdownTV(void);
void WeatherTV_DrawTV(void);
void WeatherTV_DrawScanlines(void);
void WeatherTV_DrawTVBackground(void);
void WeatherTV_DrawNumbers(void);

bool32 WeatherTV_CheckEggmanBusy(void);

void WeatherTV_Draw_Off(void);
void WeatherTV_Draw_TurningOn(void);
void WeatherTV_Draw_EnterEggTVLogo(void);
void WeatherTV_Draw_ExitEggTVLogo(void);
void WeatherTV_Draw_ShowWeatherChannel(void);
void WeatherTV_Draw_ShowCluckoid(void);
void WeatherTV_Draw_SpinningCard(void);
void WeatherTV_Draw_ShowCard(void);
void WeatherTV_Draw_LightAttack(void);
void WeatherTV_Draw_SuperHot(void);
void WeatherTV_Draw_WindAttack(void);
void WeatherTV_Draw_RainAttack(void);
void WeatherTV_Draw_Buzzing(void);
void WeatherTV_Draw_Outro(void);

void WeatherTV_State_TurnOn(void);
void WeatherTV_State_FinishTurningOn(void);
void WeatherTV_State_EnterEggTVLogo(void);
void WeatherTV_State_ExitEggTVLogo(void);
void WeatherTV_State_ShowWeatherChannel(void);
void WeatherTV_State_ShowCluckoid(void);
void WeatherTV_State_ChooseNextAttack(void);
void WeatherTV_State_CardSpinning(void);
void WeatherTV_State_ShowCard(void);
void WeatherTV_State_BeginLightAttack(void);
void WeatherTV_State_HeatingUp(void);
void WeatherTV_State_BoilingHot(void);
void WeatherTV_State_CoolDown(void);
void WeatherTV_State_BeginWindAttack(void);
void WeatherTV_State_WindyWeather(void);
void WeatherTV_State_FinishWindAttack(void);
void WeatherTV_State_BeginRainAttack(void);
void WeatherTV_State_StartRaining(void);
void WeatherTV_State_StopRaining(void);
void WeatherTV_State_Outro(void);

#endif //! OBJ_WEATHERTV_H
