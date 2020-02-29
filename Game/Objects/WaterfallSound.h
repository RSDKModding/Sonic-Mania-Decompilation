#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

#include "../../SonicMania.h"

//Object Class
class ObjectWaterfallSound : public Object {
public:

};

//Entity Class
class EntityWaterfallSound : public Entity {
public:

};

//Object Entity
ObjectWaterfallSound WaterfallSound;

//Entity Functions
void WaterfallSound_Update();
void WaterfallSound_EarlyUpdate();
void WaterfallSound_LateUpdate();
void WaterfallSound_Draw();
void WaterfallSound_Setup(void* subtype);
void WaterfallSound_StageLoad();
void WaterfallSound_GetAttributes();

#endif //!OBJ_WATERFALLSOUND_H
