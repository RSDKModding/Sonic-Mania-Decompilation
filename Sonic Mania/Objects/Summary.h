#ifndef OBJ_SUMMARY_H
#define OBJ_SUMMARY_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int value1; //= 240;
    int value2;
    int value3[4]; //= { 240, 3, 3, 3 };
} ObjectSummary;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySummary;

// Object Struct
extern ObjectSummary *Summary;

// Standard Entity Events
void Summary_Update();
void Summary_LateUpdate();
void Summary_StaticUpdate();
void Summary_Draw();
void Summary_Create(void* data);
void Summary_StageLoad();
void Summary_EditorDraw();
void Summary_EditorLoad();
void Summary_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_SUMMARY_H
