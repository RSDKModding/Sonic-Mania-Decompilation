#ifndef OBJ_HOTARUMKII_H
#define OBJ_HOTARUMKII_H

#include "../../SonicMania.h"

//Object Class
class ObjectHotaruMKII : public Object {
public:

};

//Entity Class
class EntityHotaruMKII : public Entity {
public:

};

//Object Entity
ObjectHotaruMKII HotaruMKII;

//Entity Functions
void HotaruMKII_Update();
void HotaruMKII_EarlyUpdate();
void HotaruMKII_LateUpdate();
void HotaruMKII_Draw();
void HotaruMKII_Setup(void* subtype);
void HotaruMKII_StageLoad();
void HotaruMKII_GetAttributes();

#endif //!OBJ_HOTARUMKII_H
