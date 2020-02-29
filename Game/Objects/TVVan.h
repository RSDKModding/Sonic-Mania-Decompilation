#ifndef OBJ_TVVAN_H
#define OBJ_TVVAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectTVVan : public Object {
public:

};

//Entity Class
class EntityTVVan : public Entity {
public:

};

//Object Entity
ObjectTVVan TVVan;

//Entity Functions
void TVVan_Update();
void TVVan_EarlyUpdate();
void TVVan_LateUpdate();
void TVVan_Draw();
void TVVan_Setup(void* subtype);
void TVVan_StageLoad();
void TVVan_GetAttributes();

#endif //!OBJ_TVVAN_H
