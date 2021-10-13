#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int screenID;
    int layerIDs[8];
    void (*switchCallback[8])(void);
    int field_48;
} ObjectBGSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    uint8 bgID;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
} EntityBGSwitch;

// Object Struct
extern ObjectBGSwitch *BGSwitch;

// Standard Entity Events
void BGSwitch_Update(void);
void BGSwitch_LateUpdate(void);
void BGSwitch_StaticUpdate(void);
void BGSwitch_Draw(void);
void BGSwitch_Create(void* data);
void BGSwitch_StageLoad(void);
void BGSwitch_EditorDraw(void);
void BGSwitch_EditorLoad(void);
void BGSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BGSWITCH_H
