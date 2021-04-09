#ifndef OBJ_LOGOSETUP_H
#define OBJ_LOGOSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int timer;
    ushort sfx_Sega;
} ObjectLogoSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
} EntityLogoSetup;

// Object Struct
extern ObjectLogoSetup *LogoSetup;

// Standard Entity Events
void LogoSetup_Update(void);
void LogoSetup_LateUpdate(void);
void LogoSetup_StaticUpdate(void);
void LogoSetup_Draw(void);
void LogoSetup_Create(void* data);
void LogoSetup_StageLoad(void);
void LogoSetup_EditorDraw(void);
void LogoSetup_EditorLoad(void);
void LogoSetup_Serialize(void);

// Extra Entity Functions
bool32 LogoSetup_ImageCallback(void);
void LogoSetup_CESAScreen(void);
void LogoSetup_SegaScreen(void);
void LogoSetup_Unknown2(void);
void LogoSetup_Unknown3(void);
void LogoSetup_Unknown4(void);

#endif //!OBJ_LOGOSETUP_H
