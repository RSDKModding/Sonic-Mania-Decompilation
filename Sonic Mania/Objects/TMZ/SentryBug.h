#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[84]; //= { -327680, 196608, -589824, 655360, -458752, 1048576, 458752, 196608, 196608, 655360, -196608, 1048576, -458752, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 393216, 655360, 0, 1048576, -589824, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 524288, 655360, 196608, 1048576, -589824, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 720896, 655360, 524288, 1048576, 589824, 196608, 720896, 655360, 524288, 1048576, -589824, 196608, -524288, 655360, -196608, 1048576, 458752, 196608, 720896, 655360, 524288, 1048576, -589824, 196608, -393216, 655360, 0, 1048576, 327680, 196608, 589824, 655360, 458752, 1048576, -458752, 196608, -196608, 655360, 196608, 1048576 };
    int value2[12]; //= { -65536, -65536, -131072, -32768, -65536, 32768, 65536, -65536, 131072, -32768, 65536, 32768 };
    int value3[12]; //= { 65536, -65536, 131072, -32768, 65536, 32768, -65536, -65536, -131072, -32768, -65536, 32768 };
    colour value4;
    ushort value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
} ObjectSentryBug;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySentryBug;

// Object Struct
extern ObjectSentryBug *SentryBug;

// Standard Entity Events
void SentryBug_Update(void);
void SentryBug_LateUpdate(void);
void SentryBug_StaticUpdate(void);
void SentryBug_Draw(void);
void SentryBug_Create(void* data);
void SentryBug_StageLoad(void);
void SentryBug_EditorDraw(void);
void SentryBug_EditorLoad(void);
void SentryBug_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SENTRYBUG_H
