#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[29], { 4,       0,        0, -0x20000, -0x28000, 1,        0, -0x20000, -0x28000, 2,        0, 0x20000, -0x20000, 3,       0,
                            0x28000, -0x20000, 4, 0,        -0x10000, -0x10000, 5, 0,        0x10000,  -0x10000, 6, 0,       -0x8000,  -0x10000 });
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
void WeatherMobile_Update(void);
void WeatherMobile_LateUpdate(void);
void WeatherMobile_StaticUpdate(void);
void WeatherMobile_Draw(void);
void WeatherMobile_Create(void* data);
void WeatherMobile_StageLoad(void);
void WeatherMobile_EditorDraw(void);
void WeatherMobile_EditorLoad(void);
void WeatherMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WEATHERMOBILE_H
