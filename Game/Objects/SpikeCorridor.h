#ifndef OBJ_SPIKECORRIDOR_H
#define OBJ_SPIKECORRIDOR_H

//Object Class
class SpikeCorridor : Object {

};

//Entity Class
class EntitySpikeCorridor : Entity {

};

//Entity Functions
void SpikeCorridor_Update();
void SpikeCorridor_EarlyUpdate();
void SpikeCorridor_LateUpdate();
void SpikeCorridor_Draw();
void SpikeCorridor_Setup(void* subtype);
void SpikeCorridor_StageLoad();
void SpikeCorridor_GetAttributes();

#endif //!OBJ_SPIKECORRIDOR_H
