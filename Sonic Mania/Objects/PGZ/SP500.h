#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort printLayerID;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    ushort aniFrames;
    ushort sfxBeep4;
    ushort sfxFail;
    ushort sfxButton2;
} ObjectSP500;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int len;
    int height;
    int offL;
    int offR;
    int startDir;
    int printDir;
    Vector2 srcC;
    Vector2 srcM;
    Vector2 srcY;
    int field_8C;
    int offset;
    int timer;
    byte activePlayers;
    byte playerTimers[2];
    byte field_9B;
    int field_9C;
    int flagA;
    int flagB;
    int field_A8;
    int field_AC;
    int xOffset;
    int yOffset;
    Entity *storedEntity;
    Animator animator;
} EntitySP500;

// Object Struct
extern ObjectSP500 *SP500;

// Standard Entity Events
void SP500_Update(void);
void SP500_LateUpdate(void);
void SP500_StaticUpdate(void);
void SP500_Draw(void);
void SP500_Create(void* data);
void SP500_StageLoad(void);
void SP500_EditorDraw(void);
void SP500_EditorLoad(void);
void SP500_Serialize(void);

// Extra Entity Functions
void SP500_Unknown1(void);
void SP500_Unknown2(void);
void SP500_Unknown3(void);
void SP500_Unknown4(void);
void SP500_Unknown5(void);
void SP500_Unknown6(void);
void SP500_Unknown7(void);
void SP500_Unknown8(void);
void SP500_Unknown9(void);
void SP500_Unknown10(void);

#endif //!OBJ_SP500_H
