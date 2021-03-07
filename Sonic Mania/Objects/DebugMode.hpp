#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDebugMode : Object {
    short objectIDs[0x100];
    void (*draw[0x100])();
    void (*spawn[0x100])();
    EntityAnimationData debugData;
    int objID;
    int itemCount;
    bool32 active;
    byte itemSubType;
    byte subtypeCount;
    byte field_A2A;
    byte field_A2B;
    int field_A2C;
};

// Entity Class
struct EntityDebugMode : Entity {

};

// Object Struct
extern ObjectDebugMode *DebugMode;

// Standard Entity Events
void DebugMode_Update();
void DebugMode_LateUpdate();
void DebugMode_StaticUpdate();
void DebugMode_Draw();
void DebugMode_Create(void* data);
void DebugMode_StageLoad();
void DebugMode_EditorDraw();
void DebugMode_EditorLoad();
void DebugMode_Serialize();

// Extra Entity Functions
void DebugMode_NullState();

#endif //!OBJ_DEBUGMODE_H
