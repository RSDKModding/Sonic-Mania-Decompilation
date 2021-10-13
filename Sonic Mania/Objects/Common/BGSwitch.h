#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 screenID;
    int32 layerIDs[8];
    void (*switchCallback[8])(void);
    int32 field_48;
} ObjectBGSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    uint8 bgID;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
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
