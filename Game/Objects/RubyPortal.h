#ifndef OBJ_RUBYPORTAL_H
#define OBJ_RUBYPORTAL_H

//Object Class
class RubyPortal : Object {

};

//Entity Class
class EntityRubyPortal : Entity {

};

//Entity Functions
void RubyPortal_Update();
void RubyPortal_EarlyUpdate();
void RubyPortal_LateUpdate();
void RubyPortal_Draw();
void RubyPortal_Setup(void* subtype);
void RubyPortal_StageLoad();
void RubyPortal_GetAttributes();

#endif //!OBJ_RUBYPORTAL_H
