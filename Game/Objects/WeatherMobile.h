#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

//Object Class
class WeatherMobile : Object {

};

//Entity Class
class EntityWeatherMobile : Entity {

};

//Entity Functions
void WeatherMobile_Update();
void WeatherMobile_EarlyUpdate();
void WeatherMobile_LateUpdate();
void WeatherMobile_Draw();
void WeatherMobile_Setup(void* subtype);
void WeatherMobile_StageLoad();
void WeatherMobile_GetAttributes();

#endif //!OBJ_WEATHERMOBILE_H
