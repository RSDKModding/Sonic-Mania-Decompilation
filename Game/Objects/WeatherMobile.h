#ifndef OBJ_WEATHERMOBILE_H
#define OBJ_WEATHERMOBILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectWeatherMobile : public Object {
public:

};

//Entity Class
class EntityWeatherMobile : public Entity {
public:

};

//Object Entity
ObjectWeatherMobile WeatherMobile;

//Entity Functions
void WeatherMobile_Update();
void WeatherMobile_EarlyUpdate();
void WeatherMobile_LateUpdate();
void WeatherMobile_Draw();
void WeatherMobile_Setup(void* subtype);
void WeatherMobile_StageLoad();
void WeatherMobile_GetAttributes();

#endif //!OBJ_WEATHERMOBILE_H
