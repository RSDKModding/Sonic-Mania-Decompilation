#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWeatherMobile : Object{

};

// Entity Class
struct EntityWeatherMobile : Entity {

};

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
