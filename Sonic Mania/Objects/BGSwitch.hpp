#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBGSwitch : Object {
    int layerID;
    int layerIDs[8];
    void (*switchCallback[8])();
    int field_48;
};

// Entity Class
struct EntityBGSwitch : Entity {
    Vector2 size;
    byte bgID;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
};

// Object Struct
extern ObjectBGSwitch *BGSwitch;

// Standard Entity Events
void BGSwitch_Update();
void BGSwitch_LateUpdate();
void BGSwitch_StaticUpdate();
void BGSwitch_Draw();
void BGSwitch_Create(void* data);
void BGSwitch_StageLoad();
void BGSwitch_EditorDraw();
void BGSwitch_EditorLoad();
void BGSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_BGSWITCH_H