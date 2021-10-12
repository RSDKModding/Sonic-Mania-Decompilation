#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
} ObjectSP500MkII;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte start;
    byte yDir;
    int printRows;
    int length;
    int field_68;
    int field_6C;
    bool32 field_70;
    bool32 field_74;
    int field_78;
    int field_7C;
    byte activePlayers;
    int field_84;
    Vector2 pos2;
    Vector2 pos;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int timer;
    Animator animator;
} EntitySP500MkII;

// Object Struct
extern ObjectSP500MkII *SP500MkII;

// Standard Entity Events
void SP500MkII_Update(void);
void SP500MkII_LateUpdate(void);
void SP500MkII_StaticUpdate(void);
void SP500MkII_Draw(void);
void SP500MkII_Create(void* data);
void SP500MkII_StageLoad(void);
void SP500MkII_EditorDraw(void);
void SP500MkII_EditorLoad(void);
void SP500MkII_Serialize(void);

// Extra Entity Functions
void SP500MkII_Unknown1(void);
void SP500MkII_Unknown2(void);
void SP500MkII_Unknown3(void);

void SP500MkII_Unknown4(void);
void SP500MkII_Unknown5(void);
void SP500MkII_Unknown6(void);
void SP500MkII_Unknown7(void);
void SP500MkII_Unknown8(void);

#endif //!OBJ_SP500MKII_H
