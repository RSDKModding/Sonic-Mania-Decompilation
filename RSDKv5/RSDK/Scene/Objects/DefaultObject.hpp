#ifndef OBJ_DEFAULTOBJECT_H
#define OBJ_DEFAULTOBJECT_H

namespace RSDK
{

// Object Class
struct ObjectDefaultObject : Object {
    // Nothin'
};

// Entity Class
struct EntityDefaultObject : Entity {
    // Nothin'
};

// Object Entity
extern ObjectDefaultObject *DefaultObject;

// Standard Entity Events
void DefaultObject_Update();
void DefaultObject_LateUpdate();
void DefaultObject_StaticUpdate();
void DefaultObject_Draw();
void DefaultObject_Create(void *data);
void DefaultObject_StageLoad();
void DefaultObject_EditorDraw();
void DefaultObject_EditorLoad();
void DefaultObject_Serialize();

// Extra Entity Functions

} // namespace RSDK

#endif //! OBJ_DEFAULTOBJECT_H
