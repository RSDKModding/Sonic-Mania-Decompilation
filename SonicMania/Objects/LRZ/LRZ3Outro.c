// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectLRZ3Outro *LRZ3Outro;

void LRZ3Outro_Update(void)
{
    RSDK_THIS(LRZ3Outro);

    StateMachine_Run(self->state);
}

void LRZ3Outro_LateUpdate(void) {}

void LRZ3Outro_StaticUpdate(void) {}

void LRZ3Outro_Draw(void) {}

void LRZ3Outro_Create(void *data)
{
    RSDK_THIS(LRZ3Outro);

    if (!SceneInfo->inEditor) {
        self->active      = ACTIVE_NORMAL;
        self->isPermanent = true;

        self->background1 = RSDK.GetTileLayer(1);
        self->background2 = RSDK.GetTileLayer(2);

        foreach_active(ParallaxSprite, sprite) { self->littlePlanet = sprite; }

        self->state = LRZ3Outro_State_BlastOff;
    }
}

void LRZ3Outro_StageLoad(void)
{
    LRZ3Outro->sfxBlastoff     = RSDK.GetSfx("LRZ/Blastoff.wav");
    LRZ3Outro->sfxLittlePlanet = RSDK.GetSfx("LRZ/LittlePlanet.wav");
}

void LRZ3Outro_HandleExplosions(void)
{
    RSDK_THIS(LRZ3Outro);

    int32 x                    = (RSDK.Rand(-ScreenInfo->center.x, ScreenInfo->center.x) + ScreenInfo->center.x + ScreenInfo->position.x);
    int32 y                    = ScreenInfo->position.y + 32 + ScreenInfo->size.y;
    EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), x << 16, y << 16);

    explosion->velocity.x = RSDK.Rand(-0x20000, 0x20000);
    explosion->velocity.y = self->velocity.y - 0x40000;
    explosion->drawGroup  = Zone->objectDrawGroup[1];
    explosion->drawFX     = FX_SCALE;
    explosion->inkEffect  = INK_BLEND;
    explosion->scale.x    = RSDK.Rand(0x200, 0x400);
    explosion->scale.y    = explosion->scale.x;
}

void LRZ3Outro_State_BlastOff(void)
{
    RSDK_THIS(LRZ3Outro);

    ++self->timer;
    if (!(self->timer & 7))
        Camera_ShakeScreen(0, 3, 2);

    if (self->timer == 1) {
        self->blastoffChannel = RSDK.PlaySfx(LRZ3Outro->sfxBlastoff, 56628, 0xFF);
        self->blastoffVolume  = 1.0;
    }

    if (self->timer == 60) {
        self->timer = 0;
        self->state = LRZ3Outro_State_RocketLaunch;

        RSDK.GetTileLayer(0)->drawGroup[0] = DRAWGROUP_COUNT;
        RSDK.GetTileLayer(1)->drawGroup[0] = 0;
    }
}

void LRZ3Outro_State_RocketLaunch(void)
{
    RSDK_THIS(LRZ3Outro);

    ++self->timer;
    if (!(self->timer & 1))
        LRZ3Outro_HandleExplosions();

    if (self->timer == 30) {
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 64;
    }

    if (self->timer <= 120) {
        if (!(self->timer & 7))
            Camera_ShakeScreen(0, RSDK.Rand(-6, 6), 4);
    }
    else {
        if (self->velocity.y < 0x60000)
            self->velocity.y += 0x800;

        if (!(self->timer & 7))
            Camera_ShakeScreen(0, 0, 4);
    }

    if (self->timer > 390) {
        self->blastoffVolume -= 0.0125;
        RSDK.SetChannelAttributes(self->blastoffChannel, self->blastoffVolume, 0.0, 1.0);
    }

    if (self->timer > 480) {
        self->timer = 0;
        self->state = LRZ3Outro_State_EnterLittlePlanet;
        RSDK.StopChannel(self->blastoffChannel);
    }

    TileLayer *background1 = self->background1;
    if (background1->scrollPos <= -0xE00000) {
        background1->scrollSpeed >>= 1;
    }
    else {
        if (background1->scrollSpeed > -0x2000)
            background1->scrollSpeed -= 0x80;
    }

    TileLayer *background2 = self->background2;
    if (background2->scrollPos <= -0xE00000) {
        background2->scrollSpeed >>= 1;
    }
    else {
        if (background2->scrollSpeed > -0x10000)
            background2->scrollSpeed -= 0x80;
    }

    if (self->littlePlanet->scrollSpeed.y > -0x1000)
        self->littlePlanet->scrollSpeed.y -= 0x80;
}

void LRZ3Outro_State_EnterLittlePlanet(void)
{
    RSDK_THIS(LRZ3Outro);

    if (self->background2->scrollSpeed < 0)
        self->background2->scrollSpeed += 0x80;

    EntityParallaxSprite *littlePlanet = self->littlePlanet;
    if (littlePlanet->scrollSpeed.y > -0x1000) {
        littlePlanet->scrollSpeed.y -= 0x80;
    }

    if (littlePlanet->scrollPos.y < -0x1000000) {
        littlePlanet->scrollSpeed.y = 0;
        self->state                 = StateMachine_None;
    }
}

void LRZ3Outro_StageFinish_EndAct2ST(void)
{
    EntityLRZ3Outro *cutscene = NULL;
    foreach_all(LRZ3Outro, outro)
    {
        cutscene = outro;
        foreach_break;
    }

    if (cutscene) {
        CutsceneSeq_StartSequence(cutscene, LRZ3Outro_Cutscene_StopPlayers, LRZ3Outro_Cutscene_LightUpLittlePlanet, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif

        HUD_MoveOut();
        cutscene->active = ACTIVE_NEVER;
    }
}

bool32 LRZ3Outro_Cutscene_StopPlayers(EntityCutsceneSeq *host)
{
    foreach_active(Player, player) {
        player->state = Player_State_Static;
        player->stateInput = StateMachine_None;
    }

    return true;
}

bool32 LRZ3Outro_Cutscene_LightUpLittlePlanet(EntityCutsceneSeq *host)
{
    if (host->timer > 60) {
        int32 frame = 23 - MIN((host->timer - 60) >> 2, 6);
        foreach_active(Player, player) { RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, true, frame); }
    }

    if (host->timer > 120)
        RSDK.SetLimitedFade(0, 1, 2, 4 * host->timer - 480, 245, 250);

    if (host->timer == 180)
        RSDK.PlaySfx(LRZ3Outro->sfxLittlePlanet, false, 255);

    if (host->timer > 180)
        RSDK.SetLimitedFade(0, 1, 2, 4 * host->timer - 720, 240, 244);

    if (host->timer == 320) {
        Zone_StartFadeOut(10, 0x000000);
        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void LRZ3Outro_EditorDraw(void) {}

void LRZ3Outro_EditorLoad(void) {}
#endif

void LRZ3Outro_Serialize(void) {}
#endif
