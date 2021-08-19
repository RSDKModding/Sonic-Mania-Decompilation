#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectOOZ2Outro *OOZ2Outro;

void OOZ2Outro_Update(void)
{
    RSDK_THIS(OOZ2Outro);

    Entity *entityPtr = NULL; //*&MegaOctus->field_4;
    if (globals->gameMode < MODE_TIMEATTACK && entityPtr && !entityPtr->objectID) {
        entity->scrollOffset.x = entity->moveOffset.x & 0xFFFF0000;
        entity->scrollOffset.y = entity->moveOffset.y & 0xFFFF0000;

        StateMachine_Run(entity->state);

        foreach_active(Player, player)
        {
            TileLayer *layer = entity->moveLayer;
            if (!player->sidekick) {
                layer->scrollPos               = -entity->scrollOffset.y;
                layer->scrollInfo[0].scrollPos = -entity->scrollOffset.x;
            }
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = -(int)(entity->moveOffset.x & 0xFFFF0000);
        }

        if (entity->prisonPtr) {
            entity->prisonPtr->position.x = entity->scrollOffset.x + entity->prisonPos.x;
            entity->prisonPtr->position.y = entity->scrollOffset.y + entity->prisonPos.y;
        }
    }
}

void OOZ2Outro_LateUpdate(void) {}

void OOZ2Outro_StaticUpdate(void) {}

void OOZ2Outro_Draw(void) {}

void OOZ2Outro_Create(void *data)
{
    RSDK_THIS(OOZ2Outro);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active  = ACTIVE_BOUNDS;
            entity->visible = false;
            if (!entity->size.x)
                entity->size.x = 0x1A80000;
            if (!entity->size.y)
                entity->size.y = 0xF00000;
            entity->moveLayer   = RSDK.GetSceneLayer(Zone->moveLayer);
            entity->updateRange = entity->size;
            foreach_all(EggPrison, prison)
            {
                entity->prisonPtr = prison;
                entity->prisonPos = prison->position;
            }

            entity->state = OOZ2Outro_Unknown2;
        }
        else {
            destroyEntity(entity);
        }
    }
}

void OOZ2Outro_StageLoad(void)
{
    OOZ2Outro->sfxGlug      = RSDK.GetSFX("OOZ/Glug.wav");
    OOZ2Outro->sfxSubLaunch = RSDK.GetSFX("OOZ/SubLaunch.wav");
}

void OOZ2Outro_StageFinishCB_Act2(void)
{
    Zone->screenBoundsR1[0] = 0x4000;
    foreach_active(Player, player)
    {
        player->state      = Player_State_Air;
        player->stateInput = 0;
        player->left       = false;
        player->right      = true;
        player->up         = false;
        player->down       = false;
        player->jumpPress  = false;
        player->jumpHold   = false;
    }

    foreach_active(OOZ2Outro, outro) { outro->state = OOZ2Outro_Unknown3; }

    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
}

void OOZ2Outro_Unknown2(void)
{
    RSDK_THIS(OOZ2Outro);
    entity->moveOffset.y = RSDK.Sin256(Zone->timer) << 10;
}

void OOZ2Outro_CheckSkip(void)
{
    if (RSDK_controller->keyStart.press && !(RSDK_sceneInfo->state & 1)) {
        globals->suppressTitlecard = false;
        globals->suppressAutoMusic = false;
        globals->enableIntro       = false;
        RSDK.SetGameMode(ENGINESTATE_FROZEN);
        Zone_StartFadeOut(20, 0);
        Music_FadeOut(0.03);
    }
}

void OOZ2Outro_Unknown3(void)
{
    RSDK_THIS(OOZ2Outro);
    OOZ2Outro_CheckSkip();
    entity->moveOffset.y = RSDK.Sin256(Zone->timer) << 10;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (player->playerAnimator.animationID == ANI_PUSH) {
            player->jumpPress = true;
            player->jumpHold  = true;
        }
        else if (player->velocity.y > -0x20000) {
            player->jumpHold = false;
        }
        if (player->position.x > entity->position.x - 0x100000 && player->velocity.x > 0x20000)
            player->right = false;
        if (player->position.x < entity->position.x + 0x400000) {
            if (player->right == false) {
                if (player->groundVel < 0x20000)
                    player->groundVel = 0x20000;
                if (player->velocity.x < 0x20000)
                    player->velocity.x = 0x20000;
            }
            flag |= true;
        }
        else {
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->right      = false;
        }
    }

    if (!flag)
        ++entity->timer;
    if (entity->timer > 60) {
        entity->timer = 0;
        entity->state = OOZ2Outro_Unknown4;
        foreach_active(Player, player)
        {
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->right      = false;
            player->state      = Player_State_None;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BALANCE1, &player->playerAnimator, false, 0);
            Zone->playerBoundActiveR[player->playerID] = 0;
            EntityCamera *camera                       = player->camera;
            if (camera)
                camera->state = StateMachine_None;
        }
    }
}

void OOZ2Outro_Unknown4(void)
{
    RSDK_THIS(OOZ2Outro);
    OOZ2Outro_CheckSkip();
    entity->moveOffset.y -= 0x6000;

    if (!(Zone->timer & 0xF)) {
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySFX(OOZ2Outro->sfxGlug, false, 255);
    }

    if (++entity->timer == 140)
        RSDK.PlaySFX(OOZ2Outro->sfxSubLaunch, false, 255);

    if (entity->timer > 180) {
        entity->timer      = 0;
        entity->velocity.y = -0x30000;
        entity->state      = OOZ2Outro_Unknown5;
        foreach_active(Player, player) { RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BALANCE2, &player->playerAnimator, false, 0); }
    }
}

void OOZ2Outro_Unknown5(void)
{
    RSDK_THIS(OOZ2Outro);

    if (entity->velocity.y < -0x8000)
        entity->velocity.y += 0x3800;
    entity->velocity.x += 0x1800;
    entity->moveOffset.x += entity->velocity.x;
    entity->moveOffset.y += entity->velocity.y;
    foreach_active(Player, player) { player->position.x += entity->velocity.x; }

    if (++entity->timer > 140) {
        entity->timer = 0;
        entity->state = 0;
        Zone_StartFadeOut(10, 0x000000);
        foreach_active(Player, player) { player->active = ACTIVE_NEVER; }
    }
}

void OOZ2Outro_EditorDraw(void) {}

void OOZ2Outro_EditorLoad(void) {}

void OOZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(OOZ2Outro, VAR_VECTOR2, size); }
#endif
