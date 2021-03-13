#include "../SonicMania.h"

ObjectMusic *Music;

void Music_Update()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void Music_LateUpdate() {}

void Music_StaticUpdate() {}

void Music_Draw() {}

void Music_Create(void *data)
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ((RSDK_sceneInfo->state & 3) != 3) + 1;
        if (entity->trackFile.textLength) {
            if (entity->trackID != TRACK_NOLOAD) {
                RSDK.CopyString(Music->trackNames[entity->trackID], &entity->trackFile);
                Music->trackLoops[entity->trackID] = entity->trackLoop;
                if (entity->playOnLoad)
                    entity->state = Music_State_PlayMusic;
                else
                    RSDK.ResetEntityPtr(entity, 0, 0);
            }
        }
    }
}

void Music_StageLoad()
{
    //Slot 0: stage music
    Music_SetMusicTrack("Invincible.ogg", TRACK_INVINCIBLE, 139263);
    Music_SetMusicTrack("Sneakers.ogg", TRACK_SNEAKERS, 120960);
    Music_SetMusicTrack("BossMini.ogg", TRACK_MINIBOSS, 276105);
    Music_SetMusicTrack("BossHBH.ogg", TRACK_HBHBOSS, 70560);
    Music_SetMusicTrack("BossEggman1.ogg", TRACK_EGGMAN1, 282240);
    Music_SetMusicTrack("BossEggman2.ogg", TRACK_EGGMAN2, 264600);
    Music_SetMusicTrack("ActClear.ogg", TRACK_ACTCLEAR, false);
    Music_SetMusicTrack("Drowning.ogg", TRACK_DROWNING, false);
    Music_SetMusicTrack("GameOver.ogg", TRACK_GAMEOVER, false);
    Music_SetMusicTrack("Super.ogg", TRACK_SUPER, 165375);
    Music_SetMusicTrack("HBHMischief.ogg", TRACK_HBHMISCHIEF, 381405);
    //Slot 12: unknown
    Music_SetMusicTrack("1up.ogg", TRACK_1UP, false);

    if (options->suppressAutoMusic) {
        if (options->restartMusicID > 0)
            Music->activeTrack = options->restartMusicID;
    }
    else {
        Music->activeTrack = TRACK_STAGE;
    }

    options->restartMusicID = 0;
    Music->dword250         = -1;
#if RETRO_USE_PLUS
    if (RSDK_sku->platform == PLATFORM_DEV)
        RSDK.SetDebugValue("Vape Mode", &options->vapeMode, 1, 0, 1);
#endif
}

void Music_SetMusicTrack(const char *path, byte track, uint loopPoint)
{
    track &= 0xF;

    int charID = 0;
    for (; charID < 0x20; ++charID) {
        if (!path[charID]) {
            break;
        }
        else {
            Music->trackNames[track][charID] = path[charID];
        }
    }
    Music->trackNames[track][charID] = 0;
    Music->trackLoops[track]         = loopPoint;
}

void Music_State_PlayMusic()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (options->suppressAutoMusic) {
        options->suppressAutoMusic = 0;
        switch (Music->activeTrack) {
            case 1:
            case 2:
            case 13:
                Music_Unknown9(entity->trackID, 0.025);
                RSDK.ResetEntityPtr(entity, 0, 0);
                break;
            case 10:
                Music_Unknown2(Music->activeTrack);
                RSDK.ResetEntityPtr(entity, 0, 0);
                break;
            default: RSDK.ResetEntityPtr(entity, 0, 0); break;
        }
    }
    else {
        Music_PlayTrack(entity->trackID);
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void Music_PlayMusicTrack(byte trackID)
{
    trackID &= 0xF;

    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            RSDK.ResetEntityPtr(music, 0, 0);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = 40; slot < 48; ++slot) {
        entity = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, 0, 0);
            break;
        }
    }

    entity->trackID  = trackID;
    entity->state    = Music_Unknown11;
    entity->field_80 = 0;
    entity->field_8C = 0.079999998;
    switch (trackID) {
        case 1:
        case 2:
            Music_Unknown3(entity);
            entity->field_80 = 100;
            entity->field_78 = 1;
            break;
        case 8:
            entity->timer    = 960;
            entity->field_80 = 10000;
            entity->field_78 = 1;
            break;
        case 10:
            entity->field_80 = 1000;
            entity->field_78 = 1;
            break;
        case 13:
            entity->timer    = 224;
            entity->field_80 = 100000;
            break;
        default: break;
    }

    Music_Unknown5(entity);

    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music != entity) {
            if (music->field_80 > entity->field_80) {
                entity->volume = 0.0;
                return;
            }
        }
    }

    RSDK.SoundUnknown1(Music->slotID);
    Music->activeTrack = trackID;
    Music->slotID      = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
    if (options->vapeMode)
        RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
#endif
    entity->volume = 1.0;
}

void Music_PlayTrack(byte trackID)
{
    trackID &= 0xF;
    if (trackID == TRACK_ACTCLEAR) {
        Music_Unknown8();

        RSDK.ResetEntitySlot(SLOT_MUSIC, 0, 0);
        RSDK.SoundUnknown1(Music->slotID);
        Music->activeTrack = trackID;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (options->vapeMode)
            RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
#endif
    }
    else if (Music_Unknown4()) {
        Music->dword250 = trackID;
    }
    else {
        RSDK.ResetEntitySlot(SLOT_MUSIC, 0, 0);
        RSDK.SoundUnknown1(Music->slotID);
        Music->activeTrack = trackID;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (options->vapeMode)
            RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
#endif
    }
}

void Music_Unknown1(EntityMusic *entity)
{
    RSDK.CopyString(Music->trackNames[0], &entity->trackFile);
    Music->trackLoops[0] = entity->trackLoop;
    RSDK.ResetEntitySlot(SLOT_MUSIC, 0, 0);
    RSDK.SoundUnknown1(Music->slotID);
    Music->activeTrack = 0;
    Music->dword244    = 0;
    Music->slotID      = RSDK.PlayMusic(Music->trackNames[0], 0, 0, Music->trackLoops[0], true);

#if RETRO_USE_PLUS
    if (options->vapeMode)
        RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
#endif
}

void Music_Unknown2(byte trackID)
{
    trackID &= 0xF;
    Music->dword250 = 0;

    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            RSDK.ResetEntityPtr(music, 0, 0);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = 40; slot < 48; ++slot) {
        entity = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, 0, 0);
            break;
        }
    }

    entity->trackID  = trackID;
    entity->state    = Music_Unknown11;
    entity->field_80 = 0;
    entity->volume   = 1.0;
    entity->field_8C = 0.079999998;
    switch (trackID) {
        case 1:
        case 2:
            Music_Unknown3(entity);
            entity->field_80 = 100;
            entity->field_78 = 1;
            break;
        case 8:
            entity->timer    = 960;
            entity->field_80 = 10000;
            entity->field_78 = 1;
            break;
        case 10:
            entity->field_80 = 1000;
            entity->field_78 = 1;
            break;
        case 13:
            entity->timer    = 224;
            entity->field_80 = 100000;
            break;
        default: break;
    }
}

void Music_Unknown3(EntityMusic *entity)
{
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music->field_80 == 100 && music->trackID != entity->trackID && music != entity)
            music->field_80 = 10;
    }
}
bool32 Music_Unknown4()
{
    bool32 flag = false;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music->field_80 > 0)
            flag = true;
    }
    return flag;
}
void Music_Unknown5(EntityMusic *entity)
{
    int activeCnt = 0;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && entity != music) {
            if (music->trackID == Music->activeTrack) {
                entity->field_84 = RSDK.TrackPlaying(Music->slotID);
                entity->volume   = 0.0;
            }
            activeCnt++;
        }
    }

    if (!activeCnt) {
        if (Music->activeTrack > -1) {
            if (Music->dword250 == -1) {
                Music->dword250 = Music->activeTrack;
            }
            Music->dword244 = RSDK.TrackPlaying(Music->slotID);
        }
    }
}
void Music_Unknown6(byte trackID, int a2)
{
    trackID &= 0xF;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            if (a2) {
                music->state    = Music_Unknown12;
                music->field_8C = 0.050000001;
            }
            else {
                music->timer = 1;
            }
        }
    }
}
void Music_Unknown7(EntityMusic *entity)
{
    if (entity) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
        if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
            RSDK.ResetEntityPtr(music, 0, 0);

            for (int slot = 40; slot < 48; ++slot) {
                EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
                if (music->objectID == Music->objectID && music->field_80 > entity->field_80) {
                    RSDK.ResetEntityPtr(entity, 0, 0);
                    return;
                }
            }

            bool32 flag = entity->field_78;
            RSDK.ResetEntityPtr(entity, 0, 0);
            int cnt = 0;

            EntityMusic *ptr = NULL;
            for (int slot = 40; slot < 48; ++slot) {
                EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(slot);
                if (music->objectID == Music->objectID && music->field_80 > cnt) {
                    ptr = music;
                    cnt = music->field_80;
                }
            }

            if (ptr) {
                RSDK.SoundUnknown1(Music->slotID);
                if (ptr->trackID == Music->activeTrack) {
                    ptr->field_84 = 0;
                }
                else {
                    Music->activeTrack = ptr->trackID;
                    if (flag)
                        ptr->field_84 = 0;
                    Music->slotID = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
                    if (ptr->field_84) {
#if RETRO_USE_PLUS
                        RSDK.SetSoundAttributes(0, Music->slotID, 0.0, 0.0, options->vapeMode ? 0.75 : 1.0);
#else
                        RSDK.SetSoundAttributes(0, Music->slotID, 0.0, 0.0, 1.0);
#endif
                    }
#if RETRO_USE_PLUS
                    else if (options->vapeMode) {
                        RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
                    }
#endif
                }
            }
            else if (Music->dword250 > -1) {
                RSDK.SoundUnknown1(Music->slotID);
                Music->activeTrack = Music->dword250;
                Music->dword250    = -1;
                if (flag)
                    Music->dword244 = 0;
                Music->slotID = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
                if (Music->dword244) {
#if RETRO_USE_PLUS 
                    RSDK.SetSoundAttributes(0, Music->slotID, 0.0, 0.0, options->vapeMode ? 0.75 : 1.0);
#else
                    RSDK.SetSoundAttributes(0, Music->slotID, 0.0, 0.0, 1.0);
#endif
                    music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->state    = Music_Unknown13;
                    music->volume   = 0.0;
                    music->field_8C = 0.079999998;
                }
#if RETRO_USE_PLUS
                else if (options->vapeMode) {
                    RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
                }
#endif
            }
        }
        else {
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
    }
}
void Music_Unknown8()
{
    for (int slot = 40; slot < 48; ++slot) {
        RSDK.ResetEntityPtr(RSDK.GetEntityByID(slot), 0, 0);
    }
}

void Music_Unknown9(byte trackID, float a2)
{
    trackID &= 0xF;
    switch (trackID) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 11: {
            EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
            Music->dword250    = trackID;
            if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                RSDK.ResetEntityPtr(music, Music->objectID, 0);
                music->state    = Music_Unknown15;
                music->volume   = 1.0;
                music->field_8C = a2;
            }
            break;
        }
        case 7: {
            Music_Unknown8();

            EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
            Music->dword250    = trackID;
            if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                RSDK.ResetEntityPtr(music, Music->objectID, 0);
                music->state    = Music_Unknown15;
                music->volume   = 1.0;
                music->field_8C = a2;
            }
            break;
        }
        default:
            if (Music_Unknown4()) {
                Music->dword250 = trackID;
            }
            else {
                EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
                Music->dword250    = trackID;
                if (music->objectID != Music->objectID || music->state != Music_Unknown15) {
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->state    = Music_Unknown15;
                    music->volume   = 1.0;
                    music->field_8C = a2;
                }
            }
            break;
    }
}
void Music_Unknown10(float a1)
{
    if (Music->activeTrack != TRACK_DROWNING) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
        RSDK.ResetEntityPtr(music, Music->objectID, 0);
        music->state    = Music_Unknown14;
        music->volume   = 1.0;
        music->field_8C = a1;
    }
}
void Music_Unknown11()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (Music->activeTrack == entity->trackID && RSDK.TrackPlaying(Music->slotID) > entity->field_84) {
        entity->field_84 = 0;
        if (entity->volume < 1.0) {
            entity->volume += entity->field_8C;
            RSDK.SetSoundAttributes(0, Music->slotID, entity->volume, 0.0, 1.0);
            if (entity->volume >= 1.0)
                entity->volume = 1.0;
        }
    }

    if (entity->timer > 0) {
        entity->timer--;
        if (!entity->timer)
            Music_Unknown7(entity);
    }
}
void Music_Unknown12()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (entity->volume > -0.5) {
        entity->volume -= entity->field_8C;
        if (Music->activeTrack == entity->trackID)
            RSDK.SetSoundAttributes(0, Music->slotID, entity->volume, 0.0, 1.0);
        if (entity->volume <= -0.5)
            Music_Unknown7(entity);
    }
}
void Music_Unknown13()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    if (RSDK.TrackPlaying(Music->slotID) > Music->dword244) {
        Music->dword244 = 0;
        entity->volume += entity->field_8C;
        RSDK.SetSoundAttributes(0, Music->slotID, entity->volume, 0.0, 1.0);
        if (entity->volume >= 1.0) {
            entity->volume = 1.0;
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
    }
}
void Music_Unknown14()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    entity->volume -= entity->field_8C;
    RSDK.SetSoundAttributes(0, Music->slotID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.SoundUnknown1(Music->slotID);
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void Music_Unknown15()
{
    EntityMusic *entity = (EntityMusic *)RSDK_sceneInfo->entity;
    entity->volume -= entity->field_8C;
    RSDK.SetSoundAttributes(0, Music->slotID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.SoundUnknown1(Music->slotID);
        Music->activeTrack = Music->dword250;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
        if (options->vapeMode)
            RSDK.SetSoundAttributes(0, Music->slotID, 1.0, 0.0, 0.75);
#endif
        Music->dword250 = -1;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void Music_EditorDraw() {}

void Music_EditorLoad() {}

void Music_Serialize()
{
    RSDK_EDITABLE_VAR(Music, VAR_STRING, trackFile);
    RSDK_EDITABLE_VAR(Music, VAR_STRING, soundTestTitle);
    RSDK_EDITABLE_VAR(Music, VAR_ENUM, trackID);
    RSDK_EDITABLE_VAR(Music, VAR_UINT32, trackLoop);
    RSDK_EDITABLE_VAR(Music, VAR_BOOL, playOnLoad);
}
