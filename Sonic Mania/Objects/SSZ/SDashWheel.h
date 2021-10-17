#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

#include "SonicMania.h"

// Object Class (Old)
// Found this by digging into the static object files, just looks like an older version of the "SDashWheel" obj, nothing special here
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    TABLE(int32 heightArray[33], { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 19, 22, 26 });
    uint16 sfxBumper;
} ObjectRollSwitch;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    TABLE(int32 heightArray[33], { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 19, 22, 26 });
    uint16 sfxBumper;
    uint16 value5;
} ObjectSDashWheel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    bool32 walkOnto;
    uint8 tag;
    bool32 field_64;
    int32 field_68;
    bool32 activated;
    bool32 field_70;
    bool32 field_74;
    int32 field_78;
    Animator animator1;
    Animator animator3;
    Animator animator2;
    int32 cooldown;
    uint8 rotateOffset;
} EntitySDashWheel;

// Object Struct
extern ObjectSDashWheel *SDashWheel;

// Standard Entity Events
void SDashWheel_Update(void);
void SDashWheel_LateUpdate(void);
void SDashWheel_StaticUpdate(void);
void SDashWheel_Draw(void);
void SDashWheel_Create(void* data);
void SDashWheel_StageLoad(void);
void SDashWheel_EditorDraw(void);
void SDashWheel_EditorLoad(void);
void SDashWheel_Serialize(void);

// Extra Entity Functions
bool32 SDashWheel_CheckCB(void);
void SDashWheel_UpdateCB(int32 sfx);

#endif //!OBJ_SDASHWHEEL_H
