#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTryAgainE *TryAgainE;

void TryAgainE_Update(void)
{
    RSDK_THIS(TryAgainE);
    StateMachine_Run(entity->state);
}

void TryAgainE_LateUpdate(void) {}

void TryAgainE_StaticUpdate(void)
{
    foreach_active(TryAgainE, tryAgainE) { RSDK.AddDrawListRef(2, RSDK.GetEntityID(tryAgainE)); }
}

void TryAgainE_Draw(void)
{
    RSDK_THIS(TryAgainE);
    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        Vector2 drawPos;

        RSDK.SetActivePalette(3, 0, RSDK_screens->height);
        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.SetActivePalette(0, 0, RSDK_screens->height);

        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + 0x2E0000;
        if (globals->playerID > 0xFF)
            drawPos.x = entity->position.x - 0x140000;
        if (entity->player1Animator.animationID == 2)
            drawPos.y = entity->position.y + 0x2E0000 + 0x40000;
        RSDK.DrawSprite(&entity->player1Animator, &drawPos, false);

        int drawY = drawPos.y;
        if (entity->player1Animator.animationID == 2) {
            drawY = drawPos.y - 0x40000;
            drawPos.y -= 0x40000;
        }

        if (globals->playerID > 255) {
            drawPos.x += 0x280000;
            entity->direction = FLIP_X;
            if (entity->player2Animator.animationID == 2)
                drawPos.y = drawY + 0x40000;
            RSDK.DrawSprite(&entity->player2Animator, &drawPos, false);
            entity->direction = FLIP_NONE;
        }
    }
    else {
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void TryAgainE_Create(void *data)
{
    RSDK_THIS(TryAgainE);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_FLIP;
        entity->state         = TryAgainE_Unknown2;
        RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 1, &entity->animator2, true, 4);
        RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 2, &entity->animator3, true, 3);

        int id        = -1;
        uint8 playerID = globals->playerID & 0xFF;
        if (playerID) {
            do {
                playerID >>= 1;
                ++id;
            } while (playerID > 0);
        }
        RSDK.SetSpriteAnimation(TryAgainE->playerAniFrames, 2 * id, &entity->player1Animator, true, 3);

        if (globals->playerID > 255) {
            id       = -1;
            playerID = (globals->playerID >> 8) & 0xFF;
            if (playerID) {
                do {
                    playerID >>= 1;
                    ++id;
                } while (playerID > 0);
            }
            RSDK.SetSpriteAnimation(TryAgainE->playerAniFrames, 2 * id, &entity->player2Animator, true, 3);
        }
    }
}

void TryAgainE_StageLoad(void)
{
    TryAgainE->aniFrames       = RSDK.LoadSpriteAnimation("Credits/TryAgainE.bin", SCOPE_STAGE);
    TryAgainE->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE);
    TryAgainE->active          = ACTIVE_ALWAYS;
    RSDK.CopyPalette(0, 0, 1, 0, 128);
}

void TryAgainE_Unknown1(void)
{
    int id    = 1;
    int timer = 0;
    foreach_all(TAEmerald, emerald)
    {
        int store      = timer;
        emerald->state = TAEmerald_Unknown2;
        emerald->timer = timer;
        timer += 8;
        if (SaveGame->saveRAM) {
            if ((id & SaveGame->saveRAM->chaosEmeralds) > 0)
                timer = store;
        }
        id <<= 1;
    }
}

void TryAgainE_Unknown2(void)
{
    RSDK_THIS(TryAgainE);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->player1Animator);
    if (globals->playerID > 255)
        RSDK.ProcessAnimation(&entity->player2Animator);

    if ((entity->timer & 0x7F) == 1) {
        if ((entity->timer & 0x80) != 0 && entity->animator2.animationID == 2)
            RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 1, &entity->animator2, true, 0);
        if ((int8)(entity->timer & 0xFF) >= 0 && entity->animator3.animationID == 2)
            RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 1, &entity->animator3, true, 0);
        TryAgainE_Unknown1();
    }
    else if ((entity->timer & 0x7F) == 59) {
        if ((entity->timer & 0x80) == 0 && entity->animator2.animationID == 1)
            RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 2, &entity->animator2, true, 0);
        if ((int8)(entity->timer & 0xFF) < 0 && entity->animator3.animationID == 1)
            RSDK.SetSpriteAnimation(TryAgainE->aniFrames, 2, &entity->animator3, true, 0);
    }
    if (++entity->timer == 1) {
        foreach_all(TAEmerald, emerald)
        {
            emerald->startPos.x = entity->position.x;
            emerald->startPos.y = entity->position.y;
            emerald->position.x = emerald->startPos.x;
            emerald->position.y = emerald->startPos.y;
            emerald->startPos.y += 0x100000;
            emerald->position.y = emerald->startPos.y;
            emerald->position.x += 0x480000;
            emerald->angle  = 0;
            emerald->drawFX = FX_ROTATE;
            emerald->state  = StateMachine_None;
        }
    }

    if (RSDK_controller->keyA.press || RSDK_controller->keyStart.press)
        entity->timer = 600;
#if RETRO_USE_TOUCH_CONTROLS
    else if (RSDK_touchMouse->count)
        entity->timer = 600;
#endif

    if (entity->timer == 600) {
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xFFFFFF), entity->position.x, entity->position.y);
        fxFade->speedIn      = 24;
        fxFade->speedOut     = 24;
        fxFade->fadeOutBlack = true;
    }

    if (entity->timer >= 740) {
        if (API.CheckDLC(DLC_PLUS))
            RSDK.SetScene("Presentation", "Encore Summary");
        else
            RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

void TryAgainE_EditorDraw(void) {}

void TryAgainE_EditorLoad(void) {}

void TryAgainE_Serialize(void) {}
#endif
