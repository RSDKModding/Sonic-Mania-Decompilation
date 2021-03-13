#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[29]; //= { 4, 0, 0, -131072, -163840, 1, 0, -131072, -163840, 2, 0, 131072, -131072, 3, 0, 163840, -131072, 4, 0, -65536, -65536, 5, 0, 65536, -65536, 6, 0, -32768, -65536 };
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
    int value7;
    ushort value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
    ushort value13;
    byte value14;
    int value15;
    int value16;
    ushort value17;
    byte value18;
    ushort value19;
    ushort value20;
} ObjectWeatherMobile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWeatherMobile;

// Object Struct
extern ObjectWeatherMobile *WeatherMobile;

// Standard Entity Events
void WeatherMobile_Update();
void WeatherMobile_LateUpdate();
void WeatherMobile_StaticUpdate();
void WeatherMobile_Draw();
void WeatherMobile_Create(void* data);
void WeatherMobile_StageLoad();
void WeatherMobile_EditorDraw();
void WeatherMobile_EditorLoad();
void WeatherMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_WEATHERMOBILE_H
