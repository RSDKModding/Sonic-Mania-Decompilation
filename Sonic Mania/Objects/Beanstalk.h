#ifndef OBJ_BEANSTALK_H
#define OBJ_BEANSTALK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBeanstalk;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBeanstalk;

// Object Struct
extern ObjectBeanstalk *Beanstalk;

// Standard Entity Events
void Beanstalk_Update();
void Beanstalk_LateUpdate();
void Beanstalk_StaticUpdate();
void Beanstalk_Draw();
void Beanstalk_Create(void* data);
void Beanstalk_StageLoad();
void Beanstalk_EditorDraw();
void Beanstalk_EditorLoad();
void Beanstalk_Serialize();

// Extra Entity Functions


#endif //!OBJ_BEANSTALK_H
