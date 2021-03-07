#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectStickyPlatform : Object {

};

// Entity Class
struct EntityStickyPlatform : Entity {

};

// Object Struct
extern ObjectStickyPlatform *StickyPlatform;

// Standard Entity Events
void StickyPlatform_Update();
void StickyPlatform_LateUpdate();
void StickyPlatform_StaticUpdate();
void StickyPlatform_Draw();
void StickyPlatform_Create(void* data);
void StickyPlatform_StageLoad();
void StickyPlatform_EditorDraw();
void StickyPlatform_EditorLoad();
void StickyPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_STICKYPLATFORM_H
