#include "SonicMania.h"

ObjectCrimsonEye *CrimsonEye;

void CrimsonEye_Update(void)
{
    RSDK_THIS(CrimsonEye);
    StateMachine_Run(entity->state);
}

void CrimsonEye_LateUpdate(void) {}

void CrimsonEye_StaticUpdate(void) {}

void CrimsonEye_Draw(void)
{
    RSDK_THIS(CrimsonEye);
    StateMachine_Run(entity->stateDraw);
}

void CrimsonEye_Create(void *data)
{
    RSDK_THIS(CrimsonEye);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            if (data)
                entity->type = voidToInt(data);
            switch (entity->type) {
                case CRIMSONEYE_CONTAINER:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &entity->animator, true, 0);
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    CrimsonEye->value8.x  = entity->position.x;
                    CrimsonEye->value8.y  = entity->position.y - 0xA00000;
                    entity->state         = CrimsonEye_StateBody_Unknown1;
                    entity->stateDraw     = CrimsonEye_StateDraw_Container;
                    break;
                case CRIMSONEYE_CORE:
                    entity->active    = ACTIVE_NEVER;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &entity->animator, true, 0);
                    entity->field_74      = entity->position;
                    entity->state         = CrimsonEye_StateCore_Unknown1;
                    entity->stateDraw     = CrimsonEye_StateDraw_Core;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    break;
                case CRIMSONEYE_BALL:
                    entity->drawFX    = FX_SCALE;
                    entity->active    = ACTIVE_NEVER;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &entity->animator, true, 0);
                    entity->scale.x       = 0x200;
                    entity->scale.y       = 0x200;
                    entity->state         = CrimsonEye_StateBall_Unknown1;
                    entity->stateDraw     = CrimsonEye_StateDraw_Simple;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    break;
                case CRIMSONEYE_ARROW:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 3, &entity->animator, true, 0);
                    entity->scale.x       = 0x200;
                    entity->scale.y       = 0x200;
                    entity->state         = CrimsonEye_StateArrow;
                    entity->stateDraw     = CrimsonEye_StateDraw_Arrow;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    break;
                case CRIMSONEYE_SPIKE:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &entity->animator, true, 0);
                    entity->state         = CrimsonEye_StateSpike_CheckPlayerCollisions;
                    entity->stateDraw     = CrimsonEye_StateDraw_Simple;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    break;
                case CRIMSONEYE_SHOT:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 4, &entity->animator, true, 0);
                    entity->state         = CrimsonEye_StateShot_CheckPlayerCollisions;
                    entity->stateDraw     = CrimsonEye_StateDraw_Simple;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    break;
                default: return;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void CrimsonEye_StageLoad(void)
{
    CrimsonEye->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrimsonEye.bin", SCOPE_STAGE);

    CrimsonEye->hitboxEye.left  = -12;
    CrimsonEye->hitboxEye.top  = -12;
    CrimsonEye->hitboxEye.right = 12;
    CrimsonEye->hitboxEye.bottom = 12;

    CrimsonEye->hitbox2.left  = -16;
    CrimsonEye->hitbox2.top   = -16;
    CrimsonEye->hitbox2.right = 16;
    CrimsonEye->hitbox2.bottom = 16;

    CrimsonEye->hitboxOrb.left  = -3;
    CrimsonEye->hitboxOrb.top   = -3;
    CrimsonEye->hitboxOrb.right = 3;
    CrimsonEye->hitboxOrb.bottom = 3;

    CrimsonEye->hitboxBlock.left  = -21;
    CrimsonEye->hitboxBlock.top    = -23;
    CrimsonEye->hitboxBlock.right = 21;
    CrimsonEye->hitboxBlock.bottom = 23;

    CrimsonEye->hitboxElecOrb.left  = -6;
    CrimsonEye->hitboxElecOrb.top  = -6;
    CrimsonEye->hitboxElecOrb.right = 6;
    CrimsonEye->hitboxElecOrb.bottom = 6;

    CrimsonEye->hitbox6.left  = -14;
    CrimsonEye->hitbox6.top   = -32;
    CrimsonEye->hitbox6.right = 14;
    CrimsonEye->hitbox6.bottom = -23;

    CrimsonEye->hitbox7.left  = -14;
    CrimsonEye->hitbox7.top   = 23;
    CrimsonEye->hitbox7.right = 14;
    CrimsonEye->hitbox7.bottom = 32;

    CrimsonEye->health             = 17;
    CrimsonEye->invincibilityTimer = 0;
    CrimsonEye->value4             = 0;
    CrimsonEye->value6             = 0x4800;
    CrimsonEye->value7             = 0;
    CrimsonEye->scrollPos          = 0;
    CrimsonEye->scrollLimit        = -0x8000;
    CrimsonEye->value15            = 0x800;
    CrimsonEye->destroyedBallCount            = 0;
    CrimsonEye->value17            = 0;
    CrimsonEye->liftBG             = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("Lift Background"));
    CrimsonEye->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    CrimsonEye->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    CrimsonEye->sfxHover           = RSDK.GetSFX("TMZ1/Hover.wav");
    CrimsonEye->sfxHover2          = RSDK.GetSFX("TMZ1/Hover2.wav");
    CrimsonEye->sfxElevator        = RSDK.GetSFX("TMZ1/Elevator.wav");
    CrimsonEye->sfxShot            = RSDK.GetSFX("Stage/Shot.wav");
    CrimsonEye->sfxBeep            = RSDK.GetSFX("Stage/Beep4.wav");
    CrimsonEye->sfxHullClose       = RSDK.GetSFX("Stage/HullClose.wav");
    CrimsonEye->sfxButton          = RSDK.GetSFX("Stage/Button.wav");
    CrimsonEye->sfxImpact          = RSDK.GetSFX("Stage/Impact5.wav");
}

void CrimsonEye_Explode(void)
{
    RSDK_THIS(CrimsonEye);
    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);

        int x = entity->position.x;
        int y = entity->position.y;
        if (CrimsonEye->health) {
            x += RSDK.Rand(-32, 33) << 16;
            y += RSDK.Rand(-32, 33) << 16;
        }
        else {
            x += RSDK.Rand(-19, 20) << 16;
            y += RSDK.Rand(-24, 25) << 16;
        }

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS)), x, y);
        explosion->drawOrder       = Zone->drawOrderHigh + 2;
    }
}

void CrimsonEye_HandleBGMovement(void)
{
    for (int y = 0; y < 48; ++y) {
        uint16 tile = RSDK.GetTileInfo(1, 0, y + 24 * (2 - y / 24));
        RSDK.SetTileInfo(1, 0, y, tile);
        RSDK.SetTileInfo(1, 31, y, tile);
        for (int x = 1; x < 31; ++x) RSDK.SetTileInfo(1, x, y, 0xFFFF);
    }

    TileLayer *layer = RSDK.GetSceneLayer(1);
    for (int i = 0; i < layer->scrollInfoCount; ++i) {
        layer->scrollInfo[i].parallaxFactor = 0x200;
        layer->scrollInfo[i].scrollPos      = -0x1C0000;
    }

    layer->drawLayer[0]   = Zone->hudDrawOrder - 1;
    layer->parallaxFactor = 0x200;
    CrimsonEye->layerBG1  = layer;
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder - 1, false, CrimsonEye_DrawLayerCB_A);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, CrimsonEye_DrawLayerCB_B);
}

void CrimsonEye_DrawLayerCB_B(void) { RSDK.SetActivePalette(0, 0, RSDK_screens->height); }

void CrimsonEye_DrawLayerCB_A(void) { RSDK.SetActivePalette(5u, 0, RSDK_screens->height); }

void CrimsonEye_SetArrowDir(int type)
{
    foreach_active(CrimsonEye, boss)
    {
        if (boss->stateDraw == CrimsonEye_StateDraw_Arrow)
            boss->type = type;
    }
}

void CrimsonEye_StateBody_Unknown1(void)
{
    RSDK_THIS(CrimsonEye);
    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX + 80;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 124;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - RSDK_screens->height;
        entity->active              = ACTIVE_NORMAL;
        CREATE_ENTITY(TMZ1Setup, NULL, 0, 0);

        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, CrimsonEye)->active = ACTIVE_NORMAL;
        for (int i = 2; i < 14; ++i) {
            EntityCrimsonEye *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + i, CrimsonEye);
            child->active           = ACTIVE_NORMAL;
            if (i < 10)
                child->angle = 32 * (9 - i);
        }

        entity->state = CrimsonEye_StateBody_Unknown2;
    }
}

void CrimsonEye_StateBody_Unknown2(void)
{
    RSDK_THIS(CrimsonEye);
    CrimsonEye->value4 += 4;
    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;
    EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (!entity->timer && RSDK_screens->position.x + RSDK_screens->centerX > (entity->position.x >> 16) - 256) {
        CrimsonEye_HandleBGMovement();
        entity->timer = 1;
    }

    if (player1->position.x > entity->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX - 80;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;
        entity->timer = 15;
        entity->state = CrimsonEye_StateBody_Unknown3;
    }
}

void CrimsonEye_StateBody_Unknown3(void)
{
    RSDK_THIS(CrimsonEye);
    CrimsonEye->value4 += 4;
    if (!--entity->timer) {
        entity->stateEye = CrimsonEye_StateEye_Unknown1;
        entity->state    = CrimsonEye_StateBody_Unknown4;
    }
}

void CrimsonEye_StateBody_Unknown4(void)
{
    RSDK_THIS(CrimsonEye);

    StateMachine_Run(entity->stateEye);

    if (CrimsonEye->scrollPos == CrimsonEye->scrollLimit && CrimsonEye->value15 != 0x800 && CrimsonEye->health) {
        if (--entity->timer == 30) {
            RSDK.PlaySfx(CrimsonEye->sfxBeep, false, 255);
            if (CrimsonEye->scrollLimit) {
                CrimsonEye->value14 = 0;
                CrimsonEye_SetArrowDir(CE_ARROW_OFF);
            }
            else if (RSDK.Rand(0, 3)) {
                CrimsonEye->value14 = -0x80000;
                CrimsonEye_SetArrowDir(CE_ARROW_UP);
            }
            else {
                CrimsonEye->value14 = 0x80000;
                CrimsonEye_SetArrowDir(CE_ARROW_DOWN);
            }
        }
        if (entity->timer <= 0) {
            RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);
            Camera_ShakeScreen(4, 0, 8);
            RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 255);
            entity->timer           = RSDK.Rand(120, 180);
            CrimsonEye->scrollLimit = CrimsonEye->value14;
            EntityCrimsonEye *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, CrimsonEye);

            if (CrimsonEye->scrollLimit != -0x80000) {
                if (!CrimsonEye->scrollLimit) {
                    child->velocity.y     = -CrimsonEye->scrollPos;
                    CrimsonEye->scrollPos = 0;
                }
                else if (CrimsonEye->scrollLimit == 0x80000)
                    child->velocity.y = CrimsonEye->scrollPos >> 2;
            }
            else
                child->velocity.y = CrimsonEye->scrollPos >> 2;
        }
    }

    CrimsonEye_StateBody_Unknown5(); // its the exact same code

    if (CrimsonEye->health == 9)
        entity->stateEye = StateMachine_None;

    if (CrimsonEye->health == 8 && !entity->animator.frameID) {
        entity->animator.frameID = 1;
        entity->drawOrder        = Zone->drawOrderLow;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = -0x20000;
        debris->velocity.y    = -0x20000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;
        debris->delay      = 60;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = 0x20000;
        debris->velocity.y    = -0x20000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;
        debris->delay      = 60;
    }

    if (entity->drawOrder > Zone->drawOrderLow) {
        foreach_active(Ring, ring)
        {
            if (ring->drawOrder == Zone->drawOrderLow)
                ring->drawOrder++;
        }
    }

    if (!CrimsonEye->health) {
        CrimsonEye->scrollLimit = 0;
        entity->timer           = 0;
        entity->state           = CrimsonEye_StateBody_Unknown5;
        foreach_active(CrimsonEye, part)
        {
            if (part->stateDraw == CrimsonEye_StateDraw_Arrow)
                part->type = CE_ARROW_OFF;
        }
    }
}

void CrimsonEye_StateBody_Unknown5(void)
{
    if (CrimsonEye->scrollPos != CrimsonEye->scrollLimit) {
        if (CrimsonEye->scrollPos >= CrimsonEye->scrollLimit) {
            CrimsonEye->scrollPos -= 0x800;
            if (CrimsonEye->scrollPos < CrimsonEye->scrollLimit)
                CrimsonEye->scrollPos = CrimsonEye->scrollLimit;
        }
        else {
            CrimsonEye->scrollPos += 0x800;
            if (CrimsonEye->scrollPos > CrimsonEye->scrollLimit)
                CrimsonEye->scrollPos = CrimsonEye->scrollLimit;
        }
    }

    for (int i = 0; i < PLAYER_MAX; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        if (player->objectID)
            player->gravityStrength = 0x3800 - CrimsonEye->scrollPos / 48;
    }

    TileLayer *liftLayer = CrimsonEye->liftBG;
    liftLayer->scrollPos = -0x800000;
    for (int i = 0; i < liftLayer->scrollInfoCount; ++i) {
        liftLayer->scrollInfo[i].scrollSpeed = liftLayer->scrollInfo[i].parallaxFactor * (CrimsonEye->scrollPos / 24);
    }

    CrimsonEye->layerBG1->scrollSpeed = 6 * CrimsonEye->scrollPos;
}

void CrimsonEye_StateBody_Explode(void)
{
    RSDK_THIS(CrimsonEye);

    if (entity->timer < 48) {
        if (!(Zone->timer & 3)) {
            RSDK.PlaySfx(ItemBox->sfx_Destroy, false, 255);
            int x                      = entity->position.x + RSDK.Rand(-0x200000, 0x200000);
            int y                      = entity->position.y + RSDK.Rand(-0x800000, 0x800000);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }
    if (++entity->timer == 48) {
        Debris_FallFlickerAnimSetup(CrimsonEye->aniFrames, CrimsonEye->debrisInfo, 5);
        entity->visible = false;
        Camera_ShakeScreen(0, 0, 4);
        entity->state = CrimsonEye_StateBody_Unknown6;
    }
}

void CrimsonEye_StateBody_Unknown6(void)
{
    if (CrimsonEye->scrollPos != CrimsonEye->scrollLimit) {
        if (CrimsonEye->scrollPos >= CrimsonEye->scrollLimit) {
            CrimsonEye->scrollPos -= 0x800;
            if (CrimsonEye->scrollPos < CrimsonEye->scrollLimit)
                CrimsonEye->scrollPos = CrimsonEye->scrollLimit;
        }
        else {
            CrimsonEye->scrollPos += 0x800;
            if (CrimsonEye->scrollPos > CrimsonEye->scrollLimit)
                CrimsonEye->scrollPos = CrimsonEye->scrollLimit;
        }
    }

    TileLayer *liftLayer = CrimsonEye->liftBG;
    liftLayer->scrollPos = -0x800000;
    for (int i = 0; i < liftLayer->scrollInfoCount; ++i) {
        liftLayer->scrollInfo[i].scrollSpeed = liftLayer->scrollInfo[i].parallaxFactor * (CrimsonEye->scrollPos / 24);
    }

    CrimsonEye->layerBG1->scrollSpeed = 6 * CrimsonEye->scrollPos;
    TileLayer *moveLayer              = RSDK.GetSceneLayer(Zone->moveLayer);
    moveLayer->scrollSpeed            = (CrimsonEye->scrollPos / 24) << 6;
}

void CrimsonEye_StateDraw_Container(void)
{
    RSDK_THIS(CrimsonEye);

    if (!entity->animator.frameID && (CrimsonEye->invincibilityTimer & 1))
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void CrimsonEye_StateEye_Unknown1(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    CrimsonEye->value7 += 0x40000;

    if (CrimsonEye->value7 >= 0x800000) {
        CrimsonEye->value7 = 0x800000;
        entity->stateEye   = CrimsonEye_StateEye_Unknown2;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown2(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    if (++entity->timer2 == 120) {
        entity->timer2   = 0;
        entity->stateEye = CrimsonEye_StateEye_Unknown3;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown3(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    CrimsonEye->value7 += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (CrimsonEye->value7 >= 0xEC0000) {
        CrimsonEye->value7 = 0xEC0000;

        entity->velocity.y = (entity->velocity.y >> 3) - (entity->velocity.y >> 1);
        if (entity->velocity.y > -0x10000)
            entity->stateEye = CrimsonEye_StateEye_Unknown4;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown4(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    if (++entity->timer2 == 120) {
        entity->timer2   = 0;
        entity->stateEye = CrimsonEye_StateEye_Unknown5;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown5(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    entity->field_70   = (entity->field_70 + 1) & 0x7F;
    CrimsonEye->value6 = 80 * RSDK.Sin256(entity->field_70) + 0x4800;
    if (!entity->field_70)
        entity->stateEye = CrimsonEye_StateEye_Unknown6;

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown6(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye->value4 += 4;
    if (++entity->timer2 == 120) {
        entity->timer2   = 0;
        entity->stateEye = CrimsonEye_StateEye_Unknown7;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_StateEye_Unknown7(void)
{
    RSDK_THIS(CrimsonEye);
    CrimsonEye->value4 += 4;
    CrimsonEye->value7 -= 0x40000;

    if (CrimsonEye->value7 <= 0x800000) {
        CrimsonEye->value7 = 0x800000;
        entity->stateEye   = CrimsonEye_StateEye_Unknown2;
    }

    if (!(Zone->timer % 36))
        RSDK.PlaySfx(CrimsonEye->sfxHover, false, 255);
}

void CrimsonEye_ShootShot(void)
{
    RSDK_THIS(CrimsonEye);

    EntityCrimsonEye *child =
        CREATE_ENTITY(CrimsonEye, intToVoid(CRIMSONEYE_SHOT), RSDK_sceneInfo->entity->position.x + 0x260000, RSDK_sceneInfo->entity->position.y);
    child->velocity.x = 0x280 * RSDK.Cos512(entity->rotation);
    child->velocity.y = -0x280 * RSDK.Sin512(entity->rotation);
    if (entity->rotation < 256)
        child->position.y -= 0x110000;
    else
        child->position.y += 0x110000;

    child             = CREATE_ENTITY(CrimsonEye, intToVoid(CRIMSONEYE_SHOT), entity->position.x - 0x260000, entity->position.y);
    child->velocity.x = -0x280 * RSDK.Cos512(entity->rotation);
    child->velocity.y = -0x280 * RSDK.Sin512(entity->rotation);
    if (entity->rotation < 256)
        child->position.y -= 0x110000;
    else
        child->position.y += 0x110000;
    RSDK.PlaySfx(CrimsonEye->sfxShot, false, 255);
}

void CrimsonEye_SetupPositions(void)
{
    RSDK_THIS(CrimsonEye);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    CrimsonEye->positions[0].x = entity->position.x;
    CrimsonEye->positions[0].y = entity->position.y;

    CrimsonEye->positions[0].x += clampVal(10 * ((player1->position.x - entity->position.x) >> 8), -0xC0000, 0xC0000);
    CrimsonEye->positions[0].x &= 0xFFFF0000;

    CrimsonEye->positions[1] = CrimsonEye->positions[0];
    CrimsonEye->positions[2] = CrimsonEye->positions[1];

    CrimsonEye->positions[1].x += clampVal(3 * ((player1->position.x - entity->position.x) >> 8), -0x30000, 0x30000);
    CrimsonEye->positions[1].x &= 0xFFFF0000;
}

void CrimsonEye_DestroyBall(void)
{
    RSDK_THIS(CrimsonEye);
    if (CrimsonEye->destroyedBallCount < 8) {
        EntityCrimsonEye *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + CrimsonEye->ballDestroyOrder[CrimsonEye->destroyedBallCount], CrimsonEye);
        child->state            = CrimsonEye_StateBall_Unknown2;
        child->timer            = 30;
        child->drawOrder        = Zone->drawOrderLow;
        child->scale.y          = 0x200;
        child->scale.x          = 0x200;
        ++CrimsonEye->destroyedBallCount;
    }
}

void CrimsonEye_CheckPlayerCollisions(void)
{
    RSDK_THIS(CrimsonEye);
    if (CrimsonEye->invincibilityTimer) {
        CrimsonEye->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, CrimsonEye->positions, &CrimsonEye->hitboxEye) && Player_CheckBossHit(player, entity)) {
                if (CrimsonEye->health)
                    CrimsonEye->health--;

                if (CrimsonEye->health) {
                    if (CrimsonEye->health == 9) {
                        CrimsonEye_DestroyBall();
                        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);
                        entity->timer = 120;
                        entity->state = CrimsonEye_StateCore_BreakOut;
                    }
                    else {
                        CrimsonEye_DestroyBall();
                        RSDK.PlaySfx(CrimsonEye->sfxHit, false, 255);
                        CrimsonEye->invincibilityTimer = 48;
                    }
                }
                else {
                    RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);
                    entity->timer = 120;

                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 14);
                    debris->velocity.x    = -0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 120;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 15);
                    debris->velocity.x    = 0x30000;
                    debris->velocity.y    = -0x40000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 120;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 16);
                    debris->velocity.x    = -0x20000;
                    debris->velocity.y    = -0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 120;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 17);
                    debris->velocity.x    = 0x20000;
                    debris->velocity.y    = -0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 120;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 12);
                    debris->velocity.x    = 0;
                    debris->velocity.y    = -0x30000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 13);
                    debris->velocity.x    = 0;
                    debris->velocity.y    = 0;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x160000, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 4);
                    debris->drawFX        = FX_ROTATE;
                    debris->rotation      = entity->rotation;
                    debris->velocity.x    = -0x20000;
                    debris->velocity.y    = -0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x160000, entity->position.y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 5);
                    debris->drawFX        = FX_ROTATE;
                    debris->rotation      = 0x1FF - entity->rotation;
                    debris->velocity.x    = 0x20000;
                    debris->velocity.y    = -0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    int velX = RSDK.Rand(-6, 6) << 15;
                    int velY = RSDK.Rand(-10, -6) << 15;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->positions[0].x, CrimsonEye->positions[0].y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 1);
                    debris->velocity.x    = velX;
                    debris->velocity.y    = velY;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 60;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->positions[1].x, CrimsonEye->positions[1].y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 2);
                    debris->velocity.x    = velX;
                    debris->velocity.y    = velY;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 60;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, CrimsonEye->positions[2].x, CrimsonEye->positions[2].y);
                    RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &debris->animator, true, 3);
                    debris->velocity.x    = velX;
                    debris->velocity.y    = velY;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                    debris->delay      = 60;

                    RSDK_sceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
                    entity->visible = false;
                    entity->state   = CrimsonEye_StateCore_Explode;
                }
                foreach_break;
            }
        }
    }
}

void CrimsonEye_StateCore_Unknown1(void)
{
    CrimsonEye_SetupPositions();
    CrimsonEye_CheckPlayerCollisions();
}

void CrimsonEye_StateCore_BreakOut(void)
{
    RSDK_THIS(CrimsonEye);

    CrimsonEye_SetupPositions();
    CrimsonEye_Explode();
    if (--entity->timer <= 0) {
        entity->timer = 90;
        --CrimsonEye->health;
        entity->drawOrder = Zone->drawOrderLow;
        entity->state     = CrimsonEye_StateCore_Unknown3;
    }
}

void CrimsonEye_StateCore_Unknown3(void)
{
    RSDK_THIS(CrimsonEye);
    CrimsonEye_SetupPositions();
    CrimsonEye_Explode();
    if (--entity->timer <= 0) {
        CrimsonEye->value15 = 0x4000;
        entity->velocity.x  = -0x10000;
        entity->timer       = 90;
        entity->drawOrder   = Zone->drawOrderLow + 1;
        entity->state       = CrimsonEye_StateCore_Unknown4;
    }
}

void CrimsonEye_StateCore_Unknown4(void)
{
    RSDK_THIS(CrimsonEye);

    entity->position.x += entity->velocity.x;

    if (abs(entity->velocity.y) <= 0x10000) {
        if (entity->field_74.y <= (Zone->screenBoundsB1[0] - 144) << 16) {
            entity->field_74.y += 0x10000;
            if (entity->field_74.y > (Zone->screenBoundsB1[0] - 144) << 16)
                entity->field_74.y = (Zone->screenBoundsB1[0] - 144) << 16;
        }
        else {
            entity->field_74.y -= 0x10000;
            if (entity->field_74.y < (Zone->screenBoundsB1[0] - 144) << 16)
                entity->field_74.y = (Zone->screenBoundsB1[0] - 144) << 16;
        }
    }
    entity->field_74.y -= entity->velocity.y;

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_74.y) & 0xFFFF0000;
    if (entity->position.x <= (Zone->screenBoundsL1[0] + 64) << 16 || entity->position.x >= (Zone->screenBoundsR1[0] - 64) << 16) {
        entity->velocity.x = -entity->velocity.x;
    }

    int storeY         = entity->position.y;
    entity->position.y = entity->field_74.y;

    foreach_active(InvisibleBlock, block)
    {
        int side = RSDK.CheckObjectCollisionBox(block, &block->hitbox, entity, &CrimsonEye->hitboxBlock, true);
        if (side == C_TOP) {
            if (entity->velocity.y < 0)
                entity->velocity.y = 0;
            entity->rotation    = 64;
            CrimsonEye->value17 = 8;
            CrimsonEye->value16 = 15;
            entity->state       = CrimsonEye_StateCore_Unknown5;
            RSDK.PlaySfx(CrimsonEye->sfxButton, false, 255);
            RSDK.PlaySfx(CrimsonEye->sfxImpact, false, 255);
        }
        else if (side == C_BOTTOM) {
            if (entity->velocity.y > 0)
                entity->velocity.y = 0;
            entity->rotation    = 448;
            CrimsonEye->value17 = 8;
            CrimsonEye->value16 = 15;
            entity->state       = CrimsonEye_StateCore_Unknown5;
            RSDK.PlaySfx(CrimsonEye->sfxButton, false, 255);
            RSDK.PlaySfx(CrimsonEye->sfxImpact, false, 255);
        }
    }

    entity->field_74.y = entity->position.y;
    entity->position.y = storeY;
    CrimsonEye_SetupPositions();
    CrimsonEye_CheckPlayerCollisions();

    if (!(Zone->timer & 0x3F))
        RSDK.PlaySfx(CrimsonEye->sfxHover2, false, 255);
}

void CrimsonEye_StateCore_Unknown5(void)
{
    RSDK_THIS(CrimsonEye);
    if (--CrimsonEye->value16 <= 0) {
        CrimsonEye->value16 = 15;
        if (--CrimsonEye->value17 != 0xFF) {
            CrimsonEye_ShootShot();
        }
        else {
            entity->state = CrimsonEye_StateCore_Unknown4;
        }
    }
    CrimsonEye_SetupPositions();
    CrimsonEye_CheckPlayerCollisions();
}

void CrimsonEye_StateCore_Explode(void)
{
    RSDK_THIS(CrimsonEye);
    CrimsonEye_Explode();
    if (--entity->timer <= 0) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->state = CrimsonEye_StateCore_SpawnSignPost;
    }
}

void CrimsonEye_StateCore_SpawnSignPost(void)
{
    RSDK_THIS(CrimsonEye);
    if (++entity->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->position.y = (RSDK_screens->position.y - 64) << 16;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        }

        destroyEntity(entity);
    }
}

void CrimsonEye_StateDraw_Core(void)
{
    RSDK_THIS(CrimsonEye);
    Vector2 drawPos;

    if (CrimsonEye->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    if (entity->state != CrimsonEye_StateCore_Unknown1 && entity->state != CrimsonEye_StateCore_BreakOut) {
        entity->animator.frameID = 12;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        entity->animator.frameID = 13;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        entity->drawFX |= FX_ROTATE;
        drawPos.x                = entity->position.x;
        drawPos.y                = entity->position.y;
        drawPos.x                = entity->position.x - 0x160000;
        entity->animator.frameID = 4;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->rotation = 0x1FF - entity->rotation;
        drawPos.x += 0x2C0000;
        entity->animator.frameID = 5;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->drawFX &= ~FX_ROTATE;
        entity->rotation         = 0x1FF - entity->rotation;
        entity->animator.frameID = 0;
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &CrimsonEye->positions[0], false);

    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &CrimsonEye->positions[1], false);

    entity->inkEffect        = INK_ALPHA;
    entity->alpha            = 0xC0;
    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &CrimsonEye->positions[2], false);

    entity->inkEffect = INK_NONE;
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void CrimsonEye_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(CrimsonEye);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &CrimsonEye->hitbox2)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }
}

void CrimsonEye_StateBall_Unknown1(void)
{
    RSDK_THIS(CrimsonEye);

    uint8 angle          = ((entity->angle & 0xFF) + CrimsonEye->value4);
    entity->position.x = CrimsonEye->value6 * RSDK.Cos256(angle) + CrimsonEye->value8.x;
    entity->position.y = (RSDK.Sin256(CrimsonEye->value5) << 13) + CrimsonEye->value7 + CrimsonEye->value8.y;
    if (angle <= 0x80) {
        entity->drawOrder        = Zone->drawOrderLow;
        entity->animator.frameID = 0;
        entity->scale.x          = 2 * abs(angle - 0x40) + 0x182;
        entity->scale.y          = 2 * abs(angle - 0x40) + 0x182;
    }
    else {
        entity->animator.frameID = 1;
        entity->drawOrder        = Zone->drawOrderLow + 1;
        entity->scale.x          = 2 * (0x13F - abs(angle - 0xC0));
        entity->scale.y          = 2 * (0x13F - abs(angle - 0xC0));
        CrimsonEye_CheckPlayerCollisions_Ball();
    }
}

void CrimsonEye_StateBall_Unknown2(void)
{
    RSDK_THIS(CrimsonEye);

    entity->position.y -= 0x18000;
    CrimsonEye_CheckPlayerCollisions_Ball();
    if (--entity->timer <= 0) {
        int angle = 0;
        RSDK.PlaySfx(CrimsonEye->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        for (int i = 0; i < 8; ++i) {
            EntityCrimsonEye *child = CREATE_ENTITY(CrimsonEye, intToVoid(CRIMSONEYE_SPIKE), entity->position.x, entity->position.y);
            child->animator.frameID = i + 2;
            child->velocity.x       = RSDK.Cos256(angle) << 9;
            child->velocity.y       = RSDK.Sin256(angle) << 9;
            angle += 32;
        }
        destroyEntity(entity);
    }
}

void CrimsonEye_StateDraw_Simple(void)
{
    RSDK_THIS(CrimsonEye);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void CrimsonEye_StateSpike_CheckPlayerCollisions(void)
{
    RSDK_THIS(CrimsonEye);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &CrimsonEye->hitboxOrb)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void CrimsonEye_StateSpike_Move(void)
{
    RSDK_THIS(CrimsonEye);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= 1;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void CrimsonEye_StateShot_CheckPlayerCollisions(void)
{
    RSDK_THIS(CrimsonEye);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &CrimsonEye->hitboxElecOrb)) {
            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x80000, true)) {
        entity->velocity.y = -entity->velocity.y;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void CrimsonEye_StateArrow(void) {}

void CrimsonEye_StateDraw_Arrow(void)
{
    RSDK_THIS(CrimsonEye);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->animator.frameID = entity->type;
    if (entity->type != CE_ARROW_OFF)
        RSDK.DrawSprite(&entity->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void CrimsonEye_EditorDraw(void)
{
    RSDK_THIS(CrimsonEye);
    switch (entity->type) {
        case CRIMSONEYE_CONTAINER:
            entity->drawOrder = Zone->drawOrderLow + 1;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 0, &entity->animator, true, 0);
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            CrimsonEye->value8.x  = entity->position.x;
            CrimsonEye->value8.y  = entity->position.y - 0xA00000;
            entity->stateDraw     = CrimsonEye_StateDraw_Container;
            break;
        case CRIMSONEYE_CORE:
            entity->drawOrder = Zone->drawOrderLow + 1;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 1, &entity->animator, true, 0);
            entity->field_74      = entity->position;
            entity->stateDraw     = CrimsonEye_StateDraw_Core;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case CRIMSONEYE_BALL:
            entity->drawFX    = FX_SCALE;
            entity->drawOrder = Zone->drawOrderLow + 1;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &entity->animator, true, 0);
            entity->scale.x       = 0x200;
            entity->scale.y       = 0x200;
            entity->stateDraw     = CrimsonEye_StateDraw_Simple;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case CRIMSONEYE_ARROW:
            entity->drawOrder = Zone->drawOrderLow;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 3, &entity->animator, true, 0);
            entity->scale.x       = 0x200;
            entity->scale.y       = 0x200;
            entity->stateDraw     = CrimsonEye_StateDraw_Arrow;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case CRIMSONEYE_SPIKE:
            entity->drawOrder = Zone->drawOrderLow + 1;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 2, &entity->animator, true, 0);
            entity->stateDraw     = CrimsonEye_StateDraw_Simple;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case CRIMSONEYE_SHOT:
            entity->drawOrder = Zone->drawOrderLow + 1;
            RSDK.SetSpriteAnimation(CrimsonEye->aniFrames, 4, &entity->animator, true, 0);
            entity->stateDraw     = CrimsonEye_StateDraw_Simple;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        default: break;
    }

    StateMachine_Run(entity->stateDraw);
}

void CrimsonEye_EditorLoad(void)
{
    CrimsonEye->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrimsonEye.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CrimsonEye, type);
    RSDK_ENUM_VAR("Container", CRIMSONEYE_CONTAINER);
    RSDK_ENUM_VAR("Core", CRIMSONEYE_CORE);
    RSDK_ENUM_VAR("Ball", CRIMSONEYE_BALL);
    RSDK_ENUM_VAR("Arrow", CRIMSONEYE_ARROW);
}
#endif

void CrimsonEye_Serialize(void) { RSDK_EDITABLE_VAR(CrimsonEye, VAR_ENUM, type); }
