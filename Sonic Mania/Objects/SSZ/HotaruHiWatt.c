#include "SonicMania.h"

ObjectHotaruHiWatt *HotaruHiWatt;

void HotaruHiWatt_Update(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
}

void HotaruHiWatt_LateUpdate(void) {}

void HotaruHiWatt_StaticUpdate(void) {}

void HotaruHiWatt_Draw(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void HotaruHiWatt_Create(void *data)
{
    RSDK_THIS(HotaruHiWatt);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active        = ACTIVE_XBOUNDS;
            entity->updateRange.x = 0xC00000;
            entity->updateRange.y = 0xC00000;
            entity->type          = voidToInt(data);
            entity->drawOrder     = Zone->drawOrderLow;
            switch (entity->type) {
                case HHW_0:
                    entity->visible = false;
                    entity->drawFX  = FX_FLIP;
                    entity->health  = 6;
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &entity->animator3, true, 0);
                    entity->alpha         = 256;
                    entity->state         = HotaruHiWatt_State_SetupBounds;
                    entity->hitbox.left   = -24;
                    entity->hitbox.top    = -24;
                    entity->hitbox.right  = 24;
                    entity->hitbox.bottom = 24;
                    break;
                case HHW_1:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator1, true, 3);
                    ++entity->drawOrder;
                    entity->inkEffect = INK_ADD;
                    entity->visible   = true;
                    entity->health    = 3;
                    entity->position.x += RSDK.Rand(-0x500000, 0x500000);
                    entity->position.y += RSDK.Rand(-0x800000, 0x800000);
                    entity->state         = HotaruHiWatt_State1_Unknown1;
                    entity->hitbox.left   = -6;
                    entity->hitbox.top    = -6;
                    entity->hitbox.right  = 6;
                    entity->hitbox.bottom = 6;
                    break;
                case HHW_2:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator1, true, 3);
                    entity->field_80.x    = entity->position.x;
                    entity->field_80.y    = entity->position.y;
                    entity->inkEffect     = INK_ADD;
                    entity->visible       = true;
                    entity->health        = 3;
                    entity->field_74      = 0x800;
                    entity->alpha         = -0x200;
                    entity->state         = HotaruHiWatt_State2_Unknown1;
                    entity->hitbox.left   = -6;
                    entity->hitbox.top    = -6;
                    entity->hitbox.right  = 6;
                    entity->hitbox.bottom = 6;
                    break;
                case HHW_4:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator1, true, 3);
                    entity->field_88  = entity->position;
                    entity->inkEffect = INK_ADD;
                    entity->visible  = true;
                    entity->health   = 3;
                    entity->alpha    = 0x100;
                    entity->state    = HotaruHiWatt_State1_Unknown2;
                    break;
                case HHW_5:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator1, true, 0);
                    entity->inkEffect = INK_ADD;
                    entity->visible = true;
                    entity->health  = 3;
                    entity->alpha   = 0x100;
                    entity->state   = HotaruHiWatt_State5_Unknown1;
                    break;
                case HHW_MINILASER:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 2, &entity->animator1, true, 1);
                    entity->state         = HotaruHiWatt_State7_Unknown1;
                    entity->visible       = true;
                    entity->hitbox.left   = -4;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 4;
                    entity->hitbox.bottom = 8;
                    break;
                case HHW_ELECTRICORB:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 4, &entity->animator1, true, 0);
                    entity->state         = HotaruHiWatt_State8_Unknown1;
                    entity->visible       = true;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void HotaruHiWatt_StageLoad(void)
{
    HotaruHiWatt->aniFrames    = RSDK.LoadSpriteAnimation("SSZ1/HotaruHiWatt.bin", SCOPE_STAGE);
    HotaruHiWatt->hotaruFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    HotaruHiWatt->value8       = false;
    RSDK.CopyPalette(1, 128, 0, 128, 64);
    HotaruHiWatt->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    HotaruHiWatt->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    HotaruHiWatt->sfxHHWAppear = RSDK.GetSFX("SSZ1/HHWAppear.wav");
    HotaruHiWatt->sfxFlash     = RSDK.GetSFX("SSZ1/HHWFlash.wav");
    HotaruHiWatt->sfxLaser     = RSDK.GetSFX("SSZ1/HotaruLaser.wav");
    HotaruHiWatt->sfxAppear    = RSDK.GetSFX("SSZ1/HotaruAppear.wav");
    HotaruHiWatt->sfxFly       = RSDK.GetSFX("SSZ1/HotaruFly.wav");
    HotaruHiWatt->sfxFlyUp     = RSDK.GetSFX("SSZ1/HHWFlyUp.wav");
    HotaruHiWatt->sfxCharge    = RSDK.GetSFX("SSZ1/HHWCharge.wav");

    Soundboard_LoadSFX("Stage/Zap.wav", true, HotaruHiWatt_ZapCheckCB, NULL);
    Soundboard_LoadSFX("SSZ1/HHWLaser.wav", 32606, HotaruHiWatt_LaserCheckCB, HotaruHiWatt_LaserUpdateCB);
}

bool32 HotaruHiWatt_ZapCheckCB(void)
{
    int count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_State2_Unknown4 || boss->state == HotaruHiWatt_State2_Unknown5 || boss->state == HotaruHiWatt_State2_Unknown7)
            ++count;
    }
    return count > 1;
}

bool32 HotaruHiWatt_LaserCheckCB(void)
{
    int count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_State_Unknown9 || boss->state == HotaruHiWatt_State_Unknown10)
            ++count;
    }
    return count > 0;
}

void HotaruHiWatt_LaserUpdateCB(int sfx)
{
    if (!(Soundboard->sfxUnknown6[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

void HotaruHiWatt_CheckPlayerCollisions2(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
            if (entity->type == HHW_1) {
                HotaruHiWatt_SpawnChildHotaru();
            }
            else {
                int count = 0;
                foreach_active(HotaruHiWatt, boss)
                {
                    if (boss->type == HHW_2)
                        count++;
                }
                if (count == 1)
                    HotaruHiWatt_Unknown16();
            }
            destroyEntity(entity);
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            if (--entity->health <= 0) {
                entity->stateDraw           = HotaruHiWatt_StateDraw_Destroyed;
                entity->state               = HotaruHiWatt_State_Destroyed;
                entity->timer               = 0;
                RSDK_sceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            else {
                entity->invincibilityTimer = 48;
                RSDK.PlaySfx(HotaruHiWatt->sfxHit, false, 255);
            }
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions3(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions4(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        Hitbox hitbox = entity->hitbox;
        if (!player->invincibleTimer && !player->blinkTimer)
            hitbox.bottom += 0x80;

        if (Player_CheckBadnikTouch(player, entity, &hitbox)) {
            if (entity->invincibilityTimer) {
                if (player->position.y > entity->position.y) {
                    Player_CheckHit(player, entity);
                }
            }
            else if (player->position.y < entity->position.y || player->invincibleTimer || player->blinkTimer) {
                if (Player_CheckBossHit(player, entity)) {
                    if (--entity->health <= 0) {
                        entity->stateDraw           = HotaruHiWatt_StateDraw_Destroyed;
                        entity->state               = HotaruHiWatt_State_Destroyed;
                        entity->timer               = 0;
                        RSDK_sceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                    }
                    else {
                        entity->invincibilityTimer = 48;
                        RSDK.PlaySfx(HotaruHiWatt->sfxHit, false, 255);
                    }
                }
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void HotaruHiWatt_StateDraw1_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    entity->inkEffect = INK_ADD;
    if (!(entity->timer & 2))
        RSDK.DrawSprite(&entity->animator1, NULL, false);

    int alpha     = entity->alpha;
    entity->alpha = entity->timer2;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->alpha = alpha;
}

void HotaruHiWatt_StateDraw1_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void HotaruHiWatt_StateDraw_Destroyed(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&entity->animator2) == 98)
        entity->direction = FLIP_X;

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void HotaruHiWatt_StateDraw_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&entity->animator2) == 98)
        entity->direction = FLIP_X;
    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->inkEffect = INK_NONE;
}

void HotaruHiWatt_StateDraw_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);
    Vector2 drawPos;

    if (RSDK.GetFrameID(&entity->animator2) == 98)
        entity->direction = FLIP_X;

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
    entity->inkEffect         = INK_ALPHA;
    entity->animator4.frameID = 0;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->animator4.frameID = 1;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    drawPos = entity->position;
    drawPos.y += 0x180000 + ((Zone->timer & 7) << 20);
    entity->direction = (Zone->timer >> 2) & 1;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos, false);

    entity->inkEffect = INK_NONE;
}

void HotaruHiWatt_StateDraw_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.DrawCircle(entity->position.x, entity->position.y, entity->field_74 >> 16, 0xF0F0F0, 128, INK_ADD, false);
    RSDK.DrawCircle(entity->position.x, entity->position.y, entity->field_74 / 0xCCCC, 0xF0F0F0, 128, INK_ADD, false);
}

void HotaruHiWatt_StateDraw_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&entity->animator2) == 98)
        entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator2, 0, false);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawCircle(entity->position.x, entity->position.y, entity->field_74 >> 16, 0xF0F0F0, entity->alpha >> 1, INK_ADD, false);
    RSDK.DrawCircle(entity->position.x, entity->position.y, entity->field_74 / 0xCCCC, 0xF0F0F0, entity->alpha >> 1, INK_ADD, false);
}

void HotaruHiWatt_SpawnChildHotaru(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_2), player1->position.x, HotaruHiWatt->value7 + 0x200000);
    child->isPermanent        = true;
    child->parents[0]         = (Entity *)parent;

    EntityHotaruHiWatt *child2 = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_2), player1->position.x, HotaruHiWatt->value7 + 0x200000);
    child2->angle += 0x800000;
    child2->isPermanent = true;
    child2->parents[0]  = (Entity *)parent;
    child->parents[1]   = (Entity *)child2;
}

void HotaruHiWatt_Unknown16(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= HotaruHiWatt->boundsM)
        parent->position.x = HotaruHiWatt->boundsR;
    else
        parent->position.x = HotaruHiWatt->boundsL;
    parent->position.y = HotaruHiWatt->value7;
    parent->nextState  = HotaruHiWatt_State_Unknown7;
    parent->state      = HotaruHiWatt_State_Unknown1;
}

void HotaruHiWatt_State_SetupBounds(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16);
        HotaruHiWatt->boundsL       = (Zone->screenBoundsL1[0] + 64) << 16;
        HotaruHiWatt->boundsR       = (Zone->screenBoundsR1[0] - 64) << 16;
        HotaruHiWatt->boundsM       = entity->position.x;
        HotaruHiWatt->boundsT       = (Zone->screenBoundsT1[0] + 48) << 16;
        HotaruHiWatt->boundsB       = (Zone->screenBoundsB1[0] - 8) << 16;

        foreach_active(Fireflies, fireflies)
        {
            if (fireflies->state == Fireflies_State_Unknown1)
                destroyEntity(fireflies);
        }
        entity->state = HotaruHiWatt_State_WaitForPlayer;
    }
}

void HotaruHiWatt_State_WaitForPlayer(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        entity->position.y += -0x100000 - (RSDK_screens->centerY << 16);
        HotaruHiWatt->value7 = entity->position.y;
        entity->state        = HotaruHiWatt_State_Unknown1;
        entity->nextState    = HotaruHiWatt_State_Unknown5;
    }
}

void HotaruHiWatt_State_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        entity->state = HotaruHiWatt_State_Unknown6;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Unknown6(void)
{
    RSDK_THIS(HotaruHiWatt);

    entity->velocity.y -= 0x3800;
    entity->field_80.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    HotaruHiWatt_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->position.x = HotaruHiWatt->boundsM;
        entity->position.y = HotaruHiWatt->value7;
        entity->state      = entity->nextState;
        entity->visible    = false;

        EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_1), entity->position.x, entity->position.y);
        child->isPermanent        = true;
        child->parents[0]         = (Entity *)entity;
    }
}

void HotaruHiWatt_State1_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_5), entity->position.x, entity->position.y);

    if (!((entity->position.x ^ entity->field_88.x) & 0xFFF80000)) {
        if (!((entity->position.y ^ entity->field_88.y) & 0xFFF80000)) {
            entity->position.x = entity->field_88.x;
            entity->position.y = entity->field_88.y;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            if (!entity->nextState) {
                destroyEntity(entity);
            }
            else {
                RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 255);
                entity->state = entity->nextState;
            }
        }
    }
}

void HotaruHiWatt_State5_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->alpha -= 8;
    if (entity->alpha <= 0)
        destroyEntity(entity);
}

void HotaruHiWatt_StateUnknown_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->timer >= 192)
        entity->state = StateMachine_None;
    else
        entity->timer += 8;
    RSDK.SetLimitedFade(0, 1, 2, entity->timer, 128, 192);
}

void HotaruHiWatt_StateUnknown_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->timer <= 0)
        entity->state = StateMachine_None;
    else
        entity->timer -= 8;
    RSDK.SetLimitedFade(0, 1, 2, entity->timer, 128, 192);
}

void HotaruHiWatt_State1_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;

    if (entity->timer2 >= 256) {
        entity->field_88.x = HotaruHiWatt->boundsM + RSDK.Rand(-0x800000, 0x800000);
        entity->field_88.y = HotaruHiWatt->value7 + RSDK.Rand(-0x500000, 0x500000);
        entity->stateDraw  = HotaruHiWatt_StateDraw1_Unknown2;
        entity->velocity.x = (entity->field_88.x - entity->position.x) / 48;
        entity->velocity.y = (entity->field_88.y - entity->position.y) / 48;
        RSDK.PlaySfx(HotaruHiWatt->sfxFly, false, 255);
        entity->nextState = HotaruHiWatt_State1_Unknown3;
        entity->state     = HotaruHiWatt_State1_Unknown2;
    }
    else {
        entity->timer2 += 16;
    }
}

void HotaruHiWatt_State1_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown2)
        parent->state = HotaruHiWatt_StateUnknown_Unknown2;
    RSDK.ProcessAnimation(&entity->animator3);
    if (!entity->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator3, true, 0);
    }

    entity->alpha = 8 * ++entity->timer;
    if (entity->timer == 32) {
        entity->timer     = 0;
        entity->stateDraw = HotaruHiWatt_StateDraw1_Unknown1;
        RSDK.PlaySfx(HotaruHiWatt->sfxLaser, false, 255);
        entity->state = HotaruHiWatt_State1_Unknown4;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator3);
    ++entity->timer;
    if (!(entity->timer & 1))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_MINILASER), entity->position.x, entity->position.y + 0x40000);
    if (entity->timer == 24) {
        entity->timer     = 0;
        entity->stateDraw = HotaruHiWatt_StateDraw1_Unknown2;
        entity->state     = HotaruHiWatt_State1_Unknown5;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;
    ++entity->timer;

    if (entity->alpha <= 0) {
        if (entity->health >= 1) {
            entity->timer = 0;
            entity->health--;
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator1, true, 3);
            entity->stateDraw = StateMachine_None;
            entity->state     = HotaruHiWatt_State1_Unknown1;
        }
        else {
            entity->state = HotaruHiWatt_State1_Unknown6;
        }
    }
    else {
        entity->alpha -= 8;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown6(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->timer2 <= 0) {
        HotaruHiWatt_SpawnChildHotaru();
        destroyEntity(entity);
    }
    else {
        entity->timer2 -= 32;
        HotaruHiWatt_CheckPlayerCollisions2();
    }
}

void HotaruHiWatt_State7_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    entity->position.y += 0x80000;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (HotaruHiWatt->value8) {
            destroyEntity(entity);
        }
        else {
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 3, &entity->animator1, true, 0);
            entity->position.y += 0x80000;
            entity->drawOrder    = Zone->drawOrderHigh;
            entity->state        = HotaruHiWatt_State7_Unknown2;
            HotaruHiWatt->value8 = true;
        }
    }

    if (entity->objectID == HotaruHiWatt->objectID)
        HotaruHiWatt_CheckPlayerCollisions3();
}

void HotaruHiWatt_State7_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == 2)
        HotaruHiWatt->value8 = false;
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void HotaruHiWatt_State2_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;

    entity->position.x = entity->field_74 * RSDK.Cos1024(entity->angle >> 14) + entity->field_80.x;
    entity->position.y = entity->field_74 * RSDK.Sin1024(entity->angle >> 14) + entity->field_80.y;

    if (entity->alpha >= 256)
        entity->state = HotaruHiWatt_State2_Unknown2;
    else
        entity->alpha += 16;
}

void HotaruHiWatt_State2_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (entity->timer >= 64) {
        if (entity->velocity.x > 0)
            entity->velocity.x -= 0x2000;
    }
    else {
        if (entity->velocity.x < 0x80000)
            entity->velocity.x += 0x2000;
    }

    if (entity->field_80.x < HotaruHiWatt->boundsM - 0x600000)
        entity->field_80.x += 0x20000;
    else if (entity->field_80.x > HotaruHiWatt->boundsM + 0x600000)
        entity->field_80.x -= 0x20000;

    entity->field_80.y -= entity->velocity.x >> 2;
    entity->field_74 += entity->velocity.x >> 13;
    if (!(entity->timer % 30) && entity->timer < 120)
        RSDK.PlaySfx(HotaruHiWatt->sfxFly, (entity->timer % 30), 255);
    entity->angle += entity->velocity.x;

    ++entity->timer;
    entity->position.x = entity->field_74 * RSDK.Cos1024(entity->angle >> 14) + entity->field_80.x;
    entity->position.y = entity->field_74 * RSDK.Sin1024(entity->angle >> 14) + entity->field_80.y;
    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_5), entity->position.x, entity->position.y);
    if (entity->timer == 128) {
        entity->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 255);
        entity->state = HotaruHiWatt_State2_Unknown3;
    }
}

void HotaruHiWatt_State2_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)entity->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown2)
        parent->state = HotaruHiWatt_StateUnknown_Unknown2;
    RSDK.ProcessAnimation(&entity->animator3);
    if (!entity->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &entity->animator3, true, 0);
    }

    entity->alpha = 8 * ++entity->timer;
    if (entity->timer == 32) {
        entity->timer      = 0;
        entity->velocity.y = 0;
        entity->stateDraw  = HotaruHiWatt_StateDraw1_Unknown1;
        entity->state      = HotaruHiWatt_State2_Unknown4;
        if (entity->parents[1]) {
            for (int i = 30; i < 254; i += 28) {
                EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_ELECTRICORB), entity->position.x, entity->position.y);
                child->scale.x            = i;
                child->angle              = RSDK.Rand(0, 256);
                child->parents[1]         = (Entity *)entity;
                child->parents[2]         = entity->parents[1];
            }
        }
    }
}

void HotaruHiWatt_State2_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++entity->timer == 32) {
        entity->timer = 0;
        entity->state = HotaruHiWatt_State2_Unknown5;
    }
}

void HotaruHiWatt_State2_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator3);
    entity->velocity.y += 0x800;
    if (entity->velocity.y > 0x20000) {
        entity->velocity.y = 0x20000;
        if (entity->position.y > HotaruHiWatt->value7 + 0x140000)
            entity->state = HotaruHiWatt_State2_Unknown7;
    }
    entity->position.y += entity->velocity.y;
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State2_Unknown7(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator3);
    entity->velocity.y -= 0x1000;
    if (entity->velocity.y < -0x20000)
        entity->velocity.y = -0x20000;
    entity->position.y += entity->velocity.y;
    if (entity->position.y < HotaruHiWatt->value7 - 0xC00000) {
        HotaruHiWatt_Unknown16();
        destroyEntity(entity);
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State8_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator1);
    EntityHotaruHiWatt *parent  = (EntityHotaruHiWatt *)entity->parents[1];
    EntityHotaruHiWatt *parent2 = (EntityHotaruHiWatt *)entity->parents[2];

    if (parent->objectID == HotaruHiWatt->objectID && parent2->objectID == HotaruHiWatt->objectID) {
        int scale = (RSDK.Sin256(entity->angle + 4 * Zone->timer) >> 7) + entity->scale.x;

        entity->position.x = parent->position.x + scale * ((parent2->position.x - parent->position.x) >> 8);
        entity->position.y = parent->position.y + 0xC0000 + scale * ((parent2->position.y - parent->position.y) >> 8);
        entity->position.y += RSDK.Sin256(entity->angle + 4 * Zone->timer) << 9;
        HotaruHiWatt_CheckPlayerCollisions3();
    }
    else {
        entity->alpha      = 0x100;
        entity->inkEffect  = INK_ALPHA;
        entity->state      = HotaruHiWatt_State8_Unknown2;
        entity->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        entity->velocity.y = RSDK.Rand(-0x20000, 0x20000);
    }
}

void HotaruHiWatt_State8_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 8;
}

void HotaruHiWatt_State_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround) {
        if (entity->timer >= 192) {
            if (!entity->field_A4) {
                Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
                entity->field_A4 = 1;
            }
            entity->state = HotaruHiWatt_State_Unknown2;
            RSDK.PlaySfx(HotaruHiWatt->sfxHHWAppear, false, 255);
        }
        else {
            entity->timer += 8;
        }
        RSDK.SetLimitedFade(0, 1, 2, entity->timer, 128, 192);
    }
}

void HotaruHiWatt_State_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++entity->timer == 224) {

        for (int angle = 0x20; angle < 0x120; angle += 0x40) {
            EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_4), entity->position.x, entity->position.y - 0xC0000);
            child->position.x += RSDK.Cos256(angle) << 16;
            child->position.y += RSDK.Sin256(angle) << 16;
            child->velocity.x = (child->field_88.x - child->position.x) >> 6;
            child->velocity.y = (child->field_88.y - child->position.y) >> 6;
        }

        entity->timer      = 0;
        entity->velocity.x = 0x50000;
        entity->stateDraw  = HotaruHiWatt_StateDraw_Unknown3;
        entity->visible    = true;
        entity->state      = HotaruHiWatt_State_Unknown3;
    }
}

void HotaruHiWatt_State_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++entity->timer >= 60) {
        entity->velocity.x -= 0x3800;
        entity->field_74 += entity->velocity.x;

        if (entity->velocity.x <= 0) {
            entity->field_80.x = entity->position.x;
            entity->field_80.y = entity->position.y;
            entity->timer      = 192;
            entity->stateDraw  = HotaruHiWatt_StateDraw_Unknown4;
            entity->state      = HotaruHiWatt_State_Unknown4;
            RSDK.StopSFX(HotaruHiWatt->sfxHHWAppear);
            RSDK.PlaySfx(HotaruHiWatt->sfxFlash, false, 255);
            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
            fxFade->speedIn      = 128;
            fxFade->speedOut     = 16;
        }
    }
}

void HotaruHiWatt_State_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    entity->velocity.x -= 0x3800;
    entity->field_74 += entity->velocity.x;

    if (entity->timer > 0)
        entity->timer -= 8;
    RSDK.SetLimitedFade(0, 1, 2, entity->timer, 128, 192);

    if (entity->alpha > 0) {
        entity->alpha -= 8;
    }
    if (entity->field_74 <= 0) {
        entity->stateDraw = HotaruHiWatt_StateDraw_Destroyed;
        entity->state     = entity->nextState;
    }
}

void HotaruHiWatt_State_Unknown7(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    if (++entity->timer >= 24) {
        entity->timer     = 0;
        entity->alpha     = 0;
        entity->stateDraw = HotaruHiWatt_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 2, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 7, &entity->animator1, true, 0);
        RSDK.PlaySfx(HotaruHiWatt->sfxCharge, false, 255);
        entity->state = HotaruHiWatt_State_Unknown8;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Unknown8(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    HotaruHiWatt_CheckPlayerCollisions();
    if (entity->state == HotaruHiWatt_State_Unknown8) {
        if (entity->alpha < 256)
            entity->alpha += 4;

        if (++entity->timer >= 64) {
            entity->timer      = 0;
            entity->velocity.x = 0;
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &entity->animator4, true, 0);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &entity->animator1, true, 2);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 4, &entity->animator5, true, 0);
            entity->stateDraw = HotaruHiWatt_StateDraw_Unknown2;
            RSDK.StopSFX(HotaruHiWatt->sfxCharge);

            if (entity->position.x < HotaruHiWatt->boundsM)
                entity->state = HotaruHiWatt_State_Unknown10;
            else
                entity->state = HotaruHiWatt_State_Unknown9;
        }
    }
}

void HotaruHiWatt_State_Unknown9(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator5);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    entity->velocity.x -= 0x1000;
    if (entity->velocity.x < -0x20000)
        entity->velocity.x = -0x20000;
    entity->position.x += entity->velocity.x;

    if (entity->position.x <= HotaruHiWatt->boundsL) {
        entity->position.x = HotaruHiWatt->boundsL;
        entity->state      = HotaruHiWatt_State_Unknown11;
    }
    int x = entity->position.x;
    int y = entity->position.y;
    entity->position.y += 0x800000;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, 32);
    entity->drawPos.x  = entity->position.x;
    entity->drawPos.y  = entity->position.y;
    entity->position.x = x;
    entity->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions4();
}

void HotaruHiWatt_State_Unknown10(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator5);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    entity->velocity.x += 4096;
    if (entity->velocity.x > 0x20000)
        entity->velocity.x = 0x20000;
    entity->position.x += entity->velocity.x;

    if (entity->position.x >= HotaruHiWatt->boundsR) {
        entity->position.x = HotaruHiWatt->boundsR;
        entity->state      = HotaruHiWatt_State_Unknown11;
    }
    int x = entity->position.x;
    int y = entity->position.y;
    entity->position.y += 0x800000;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, 32);
    entity->drawPos.x  = entity->position.x;
    entity->drawPos.y  = entity->position.y;
    entity->position.x = x;
    entity->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions4();
}

void HotaruHiWatt_State_Unknown11(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80.y) & 0xFFFF0000;

    if (entity->alpha > 0)
        entity->alpha -= 4;
    if (++entity->timer >= 64) {
        entity->drawPos.y += 0x800000;
        entity->timer      = 0;
        entity->stateDraw  = HotaruHiWatt_StateDraw_Destroyed;
        entity->velocity.y = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        entity->state = HotaruHiWatt_State_Unknown6;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Destroyed(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(HotaruHiWatt->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 60) {
        Debris_FallFlickerAnimSetup(HotaruHiWatt->aniFrames, HotaruHiWatt->debrisInfo, 8);
        entity->visible = false;
    }
    else if (entity->timer == 90) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->timer   = 0;
        entity->state   = HotaruHiWatt_State_Finished;
        entity->visible = false;
    }
}

void HotaruHiWatt_State_Finished(void)
{
    RSDK_THIS(HotaruHiWatt);
    if (++entity->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        }
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void HotaruHiWatt_EditorDraw(void)
{
    RSDK_THIS(HotaruHiWatt);
    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &entity->animator3, false, 0);
}

void HotaruHiWatt_EditorLoad(void)
{
    HotaruHiWatt->aniFrames    = RSDK.LoadSpriteAnimation("SSZ1/HotaruHiWatt.bin", SCOPE_STAGE);
    HotaruHiWatt->hotaruFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruHiWatt_Serialize(void) {}
