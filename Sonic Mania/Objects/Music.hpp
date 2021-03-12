#ifndef OBJ_MUSIC_H
#define OBJ_MUSIC_H

#include "../SonicMania.hpp"

enum MusicTracks {
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
    TRACK_HBHMISCHIEF = 11,
    TRACK_NOLOAD      = 12,
    TRACK_1UP         = 13,
};

// Object Class
struct ObjectMusic : Object {
    char trackNames[16][32];
    uint trackLoops[16];
    int dword244;
    int slotID;
    int activeTrack;
    int dword250;
    int field_254;
    int field_258;
};

// Entity Class
struct EntityMusic : Entity {
    void (*state)();
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
    float field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
};

// Object Struct
extern ObjectMusic *Music;

// Standard Entity Events
void Music_Update();
void Music_LateUpdate();
void Music_StaticUpdate();
void Music_Draw();
void Music_Create(void* data);
void Music_StageLoad();
void Music_EditorDraw();
void Music_EditorLoad();
void Music_Serialize();

// Extra Entity Functions
void Music_SetMusicTrack(const char *path, byte track, uint loopPoint);
void Music_State_PlayMusic();
void Music_PlayMusicTrack(byte trackID);
void Music_PlayTrack(byte trackID);

void Music_Unknown1(EntityMusic *entity);
void Music_Unknown2(byte trackID);
void Music_Unknown3(EntityMusic *entity);
bool32 Music_Unknown4();
void Music_Unknown5(EntityMusic *entity);
void Music_Unknown6(byte trackID, int a2);
void Music_Unknown7(EntityMusic *entity);
void Music_Unknown8();
void Music_Unknown9(byte trackID, float a2);
void Music_Unknown10(float a1);
void Music_Unknown11();
void Music_Unknown12();
void Music_Unknown13();
void Music_Unknown14();
void Music_Unknown15();

#endif //!OBJ_MUSIC_H
