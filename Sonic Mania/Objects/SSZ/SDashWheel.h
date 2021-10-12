#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox;
    int heightArray[33]; //= { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 19, 22, 26 };
    ushort sfxBumper;
    ushort value5;
} ObjectSDashWheel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    bool32 walkOnto;
    byte tag;
    bool32 field_64;
    int field_68;
    bool32 activated;
    bool32 field_70;
    bool32 field_74;
    int field_78;
    Animator animator1;
    Animator animator3;
    Animator animator2;
    int cooldown;
    byte rotateOffset;
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
void SDashWheel_UpdateCB(int sfx);

#endif //!OBJ_SDASHWHEEL_H
