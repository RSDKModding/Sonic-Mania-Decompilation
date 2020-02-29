#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "../../SonicMania.h"

//Object Class
class ObjectWeatherTV : public Object {
public:

};

//Entity Class
class EntityWeatherTV : public Entity {
public:

};

//Object Entity
ObjectWeatherTV WeatherTV;

//Entity Functions
void WeatherTV_Update();
void WeatherTV_EarlyUpdate();
void WeatherTV_LateUpdate();
void WeatherTV_Draw();
void WeatherTV_Setup(void* subtype);
void WeatherTV_StageLoad();
void WeatherTV_GetAttributes();

#endif //!OBJ_WEATHERTV_H
