#include "SonicMania.h"

ObjectCPZBoss *CPZBoss;

void CPZBoss_Update(void)
{
    RSDK_THIS(CPZBoss);
    StateMachine_Run(entity->state);
}

void CPZBoss_LateUpdate(void) {}

void CPZBoss_StaticUpdate(void) {}

void CPZBoss_Draw(void)
{
    RSDK_THIS(CPZBoss);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->playerAnimator, &entity->startPos, false);
    RSDK.DrawSprite(&entity->animator2, &entity->startPos, false);
}

void CPZBoss_Create(void *data)
{
    RSDK_THIS(CPZBoss);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            entity->drawOrder     = Zone->playerDrawLow;
            entity->startPos.x    = entity->position.x;
            entity->startPos.y    = entity->position.y;
            entity->active        = ACTIVE_BOUNDS;
            entity->visible       = true;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            if (!entity->type) {
                entity->drawFX = FX_FLIP;
                entity->state  = CPZBoss_State_SetupPlayer;
            }
            else {
                RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 2, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 0, &entity->animator2, true, 0);
            }
        }
    }
}

void CPZBoss_StageLoad(void)
{
    CPZBoss->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanCPZ.bin", SCOPE_STAGE);
    CPZBoss->hitbox.left   = -16;
    CPZBoss->hitbox.top    = 8;
    CPZBoss->hitbox.right  = 16;
    CPZBoss->hitbox.bottom = 16;
    CPZBoss->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, NULL);
    RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, NULL);
}

void CPZBoss_DrawLayerCB_1(void) { RSDK.SetClipBounds(0, 0, 24, RSDK_screens->width, RSDK_screens->height); }
void CPZBoss_DrawLayerCB_2(void) { RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height); }

bool32 CPZBoss_Unknown1(void)
{
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0)) {
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, NULL);
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, NULL);
        if (Music_CheckMusicStack()) {
            Music->nextTrack = TRACK_STAGE;
        }
        else {
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
        }
        // v3                 = 48;
        // PuyoBean->field_20 = 0;
        // PuyoBean->field_28 = 0;
        // PuyoBean->field_24 = 0;
        // PuyoBean->field_2C = 0;
        // do {
        //    *(&PuyoBean->base + v3) = 0;
        //    v3 += 4;
        //} while (v3 < 1072);
        foreach_all(PuyoBean, bean) { destroyEntity(bean); }
        int layerID = RSDK.GetSceneLayerID("FG High");
        RSDK.CopyTileLayer(layerID, 438, 150, layerID, 452, 150, 6, 2);
        RSDK_sceneInfo->timeEnabled = true;
        CPZBoss_Create(NULL);
        return true;
    }
    return false;
}

void CPZBoss_State_SetupPlayer(void)
{
    RSDK_THIS(CPZBoss);
    if (++entity->timer >= 8) {
        RSDK_sceneInfo->timeEnabled = false;
        Music_SetMusicTrack("BossPuyo.ogg", TRACK_EGGMAN2, 846720);
        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
        Entity *entPtr = RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 1);
        Camera_Unknown3(2, 0, entPtr->position.x, entPtr->position.y, 8);
        foreach_active(HUD, hud)
        {
            CPZBoss->hudSlotID = RSDK.GetEntityID(hud);
            hud->state         = HUD_State_GoOffScreen;
        }

        foreach_active(Player, player)
        {
            if (player->sidekick)
                player->active = ACTIVE_NEVER;
            else
                player->velocity.y = 0;
        }
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMTails.bin", SCOPE_STAGE); break;
            case ID_KNUCKLES: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMMighty.bin", SCOPE_STAGE); break;
            case ID_RAY: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMRay.bin", SCOPE_STAGE); break;
#endif
            default: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMSonic.bin", SCOPE_STAGE); break;
        }
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &entity->playerAnimator, true, 0);
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 0, &entity->animator2, true, 0);
        entity->timer  = 0;
        entity->active = ACTIVE_NORMAL;
        entity->state  = CPZBoss_State_Unknown1;
    }
}

void CPZBoss_State_Unknown1(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1() && ++entity->timer >= 30) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->velocity.y = 0x80000;
        }
        entity->state = CPZBoss_State_Unknown2;
    }
}

void CPZBoss_State_Unknown2(void)
{
    RSDK_THIS(CPZBoss);
    if (!CPZBoss_Unknown1()) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &CPZBoss->hitbox)) {
                if (player->sidekick) {
                    player->state = Player_State_Air;
                }
                else {
                    player->visible    = false;
                    player->active     = ACTIVE_NEVER;
                    player->up         = false;
                    player->down       = false;
                    player->left       = false;
                    player->right      = false;
                    player->jumpPress  = false;
                    player->jumpHold   = false;
                    player->position.x = entity->position.x;
                    player->position.y = entity->position.y;
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->playerAnimator, true, 0);
                    RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &entity->animator1, true, 1);
                    entity->direction = FLIP_X;
                    entity->state     = CPZBoss_State_Unknown3;
                }
            }
        }
    }
}

void CPZBoss_State_Unknown3(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        RSDK.ProcessAnimation(&entity->animator1);
        if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
            foreach_active(CPZBoss, boss)
            {
                if (boss->type == 1) {
                    RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 4, &boss->animator1, true, 0);
                    boss->state = CPZBoss_State_HandlePlayerMatch;
                }
            }
            entity->state = CPZBoss_State_SetupMatch;
        }
    }
}

void CPZBoss_State_SetupMatch(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1() && ++entity->timer == 60) {
        foreach_active(CPZShutter, shutter) { shutter->state = CPZShutter_State_Open; }
        uint key = time(0);
        int rand = RSDK.Random(0, 512, &key);

        int id = 0;
        foreach_active(PuyoMatch, match)
        {
            /*RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator2, true, 0);
            match->field_58 = PuyoMatch_Unknown6;
            match->field_A4 = rand;
            PuyoMatch_Unknown2(&match->position.x);
            if (match->field_68 == 1) {
                match->field_5C                       = PuyoAI_Unknown5;
                PuyoAI->field_2C[match->field_68]     = 0;
                PuyoAI->field_2C[match->field_68 + 2] = 14;
                PuyoAI->field_3C[match->field_68]     = 0;
                PuyoAI->field_3C[match->field_68 + 2] = 0;
                PuyoAI->field_4C[match->field_68]     = 0;
                PuyoAI->field_2C[match->field_68]     = 1;
                PuyoAI->field_4C[match->field_68]     = 16;
                PuyoAI->field_4C[match->field_68 + 2] = 50;
            }*/
            CPZBoss->managers[id++] = (Entity *)match;
        }
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 2, &entity->animator1, false, 0);
        entity->state = CPZBoss_State_HandleBossMatch;
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, CPZBoss_DrawLayerCB_1);
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, CPZBoss_DrawLayerCB_2);
    }
}

void CPZBoss_State_HandleBossMatch(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        EntityPuyoMatch *manager = (EntityPuyoMatch *)CPZBoss->managers[entity->type];
        RSDK.ProcessAnimation(&entity->animator1);
        if (entity->animator1.animationID == 2) {
            if (manager) {
                /*v4 = *&manager->field_A0;
                if (v4) {
                    if (*(v4 + 88) == PuyoBean_Unknown19) {
                        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 3, &entity->animator1, false, 0);
                    }
                    else {
                        if (*(v4 + 216) == 1) {
                            entity->animator1.frameID = 3;
                        }
                        else if (*(v4 + 220) == 1) {
                            entity->animator1.frameID = 1;
                        }
                        else if (*(v4 + 224) == 1) {
                            entity->animator1.frameID = 2;
                        }
                        if (*(v4 + 228) == 1) {
                            entity->animator1.frameID = 4;
                        }
                        else if (*(v4 + 232) == 1) {
                            entity->animator1.frameID = 5;
                        }
                    }
                }*/
            }
        }
        else if (entity->animator1.animationID == 3 && entity->animator1.frameID == entity->animator1.frameCount - 1) {
            RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 2, &entity->animator1, false, 0);
        }

        if (RSDK_controller[1].keyStart.press) {
            if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == TYPE_BLANK) {
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                RSDK.PlaySFX(PauseMenu->sfxAccept, 0, 255);
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
            }
        }
        if (manager) {
            /*if (manager->field_58 == PuyoMatch_Unknown9) {
                PuyoBean_Unknown2();*/
            entity->timer = 0;
            entity->state = CPZBoss_State_HandleBossMatchFinish;
            /*PuyoAI->field_2C[0] = 0;
            PuyoAI->field_2C[1] = 0;
            RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 3, &entity->animator1, false, 0);
        }*/
        }
    }
}

void CPZBoss_State_HandlePlayerMatch(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        EntityPuyoMatch *manager = (EntityPuyoMatch *)CPZBoss->managers[entity->type];
        if (manager || true) {
            RSDK.ProcessAnimation(&entity->animator1);
            if (entity->animator1.animationID == 2) {
                // result = *&manager->field_A0;
                // if (result && result->state == PuyoBean_Unknown19)
                //    RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 3, &entity->animator1, false, 0);
            }
            else {
                if (entity->animator1.animationID == 3 || entity->animator1.animationID == 4) {
                    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
                        RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 2, &entity->animator1, false, 0);
                }
            }
            if (manager || true) {
                /*if (manager->state == PuyoMatch_Unknown9) {
                    PuyoBean_Unknown2();*/
                entity->state    = CPZBoss_State_HandlePlayerMatchFinish;
                entity->field_6C = entity->position.x + 0x400000;
                entity->field_70 = (RSDK_screens->height + RSDK_screens->position.y) << 16;
                RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 3, &entity->animator1, false, 0);
                //}
            }
        }
    }
}

void CPZBoss_State_HandlePlayerMatchFinish(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        RSDK.ProcessAnimation(&entity->animator1);
        if (!(Zone->timer % 3)) {
            RSDK.PlaySFX(CPZBoss->sfxExplosion, 0, 255);
            if ((Zone->timer & 4) != 0) {
                int data = ((RSDK.Rand(0, 256) > 192) + 2);
                CREATE_ENTITY(Explosion, intToVoid(data), entity->field_6C + RSDK.Rand(-0x300000, 0x300000),
                              entity->field_70 + RSDK.Rand(-0x100000, 0x100000))
                    ->drawOrder = Zone->hudDrawOrder;
            }
        }
        entity->field_70 -= 0x40000;
        if (entity->field_70 < entity->position.y - 0xD00000) {
            RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 5, &entity->animator1, false, 0);
            entity->state = CPZBoss_State_PlayerWin;
            foreach_active(CPZShutter, shutter) { shutter->state = CPZShutter_State_Close; }
        }
    }
}

void CPZBoss_State_PlayerWin(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        RSDK.ProcessAnimation(&entity->animator1);
        if (!(Zone->timer % 3)) {
            RSDK.PlaySFX(CPZBoss->sfxExplosion, 0, 255);
            if ((Zone->timer & 4)) {
                EntityExplosion *explosion =
                    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), entity->position.x + RSDK.Rand(-0x100000, 0x100000),
                                  entity->position.y + RSDK.Rand(-0x100000, 0x100000));
                explosion->drawOrder = Zone->hudDrawOrder;
            }
        }

        if (++entity->timer == 60) {
            entity->timer  = 0;
            entity->active = ACTIVE_NORMAL;
            entity->state  = CPZBoss_State_Unknown10;
            RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &entity->playerAnimator, true, 0);
            RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 6, &entity->animator1, false, 0);
            if (Music_CheckMusicStack()) {
                Music->nextTrack = 0;
            }
            else {
                Music_TransitionTrack(TRACK_STAGE, 0.0125);
            }

            // RSDK.PlaySFX(PuyoBean->field_DB4, 0, 255);
            foreach_active(TippingPlatform, platform)
            {
                if (platform->bossID == 2) {
                    platform->state         = TippingPlatform_Unknown1;
                    platform->collapseDelay = 120;
                }
            }

            foreach_active(CPZBoss, boss)
            {
                if (!boss->type) {
                    boss->state = CPZBoss_State_Unknown11;
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &boss->animator1, false, 0);
                    RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &boss->playerAnimator, true, 0);
                    foreach_all(Player, player)
                    {
                        player->up         = false;
                        player->down       = false;
                        player->left       = false;
                        player->right      = false;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
                        player->groundVel  = 0;
                        if (!player->sidekick) {
                            player->stateInput = StateMachine_None;
                        }
                        player->visible    = true;
                        player->active     = ACTIVE_NORMAL;
                        player->position.x = boss->position.x;
                        player->position.y = boss->position.y;
                    }
                }
            }
        }
    }
}

void CPZBoss_State_HandleBossMatchFinish(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_Unknown1()) {
        RSDK.ProcessAnimation(&entity->animator1);
        if (++entity->timer == 60) {
            entity->timer = 0;
            entity->state = CPZBoss_Unknown1;
            // RSDK.PlaySFX(PuyoBean->field_DB4, 0, 255);
            foreach_active(TippingPlatform, platform)
            {
                if (platform->bossID == 1) {
                    platform->state         = TippingPlatform_Unknown1;
                    platform->collapseDelay = 120;
                }
            }

            foreach_all(Player, player)
            {
                player->visible    = true;
                player->active     = ACTIVE_NORMAL;
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->drawOrder  = Zone->playerDrawLow;
                player->state      = Player_State_Air;
                player->onGround   = false;
                player->velocity.y = -0x20000;
                RSDK.AddDrawListRef(Zone->playerDrawLow, RSDK.GetEntityID(&player));
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, false, 0);
                RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &entity->playerAnimator, true, 0);
            }

            foreach_active(CPZShutter, shutter)
            {
                shutter->active = ACTIVE_NORMAL;
                shutter->state  = CPZShutter_State_Close;
            }

            EntityHUD *hud = RSDK_GET_ENTITY(CPZBoss->hudSlotID, HUD);
            RSDK.ResetEntityPtr(hud, HUD->objectID, NULL);
            hud->dword5C[1].x -= 0x1100000;
            hud->dword5C[2].x -= 0x1200000;
            hud->field_9C = hud->dword5C[0].x;
            hud->dword5C[3].x -= 0x1300000;
            hud->dword5C[0].x -= 0x1000000;
            hud->state = HUD_State_ComeOnScreen;
        }
    }
}

void CPZBoss_State_Unknown10(void)
{
    RSDK_THIS(CPZBoss);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->timer > 32) {
        entity->velocity.y += 0x2800;
        entity->position.y += entity->velocity.y;
    }
    entity->timer++;
    if (entity->timer == 95)
        entity->state = StateMachine_None;
}

void CPZBoss_State_Unknown11(void)
{
    RSDK_THIS(CPZBoss);
    if (++entity->timer == 240) {
        entity->timer = 0;
        foreach_active(Player, player)
        {
            player->state           = Player_State_Air;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->jumpAbility     = 0;
            player->onGround        = false;
            player->position.x      = entity->position.x;
            player->position.y      = entity->position.y;
            player->velocity.x      = 0;
            player->velocity.y      = -0x80000;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
            Zone->playerBoundActiveR[player->playerID] = true;
        }

        EntityTransportTube *tube = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 3, TransportTube);
        tube->dirMask             = 10;
        TransportTube_SetupDirections(tube);

        EntityHUD *hud = RSDK_GET_ENTITY(CPZBoss->hudSlotID, HUD);
        RSDK.ResetEntityPtr(hud, HUD->objectID, NULL);
        hud->dword5C[1].x -= 0x1100000;
        hud->dword5C[2].x -= 0x1200000;
        hud->field_9C = hud->dword5C[0].x;
        hud->dword5C[3].x -= 0x1300000;
        hud->dword5C[0].x -= 0x1000000;
        hud->state = HUD_State_ComeOnScreen;

        entity->active = ACTIVE_NORMAL;
        entity->state  = CPZBoss_State_Unknown12;
    }
}

void CPZBoss_State_Unknown12(void)
{
    RSDK_THIS(CPZBoss);

    if (++entity->timer == 24) {
        for (int i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (player->camera)
                player->camera->state = Camera_State_Follow;
            if (!player->sidekick)
                player->stateInput = Player_ProcessP1Input;
        }
        entity->active = ACTIVE_BOUNDS;
        entity->state  = StateMachine_None;
    }
}

void CPZBoss_EditorDraw(void) {}

void CPZBoss_EditorLoad(void) {}

void CPZBoss_Serialize(void) { RSDK_EDITABLE_VAR(CPZBoss, VAR_UINT8, type); }
