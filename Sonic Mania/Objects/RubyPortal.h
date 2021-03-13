#ifndef OBJ_RUBYPORTAL_H
#define OBJ_RUBYPORTAL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRubyPortal;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRubyPortal;

// Object Struct
extern ObjectRubyPortal *RubyPortal;

// Standard Entity Events
void RubyPortal_Update();
void RubyPortal_LateUpdate();
void RubyPortal_StaticUpdate();
void RubyPortal_Draw();
void RubyPortal_Create(void* data);
void RubyPortal_StageLoad();
void RubyPortal_EditorDraw();
void RubyPortal_EditorLoad();
void RubyPortal_Serialize();

// Extra Entity Functions


#endif //!OBJ_RUBYPORTAL_H
