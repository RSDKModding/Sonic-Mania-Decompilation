#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHeavyShinobi : Object {

};

// Entity Class
struct EntityHeavyShinobi : Entity {

};

// Object Entity
extern ObjectHeavyShinobi HeavyShinobi;

// Standard Entity Events
void HeavyShinobi_Update();
void HeavyShinobi_LateUpdate();
void HeavyShinobi_StaticUpdate();
void HeavyShinobi_Draw();
void HeavyShinobi_Create(void* data);
void HeavyShinobi_StageLoad();
void HeavyShinobi_EditorDraw();
void HeavyShinobi_EditorLoad();
void HeavyShinobi_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYSHINOBI_H
