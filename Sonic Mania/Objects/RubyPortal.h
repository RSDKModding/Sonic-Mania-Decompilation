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
void RubyPortal_Update(void);
void RubyPortal_LateUpdate(void);
void RubyPortal_StaticUpdate(void);
void RubyPortal_Draw(void);
void RubyPortal_Create(void* data);
void RubyPortal_StageLoad(void);
void RubyPortal_EditorDraw(void);
void RubyPortal_EditorLoad(void);
void RubyPortal_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_RUBYPORTAL_H
