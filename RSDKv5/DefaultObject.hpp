#ifndef OBJ_DEFAULTOBJECT_H
#define OBJ_DEFAULTOBJECT_H

// Object Class
struct ObjectDefaultObject : Object {

};

// Entity Class
struct EntityDefaultObject : Entity {

};

// Object Entity
extern ObjectDefaultObject *DefaultObject;

// Standard Entity Events
void DefaultObject_Update();
void DefaultObject_Create(void* data);

// Extra Entity Functions


#endif //!OBJ_DEFAULTOBJECT_H
