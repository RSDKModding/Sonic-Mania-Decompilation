#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 printLayerID;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxBeep4;
    uint16 sfxFail;
    uint16 sfxButton2;
} ObjectSP500;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 len;
    int32 height;
    int32 offL;
    int32 offR;
    int32 startDir;
    int32 printDir;
    Vector2 srcC;
    Vector2 srcM;
    Vector2 srcY;
    int32 field_8C;
    int32 offset;
    int32 timer;
    uint8 activePlayers;
    uint8 playerTimers[2];
    uint8 field_9B;
    int32 field_9C;
    bool32 successFlag;
    bool32 failFlag;
    int32 field_A8;
    int32 inkColour;
    int32 xOffset;
    int32 yOffset;
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
#if RETRO_INCLUDE_EDITOR
void SP500_EditorDraw(void);
void SP500_EditorLoad(void);
#endif
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
