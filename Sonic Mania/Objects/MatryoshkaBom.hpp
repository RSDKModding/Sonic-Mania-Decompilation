#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMatryoshkaBom : Object {

};

// Entity Class
struct EntityMatryoshkaBom : Entity {

};

// Object Entity
extern ObjectMatryoshkaBom MatryoshkaBom;

// Standard Entity Events
void MatryoshkaBom_Update();
void MatryoshkaBom_LateUpdate();
void MatryoshkaBom_StaticUpdate();
void MatryoshkaBom_Draw();
void MatryoshkaBom_Create(void* data);
void MatryoshkaBom_StageLoad();
void MatryoshkaBom_EditorDraw();
void MatryoshkaBom_EditorLoad();
void MatryoshkaBom_Serialize();

// Extra Entity Functions


#endif //!OBJ_MATRYOSHKABOM_H
