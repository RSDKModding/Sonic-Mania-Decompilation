#ifndef OBJ_SPIKES_H
#define OBJ_SPIKES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void (*stateDraw)();
    EntityAnimationData vData;
    EntityAnimationData hData;
    ushort spriteIndex;
    int dword3C;
    int dword40;
    ushort sfx_Move;
    ushort sfx_Spike;
} ObjectSpikes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int type;
    bool32 moving;
    byte count;
    byte stagger;
    short timer;
    int planeFilter;
    int dword6C;
    int dword70;
    Vector2 offset;
    short timer2;
    byte field_7E;
    Hitbox hitbox;
    EntityAnimationData animData;
} EntitySpikes;

// Object Struct
extern ObjectSpikes *Spikes;

// Standard Entity Events
void Spikes_Update();
void Spikes_LateUpdate();
void Spikes_StaticUpdate();
void Spikes_Draw();
void Spikes_Create(void* data);
void Spikes_StageLoad();
void Spikes_EditorDraw();
void Spikes_EditorLoad();
void Spikes_Serialize();

// Extra Entity Functions
void Spikes_StateDraw_Global();
void Spikes_StateDraw_Stage();

void Spikes_Unknown1(int a1, int a2);
void Spikes_CheckHit(EntityPlayer *player, int playerVelX, int playerVelY);

#endif //!OBJ_SPIKES_H
