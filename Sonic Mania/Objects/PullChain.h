#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[18]; //= { 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3 };
    ushort aniFrames;
    ushort sfxPullChain;
} ObjectPullChain;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    bool32 walkOnto;
    byte tag;
    int field_64;
    int field_68;
    bool32 flag;
    int field_70;
    int field_74;
    int field_78;
    Animator animator;
    int length;
    int decorMode;
    byte activePlayers1;
    byte activePlayers2;
    Vector2 basePos;
    int field_A8;
    int chainOffset;
    int timer[4];
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    Hitbox hitbox;
    Animator animator2;
} EntityPullChain;

// Object Struct
extern ObjectPullChain *PullChain;

// Standard Entity Events
void PullChain_Update(void);
void PullChain_LateUpdate(void);
void PullChain_StaticUpdate(void);
void PullChain_Draw(void);
void PullChain_Create(void* data);
void PullChain_StageLoad(void);
void PullChain_EditorDraw(void);
void PullChain_EditorLoad(void);
void PullChain_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PULLCHAIN_H
