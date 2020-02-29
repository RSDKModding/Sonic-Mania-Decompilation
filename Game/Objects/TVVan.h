#ifndef OBJ_TVVAN_H
#define OBJ_TVVAN_H

//Object Class
class TVVan : Object {

};

//Entity Class
class EntityTVVan : Entity {

};

//Entity Functions
void TVVan_Update();
void TVVan_EarlyUpdate();
void TVVan_LateUpdate();
void TVVan_Draw();
void TVVan_Setup(void* subtype);
void TVVan_StageLoad();
void TVVan_GetAttributes();

#endif //!OBJ_TVVAN_H
