#ifndef OBJ_SMOG_H
#define OBJ_SMOG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    ScanlineInfo *scanlines;
    uint8 scanlineData[0x100 * sizeof(ScanlineInfo)];
    uint16 aniFrames;
    int32 starPostID;
} ObjectSmog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 timer;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
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
