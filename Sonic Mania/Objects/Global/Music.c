#include "SonicMania.h"

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
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
            }
        }
    }
}

void Music_StageLoad(void)
{
    // Slot 0: stage music
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
    // Slot 12: "no load"
    Music_SetMusicTrack("1up.ogg", TRACK_1UP, false);

    if (globals->suppressAutoMusic) {
        if (globals->restartMusicID > TRACK_STAGE)
            Music->activeTrack = globals->restartMusicID;
    }
    else {
        Music->activeTrack = TRACK_STAGE;
    }

    globals->restartMusicID = TRACK_STAGE;
    Music->nextTrack        = TRACK_NONE;
#if RETRO_USE_PLUS
    if (RSDK_sku->platform == PLATFORM_DEV)
        RSDK.SetDebugValue("Vape Mode", &globals->vapeMode, DTYPE_BOOL, false, true);
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
            case TRACK_INVINCIBLE:
            case TRACK_SNEAKERS:
            case TRACK_1UP: Music_TransitionTrack(entity->trackID, 0.025); break;
            case TRACK_SUPER: Music_Unknown2(Music->activeTrack); break;
            default: break;
        }
    }
    else {
        Music_PlayTrack(entity->trackID);
    }
    destroyEntity(entity);
}

void Music_PlayMusicTrack(byte trackID)
{
    trackID &= 0xF;

    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            destroyEntity(music);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, Music->objectID, NULL);
            break;
        }
    }

    entity->trackID   = trackID;
    entity->state     = Music_State_Unknown11;
    entity->field_80  = 0;
    entity->fadeSpeed = 0.08;
    switch (trackID) {
        case TRACK_INVINCIBLE:
        case TRACK_SNEAKERS:
            Music_Unknown3(entity);
            entity->field_80 = 100;
            entity->field_78 = 1;
            break;
        case TRACK_DROWNING:
            entity->timer    = 960;
            entity->field_80 = 10000;
            entity->field_78 = 1;
            break;
        case TRACK_SUPER:
            entity->field_80 = 1000;
            entity->field_78 = 1;
            break;
        case TRACK_1UP:
            entity->timer    = 224;
            entity->field_80 = 100000;
            break;
        default: break;
    }

    Music_Unknown5(entity);

    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music != entity) {
            if (music->field_80 > entity->field_80) {
                entity->volume = 0.0;
                return;
            }
        }
    }

    RSDK.StopChannel(Music->channelID);
    Music->activeTrack = trackID;
    Music->channelID   = RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
    if (globals->vapeMode)
        RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
    entity->volume = 1.0;
}

void Music_PlayTrack(byte trackID)
{
    trackID &= 0xF;
    if (trackID == TRACK_ACTCLEAR) {
        Music_ClearMusicStack();

        RSDK.ResetEntitySlot(SLOT_MUSIC, TYPE_BLANK, NULL);
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack   = trackID;
        Music->trackStartPos = 0;
        Music->channelID     = RSDK.PlayStream(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
    }
    else if (Music_CheckMusicStack()) {
        Music->nextTrack = trackID;
    }
    else {
        RSDK.ResetEntitySlot(SLOT_MUSIC, TYPE_BLANK, NULL);
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack   = trackID;
        Music->trackStartPos = 0;
        Music->channelID     = RSDK.PlayStream(Music->trackNames[trackID], 0, 0, Music->trackLoops[trackID], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
    }
}

void Music_PlayTrackPtr(EntityMusic *entity)
{
    RSDK.GetCString(Music->trackNames[0], &entity->trackFile);
    Music->trackLoops[0] = entity->trackLoop;
    RSDK.ResetEntitySlot(SLOT_MUSIC, TYPE_BLANK, NULL);
    RSDK.StopChannel(Music->channelID);
    Music->activeTrack   = 0;
    Music->trackStartPos = 0;
    Music->channelID     = RSDK.PlayStream(Music->trackNames[0], 0, 0, Music->trackLoops[0], true);

#if RETRO_USE_PLUS
    if (globals->vapeMode)
        RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
}

void Music_Unknown2(byte trackID)
{
    trackID &= 0xF;
    Music->nextTrack = TRACK_STAGE;

    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            RSDK.ResetEntityPtr(music, TYPE_BLANK, NULL);
        }
    }

    EntityMusic *entity = NULL;
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
            break;
        }
    }

    entity->trackID   = trackID;
    entity->state     = Music_State_Unknown11;
    entity->field_80  = 0;
    entity->volume    = 1.0;
    entity->fadeSpeed = 0.08;
    switch (trackID) {
        case TRACK_INVINCIBLE:
        case TRACK_SNEAKERS:
            Music_Unknown3(entity);
            entity->field_80 = 100;
            entity->field_78 = 1;
            break;
        case TRACK_DROWNING:
            entity->timer    = 960;
            entity->field_80 = 10000;
            entity->field_78 = 1;
            break;
        case TRACK_SUPER:
            entity->field_80 = 1000;
            entity->field_78 = 1;
            break;
        case TRACK_1UP:
            entity->timer    = 224;
            entity->field_80 = 100000;
            break;
        default: break;
    }
}

void Music_Unknown3(EntityMusic *entity)
{
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->field_80 == 100 && music->trackID != entity->trackID && music != entity)
            music->field_80 = 10;
    }
}
bool32 Music_CheckMusicStack(void)
{
    bool32 flag = false;
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->field_80 > 0)
            flag = true;
    }
    return flag;
}
void Music_Unknown5(EntityMusic *entity)
{
    int activeCnt = 0;
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && entity != music) {
            if (music->trackID == Music->activeTrack) {
                entity->trackStartPos = RSDK.GetChannelPos(Music->channelID);
                entity->volume        = 0.0;
            }
            activeCnt++;
        }
    }

    if (!activeCnt) {
        if (Music->activeTrack > TRACK_NONE) {
            if (Music->nextTrack == TRACK_NONE) {
                Music->nextTrack = Music->activeTrack;
            }
            Music->trackStartPos = RSDK.GetChannelPos(Music->channelID);
        }
    }
}
void Music_ResumePrevTrack(byte trackID, bool32 transitionFade)
{
    trackID &= 0xF;
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            if (transitionFade) {
                music->state     = Music_State_Unknown12;
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
            RSDK.ResetEntityPtr(music, TYPE_BLANK, NULL);

            for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
                if (music->objectID == Music->objectID && music->field_80 > entity->field_80) {
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                    return;
                }
            }

            bool32 flag = entity->field_78;
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
            int cnt = 0;

            EntityMusic *ptr = NULL;
            for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
                if (music->objectID == Music->objectID && music->field_80 > cnt) {
                    ptr = music;
                    cnt = music->field_80;
                }
            }

            if (ptr) {
                RSDK.StopChannel(Music->channelID);
                if (ptr->trackID == Music->activeTrack) {
                    ptr->trackStartPos = 0;
                }
                else {
                    Music->activeTrack = ptr->trackID;
                    if (flag)
                        ptr->trackStartPos = 0;
                    Music->channelID =
                        RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, ptr->trackStartPos, Music->trackLoops[Music->activeTrack], true);
                    if (ptr->trackStartPos) {
#if RETRO_USE_PLUS
                        RSDK.SetChannelAttributes(Music->channelID, 0.0, 0.0, globals->vapeMode ? 0.75 : 1.0);
#else
                        RSDK.SetChannelAttributes(Music->channelID, 0.0, 0.0, 1.0);
#endif
                    }
#if RETRO_USE_PLUS
                    else if (globals->vapeMode) {
                        RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
                    }
#endif
                }
            }
            else if (Music->nextTrack > -1) {
                RSDK.StopChannel(Music->channelID);
                Music->activeTrack = Music->nextTrack;
                Music->nextTrack   = TRACK_NONE;
                if (flag)
                    Music->trackStartPos = 0;
                Music->channelID =
                    RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, Music->trackStartPos, Music->trackLoops[Music->activeTrack], true);
                if (Music->trackStartPos) {
#if RETRO_USE_PLUS
                    RSDK.SetChannelAttributes(Music->channelID, 0.0, 0.0, globals->vapeMode ? 0.75 : 1.0);
#else
                    RSDK.SetChannelAttributes(Music->channelID, 0.0, 0.0, 1.0);
#endif
                    music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
                    RSDK.ResetEntityPtr(music, Music->objectID, 0);
                    music->state     = Music_State_Unknown13;
                    music->volume    = 0.0;
                    music->fadeSpeed = 0.08;
                }
#if RETRO_USE_PLUS
                else if (globals->vapeMode) {
                    RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
                }
#endif
            }
        }
        else {
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
        }
    }
}
void Music_ClearMusicStack(void)
{
    for (int slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        RSDK.ResetEntityPtr(RSDK.GetEntityByID(slot), TYPE_BLANK, NULL);
    }
}

void Music_TransitionTrack(byte trackID, float fadeSpeed)
{
    trackID &= 0xF;
    switch (trackID) {
        case TRACK_MINIBOSS:
        case TRACK_HBHBOSS:
        case TRACK_EGGMAN1:
        case TRACK_EGGMAN2:
        case TRACK_HBHMISCHIEF: {
            EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
            Music->nextTrack   = trackID;
            if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                RSDK.ResetEntityPtr(music, Music->objectID, NULL);
                music->state     = Music_State_TransitionTrack;
                music->volume    = 1.0;
                music->fadeSpeed = fadeSpeed;
            }
            break;
        }
        case TRACK_ACTCLEAR: {
            Music_ClearMusicStack();

            EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
            Music->nextTrack   = trackID;
            if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                RSDK.ResetEntityPtr(music, Music->objectID, NULL);
                music->state     = Music_State_TransitionTrack;
                music->volume    = 1.0;
                music->fadeSpeed = fadeSpeed;
            }
            break;
        }
        default:
            if (Music_CheckMusicStack()) {
                Music->nextTrack = trackID;
            }
            else {
                EntityMusic *music = RSDK_GET_ENTITY(SLOT_MUSIC, Music);
                Music->nextTrack   = trackID;
                if (music->objectID != Music->objectID || music->state != Music_State_TransitionTrack) {
                    RSDK.ResetEntityPtr(music, Music->objectID, NULL);
                    music->state     = Music_State_TransitionTrack;
                    music->volume    = 1.0;
                    music->fadeSpeed = fadeSpeed;
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
        music->state     = Music_State_FadeOut;
        music->volume    = 1.0;
        music->fadeSpeed = fadeSpeed;
    }
}
void Music_State_Unknown11(void)
{
    RSDK_THIS(Music);
    if (Music->activeTrack == entity->trackID && RSDK.GetChannelPos(Music->channelID) > entity->trackStartPos) {
        entity->trackStartPos = 0;
        if (entity->volume < 1.0) {
            entity->volume += entity->fadeSpeed;
            RSDK.SetChannelAttributes(Music->channelID, entity->volume, 0.0, 1.0);
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
            RSDK.SetChannelAttributes(Music->channelID, entity->volume, 0.0, 1.0);
        if (entity->volume <= -0.5)
            Music_Unknown7(entity);
    }
}
void Music_State_Unknown13(void)
{
    RSDK_THIS(Music);
    if (RSDK.GetChannelPos(Music->channelID) > Music->trackStartPos) {
        Music->trackStartPos = 0;
        entity->volume += entity->fadeSpeed;
        RSDK.SetChannelAttributes(Music->channelID, entity->volume, 0.0, 1.0);
        if (entity->volume >= 1.0) {
            entity->volume = 1.0;
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
        }
    }
}
void Music_State_FadeOut(void)
{
    RSDK_THIS(Music);
    entity->volume -= entity->fadeSpeed;
    RSDK.SetChannelAttributes(Music->channelID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.StopChannel(Music->channelID);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Music_State_TransitionTrack(void)
{
    RSDK_THIS(Music);
    entity->volume -= entity->fadeSpeed;
    RSDK.SetChannelAttributes(Music->channelID, entity->volume, 0.0, 1.0);
    if (entity->volume < -0.5) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack   = Music->nextTrack;
        Music->trackStartPos = 0;
        Music->channelID     = RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
        Music->nextTrack = TRACK_NONE;
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Music_EditorDraw(void)
{
    RSDK_THIS(Music);
    //field 254 doesnt get used so /shrug
    Animator animator;
    RSDK.SetSpriteAnimation(Music->field_254, 0, &animator, true, 1);
    RSDK.DrawSprite(&animator, NULL, 0);
}

void Music_EditorLoad(void) { Music->field_254 = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }

void Music_Serialize(void)
{
    RSDK_EDITABLE_VAR(Music, VAR_STRING, trackFile);
    RSDK_EDITABLE_VAR(Music, VAR_STRING, soundTestTitle);
    RSDK_EDITABLE_VAR(Music, VAR_ENUM, trackID);
    RSDK_EDITABLE_VAR(Music, VAR_UINT32, trackLoop);
    RSDK_EDITABLE_VAR(Music, VAR_BOOL, playOnLoad);
}
