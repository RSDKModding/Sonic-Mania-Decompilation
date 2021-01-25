#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomShinobi : Object {

};

// Entity Class
struct EntityPhantomShinobi : Entity {

};

// Object Entity
extern ObjectPhantomShinobi PhantomShinobi;

// Standard Entity Events
void PhantomShinobi_Update();
void PhantomShinobi_LateUpdate();
void PhantomShinobi_StaticUpdate();
void PhantomShinobi_Draw();
void PhantomShinobi_Create(void* data);
void PhantomShinobi_StageLoad();
void PhantomShinobi_EditorDraw();
void PhantomShinobi_EditorLoad();
void PhantomShinobi_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMSHINOBI_H
