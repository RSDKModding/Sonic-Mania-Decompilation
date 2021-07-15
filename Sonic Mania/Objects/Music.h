#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

#include "../SonicMania.h"

enum MusicTracks {
    TRACK_NONE       = -1,
    TRACK_STAGE       = 0,
    TRACK_INVINCIBLE  = 1,
    TRACK_SNEAKERS    = 2,
    TRACK_MINIBOSS    = 3,
    TRACK_HBHBOSS     = 4,
    TRACK_EGGMAN1     = 5,
    TRACK_EGGMAN2     = 6,
    TRACK_ACTCLEAR    = 7,
    TRACK_DROWNING    = 8,
    TRACK_GAMEOVER    = 9,
    TRACK_SUPER       = 10,
#if RETRO_USE_PLUS
    TRACK_HBHMISCHIEF = 11,
    TRACK_NOLOAD      = 12,
#else
    TRACK_NOLOAD      = 11,
    TRACK_HBHMISCHIEF = 12,
#endif
    TRACK_1UP         = 13,
};

// Object Class
typedef struct {
    RSDK_OBJECT
    char trackNames[16][32];
    uint trackLoops[16];
    int dword244;
    int channelID;
    int activeTrack;
    int nextTrack;
    int field_254;
    int field_258;
} ObjectMusic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    TextInfo trackFile;
    TextInfo soundTestTitle;
    int trackID;
    uint trackLoop;
    bool32 playOnLoad;
    int field_78;
    int timer;
    int field_80;
    int field_84;
    float volume;
    float fadeSpeed;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
} EntityMusic;

// Object Struct
extern ObjectMusic *Music;

// Standard Entity Events
void Music_Update(void);
void Music_LateUpdate(void);
void Music_StaticUpdate(void);
void Music_Draw(void);
void Music_Create(void* data);
void Music_StageLoad(void);
void Music_EditorDraw(void);
void Music_EditorLoad(void);
void Music_Serialize(void);

// Extra Entity Functions
void Music_SetMusicTrack(const char *path, byte track, uint loopPoint);
void Music_State_PlayMusic(void);
void Music_PlayMusicTrack(byte trackID);
void Music_PlayTrack(byte trackID);
void Music_PlayTrackPtr(EntityMusic *entity);

void Music_Unknown2(byte trackID);
void Music_Unknown3(EntityMusic *entity);
bool32 Music_CheckMusicStack(void);
void Music_Unknown5(EntityMusic *entity);
void Music_ResumePrevTrack(byte trackID, bool32 transitionFade);
void Music_Unknown7(EntityMusic *entity);
void Music_ClearMusicStack(void);
void Music_TransitionTrack(byte trackID, float a2);
void Music_FadeOut(float fadeSpeed);
void Music_State_Unknown11(void);
void Music_State_Unknown12(void);
void Music_State_Unknown13(void);
void Music_State_FadeOut(void);
void Music_State_TransitionTrack(void);

#endif //!OBJ_MUSIC_H
