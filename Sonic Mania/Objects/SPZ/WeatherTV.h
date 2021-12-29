#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "SonicMania.h"

// Object Class
struct ObjectWeatherTV {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 field_8;
    uint16 sfxCardAppear;
    uint16 sfxCardFlip;
    uint16 sfxCardSelected;
    uint16 sfxBuzz;
    bool32 playingTheBuzz;
    bool32 useHighLayer;
    uint8 field_1C;
};

// Entity Class
struct EntityWeatherTV {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer2;
    int32 timer;
    int32 field_68;
    int32 rectAlpha;
    Vector2 field_70;
    Vector2 drawPos1;
    Vector2 drawPos2;
    Vector2 drawPos3;
    Vector2 drawPos4;
    Vector2 drawPos5;
    Vector2 drawPos6;
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    int32 field_BC;
    int32 field_C0;
    int32 value;
    Vector2 field_C8;
    EntityWeatherMobile *eggman;
    Animator animator0;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
};

// Object Struct
extern ObjectWeatherTV *WeatherTV;

// Standard Entity Events
void WeatherTV_Update(void);
void WeatherTV_LateUpdate(void);
void WeatherTV_StaticUpdate(void);
void WeatherTV_Draw(void);
void WeatherTV_Create(void* data);
void WeatherTV_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void WeatherTV_EditorDraw(void);
void WeatherTV_EditorLoad(void);
#endif
void WeatherTV_Serialize(void);

// Extra Entity Functions
void WeatherTV_ShutdownTV(void);
void WeatherTV_DrawTV(void);
void WeatherTV_Unknown2(void);
void WeatherTV_Unknown3(void);
void WeatherTV_DrawNumbers(void);

void WeatherTV_StateDraw_Unknown1(void);
void WeatherTV_StateDraw_Unknown2(void);
void WeatherTV_StateDraw_Unknown3(void);
void WeatherTV_StateDraw_Unknown4(void);
void WeatherTV_StateDraw_Unknown5(void);
void WeatherTV_StateDraw_Unknown6(void);
void WeatherTV_StateDraw_Unknown7(void);
void WeatherTV_StateDraw_Unknown8(void);
void WeatherTV_StateDraw_Unknown9(void);
void WeatherTV_StateDraw_Unknown10(void);
void WeatherTV_StateDraw_Unknown11(void);
void WeatherTV_StateDraw_Unknown12(void);
void WeatherTV_StateDraw_Unknown13(void);
void WeatherTV_StateDraw_Unknown14(void);

void WeatherTV_State_Unknown1(void);
void WeatherTV_State_Unknown2(void);
void WeatherTV_State_Unknown3(void);
void WeatherTV_State_Unknown4(void);
void WeatherTV_State_Unknown5(void);
void WeatherTV_State_Unknown6(void);
void WeatherTV_State_Unknown13(void);
void WeatherTV_State_Unknown14(void);
void WeatherTV_State_Unknown8(void);
bool32 WeatherTV_Unknown30(void);
void WeatherTV_State_Unknown9(void);
void WeatherTV_State_Unknown10(void);
void WeatherTV_State_Unknown11(void);
void WeatherTV_State_Unknown12(void);
void WeatherTV_State_Unknown15(void);
void WeatherTV_State_Unknown16(void);
void WeatherTV_State_Unknown17(void);
void WeatherTV_State_Unknown18(void);
void WeatherTV_State_Unknown19(void);
void WeatherTV_State_Unknown20(void);
void WeatherTV_State_Outro(void);

#endif //!OBJ_WEATHERTV_H
