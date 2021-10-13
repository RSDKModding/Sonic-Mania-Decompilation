#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    bool32 hasEggman;
    bool32 hasPhantomRider;
    Hitbox hitboxV;
    Hitbox hitboxH;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    uint16 sfxButton;
    uint16 field_32;
} ObjectButton;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    bool32 walkOnto;
    uint8 tag;
    int field_64;
    int field_68;
    bool32 activated;
    int field_70;
    int field_74;
    int field_78;
    Hitbox hitbox;
    Animator data1;
    Animator animator2;
} EntityButton;

// Object Struct
extern ObjectButton *Button;

// Standard Entity Events
void Button_Update(void);
void Button_LateUpdate(void);
void Button_StaticUpdate(void);
void Button_Draw(void);
void Button_Create(void* data);
void Button_StageLoad(void);
void Button_EditorDraw(void);
void Button_EditorLoad(void);
void Button_Serialize(void);

// Extra Entity Functions
void Button_CheckEggmanCollisions(void);
void Button_CheckPRiderCollisions(void);

void Button_TypeFloor(void);
void Button_TypeRoof(void);
void Button_TypeRWall(void);
void Button_TypeLWall(void);

#endif //!OBJ_BUTTON_H
