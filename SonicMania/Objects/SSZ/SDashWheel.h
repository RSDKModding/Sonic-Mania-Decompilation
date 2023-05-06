#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

#include "Game.h"

// Object Class (Old)
// Found this by digging into the static object files, just looks like an older version of the "SDashWheel" obj, nothing special here
struct ObjectRollSwitch {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    TABLE(int32 heightTable[33], { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 19, 22, 26 });
    uint16 sfxBumper;
};

// Object Class
struct ObjectSDashWheel {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxWheel;
    TABLE(int32 heightTable[33], { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 19, 22, 26 });
    uint16 sfxBumper;
    uint16 unused;
};

// Entity Class
struct EntitySDashWheel {
    MANIA_BUTTON_BASE

    Animator mainAnimator;
    Animator knobAnimator;
    Animator shineAnimator;
    int32 cooldown;
    uint8 rotateOffset;
};

// Object Struct
extern ObjectSDashWheel *SDashWheel;

// Standard Entity Events
void SDashWheel_Update(void);
void SDashWheel_LateUpdate(void);
void SDashWheel_StaticUpdate(void);
void SDashWheel_Draw(void);
void SDashWheel_Create(void *data);
void SDashWheel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SDashWheel_EditorDraw(void);
void SDashWheel_EditorLoad(void);
#endif
void SDashWheel_Serialize(void);

// Extra Entity Functions
bool32 SDashWheel_SfxCheck_MGZDoor(void);
void SDashWheel_SfxUpdate_MGZDoor(int32 sfx);

#endif //! OBJ_SDASHWHEEL_H
