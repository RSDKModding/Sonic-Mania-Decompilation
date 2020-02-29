#ifndef OBJ_TWISTEDTUBES_H
#define OBJ_TWISTEDTUBES_H

//Object Class
class TwistedTubes : Object {

};

//Entity Class
class EntityTwistedTubes : Entity {

};

//Entity Functions
void TwistedTubes_Update();
void TwistedTubes_EarlyUpdate();
void TwistedTubes_LateUpdate();
void TwistedTubes_Draw();
void TwistedTubes_Setup(void* subtype);
void TwistedTubes_StageLoad();
void TwistedTubes_GetAttributes();

#endif //!OBJ_TWISTEDTUBES_H
