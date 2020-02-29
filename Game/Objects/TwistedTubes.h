#ifndef OBJ_TWISTEDTUBES_H
#define OBJ_TWISTEDTUBES_H

#include "../../SonicMania.h"

//Object Class
class ObjectTwistedTubes : public Object {
public:

};

//Entity Class
class EntityTwistedTubes : public Entity {
public:

};

//Object Entity
ObjectTwistedTubes TwistedTubes;

//Entity Functions
void TwistedTubes_Update();
void TwistedTubes_EarlyUpdate();
void TwistedTubes_LateUpdate();
void TwistedTubes_Draw();
void TwistedTubes_Setup(void* subtype);
void TwistedTubes_StageLoad();
void TwistedTubes_GetAttributes();

#endif //!OBJ_TWISTEDTUBES_H
