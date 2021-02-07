#ifndef OBJ_TWISTEDTUBES_H
#define OBJ_TWISTEDTUBES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTwistedTubes : Object{

};

// Entity Class
struct EntityTwistedTubes : Entity {

};

// Object Struct
extern ObjectTwistedTubes *TwistedTubes;

// Standard Entity Events
void TwistedTubes_Update();
void TwistedTubes_LateUpdate();
void TwistedTubes_StaticUpdate();
void TwistedTubes_Draw();
void TwistedTubes_Create(void* data);
void TwistedTubes_StageLoad();
void TwistedTubes_EditorDraw();
void TwistedTubes_EditorLoad();
void TwistedTubes_Serialize();

// Extra Entity Functions


#endif //!OBJ_TWISTEDTUBES_H
