#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEncoreIntro : Object {

};

// Entity Class
struct EntityEncoreIntro : Entity {

};

// Object Entity
extern ObjectEncoreIntro EncoreIntro;

// Standard Entity Events
void EncoreIntro_Update();
void EncoreIntro_LateUpdate();
void EncoreIntro_StaticUpdate();
void EncoreIntro_Draw();
void EncoreIntro_Create(void* data);
void EncoreIntro_StageLoad();
void EncoreIntro_EditorDraw();
void EncoreIntro_EditorLoad();
void EncoreIntro_Serialize();

// Extra Entity Functions


#endif //!OBJ_ENCOREINTRO_H
