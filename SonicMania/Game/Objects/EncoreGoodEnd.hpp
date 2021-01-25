#ifndef OBJ_ENCOREGOODEND_H
#define OBJ_ENCOREGOODEND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEncoreGoodEnd : Object {

};

// Entity Class
struct EntityEncoreGoodEnd : Entity {

};

// Object Entity
extern ObjectEncoreGoodEnd EncoreGoodEnd;

// Standard Entity Events
void EncoreGoodEnd_Update();
void EncoreGoodEnd_LateUpdate();
void EncoreGoodEnd_StaticUpdate();
void EncoreGoodEnd_Draw();
void EncoreGoodEnd_Create(void* data);
void EncoreGoodEnd_StageLoad();
void EncoreGoodEnd_EditorDraw();
void EncoreGoodEnd_EditorLoad();
void EncoreGoodEnd_Serialize();

// Extra Entity Functions


#endif //!OBJ_ENCOREGOODEND_H
