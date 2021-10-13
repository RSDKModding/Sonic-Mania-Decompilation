#ifndef OBJ_SMOG_H
#define OBJ_SMOG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    ScanlineInfo *scanlines;
    uint8 scanlineData[0x100 * sizeof(ScanlineInfo)];
    uint16 aniFrames;
    int starPostID;
} ObjectSmog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int timer;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
} EntitySmog;

// Object Struct
extern ObjectSmog *Smog;

// Standard Entity Events
void Smog_Update(void);
void Smog_LateUpdate(void);
void Smog_StaticUpdate(void);
void Smog_Draw(void);
void Smog_Create(void* data);
void Smog_StageLoad(void);
void Smog_EditorDraw(void);
void Smog_EditorLoad(void);
void Smog_Serialize(void);

// Extra Entity Functions
void Smog_DrawLayerCB(void);

#endif //!OBJ_SMOG_H
