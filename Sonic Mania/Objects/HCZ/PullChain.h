#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "SonicMania.h"

// Object Class
struct ObjectPullChain {
    RSDK_OBJECT
    TABLE(int32 dunkeyCode[18], { 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3 });
    uint16 aniFrames;
    uint16 sfxPullChain;
};

// Entity Class
struct EntityPullChain {
    RSDK_ENTITY
    int32 type;
    bool32 walkOnto;
    uint8 tag;
    bool32 down;            // 64
    bool32 toggled;         // 68
    bool32 currentlyActive; // 6B
    bool32 activated;       // 70
    bool32 wasActivated;    // 74
    int32 pressPos;         // 78
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
};

// Object Struct
extern ObjectPullChain *PullChain;

// Standard Entity Events
void PullChain_Update(void);
void PullChain_LateUpdate(void);
void PullChain_StaticUpdate(void);
void PullChain_Draw(void);
void PullChain_Create(void* data);
void PullChain_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PullChain_EditorDraw(void);
void PullChain_EditorLoad(void);
#endif
void PullChain_Serialize(void);

// Extra Entity Functions
#if RETRO_GAMEVER == VER_100
bool32 PullChain_HandleDunkeyCode(EntityPlayer *player);
#endif

#endif //!OBJ_PULLCHAIN_H
