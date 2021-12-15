// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
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
        self->bg1         = RSDK.GetSceneLayer(1);
        self->bg2         = RSDK.GetSceneLayer(2);
        foreach_active(ParallaxSprite, sprite) { self->littlePlanet = (Entity *)sprite; }
        self->state = LRZ3Outro_State_Unknown1;
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

    int x                      = (RSDK.Rand(-ScreenInfo->centerX, ScreenInfo->centerX) + ScreenInfo->centerX + ScreenInfo->position.x);
    int y                      = (ScreenInfo->position.y + 32 + ScreenInfo->height);
    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x << 16, y << 16);

    explosion->velocity.x = RSDK.Rand(-0x20000, 0x20000);
    explosion->velocity.y = self->velocity.y - 0x40000;
    explosion->drawOrder  = Zone->drawOrderHigh;
    explosion->drawFX     = FX_SCALE;
    explosion->inkEffect  = INK_BLEND;
    explosion->scale.x    = RSDK.Rand(0x200, 0x400);
    explosion->scale.y    = explosion->scale.x;
}

void LRZ3Outro_State_Unknown1(void)
{
    RSDK_THIS(LRZ3Outro);

    ++self->timer;
    if (!(self->timer & 7))
        Camera_ShakeScreen(0, 3, 2);

    if (self->timer == 1) {
        self->blastoffChannel = RSDK.PlaySfx(LRZ3Outro->sfxBlastoff, 56628, 0xFF);
        self->blastoffVolume = 1.0;
    }

    if (self->timer == 60) {
        self->timer                       = 0;
        self->state                       = LRZ3Outro_State_Unknown2;
        RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(1)->drawLayer[0] = 0;
    }
}

void LRZ3Outro_State_Unknown2(void)
{
    RSDK_THIS(LRZ3Outro);

    ++self->timer;
    if (!(self->timer & 1))
        LRZ3Outro_HandleExplosions();
    if (self->timer == 30) {
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 64;
    }

    if (self->timer <= 120) {
        if (!(self->timer & 7)) {
            Camera_ShakeScreen(0, RSDK.Rand(-6, 6), 4);
        }
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
        self->state = LRZ3Outro_State_Unknown3;
        RSDK.StopChannel(self->blastoffChannel);
    }

    TileLayer *bg1 = self->bg1;
    if (bg1->scrollPos <= -0xE00000) {
        bg1->scrollSpeed >>= 1;
    }
    else {
        if (bg1->scrollSpeed > -0x2000)
            bg1->scrollSpeed -= 0x80;
    }

    TileLayer *bg2 = self->bg2;
    if (bg2->scrollPos <= -0xE00000) {
        bg2->scrollSpeed >>= 1;
    }
    else {
        if (bg2->scrollSpeed > -0x10000)
            bg2->scrollSpeed -= 0x80;
    }

    EntityParallaxSprite *littlePlanet = (EntityParallaxSprite *)self->littlePlanet;
    if (littlePlanet->scrollSpeed.y > -0x1000)
        littlePlanet->scrollSpeed.y -= 0x80;
}

void LRZ3Outro_State_Unknown3(void)
{
    RSDK_THIS(LRZ3Outro);

    if (self->bg2->scrollSpeed < 0)
        self->bg2->scrollSpeed += 0x80;

    EntityParallaxSprite *littlePlanet = (EntityParallaxSprite *)self->littlePlanet;
    if (littlePlanet->scrollSpeed.y > -0x1000) {
        littlePlanet->scrollSpeed.y -= 0x80;
    }

    if (littlePlanet->scrollPos.y < -0x1000000) {
        littlePlanet->scrollSpeed.y = 0;
        self->state               = StateMachine_None;
    }
}

void LRZ3Outro_StageFinishCB(void)
{
    EntityLRZ3Outro *entity = NULL;
    foreach_all(LRZ3Outro, outro)
    {
        entity = outro;
        foreach_break;
    }

    if (entity) {
        void *states[] = { LRZ3Outro_CutsceneState_Unknown1, LRZ3Outro_CutsceneState_Unknown2, NULL };

        CutsceneSeq_StartSequence((Entity*)entity, states);
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;

        foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
        entity->active = ACTIVE_NEVER;
    }
}

bool32 LRZ3Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    foreach_active(Player, player) { player->state = Player_State_None; }
    return true;
}

bool32 LRZ3Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    if (host->timer > 60) {
        int frame = 23 - minVal((host->timer - 60) >> 2, 6);
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

void LRZ3Outro_EditorDraw(void) {}

void LRZ3Outro_EditorLoad(void) {}

void LRZ3Outro_Serialize(void) {}
#endif
