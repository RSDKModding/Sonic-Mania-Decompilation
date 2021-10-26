#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Crane *PBL_Crane;

void PBL_Crane_Update(void)
{
    RSDK_THIS(PBL_Crane);
    StateMachine_Run(entity->state);
}

void PBL_Crane_LateUpdate(void) {}

void PBL_Crane_StaticUpdate(void) {}

void PBL_Crane_Draw(void)
{
    RSDK_THIS(PBL_Crane);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, true);
    }
}

void PBL_Crane_Create(void *data)
{
    RSDK_THIS(PBL_Crane);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder = 9;
        entity->active    = ACTIVE_NORMAL;
        entity->type      = voidToInt(data);
        switch (entity->type) {
            case 0:
                entity->position.x = RSDK_screens->centerX << 16;
                entity->position.y = 0;
                entity->alpha      = 0xFF;
                entity->scale.x    = 0x600;
                entity->scale.y    = 0x200;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 0, &entity->animator5, true, 1);
                entity->stateDraw = PBL_Crane_StateDraw_Unknown1;
                break;
            case 1:
                entity->visible = true;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 3, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 4, &entity->animator3, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 5, &entity->animator4, true, 0);
                entity->scale.x   = 0x200;
                entity->scale.y   = 0x200;
                entity->state     = PBL_Crane_State_Unknown1;
                entity->stateDraw = PBL_Crane_StateDraw_Unknown2;
                break;
            case 2:
                entity->visible = true;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 6, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &entity->animator2, true, 4);
                entity->state     = PBL_Crane_Unknown7;
                entity->stateDraw = PBL_Crane_StateDraw_Unknown3;
                break;
            case 3:
                entity->drawOrder = 12;
                entity->visible   = true;
                entity->scale.x   = 0x180;
                entity->scale.y   = 0x180;
                entity->drawFX    = FX_SCALE;
                entity->state     = PBL_Crane_State2_Unknown1;
                break;
            default: break;
        }
    }
}

void PBL_Crane_StageLoad(void)
{
    PBL_Crane->aniFrames    = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE);
    PBL_Crane->sfxBackGlass = RSDK.GetSFX("Pinball/Backglass.wav");
    PBL_Crane->sfxCraneMove = RSDK.GetSFX("Pinball/CraneMove.wav");
    PBL_Crane->sfxCraneDrop = RSDK.GetSFX("Pinball/CraneDrop.wav");
    PBL_Crane->sfxCraneGrab = RSDK.GetSFX("Pinball/CraneGrab.wav");
    PBL_Crane->sfxCraneRise = RSDK.GetSFX("Pinball/CraneRise.wav");
    PBL_Crane->sfxPrizeGood = RSDK.GetSFX("Pinball/PrizeGood.wav");
    PBL_Crane->sfxPrizeBad  = RSDK.GetSFX("Pinball/PrizeBad.wav");
}

void PBL_Crane_HandlePrizes(void)
{
    RSDK_THIS(PBL_Crane);
    switch (entity->animator1.frameID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4: {
            int32 playerID = 1 << entity->animator1.frameID;
            globals->characterFlags |= playerID;
            PBL_Crane->prizeID = 3;

            if (!globals->playerID) {
                globals->playerID |= playerID;
            }
            else if (!(globals->playerID & 0xFF00)) {
                globals->playerID |= playerID << 8;
            }
            else if (!globals->stock) {
                globals->stock |= playerID;
            }
            else if (!(globals->stock & 0xFF00)) {
                globals->stock |= playerID << 8;
            }
            else if (!(globals->stock & 0xFF0000)) {
                globals->stock |= playerID << 16;
            }
            break;
        }
        case 5:
            PBL_Setup->rings   = 0;
            PBL_Setup->score   = 0;
            PBL_Crane->prizeID = 2;
            break;
        case 6:
            globals->restartRings += PBL_Setup->rings;
            globals->restartRings = clampVal(globals->restartRings, 0, 999);
            if (globals->gameMode == MODE_MANIA && globals->restartRings >= globals->restart1UP) {
                PBL_Setup_GiveLife();
                globals->restart1UP += 100;
                if (globals->restart1UP > 300)
                    globals->restart1UP = 1000;
            }
            PBL_Setup->rings   = 0;
            PBL_Crane->prizeID = 4;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            globals->restartPowerups &= -0x40;
            globals->restartPowerups |= entity->animator1.frameID - 6;
            PBL_Crane->prizeID = 5;
            break;
        case 11: {
            foreach_all(PBL_Bumper, bumper)
            {
                if (!bumper->state) {
                    bumper->velocity.y = 32;
                    bumper->state      = PBL_TargetBumper_Unknown4;
                    bumper->active     = ACTIVE_NORMAL;
                }
            }

            foreach_all(PBL_Ring, ring)
            {
                if (ring->active == ACTIVE_NEVER) {
                    ring->active = ACTIVE_BOUNDS;
                    RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 1, &ring->animator, true, 0);
                    ring->state = PBL_Ring_State_Ring;
                }
            }
            PBL_Crane->prizeID = 6;
            break;
        }
        case 12:
            if (SaveGame->saveRAM->lives < 99)
                SaveGame->saveRAM->lives++;
            if (globals->restartLives[0] < 99)
                globals->restartLives[0]++;
            PBL_Crane->prizeID = 7;
            break;
        default: break;
    }
}

void PBL_Crane_StateDraw_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);
    entity->position.x = RSDK_screens->centerX << 16;
    if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder) {
        entity->animator1.frameID = 1;
        entity->inkEffect         = INK_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, true);
        RSDK.DrawSprite(&entity->animator5, NULL, true);

        RSDK.AddDrawListRef(11, RSDK_sceneInfo->entitySlot);
        RSDK.SetClipBounds(0, RSDK_screens->centerX - 96, 0, RSDK_screens->centerX + 96, (entity->position.y >> 16) + 64);
    }
    else {
        RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, true);

        entity->animator1.frameID = 2;
        entity->drawFX            = FX_SCALE;
        entity->inkEffect         = INK_ADD;
        RSDK.DrawSprite(&entity->animator1, NULL, true);

        entity->animator1.frameID = 3;
        RSDK.DrawSprite(&entity->animator1, NULL, true);

        entity->drawFX = FX_NONE;
    }
}

void PBL_Crane_StateDraw_Unknown3(void)
{
    RSDK_THIS(PBL_Crane);
    RSDK.DrawSprite(&entity->animator2, NULL, true);
    RSDK.DrawSprite(&entity->animator1, NULL, true);
}

void PBL_Crane_StateDraw_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + 0x1A0000;
    RSDK.DrawSprite(&entity->animator3, NULL, true);

    entity->drawFX = FX_SCALE;
    RSDK.DrawSprite(&entity->animator1, &drawPos, true);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, true);
    RSDK.DrawSprite(&entity->animator4, NULL, true);
}

void PBL_Crane_State_CreatePrizes(void)
{
    RSDK_THIS(PBL_Crane);
    entity->position.y       = 0;
    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    camera->worldY           = 0x1000000;
    entity->field_78         = camera->targetPtr;
    camera->targetPtr        = NULL;
    entity->visible          = true;
    int32 spawnX               = entity->position.x - 0x6C0000;
    int32 spawnY               = 0x600000 + entity->position.y;
    entity->state            = PBL_Crane_Unknown6;

    for (int32 i = 1; i < 6; ++i) {
        EntityPBL_Crane *prize = CREATE_ENTITY(PBL_Crane, intToVoid(2), spawnX, spawnY);
        if (globals->gameMode == MODE_ENCORE) {
            if (!((1 << i) & globals->characterFlags) || i == 5) {
                prize->animator1.frameID = i;
            }
            else {
                prize->animator1.frameID = RSDK.Rand(6, 12);
            }
        }
        else {
            if (i == 5) {
                prize->animator1.frameID = 5;
            }
            else {
                prize->animator1.frameID = RSDK.Rand(6, 13);
            }
        }

        spawnX += 0x280000;
        prize->parent = (Entity *)entity;
    }

    CREATE_ENTITY(PBL_Crane, intToVoid(1), entity->position.x, -0x180000)->parent = (Entity *)entity;
    if (PBL_Setup->sectorID < PBL_Setup->sectorCount)
        PBL_Setup->sectorID++;
    PBL_Crane->prizeID = 1;
}

void PBL_Crane_Unknown6(void)
{
    RSDK_THIS(PBL_Crane);

    if (entity->position.y >= 0x300000) {
        entity->timer = 0;
        entity->state = StateMachine_None;
    }
    else {
        entity->position.y += 0x10000;
        EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
        camera->worldY += 0x28000;
        camera->position.y -= 0x14000;
        if (!entity->timer)
            --camera->rotationY;
        entity->timer = (entity->timer + 1) & 3;
    }
}

void PBL_Crane_Unknown7(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)entity->parent;

    entity->position.x -= 0x10000;
    int32 dist           = abs(entity->position.x - parent->position.x);
    entity->position.y = (dist >> 13) * (dist >> 13) + (parent->position.y + 0x300000);
    if (entity->position.x < parent->position.x - 0x980000) {
        entity->position.x += 0x1100000;
    }
}

void PBL_Crane_State_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)entity->parent;

    if (!parent->state) {
        if (entity->position.y >= 0x100000) {
            entity->state = PBL_Crane_State_Unknown2;
        }
        else {
            entity->position.y += 0x8000;
        }
    }
}

void PBL_Crane_State_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);

    entity->velocity.x = (-((RSDK_controller[1].keyLeft.down || RSDK_stickL[1].keyLeft.down) != 0) & 0xFFFF0000) + 0x8000;
    if (++entity->timer == 30) {
        entity->timer = 0;
        entity->state = PBL_Crane_State_Unknown3;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
}

void PBL_Crane_State_Unknown3(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)entity->parent;

    if (RSDK_stickL[1].keyLeft.press || RSDK_controller[1].keyLeft.press) {
        ++entity->timer;
        entity->velocity.x = -0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
    else if (RSDK_stickL[1].keyRight.press || RSDK_controller[1].keyRight.press) {
        ++entity->timer;
        entity->velocity.x = 0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
    else if (RSDK_stickL[1].keyDown.press || RSDK_controller[1].keyDown.press) {
        entity->timer = 4;
    }

    entity->position.x += entity->velocity.x;
    if (entity->velocity.x <= 0) {
        if (entity->position.x <= parent->position.x - 0x600000)
            entity->velocity.x = -entity->velocity.x;
    }
    else {
        if (entity->position.x >= parent->position.x + 0x600000)
            entity->velocity.x = -entity->velocity.x;
    }

    if (entity->timer == 4) {
        entity->timer = 0;
        entity->state = PBL_Crane_State_Unknown4;
        RSDK.PlaySfx(PBL_Crane->sfxCraneDrop, false, 255);
    }
}

void PBL_Crane_State_Unknown4(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)entity->parent;

    entity->position.y += 0x10000;
    if (entity->position.y >= parent->position.y + 0x180000) {
        foreach_active(PBL_Crane, crane)
        {
            if (crane->type == 2) {
                if (abs(crane->position.x - entity->position.x) < 0x100000) {
                    memcpy(&entity->animator1, &crane->animator1, sizeof(Animator));
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &crane->animator1, false, 0);
                }
            }
        }
        entity->state = PBL_Crane_State_Unknown5;
        RSDK.PlaySfx(PBL_Crane->sfxCraneGrab, false, 255);
    }
}

void PBL_Crane_State_Unknown5(void)
{
    RSDK_THIS(PBL_Crane);
    ++entity->timer;
    if (entity->scale.x > 384) {
        entity->scale.x -= 16;
        entity->scale.y -= 16;
    }

    if (entity->timer > 8) {
        entity->animator3.frameID = (entity->timer - 8) >> 1;
        entity->animator4.frameID = (entity->timer - 8) >> 1;
    }

    if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
        entity->timer = 0;
        entity->state = PBL_Crane_State_Unknown6;
        RSDK.PlaySfx(PBL_Crane->sfxCraneRise, false, 255);
    }
}

void PBL_Crane_State_Unknown6(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)entity->parent;

    entity->position.y -= 0x10000;
    if (entity->scale.x > 0x180) {
        entity->scale.x -= 16;
        entity->scale.y -= 16;
    }

    if (entity->position.y <= 0x100000) {
        if (entity->animator1.animationID == 6) {
            EntityPBL_Crane *crane = CREATE_ENTITY(PBL_Crane, intToVoid(3), entity->position.x, entity->position.y + 0x1A0000);
            crane->animator1       = entity->animator1;
            crane->parent          = entity->parent;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, false, 0);

            crane->velocity.y = -0x20000;
            if (entity->position.x > parent->position.x)
                crane->velocity.x = 0x20000;
            else
                crane->velocity.x = -0x20000;
            if (crane->animator1.frameID != 5) {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeGood, false, 255);
                entity->state = StateMachine_None;
            }
            else {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
                entity->state = StateMachine_None;
            }
        }
        else {
            parent->state = PBL_Crane_State2_Unknown3;
            RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
            entity->state = StateMachine_None;
        }
    }
}

void PBL_Crane_State2_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);

    if (entity->scale.x >= 512) {
        entity->drawFX = FX_NONE;
    }
    else {
        entity->scale.x += 16;
        entity->scale.y += 16;
    }

    entity->velocity.y += 0x2800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->position.y > 0x500000) {
        entity->velocity.y = -(entity->velocity.y >> 1);
        if (++entity->timer == 2) {
            entity->timer = 0;
            entity->state = PBL_Crane_State2_Unknown2;
        }
    }
}

void PBL_Crane_State2_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);
    entity->visible = (++entity->timer >> 2) & 1;
    if (entity->timer == 60) {
        PBL_Crane_HandlePrizes();
        ((EntityPBL_Crane *)entity->parent)->state = PBL_Crane_State2_Unknown3;
        destroyEntity(entity);
    }
}

void PBL_Crane_State2_Unknown3(void)
{
    RSDK_THIS(PBL_Crane);
    if (++entity->timer == 24) {
        foreach_active(PBL_HUD, hud)
        {
            hud->state   = PBL_HUD_Unknown13;
            hud->visible = true;
        }
    }

    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    if (entity->position.y <= 0) {
        foreach_active(PBL_Crane, crane)
        {
            if (crane != entity)
                destroyEntity(crane);
        }
        entity->timer = 0;
        entity->state = StateMachine_None;

        foreach_all(PBL_Sector, sector)
        {
            if (sector->craneID == PBL_Setup->sectorID)
                sector->active = ACTIVE_NORMAL;
        }
        RSDK.PrintInteger(PRINT_NORMAL, "Sector", PBL_Setup->sectorID);
        camera->targetPtr = entity->field_78;
        camera->rotationY = -96;
        foreach_active(PBL_HUD, hud)
        {
            switch (PBL_Crane->prizeID) {
                case 1: PBL_HUD_DisplayMessage(hud, "TOO BAD!", 6); continue;
                case 2: PBL_HUD_DisplayMessage(hud, "OH NO!", 6); continue;
                case 3: PBL_HUD_DisplayMessage(hud, "NICE SAVE!", 3); continue;
                case 4: PBL_HUD_DisplayMessage(hud, "!RINGS DEPOSITED!", 1); break;
                case 5: PBL_HUD_DisplayMessage(hud, "!ITEM COLLECTED!", 1); break;
                case 6: PBL_HUD_DisplayMessage(hud, "!TABLE RESTORED!", 1); break;
                case 7: PBL_HUD_DisplayMessage(hud, "!EXTRA LIFE!", 1); break;
                default: break;
            }
        }
    }
    else {
        entity->position.y = entity->position.y - 0x10000;
        camera->worldY -= 0x28000;
        camera->position.y += 0x14000;
        if ((entity->timer & 3) == 1)
            ++camera->rotationY;
    }
}

#if RETRO_INCLUDE_EDITOR
void PBL_Crane_EditorDraw(void) {}

void PBL_Crane_EditorLoad(void) {}
#endif

void PBL_Crane_Serialize(void) {}
#endif
