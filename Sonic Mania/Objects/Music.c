#include "../SonicMania.h"

ObjectMusic *Music;

void Music_Update(void)
{
    RSDK_THIS(Music);
    StateMachine_Run(entity->state);
}

void Music_LateUpdate(void) {}

void Music_StaticUpdate(void) {}

void Music_Draw(void) {}

void Music_Create(void *data)
{
    RSDK_THIS(Music);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ((RSDK_sceneInfo->state & 3) != 3) + 1;
        if (entity->trackFile.textLength) {
            if (entity->trackID != TRACK_NOLOAD) {
                RSDK.GetCString(Music->trackNames[entity->trackID], &entity->trackFile);
                Music->trackLoops[entity->trackID] = entity->trackLoop;
                if (entity->playOnLoad)
                    entity->state = Music_State_PlayMusic;
                else
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
            }
        }
    }
}

void Music_StageLoad(void)
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
    //Slot 12: "no load"
    Music_SetMusicTrack("1up.ogg", TRACK_1UP, false);

    if (globals->suppressAutoMusic) {
        if (globals->restartMusicID > 0)
            Music->activeTrack = globals->restartMusicID;
    }
    else {
        Music->activeTrack = TRACK_STAGE;
    }

    globals->restartMusicID = 0;
    Music->dword250         = -1;
#if RETRO_USE_PLUS
    if (RSDK_sku->platform == PLATFORM_DEV)
        RSDK.SetDebugValue("Vape Mode", &globals->vapeMode, 1, 0, 1);
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

void Music_State_PlayMusic(void)
{
    RSDK_THIS(Music);
    if (globals->suppressAutoMusic) {
        globals->suppressAutoMusic = false;
        switch (Music->activeTrack) {
            case 1:
            case 2:
            case 13:
                Music_Unknown9(entity->trackID, 0.025);
                break;
            case 10:
                Music_Unknown2(Music->activeTrack);
                break;
            default: break;
        }
    }
    else {
        Music_PlayTrack(entity->trackID);
    }
    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
}

void Music_PlayMusicTrack(byte trackID)
{
    trackID &= 0xF;

    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            RSDK.ResetEntityPtr(music, 0, 0);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = 40; slot < 48; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, 0, 0);
            break;
        }
    }

    entity->trackID  = trackID;
    entity->state    = Music_State_Unknown11;
    entity->field_80 = 0;
    entity->fadeSpeed = 0.08;
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
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music != entity) {
            if (music->field_80 > entity->field_80) {
                entity->volume = 0.0;
                return;
            }
        }
    }

    RSDK.StopChannel(Music->slotID);
    Music->activeTrack = trackID;
    Music->slotID      = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
    if (globals->vapeMode)
        RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
#endif
    entity->volume = 1.0;
}

void Music_PlayTrack(byte trackID)
{
    trackID &= 0xF;
    if (trackID == TRACK_ACTCLEAR) {
        Music_RemoveStoredEntities();

        RSDK.ResetEntitySlot(SLOT_MUSIC, TYPE_BLANK, NULL);
        RSDK.StopChannel(Music->slotID);
        Music->activeTrack = trackID;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
#endif
    }
    else if (Music_Unknown4()) {
        Music->dword250 = trackID;
    }
    else {
        RSDK.ResetEntitySlot(SLOT_MUSIC, 0, 0);
        RSDK.StopChannel(Music->slotID);
        Music->activeTrack = trackID;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
#endif
    }
}

void Music_Unknown1(EntityMusic *entity)
{
    RSDK.GetCString(Music->trackNames[0], &entity->trackFile);
    Music->trackLoops[0] = entity->trackLoop;
    RSDK.ResetEntitySlot(SLOT_MUSIC, TYPE_BLANK, 0);
    RSDK.StopChannel(Music->slotID);
    Music->activeTrack = 0;
    Music->dword244    = 0;
    Music->slotID      = RSDK.PlayMusic(Music->trackNames[0], 0, 0, Music->trackLoops[0], true);

#if RETRO_USE_PLUS
    if (globals->vapeMode)
        RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
#endif
}

void Music_Unknown2(byte trackID)
{
    trackID &= 0xF;
    Music->dword250 = 0;

    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            RSDK.ResetEntityPtr(music, 0, 0);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = 40; slot < 48; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, 0, 0);
            break;
        }
    }

    entity->trackID  = trackID;
    entity->state    = Music_State_Unknown11;
    entity->field_80 = 0;
    entity->volume   = 1.0;
    entity->fadeSpeed = 0.08;
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
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->field_80 == 100 && music->trackID != entity->trackID && music != entity)
            music->field_80 = 10;
    }
}
bool32 Music_Unknown4(void)
{
    bool32 flag = false;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->field_80 > 0)
            flag = true;
    }
    return flag;
}
void Music_Unknown5(EntityMusic *entity)
{
    int activeCnt = 0;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && entity != music) {
            if (music->trackID == Music->activeTrack) {
                entity->field_84 = RSDK.GetChannelPos(Music->slotID);
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
            Music->dword244 = RSDK.GetChannelPos(Music->slotID);
        }
    }
}
void Music_Unknown6(byte trackID, int a2)
{
    trackID &= 0xF;
    for (int slot = 40; slot < 48; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            if (a2) {
                music->state    = Music_State_Unknown12;
                music->fadeSpeed = 0.05;
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
        EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
        if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
            RSDK.ResetEntityPtr(music, 0, 0);

            for (int slot = 40; slot < 48; ++slot) {
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
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
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
                if (music->objectID == Music->objectID && music->field_80 > cnt) {
                    ptr = music;
                    cnt = music->field_80;
                }
            }

            if (ptr) {
                RSDK.StopChannel(Music->slotID);
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
                        RSDK.SetChannelAttributes(Music->slotID, 0.0, 0.0, globals->vapeMode ? 0.75 : 1.0);
#else
                        RSDK.SetChannelAttributes(Music->slotID, 0.0, 0.0, 1.0);
#endif
                    }
#if RETRO_USE_PLUS
                    else if (globals->vapeMode) {
                        RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
                    }
#endif
                }
            }
            else if (Music->dword250 > -1) {
                RSDK.StopChannel(Music->slotID);
                Music->activeTrack = Music->dword250;
                Music->dword250    = -1;
                if (flag)
                    Music->dword244 = 0;
                Music->slotID = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
                if (Music->dword244) {
#if RETRO_USE_PLUS 
                    RSDK.SetChannelAttributes(Music->slotID, 0.0, 0.0, globals->vapeMode ? 0.75 : 1.0);
#else
                    RSDK.SetChannelAttributes(Music->slotID, 0.0, 0.0, 1.0);
#endif
                    music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->state    = Music_State_Unknown13;
                    music->volume   = 0.0;
                    music->fadeSpeed = 0.08;
                }
#if RETRO_USE_PLUS
                else if (globals->vapeMode) {
                    RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
                }
#endif
            }
        }
        else {
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
    }
}
void Music_RemoveStoredEntities(void)
{
    for (int slot = 40; slot < 48; ++slot) {
        RSDK.ResetEntityPtr(RSDK.GetEntityByID(slot), TYPE_BLANK, NULL);
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
            EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
            Music->dword250    = trackID;
            if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                RSDK.ResetEntityPtr(music, Music->objectID, 0);
                music->state    = Music_State_TransitionTrack;
                music->volume   = 1.0;
                music->fadeSpeed = a2;
            }
            break;
        }
        case 7: {
            Music_RemoveStoredEntities();

            EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
            Music->dword250    = trackID;
            if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                RSDK.ResetEntityPtr(music, Music->objectID, 0);
                music->state    = Music_State_TransitionTrack;
                music->volume   = 1.0;
                music->fadeSpeed = a2;
            }
            break;
        }
        default:
            if (Music_Unknown4()) {
                Music->dword250 = trackID;
            }
            else {
                EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
                Music->dword250    = trackID;
                if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->state    = Music_State_TransitionTrack;
                    music->volume   = 1.0;
                    music->fadeSpeed = a2;
                }
            }
            break;
    }
}
void Music_FadeOut(float fadeSpeed)
{
    if (Music->activeTrack != TRACK_DROWNING) {
        EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
        RSDK.ResetEntityPtr(music, Music->objectID, NULL);
        music->state    = Music_State_FadeOut;
        music->volume   = 1.0;
        music->fadeSpeed = fadeSpeed;
    }
}
void Music_State_Unknown11(void)
{
    RSDK_THIS(Music);
    if (Music->activeTrack == entity->trackID && RSDK.GetChannelPos(Music->slotID) > entity->field_84) {
        entity->field_84 = 0;
        if (entity->volume < 1.0) {
            entity->volume += entity->fadeSpeed;
            RSDK.SetChannelAttributes(Music->slotID, entity->volume, 0.0, 1.0);
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
void Music_State_Unknown12(void)
{
    RSDK_THIS(Music);
    if (entity->volume > -0.5) {
        entity->volume -= entity->fadeSpeed;
        if (Music->activeTrack == entity->trackID)
            RSDK.SetChannelAttributes(Music->slotID, entity->volume, 0.0, 1.0);
        if (entity->volume <= -0.5)
            Music_Unknown7(entity);
    }
}
void Music_State_Unknown13(void)
{
    RSDK_THIS(Music);
    if (RSDK.GetChannelPos(Music->slotID) > Music->dword244) {
        Music->dword244 = 0;
        entity->volume += entity->fadeSpeed;
        RSDK.SetChannelAttributes(Music->slotID, entity->volume, 0.0, 1.0);
        if (entity->volume >= 1.0) {
            entity->volume = 1.0;
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
    }
}
void Music_State_FadeOut(void)
{
    RSDK_THIS(Music);
    entity->volume -= entity->fadeSpeed;
    RSDK.SetChannelAttributes(Music->slotID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.StopChannel(Music->slotID);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void Music_State_TransitionTrack(void)
{
    RSDK_THIS(Music);
    entity->volume -= entity->fadeSpeed;
    RSDK.SetChannelAttributes(Music->slotID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.StopChannel(Music->slotID);
        Music->activeTrack = Music->dword250;
        Music->dword244    = 0;
        Music->slotID      = RSDK.PlayMusic(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->slotID, 1.0, 0.0, 0.75);
#endif
        Music->dword250 = -1;
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void Music_EditorDraw(void) {}

void Music_EditorLoad(void) {}

void Music_Serialize(void)
{
    RSDK_EDITABLE_VAR(Music, VAR_STRING, trackFile);
    RSDK_EDITABLE_VAR(Music, VAR_STRING, soundTestTitle);
    RSDK_EDITABLE_VAR(Music, VAR_ENUM, trackID);
    RSDK_EDITABLE_VAR(Music, VAR_UINT32, trackLoop);
    RSDK_EDITABLE_VAR(Music, VAR_BOOL, playOnLoad);
}
