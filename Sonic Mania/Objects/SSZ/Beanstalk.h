#ifndef OBJ_BEANSTALK_H
#define OBJ_BEANSTALK_H

#include "SonicMania.h"

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
void Beanstalk_Update(void);
void Beanstalk_LateUpdate(void);
void Beanstalk_StaticUpdate(void);
void Beanstalk_Draw(void);
void Beanstalk_Create(void* data);
void Beanstalk_StageLoad(void);
void Beanstalk_EditorDraw(void);
void Beanstalk_EditorLoad(void);
void Beanstalk_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BEANSTALK_H
