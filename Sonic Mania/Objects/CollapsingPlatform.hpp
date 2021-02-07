#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCollapsingPlatform : Object{

};

// Entity Class
struct EntityCollapsingPlatform : Entity {

};

// Object Struct
extern ObjectCollapsingPlatform *CollapsingPlatform;

// Standard Entity Events
void CollapsingPlatform_Update();
void CollapsingPlatform_LateUpdate();
void CollapsingPlatform_StaticUpdate();
void CollapsingPlatform_Draw();
void CollapsingPlatform_Create(void* data);
void CollapsingPlatform_StageLoad();
void CollapsingPlatform_EditorDraw();
void CollapsingPlatform_EditorLoad();
void CollapsingPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_COLLAPSINGPLATFORM_H
