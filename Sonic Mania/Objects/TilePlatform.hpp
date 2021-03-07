#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTilePlatform : Object {

};

// Entity Class
struct EntityTilePlatform : Entity {

};

// Object Struct
extern ObjectTilePlatform *TilePlatform;

// Standard Entity Events
void TilePlatform_Update();
void TilePlatform_LateUpdate();
void TilePlatform_StaticUpdate();
void TilePlatform_Draw();
void TilePlatform_Create(void* data);
void TilePlatform_StageLoad();
void TilePlatform_EditorDraw();
void TilePlatform_EditorLoad();
void TilePlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_TILEPLATFORM_H
