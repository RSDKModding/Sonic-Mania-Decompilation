#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "SonicMania.h"

typedef enum {
    WEATHERMOBILE_EGGMAN,
    WEATHERMOBILE_LIGHTS,
    WEATHERMOBILE_WIND_MANAGER,
    WEATHERMOBILE_WIND,
    WEATHERMOBILE_RAIN_MANAGER,
    WEATHERMOBILE_RAIN,
    WEATHERMOBILE_CLOUD,
}WeatherMobileTypes;

// Object Class
struct ObjectWeatherMobile {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0,
                            0x28000, -0x20000, 4, 0,        -0x10000, -0x10000, 5, 0,        0x10000,  -0x10000, 6, 0,       -0x8000,  -0x10000 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 field_8C;
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
    int32 timer2;
    Vector2 drawPos;
    Entity *parent;
    Entity *weatherTV;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Hitbox hitbox;
};

// Object Struct
extern ObjectWeatherMobile *WeatherMobile;

// Standard Entity Events
void WeatherMobile_Update(void);
void WeatherMobile_LateUpdate(void);
void WeatherMobile_StaticUpdate(void);
void WeatherMobile_Draw(void);
void WeatherMobile_Create(void* data);
void WeatherMobile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void WeatherMobile_EditorDraw(void);
void WeatherMobile_EditorLoad(void);
#endif
void WeatherMobile_Serialize(void);

// Extra Entity Functions
void WeatherMobile_HandleAnimations(void);
void WeatherMobile_CheckPlayerCollisions(void);
void WeatherMobile_HandleDefeat(void);

void WeatherMobile_StateDraw_Unknown1(void);
void WeatherMobile_StateDraw1_Unknown(void);
void WeatherMobile_StateDraw6_Unknown1(void);

void WeatherMobile_State_SetupArena(void);
void WeatherMobile_State_StartBoss(void);
void WeatherMobile_State_Unknown2(void);
void WeatherMobile_State_Unknown3(void);
void WeatherMobile_State_Unknown4(void);
void WeatherMobile_State_Unknown5(void);

void WeatherMobile_State1_Unknown1(void);
void WeatherMobile_State1_Unknown2(void);
void WeatherMobile_State1_Unknown3(void);

void WeatherMobile_State2_Unknown1(void);
void WeatherMobile_State2_Unknown2(void);
void WeatherMobile_State2_Unknown3(void);

void WeatherMobile_State3_Unknown(void);

void WeatherMobile_State4_Unknown(void);

void WeatherMobile_State5_Unknown1(void);
void WeatherMobile_State5_Unknown2(void);

void WeatherMobile_State6_Unknown1(void);
void WeatherMobile_State6_Unknown2(void);
void WeatherMobile_State6_Unknown3(void);
void WeatherMobile_State6_Unknown4(void);

void WeatherMobile_State_Defeated(void);
void WeatherMobile_State_Unknown6(void);
void WeatherMobile_State_Unknown7(void);
void WeatherMobile_State_Unknown8(void);
void WeatherMobile_State_Unknown9(void);

#endif //!OBJ_WEATHERMOBILE_H
