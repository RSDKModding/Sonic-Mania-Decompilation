#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 dunkeyCode[18], { 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3 });
    uint16 aniFrames;
    uint16 sfxPullChain;
} ObjectPullChain;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    bool32 walkOnto;
    uint8 tag;
    int32 field_64;
    int32 field_68;
    bool32 activated;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Animator animator;
    int32 length;
    int32 decorMode;
    uint8 activePlayers1;
    uint8 activePlayers2;
    Vector2 basePos;
    int32 field_A8;
    int32 chainOffset;
    int32 timer[4];
    uint8 codeInputFlags;
    int32 field_D0[18];
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
#if RETRO_GAMEVER == VER_100
bool32 PullChain_HandleDunkeyCode(EntityPlayer *player);
#endif

#endif //!OBJ_PULLCHAIN_H
