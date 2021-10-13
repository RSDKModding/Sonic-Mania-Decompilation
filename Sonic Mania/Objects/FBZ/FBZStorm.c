#include "SonicMania.h"

ObjectFBZStorm *FBZStorm;

void FBZStorm_Update(void)
{
    RSDK_THIS(FBZStorm);
    StateMachine_Run(entity->state);

    entity->velocity.x = -0x40000;
    // foreach_active(Current, current)
    //{
    //    v2 = -0x10000 * LOBYTE(current[1].velocity.x);
    //    if (v2 < entity->velocity.x)
    //        entity->velocity.x = v2;
    //}

    for (int32 p = 0; p < Player->playerCount; ++p) {
        if (!RSDK_GET_ENTITY(p, Player)->sidekick) {
            entity->screenPosX[p] = RSDK_screens[p].position.x;
            int32 x                 = ((entity->screenPosX[p] << 14) - (RSDK_screens[p].position.x << 14)) + entity->velocity.x;
            entity->stormAngle[p] = RSDK.ATan2(entity->velocity.y, -x) << 1;
            Vector2 *pos          = &FBZStorm->positions[0x40 * p];

            for (int32 i = 0; i < 0x40; ++i) {
                pos->x += x;
                pos->y += entity->velocity.y;

                if (pos->x < -0x1000000)
                    pos->x += 0x2000000;
                if (pos->y < -0xA00000)
                    pos->y += 0x1400000;
                if (pos->x > 0x1000000)
                    pos->x -= 0x2000000;
                if (pos->y > 0xA00000)
                    pos->y -= 0x1400000;
                ++pos;
            }
        }
    }

    if (RSDK.GetSceneLayer(3)->drawLayer[0] < DRAWLAYER_COUNT) {
        entity->flag = true;
        if (entity->blendAmount < 0x100)
            entity->blendAmount += 8;
    }
    else {
        entity->flag = false;
        if (entity->blendAmount > 0)
            entity->blendAmount -= 8;
    }

    RSDK.SetLimitedFade(0, FBZStorm->srcPal, 2, entity->blendAmount, 0x80, 0x100);
}

void FBZStorm_LateUpdate(void) {}

void FBZStorm_StaticUpdate(void)
{
    bool32 flag = false;
    foreach_all(FBZStorm, storm)
    {
        if (storm->flag)
            flag = true;
    }

    if (!flag || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == PauseMenu->objectID) {
        if (FBZStorm->playingRainSFX) {
            RSDK.StopSFX(FBZStorm->sfxRain);
            FBZStorm->playingRainSFX = false;
        }
    }
}

void FBZStorm_Draw(void)
{
    RSDK_THIS(FBZStorm);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];

    int32 centerX      = screen->centerX << 16;
    int32 centerY      = screen->centerY << 16;
    entity->rotation = entity->stormAngle[RSDK_sceneInfo->currentScreenID];

    Vector2 *pos = &FBZStorm->positions[0x40 * RSDK_sceneInfo->currentScreenID];
    for (int32 i = 0; i < 0x40; ++i) {
        Vector2 drawPos;
        drawPos.x = centerX + pos->x;
        drawPos.y = centerY + pos->y;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
        ++pos;
    }
}

void FBZStorm_Create(void *data)
{
    RSDK_THIS(FBZStorm);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active     = ACTIVE_NORMAL;
        entity->drawOrder  = Zone->drawOrderHigh;
        entity->drawFX     = FX_ROTATE;
        entity->inkEffect  = INK_ALPHA;
        entity->velocity.x = -0x40000;
        entity->velocity.y = 0xC0000;
        entity->state      = FBZStorm_Unknown1;

        for (int32 p = 0; p < Player->playerCount; ++p) {
            for (int32 i = 0; i < 0x40; ++i) {
                FBZStorm->positions[i].x = RSDK.Rand(-0x1000000, 0x1000000);
                FBZStorm->positions[i].y = RSDK.Rand(-0x800000, 0x800000);
            }
        }

        RSDK.SetSpriteAnimation(FBZStorm->aniFrames, 0, &entity->animator, true, 0);
    }
}

void FBZStorm_StageLoad(void)
{
    FBZStorm->active     = ACTIVE_ALWAYS;
    FBZStorm->aniFrames  = RSDK.LoadSpriteAnimation("FBZ/Storm.bin", SCOPE_STAGE);
    FBZStorm->sfxRain    = RSDK.GetSFX("FBZ/Rain.wav");
    FBZStorm->sfxThunder = RSDK.GetSFX("FBZ/Thunder.wav");
}

void FBZStorm_Unknown1(void)
{
    RSDK_THIS(FBZStorm);
    if (entity->flag) {
        if (RSDK.GetEntityCount(Current->objectID, true) > 0)
            entity->state = FBZStorm_Unknown2;
    }
}

void FBZStorm_Unknown2(void)
{
    RSDK_THIS(FBZStorm);

    entity->visible = true;
    if (entity->alpha >= 128) {
        FBZStorm->srcPal = 4;
        entity->inkEffect   = INK_BLEND;
        entity->state  = FBZStorm_Unknown4;
    }
    else {
        entity->alpha += 4;
    }

    bool32 flag = false;
    foreach_all(FBZStorm, storm)
    {
        if (storm->flag)
            flag = true;
    }

    if (FBZStorm->playingRainSFX) {
        if (!flag || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == PauseMenu->objectID) {
            RSDK.StopSFX(FBZStorm->sfxRain);
            FBZStorm->playingRainSFX = false;
        }
    }
    else {
        RSDK.PlaySfx(FBZStorm->sfxRain, true, 0xFF);
        FBZStorm->playingRainSFX = true;
    }
}

void FBZStorm_Unknown3(void)
{
    RSDK_THIS(FBZStorm);

    if (entity->alpha <= 0) {
        entity->visible = false;
        if (FBZStorm->playingRainSFX) {
            RSDK.StopSFX(FBZStorm->sfxRain);
        }
        entity->state = FBZStorm_Unknown1;
    }
    else {
        entity->alpha -= 4;
    }
}

void FBZStorm_Unknown4(void)
{
    RSDK_THIS(FBZStorm);
    if (entity->timer <= 0) {
        entity->state = FBZStorm_Unknown5;
    }
    else {
        entity->timer--;
        if (!entity->flag) {
            entity->inkEffect = INK_ALPHA;
            entity->state          = FBZStorm_Unknown3;
        }
    }
}

void FBZStorm_Unknown5(void)
{
    RSDK_THIS(FBZStorm);
    RSDK.SetLimitedFade(0, 2, 3, FBZStorm->array1[entity->timer], 0x80, 0x100);
    if (!entity->timer)
        RSDK.PlaySfx(FBZStorm->sfxThunder, 0, 255);
    if (++entity->timer == 20) {
        entity->timer = RSDK.Rand(180, 320);
        entity->state = FBZStorm_Unknown4;
    }
}

void FBZStorm_EditorDraw(void) {}

void FBZStorm_EditorLoad(void) {}

void FBZStorm_Serialize(void) {}
