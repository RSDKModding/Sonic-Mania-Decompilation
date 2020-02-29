#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

//Object Class
class ZipLine : Object {

};

//Entity Class
class EntityZipLine : Entity {

};

//Entity Functions
void ZipLine_Update();
void ZipLine_EarlyUpdate();
void ZipLine_LateUpdate();
void ZipLine_Draw();
void ZipLine_Setup(void* subtype);
void ZipLine_StageLoad();
void ZipLine_GetAttributes();

#endif //!OBJ_ZIPLINE_H
