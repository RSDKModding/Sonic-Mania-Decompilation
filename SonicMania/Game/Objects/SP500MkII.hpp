#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSP500MkII : Object {

};

// Entity Class
struct EntitySP500MkII : Entity {

};

// Object Entity
extern ObjectSP500MkII SP500MkII;

// Standard Entity Events
void SP500MkII_Update();
void SP500MkII_LateUpdate();
void SP500MkII_StaticUpdate();
void SP500MkII_Draw();
void SP500MkII_Create(void* data);
void SP500MkII_StageLoad();
void SP500MkII_EditorDraw();
void SP500MkII_EditorLoad();
void SP500MkII_Serialize();

// Extra Entity Functions


#endif //!OBJ_SP500MKII_H
