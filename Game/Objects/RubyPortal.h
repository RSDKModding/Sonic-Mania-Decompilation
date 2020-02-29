#ifndef OBJ_RUBYPORTAL_H
#define OBJ_RUBYPORTAL_H

#include "../../SonicMania.h"

//Object Class
class ObjectRubyPortal : public Object {
public:

};

//Entity Class
class EntityRubyPortal : public Entity {
public:

};

//Object Entity
ObjectRubyPortal RubyPortal;

//Entity Functions
void RubyPortal_Update();
void RubyPortal_EarlyUpdate();
void RubyPortal_LateUpdate();
void RubyPortal_Draw();
void RubyPortal_Setup(void* subtype);
void RubyPortal_StageLoad();
void RubyPortal_GetAttributes();

#endif //!OBJ_RUBYPORTAL_H
