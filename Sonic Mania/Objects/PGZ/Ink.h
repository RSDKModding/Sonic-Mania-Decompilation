#ifndef OBJ_INK_H
#define OBJ_INK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 playerColours[4];
    uint16 spriteIndex;
    Hitbox hitbox;
    Hitbox hitbox2;
    Hitbox hitbox3;
} ObjectInk;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    uint8 inkedPlayers;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityInk;

// Object Struct
extern ObjectInk *Ink;

// Standard Entity Events
void Ink_Update(void);
void Ink_LateUpdate(void);
void Ink_StaticUpdate(void);
void Ink_Draw(void);
void Ink_Create(void* data);
void Ink_StageLoad(void);
void Ink_EditorDraw(void);
void Ink_EditorLoad(void);
void Ink_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_INK_H
