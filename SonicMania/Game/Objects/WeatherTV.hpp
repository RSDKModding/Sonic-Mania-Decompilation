#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWeatherTV : Object {

};

// Entity Class
struct EntityWeatherTV : Entity {

};

// Object Entity
extern ObjectWeatherTV WeatherTV;

// Standard Entity Events
void WeatherTV_Update();
void WeatherTV_LateUpdate();
void WeatherTV_StaticUpdate();
void WeatherTV_Draw();
void WeatherTV_Create(void* data);
void WeatherTV_StageLoad();
void WeatherTV_EditorDraw();
void WeatherTV_EditorLoad();
void WeatherTV_Serialize();

// Extra Entity Functions


#endif //!OBJ_WEATHERTV_H
