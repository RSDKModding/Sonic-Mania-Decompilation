#include "SonicMania.h"

ObjectMusic *Music;

void Music_Update(void)
{
    RSDK_THIS(Music);
    StateMachine_Run(self->state);
}

void Music_LateUpdate(void) {}

void Music_StaticUpdate(void) {}

void Music_Draw(void) {}

void Music_Create(void *data)
{
    RSDK_THIS(Music);
    if (!SceneInfo->inEditor) {
        self->active = ((SceneInfo->state & 3) != 3) + ACTIVE_ALWAYS;
        if (self->trackFile.length) {
            if (self->trackID != TRACK_NOLOAD) {
                RSDK.GetCString(Music->trackNames[self->trackID], &self->trackFile);
                Music->trackLoops[self->trackID] = self->trackLoop;
                if (self->playOnLoad)
                    self->state = Music_State_PlayMusic;
                else
                    destroyEntity(self);
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
    if (sku_platform == PLATFORM_DEV)
        RSDK.SetDebugValue("Vape Mode", &globals->vapeMode, DTYPE_BOOL, false, true);
#endif
}

void Music_SetMusicTrack(const char *path, uint8 track, uint32 loopPoint)
{
    track &= 0xF;

    int32 charID = 0;
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
            case TRACK_1UP: Music_TransitionTrack(self->trackID, 0.025); break;
            case TRACK_SUPER: Music_Unknown2(Music->activeTrack); break;
            default: break;
        }
    }
    else {
        Music_PlayTrack(self->trackID);
    }
    destroyEntity(self);
}

void Music_PlayMusicTrack(uint8 trackID)
{
    trackID &= 0xF;

    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            destroyEntity(music);
        }
    }

    EntityMusic *entity = NULL;
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            RSDK.ResetEntityPtr(entity, Music->objectID, NULL);
            break;
        }
    }

    entity->trackID       = trackID;
    entity->state         = Music_State_Unknown11;
    entity->trackPriority = TRACK_PRIORITY_NONE;
    entity->fadeSpeed     = 0.08;
    switch (trackID) {
        case TRACK_INVINCIBLE:
        case TRACK_SNEAKERS:
            Music_CheckMusicStack_Powerup(entity);
            entity->trackPriority    = TRACK_PRIORITY_SUPER;
            entity->restartTrackFlag = true;
            break;
        case TRACK_DROWNING:
            entity->timer            = 960;
            entity->trackPriority    = TRACK_PRIORITY_DROWN;
            entity->restartTrackFlag = true;
            break;
        case TRACK_SUPER:
            entity->trackPriority    = TRACK_PRIORITY_SUPER;
            entity->restartTrackFlag = true;
            break;
        case TRACK_1UP:
            entity->timer         = 224;
            entity->trackPriority = TRACK_PRIORITY_1UP;
            break;
        default: break;
    }

    Music_GetNextTrack(entity);

    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music != entity) {
            if (music->trackPriority > entity->trackPriority) {
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

void Music_PlayTrack(uint8 trackID)
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
    else if (Music_CheckMusicStack_Active()) {
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
    destroyEntitySlot(SLOT_MUSIC);
    RSDK.StopChannel(Music->channelID);
    Music->activeTrack   = 0;
    Music->trackStartPos = 0;
    Music->channelID     = RSDK.PlayStream(Music->trackNames[0], 0, 0, Music->trackLoops[0], true);

#if RETRO_USE_PLUS
    if (globals->vapeMode)
        RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
}

void Music_Unknown2(uint8 trackID)
{
    trackID &= 0xF;
    Music->nextTrack = TRACK_STAGE;

    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            destroyEntity(music);
        }
    }

    EntityMusic *entity = NULL;
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        entity = RSDK_GET_ENTITY(slot, Music);
        if (entity->objectID != Music->objectID) {
            destroyEntity(entity);
            break;
        }
    }

    entity->trackID       = trackID;
    entity->state         = Music_State_Unknown11;
    entity->trackPriority = TRACK_PRIORITY_NONE;
    entity->volume        = 1.0;
    entity->fadeSpeed     = 0.08;
    switch (trackID) {
        case TRACK_INVINCIBLE:
        case TRACK_SNEAKERS:
            Music_CheckMusicStack_Powerup(entity);
            entity->trackPriority    = TRACK_PRIORITY_POWERUP;
            entity->restartTrackFlag = true;
            break;
        case TRACK_DROWNING:
            entity->timer            = 960;
            entity->trackPriority    = TRACK_PRIORITY_DROWN;
            entity->restartTrackFlag = true;
            break;
        case TRACK_SUPER:
            entity->trackPriority    = TRACK_PRIORITY_SUPER;
            entity->restartTrackFlag = true;
            break;
        case TRACK_1UP:
            entity->timer         = 224;
            entity->trackPriority = TRACK_PRIORITY_1UP;
            break;
        default: break;
    }
}

void Music_CheckMusicStack_Powerup(EntityMusic *entity)
{
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackPriority == TRACK_PRIORITY_POWERUP && music->trackID != entity->trackID
            && music != entity)
            music->trackPriority = TRACK_PRIORITY_ANY;
    }
}
bool32 Music_CheckMusicStack_Active(void)
{
    bool32 flag = false;
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackPriority > TRACK_PRIORITY_NONE)
            flag = true;
    }
    return flag;
}
void Music_GetNextTrack(EntityMusic *entity)
{
    int32 activeCnt = 0;
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
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
void Music_ResumePrevTrack(uint8 trackID, bool32 transitionFade)
{
    trackID &= 0xF;
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
        if (music->objectID == Music->objectID && music->trackID == trackID) {
            if (transitionFade) {
                music->state     = Music_State_FadeTrackOut;
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

            for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
                if (music->objectID == Music->objectID && music->trackPriority > entity->trackPriority) {
                    destroyEntity(entity);
                    return;
                }
            }

            bool32 restartFlag = entity->restartTrackFlag;
            destroyEntity(entity);
            int32 priority = 0;

            EntityMusic *trackPtr = NULL;
            for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
                EntityMusic *music = RSDK_GET_ENTITY(slot, Music);
                if (music->objectID == Music->objectID && music->trackPriority > priority) {
                    trackPtr = music;
                    priority = music->trackPriority;
                }
            }

            if (trackPtr) {
                RSDK.StopChannel(Music->channelID);
                if (trackPtr->trackID == Music->activeTrack) {
                    trackPtr->trackStartPos = 0;
                }
                else {
                    Music->activeTrack = trackPtr->trackID;
                    if (restartFlag)
                        trackPtr->trackStartPos = 0;
                    Music->channelID = RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, trackPtr->trackStartPos,
                                                       Music->trackLoops[Music->activeTrack], true);
                    if (trackPtr->trackStartPos) {
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
                if (restartFlag)
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
                    music->state     = Music_State_FadeTrackIn;
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
            destroyEntity(entity);
        }
    }
}
void Music_ClearMusicStack(void)
{
    for (int32 slot = SLOT_MUSICSTACK_START; slot < SLOT_MUSICSTACK_END; ++slot) {
        destroyEntity(RSDK.GetEntityByID(slot));
    }
}

void Music_TransitionTrack(uint8 trackID, float fadeSpeed)
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
            if (Music_CheckMusicStack_Active()) {
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
    if (Music->activeTrack == self->trackID && RSDK.GetChannelPos(Music->channelID) > self->trackStartPos) {
        self->trackStartPos = 0;
        if (self->volume < 1.0) {
            self->volume += self->fadeSpeed;
            RSDK.SetChannelAttributes(Music->channelID, self->volume, 0.0, 1.0);
            if (self->volume >= 1.0)
                self->volume = 1.0;
        }
    }

    if (self->timer > 0) {
        if (!--self->timer)
            Music_Unknown7(self);
    }
}
void Music_State_FadeTrackOut(void)
{
    RSDK_THIS(Music);
    if (self->volume > -0.5) {
        self->volume -= self->fadeSpeed;
        if (Music->activeTrack == self->trackID)
            RSDK.SetChannelAttributes(Music->channelID, self->volume, 0.0, 1.0);
        if (self->volume <= -0.5)
            Music_Unknown7(self);
    }
}
void Music_State_FadeTrackIn(void)
{
    RSDK_THIS(Music);
    if (RSDK.GetChannelPos(Music->channelID) > Music->trackStartPos) {
        Music->trackStartPos = 0;
        self->volume += self->fadeSpeed;
        RSDK.SetChannelAttributes(Music->channelID, self->volume, 0.0, 1.0);
        if (self->volume >= 1.0) {
            self->volume = 1.0;
            destroyEntity(self);
        }
    }
}
void Music_State_FadeOut(void)
{
    RSDK_THIS(Music);
    self->volume -= self->fadeSpeed;
    RSDK.SetChannelAttributes(Music->channelID, self->volume, 0.0, 1.0);
    if (self->volume < -0.5) {
        RSDK.StopChannel(Music->channelID);
        destroyEntity(self);
    }
}

void Music_State_TransitionTrack(void)
{
    RSDK_THIS(Music);
    self->volume -= self->fadeSpeed;
    RSDK.SetChannelAttributes(Music->channelID, self->volume, 0.0, 1.0);
    if (self->volume < -0.5) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack   = Music->nextTrack;
        Music->trackStartPos = 0;
        Music->channelID     = RSDK.PlayStream(Music->trackNames[Music->activeTrack], 0, 0, Music->trackLoops[Music->activeTrack], true);
#if RETRO_USE_PLUS
        if (globals->vapeMode)
            RSDK.SetChannelAttributes(Music->channelID, 1.0, 0.0, 0.75);
#endif
        Music->nextTrack = TRACK_NONE;
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Music_EditorDraw(void)
{
    RSDK_THIS(Music);
    RSDK.SetSpriteAnimation(Music->aniFrames, 0, &self->animator, true, 1);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Music_EditorLoad(void) { Music->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void Music_Serialize(void)
{
    RSDK_EDITABLE_VAR(Music, VAR_STRING, trackFile);
    RSDK_EDITABLE_VAR(Music, VAR_STRING, soundTestTitle);
    RSDK_EDITABLE_VAR(Music, VAR_ENUM, trackID);
    RSDK_EDITABLE_VAR(Music, VAR_UINT32, trackLoop);
    RSDK_EDITABLE_VAR(Music, VAR_BOOL, playOnLoad);
}
