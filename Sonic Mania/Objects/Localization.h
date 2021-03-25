#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

#include "../SonicMania.h"

typedef enum {
    STR_TESTSTR,
    STR_FEATUREUNIMPLIMENTED,
    STR_RESTARTWARNING,
    STR_QUITWARNINGLOSEPROGRESS,
    STR_QUITWARNING,
    STR_DELETEPOPUP,
    STR_CONNECTING,
    STR_LOADINGFROMSERVER,
    STR_COMMERROR,
    STR_NOWIFI,
    STR_EXITCOMP,
    STR_COMPTOTAL,
    STR_TAUNLOCKED,
    STR_COMPUNLOCKED,
    STR_PEELOUTUNLOCKED,
    STR_INSTASHIELDUNLOCKED,
    STR_ANDKNUXUNLOCKED,
    STR_DEBUGMODEUNLOCKED,
    STR_BLUESPHERESUNLOCKED,
    STR_MBMUNLOCKED,
    STR_DAGARDENUNLOCKED,
    STR_CONNECTXBOX,
    STR_LOADXBOX,
    STR_CONNECTPSN,
    STR_LOADPSN,
    STR_CONNECTSTEAM,
    STR_LOADSTEAM,
    STR_CONNECTNINTENDO,
    STR_LOADNINTENDO,
    STR_PLACEHOLDERLOGIN,
    STR_SAVELOADFAIL,
    STR_CORRUPTSAVE,
    STR_NOSAVESPACE,
    STR_NOXBOXPROFILE,
    STR_RECONNECTCONTROLLER,
    STR_RECONNECTWIRELESSCONTROLLER,
    STR_SIGNOUTDETECTED,
    STR_AUTOSAVENOTIF,
    STR_KEYALREADYBOUND,
    STR_KEYALREADYBOUNDP1,
    STR_KEYALREADYBOUNDP2,
    STR_APPLYCHANGEDSETTINGS,
    STR_VIDEOCHANGESAPPLIED,
    STR_STEAMOVERLAYUNAVALIABLE,
    STR_SAVEREPLAY,
    STR_NOREPLAYSPACE,
    STR_DELETEREPLAY,
    STR_ERRORLOADINGREPLAY,
    STR_CANNOTLOADREPLAY,
    STR_LOADING,
    STR_NOREPLAYS,
    STR_AREYOUSURE,
    STR_AREYOUSURESAVE,
    STR_UPTO4PLAYERS,
    STR_RESETTIMESWARNING,
    STR_MIGHTYRAYPLUS,
    STR_ENCOREREQUIRED,
    STR_RETRURNINGTOTITLE,
    STR_DEFAULT,
    STR_SELECTATRACK,
    STR_SELECTDATATOERASE,
    STR_RPC_PLAYING,
    STR_RPC_MENU,
    STR_RPC_MANIA,
    STR_RPC_ENCORE,
    STR_RPC_TA,
    STR_RPC_COMP,
    STR_RPC_TITLE,
} GameStrings;

// Object Class
typedef struct {
    RSDK_OBJECT
    TextInfo text;
    TextInfo strings[68];
    bool32 loaded;
    byte language;
} ObjectLocalization;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLocalization;

// Object Struct
extern ObjectLocalization *Localization;

// Standard Entity Events
void Localization_Update();
void Localization_LateUpdate();
void Localization_StaticUpdate();
void Localization_Draw();
void Localization_Create(void* data);
void Localization_StageLoad();
void Localization_EditorDraw();
void Localization_EditorLoad();
void Localization_Serialize();

// Extra Entity Functions
void Localization_LoadStrings();
void Localization_GetString(TextInfo *textInfo, byte id);
void Localization_GetZoneName(TextInfo *info, byte zone);
void Localization_SetZoneNameShort(TextInfo *info, byte zone);

#endif //!OBJ_LOCALIZATION_H
