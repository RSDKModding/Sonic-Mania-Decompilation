#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

#include "Game.h"

typedef enum {
    TRACK_NONE         = -1,
    TRACK_STAGE        = 0,
    TRACK_INVINCIBLE   = 1,
    TRACK_SNEAKERS     = 2,
    TRACK_MINIBOSS     = 3,
    TRACK_HBHBOSS      = 4,
    TRACK_EGGMAN1      = 5,
    TRACK_EGGMAN2      = 6,
    TRACK_RUBYPRESENCE = 5,
    TRACK_BUDDYBEAT    = 6,
    TRACK_ACTCLEAR     = 7,
    TRACK_DROWNING     = 8,
    TRACK_GAMEOVER     = 9,
    TRACK_SUPER        = 10,
    TRACK_ERZBOSS      = 10,
#if MANIA_USE_PLUS
    TRACK_HBHMISCHIEF = 11,
    TRACK_NOLOAD      = 12,
#else
    TRACK_NOLOAD      = 11,
    TRACK_HBHMISCHIEF = 12,
#endif
    TRACK_1UP = 13,
} MusicTracks;

typedef enum {
    TRACK_PRIORITY_NONE    = 0,
    TRACK_PRIORITY_ANY     = 10,
    TRACK_PRIORITY_POWERUP = 100,
    TRACK_PRIORITY_SUPER   = 1000,
    TRACK_PRIORITY_DROWN   = 1000,
    TRACK_PRIORITY_1UP     = 10000,
} TrackPriorityValues;

// Object Class
struct ObjectMusic {
    RSDK_OBJECT
    char trackNames[16][32];
    uint32 trackLoops[16];
    int32 trackStartPos;
    int32 channelID;
    int32 activeTrack;
#if MANIA_USE_PLUS
    int32 nextTrack;
    int32 restartTrackID;
#else
    int32 prevTrack;
    int32 queuedTrack;
    int32 nextTrack;
    bool32 playingRegularTrack;
    bool32 playingDrownTrack;
    bool32 playing1UPTrack;
#endif
    uint16 aniFrames;
};

// Entity Class
struct EntityMusic {
    RSDK_ENTITY
    StateMachine(state);
    String trackFile;
    String soundTestTitle;
    int32 trackID;
    uint32 trackLoop;
    bool32 playOnLoad;
    bool32 restartTrack;
    int32 timer;
    int32 trackPriority;
    int32 trackStartPos;
    float volume;
    float fadeSpeed;
    Animator animator;
};

// Object Struct
extern ObjectMusic *Music;

// Standard Entity Events
void Music_Update(void);
void Music_LateUpdate(void);
void Music_StaticUpdate(void);
void Music_Draw(void);
void Music_Create(void *data);
void Music_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Music_EditorDraw(void);
void Music_EditorLoad(void);
#endif
void Music_Serialize(void);

// Extra Entity Functions
void Music_SetMusicTrack(const char *path, uint8 track, uint32 loopPoint);
void Music_State_PlayOnLoad(void);
// Plays a track, and manages the music stack so it can be returned to
void Music_PlayQueuedTrack(uint8 trackID);
// Plays a track, doesn't use the music stack at all
void Music_PlayTrack(uint8 trackID);
// Play a track using the info from a music entity
void Music_PlayTrackPtr(EntityMusic *entity);

#if MANIA_USE_PLUS
void Music_PlayAutoMusicQueuedTrack(uint8 trackID);
void Music_HandleMusicStack_Powerups(EntityMusic *entity);
bool32 Music_CheckMusicStack_Active(void);
void Music_GetNextTrackStartPos(EntityMusic *entity);
#endif
void Music_EndQueuedTrack(uint8 trackID, bool32 transitionFade);
#if MANIA_USE_PLUS
void Music_HandleMusicStackTrackRemoval(EntityMusic *entity);
void Music_ClearMusicStack(void);
#endif
void Music_TransitionTrack(uint8 trackID, float fadeSpeed);
void Music_FadeOut(float fadeSpeed);

#if MANIA_USE_PLUS
void Music_State_HandleQueuedTrack(void);
void Music_State_FadeTrackOut(void);
#endif
void Music_State_FadeTrackIn(void);
void Music_State_FadeOut(void);
void Music_State_TransitionTrack(void);
#if !MANIA_USE_PLUS
void Music_State_1UPJingle(void);
#endif

#endif //! OBJ_MUSIC_H
