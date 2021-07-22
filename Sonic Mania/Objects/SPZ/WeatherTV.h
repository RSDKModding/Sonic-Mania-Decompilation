#ifndef OBJ_WEATHERTV_H
#define OBJ_WEATHERTV_H

#include "SonicMania.h"

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
void WeatherTV_Update(void);
void WeatherTV_LateUpdate(void);
void WeatherTV_StaticUpdate(void);
void WeatherTV_Draw(void);
void WeatherTV_Create(void* data);
void WeatherTV_StageLoad(void);
void WeatherTV_EditorDraw(void);
void WeatherTV_EditorLoad(void);
void WeatherTV_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WEATHERTV_H
