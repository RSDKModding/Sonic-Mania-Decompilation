#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

//Object Class
class WeatherTV : Object {

};

//Entity Class
class EntityWeatherTV : Entity {

};

//Entity Functions
void WeatherTV_Update();
void WeatherTV_EarlyUpdate();
void WeatherTV_LateUpdate();
void WeatherTV_Draw();
void WeatherTV_Setup(void* subtype);
void WeatherTV_StageLoad();
void WeatherTV_GetAttributes();

#endif //!OBJ_WEATHERTV_H
