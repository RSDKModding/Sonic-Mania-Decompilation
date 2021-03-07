#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSentryBug : Object {
    int value1[84]; //= { -327680, 196608, -589824, 655360, -458752, 1048576, 458752, 196608, 196608, 655360, -196608, 1048576, -458752, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 393216, 655360, 0, 1048576, -589824, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 524288, 655360, 196608, 1048576, -589824, 196608, -720896, 655360, -524288, 1048576, 589824, 196608, 720896, 655360, 524288, 1048576, 589824, 196608, 720896, 655360, 524288, 1048576, -589824, 196608, -524288, 655360, -196608, 1048576, 458752, 196608, 720896, 655360, 524288, 1048576, -589824, 196608, -393216, 655360, 0, 1048576, 327680, 196608, 589824, 655360, 458752, 1048576, -458752, 196608, -196608, 655360, 196608, 1048576 };
    int value2[12]; //= { -65536, -65536, -131072, -32768, -65536, 32768, 65536, -65536, 131072, -32768, 65536, 32768 };
    int value3[12]; //= { 65536, -65536, 131072, -32768, 65536, 32768, -65536, -65536, -131072, -32768, -65536, 32768 };
    colour value4;
    ushort value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
};

// Entity Class
struct EntitySentryBug : Entity {

};

// Object Struct
extern ObjectSentryBug *SentryBug;

// Standard Entity Events
void SentryBug_Update();
void SentryBug_LateUpdate();
void SentryBug_StaticUpdate();
void SentryBug_Draw();
void SentryBug_Create(void* data);
void SentryBug_StageLoad();
void SentryBug_EditorDraw();
void SentryBug_EditorLoad();
void SentryBug_Serialize();

// Extra Entity Functions


#endif //!OBJ_SENTRYBUG_H
