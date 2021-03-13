#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWeatherTV;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWeatherTV;

// Object Struct
extern ObjectWeatherTV *WeatherTV;

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
