#ifndef OBJ_MEGAOCTUS_H
#define OBJ_MEGAOCTUS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMegaOctus : Object {

};

// Entity Class
struct EntityMegaOctus : Entity {

};

// Object Entity
extern ObjectMegaOctus MegaOctus;

// Standard Entity Events
void MegaOctus_Update();
void MegaOctus_LateUpdate();
void MegaOctus_StaticUpdate();
void MegaOctus_Draw();
void MegaOctus_Create(void* data);
void MegaOctus_StageLoad();
void MegaOctus_EditorDraw();
void MegaOctus_EditorLoad();
void MegaOctus_Serialize();

// Extra Entity Functions


#endif //!OBJ_MEGAOCTUS_H
