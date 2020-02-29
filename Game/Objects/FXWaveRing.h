#ifndef OBJ_FXWAVERING_H
#define OBJ_FXWAVERING_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXWaveRing : public Object {
public:

};

//Entity Class
class EntityFXWaveRing : public Entity {
public:

};

//Object Entity
ObjectFXWaveRing FXWaveRing;

//Entity Functions
void FXWaveRing_Update();
void FXWaveRing_EarlyUpdate();
void FXWaveRing_LateUpdate();
void FXWaveRing_Draw();
void FXWaveRing_Setup(void* subtype);
void FXWaveRing_StageLoad();
void FXWaveRing_GetAttributes();

#endif //!OBJ_FXWAVERING_H
