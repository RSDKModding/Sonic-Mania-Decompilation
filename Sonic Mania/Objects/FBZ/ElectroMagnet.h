#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    Vector2 onScreenRange;
    uint8 field_18;
    bool32 playingMagnetSFX;
    uint16 sfxMagnet;
} ObjectElectroMagnet;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    int32 height;
    int32 shieldRange;
    int32 timer;
    bool32 invisible;
    Hitbox hitbox;
    Hitbox playerHitbox;
} EntityElectroMagnet;

// Object Struct
extern ObjectElectroMagnet *ElectroMagnet;

// Standard Entity Events
void ElectroMagnet_Update(void);
void ElectroMagnet_LateUpdate(void);
void ElectroMagnet_StaticUpdate(void);
void ElectroMagnet_Draw(void);
void ElectroMagnet_Create(void* data);
void ElectroMagnet_StageLoad(void);
void ElectroMagnet_EditorDraw(void);
void ElectroMagnet_EditorLoad(void);
void ElectroMagnet_Serialize(void);

// Extra Entity Functions
void ElectroMagnet_Unknown1(void);
void ElectroMagnet_Unknown2(void);

#endif //!OBJ_ELECTROMAGNET_H
