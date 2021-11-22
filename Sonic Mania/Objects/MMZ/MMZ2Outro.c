#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectMMZ2Outro *MMZ2Outro;

void MMZ2Outro_Update(void)
{
    RSDK_THIS(MMZ2Outro);
    MMZ2Outro_StartCutscene();
    self->active = ACTIVE_NEVER;
}

void MMZ2Outro_LateUpdate(void) {}

void MMZ2Outro_StaticUpdate(void) {}

void MMZ2Outro_Draw(void) {}

void MMZ2Outro_Create(void *data)
{
    RSDK_THIS(MMZ2Outro);
    self->active      = ACTIVE_NORMAL;
    self->isPermanent = true;
}

void MMZ2Outro_StageLoad(void)
{
    MMZ2Outro->sfxThunda    = RSDK.GetSfx("MMZ/ThundaVerb.wav");
    MMZ2Outro->sfxLightsOut = RSDK.GetSfx("MMZ/LightsOut.wav");
    MMZ2Outro->sfxTMZEyes   = RSDK.GetSfx("MMZ/TMZEyes.wav");
}

void MMZ2Outro_StartCutscene(void)
{
    RSDK_THIS(MMZ2Outro);
    void *states[] = { MMZ2Outro_CutsceneState_Unknown1,
                       MMZ2Outro_CutsceneState_Unknown2,
                       MMZ2Outro_CutsceneState_Unknown3,
                       MMZ2Outro_CutsceneState_Unknown4,
                       MMZ2Outro_CutsceneState_Unknown5,
                       MMZ2Outro_CutsceneState_Unknown6,
                       NULL };

    CutsceneSeq_StartSequence((Entity *)self, states);
    RSDK.CopyPalette(0, 1, 1, 1, 255);
    for (int32 i = 128; i < 256; ++i) RSDK.SetPaletteEntry(2, i, 0);
    for (int32 i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0xFFFFFF);
    RSDK.GetSceneLayer(0)->drawLayer[0] = 16;
    RSDK.GetSceneLayer(1)->drawLayer[0] = 16;
    RSDK.GetSceneLayer(2)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(4)->drawLayer[0] = 0;
    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;

    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
}

bool32 MMZ2Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);
    if (++self->timer == 60) {
        self->timer = 0;
        Camera_ShakeScreen(0, 0, 4);

        foreach_active(Player, player)
        {
            player->stateInput = StateMachine_None;
            if (player->onGround) {
                player->state = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_BALANCE1, &player->animator, false, 0);
            }
            else {
                player->state      = Player_State_Air;
                player->velocity.x = 0;
            }
        }
        RSDK.PlaySfx(MMZ2Outro->sfxLightsOut, false, 255);
        Zone->cameraBoundsT[0] = 0;
        return true;
    }
    return false;
}

bool32 MMZ2Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);

    self->timer += 16;
    if (self->timer <= 128)
        RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 224);

    RSDK.SetLimitedFade(0, 1, 2, self->timer, 224, 256);
    if (self->timer == 1024) {
        self->timer = -256;
        foreach_active(Player, player) { player->state = Player_State_Ground; }
        return true;
    }
    else {
        foreach_active(Player, player)
        {
            if (player->onGround && player->state != Player_State_None) {
                player->state     = Player_State_None;
                player->groundVel = 0;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_BALANCE1, &player->animator, false, 0);
            }
        }
    }
    return false;
}

bool32 MMZ2Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);

    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state     = Player_State_Ground;
        player->right     = true;
        player->groundVel = 1;

        if (player->camera) {
            player->camera->boundsR += 0x240;
            player->camera = NULL;
        }
    }

    for (int32 p = 0; p < Player->playerCount; ++p) {
        Zone->cameraBoundsR[p] += 0x240;
        Zone->playerBoundsR[p] += 0x2400000;
    }

    self->position.x                                = (Zone->cameraBoundsR[0] - ScreenInfo->centerX) << 16;
    self->position.y                                = (Zone->cameraBoundsB[0] - ScreenInfo->centerY) << 16;
    RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    Camera_SetupLerp(3, 0, self->position.x, self->position.y, 2);
    return true;
}

bool32 MMZ2Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);

    bool32 flag = true;

    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (player->animator.animationID == ANI_PUSH) {
            player->jumpPress = true;
            player->jumpHold  = true;
        }
        else if (player->velocity.y > 0x20000) {
            player->jumpHold = false;
        }
        if (player->groundVel > 0x40000)
            player->groundVel = 0x40000;
        if (player->position.x <= self->position.x - 0x180000 * player->playerID - 0x200000) {
            flag = false;
        }
        else {
            player->groundVel = 0;
            player->right     = false;
        }
    }

    return flag;
}

bool32 MMZ2Outro_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);

    self->timer += 8;
    RSDK.SetLimitedFade(0, 2, 1, self->timer, 231, 234);

    if (self->timer == 256)
        RSDK.PlaySfx(MMZ2Outro->sfxTMZEyes, false, 255);

    if (self->timer == 512) {
        foreach_active(Player, player) { player->up = true; }
    }
    if (self->timer == 768) {
        self->timer      = 0;
        self->flashTimer = 0;
        return true;
    }
    return false;
}

bool32 MMZ2Outro_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_THIS(MMZ2Outro);

    if (--self->flashTimer < 1) {
        CREATE_ENTITY(MMZLightning, MMZLightning_Unknown1, 0, 0);
        CREATE_ENTITY(MMZLightning, MMZLightning_Unknown3, 0, 0);
        RSDK.PlaySfx(MMZ2Outro->sfxThunda, false, 255);
        self->flashTimer = RSDK.Rand(120, 240);

        foreach_active(Player, player)
        {
            if (player->characterID == ID_TAILS) {
                player->state      = Player_State_Air;
                player->onGround   = false;
                player->velocity.y = -0x30000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            }
        }
    }
    if ((self->timer & 7) == 4 && self->flashTimer < 48)
        CREATE_ENTITY(MMZLightning, MMZLightning_Unknown6, 0, 0);

    if (++self->timer == 120) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        Camera_SetupLerp(3, 0, camera->position.x, camera->position.y - 0x1000000, 1);
    }
    if (self->timer == 600) {
        Zone_StartFadeOut(10, 0x000000);
        return true;
    }
    return false;
}

void MMZ2Outro_EditorDraw(void) {}

void MMZ2Outro_EditorLoad(void) {}

void MMZ2Outro_Serialize(void) {}
#endif
