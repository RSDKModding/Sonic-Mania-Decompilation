#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

//Object Class
class WaterfallSound : Object {

};

//Entity Class
class EntityWaterfallSound : Entity {

};

//Entity Functions
void WaterfallSound_Update();
void WaterfallSound_EarlyUpdate();
void WaterfallSound_LateUpdate();
void WaterfallSound_Draw();
void WaterfallSound_Setup(void* subtype);
void WaterfallSound_StageLoad();
void WaterfallSound_GetAttributes();

#endif //!OBJ_WATERFALLSOUND_H
