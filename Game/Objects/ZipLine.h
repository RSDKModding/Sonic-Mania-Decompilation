#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectZipLine : public Object {
public:

};

//Entity Class
class EntityZipLine : public Entity {
public:

};

//Object Entity
ObjectZipLine ZipLine;

//Entity Functions
void ZipLine_Update();
void ZipLine_EarlyUpdate();
void ZipLine_LateUpdate();
void ZipLine_Draw();
void ZipLine_Setup(void* subtype);
void ZipLine_StageLoad();
void ZipLine_GetAttributes();

#endif //!OBJ_ZIPLINE_H
