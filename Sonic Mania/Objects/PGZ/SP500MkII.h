#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
} ObjectSP500MkII;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 start;
    uint8 yDir;
    int32 printRows;
    int32 length;
    int32 printRowID;
    int32 field_6C;
    bool32 field_70;
    bool32 field_74;
    bool32 printStarted;
    int32 field_7C;
    uint8 activePlayers;
    int32 field_84;
    Vector2 pos2;
    Vector2 pos;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 timer;
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
#if RETRO_INCLUDE_EDITOR
void SP500MkII_EditorDraw(void);
void SP500MkII_EditorLoad(void);
#endif
void SP500MkII_Serialize(void);

// Extra Entity Functions
void SP500MkII_Unknown1(void);
void SP500MkII_Unknown2(void);
void SP500MkII_Unknown3(void);

void SP500MkII_CheckPlayerCollisions(void);
void SP500MkII_Unknown5(void);
void SP500MkII_Unknown6(void);
void SP500MkII_Unknown7(void);
void SP500MkII_Unknown8(void);

#endif //!OBJ_SP500MKII_H
