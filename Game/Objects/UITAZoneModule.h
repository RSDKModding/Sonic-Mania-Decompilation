#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

//Object Class
class UITAZoneModule : Object {

};

//Entity Class
class EntityUITAZoneModule : Entity {

};

//Entity Functions
void UITAZoneModule_Update();
void UITAZoneModule_EarlyUpdate();
void UITAZoneModule_LateUpdate();
void UITAZoneModule_Draw();
void UITAZoneModule_Setup(void* subtype);
void UITAZoneModule_StageLoad();
void UITAZoneModule_GetAttributes();

#endif //!OBJ_UITAZONEMODULE_H
