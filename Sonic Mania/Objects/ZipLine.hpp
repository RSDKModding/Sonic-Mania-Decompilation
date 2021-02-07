#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectZipLine : Object{

};

// Entity Class
struct EntityZipLine : Entity {

};

// Object Struct
extern ObjectZipLine *ZipLine;

// Standard Entity Events
void ZipLine_Update();
void ZipLine_LateUpdate();
void ZipLine_StaticUpdate();
void ZipLine_Draw();
void ZipLine_Create(void* data);
void ZipLine_StageLoad();
void ZipLine_EditorDraw();
void ZipLine_EditorLoad();
void ZipLine_Serialize();

// Extra Entity Functions


#endif //!OBJ_ZIPLINE_H
