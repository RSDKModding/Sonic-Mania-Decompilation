#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMonkeyDude;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMonkeyDude;

// Object Struct
extern ObjectMonkeyDude *MonkeyDude;

// Standard Entity Events
void MonkeyDude_Update(void);
void MonkeyDude_LateUpdate(void);
void MonkeyDude_StaticUpdate(void);
void MonkeyDude_Draw(void);
void MonkeyDude_Create(void* data);
void MonkeyDude_StageLoad(void);
void MonkeyDude_EditorDraw(void);
void MonkeyDude_EditorLoad(void);
void MonkeyDude_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MONKEYDUDE_H
