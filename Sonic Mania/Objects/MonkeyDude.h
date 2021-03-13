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
void MonkeyDude_Update();
void MonkeyDude_LateUpdate();
void MonkeyDude_StaticUpdate();
void MonkeyDude_Draw();
void MonkeyDude_Create(void* data);
void MonkeyDude_StageLoad();
void MonkeyDude_EditorDraw();
void MonkeyDude_EditorLoad();
void MonkeyDude_Serialize();

// Extra Entity Functions


#endif //!OBJ_MONKEYDUDE_H
