#include "SonicMania.h"

ObjectERZMystic *ERZMystic;

void ERZMystic_Update(void)
{
    RSDK_THIS(ERZMystic);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator1);
}

void ERZMystic_LateUpdate(void) {}

void ERZMystic_StaticUpdate(void) {}

void ERZMystic_Draw(void)
{
    RSDK_THIS(ERZMystic);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void ERZMystic_Create(void *data)
{
    RSDK_THIS(ERZMystic);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->field_90      = 0;
        entity->field_94      = 1;
        entity->field_98      = 2;
        entity->cupAlpha[0]   = 0xC0;
        entity->cupAlpha[1]   = 0x00;
        entity->cupAlpha[2]   = 0xC0;
        entity->hitbox.left   = -12;
        entity->hitbox.top    = -12;
        entity->hitbox.right  = 12;
        entity->hitbox.bottom = 12;
        entity->correctCup    = 1;
        entity->state         = ERZMystic_State_Unknown1;
        entity->stateDraw     = ERZMystic_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &entity->animator3, true, 1);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &entity->animator4, true, 0);
    }
}

void ERZMystic_StageLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }

void ERZMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZMystic);

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->mysticPos.x;
    entity->position.y = entity->mysticPos.y;

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            entity->invincibilityTimer = 48;
            RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
        }

        if (entity->animator5.frameID > 8 && entity->animator5.frameID < 26) {
            for (int i = 0; i < 3; ++i) {
                if (i != entity->correctCup) {
                    entity->position.x = storeX + entity->cupPos[i];
                    if (abs(entity->position.x - player->position.x) < 0x400000 && player->position.y > entity->position.y)
                        Player_CheckHit(player, entity);
                }
            }
        }

        entity->position.x = entity->mysticPos.x;
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void ERZMystic_HandleCups(void)
{
    RSDK_THIS(ERZMystic);

    entity->field_B4 = RSDK.Rand(0, 3);

    switch (entity->field_B4) {
        case 0: entity->field_B8 = (RSDK.Rand(0, 256) > 128) + 1; break;
        case 1: entity->field_B8 = RSDK.Rand(0, 256) <= 128 ? 2 : 0; break;
        case 2: entity->field_B8 = RSDK.Rand(0, 256) <= 128; break;
    }

    entity->field_BC = entity->cupPos[entity->field_B4];
    entity->field_C4 = entity->cupAlpha[entity->field_B4];

    entity->field_C0 = entity->cupPos[entity->field_B8];
    entity->field_C8 = entity->cupAlpha[entity->field_B8];
}

void ERZMystic_StateDraw_Unknown1(void)
{
    RSDK_THIS(ERZMystic);
    Vector2 drawPos;

    RSDK.DrawSprite(&entity->animator1, &entity->mysticPos, false);
    for (int i = 0; i < 3; ++i) {
        drawPos.x = entity->position.x + entity->cupPos[i];
        if (i == 1) {
            drawPos.y      = entity->cupY;
            entity->drawFX = FX_ROTATE | FX_FLIP;
        }
        else {
            drawPos.y = entity->position.y;
        }

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->inkEffect = INK_ALPHA;
        entity->drawFX    = FX_FLIP;
        entity->alpha     = entity->cupAlpha[i];
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->inkEffect = INK_NONE;
    }
}

void ERZMystic_StateDraw_Unknown2(void)
{
    RSDK_THIS(ERZMystic);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);
        RSDK.DrawSprite(&entity->animator1, &entity->mysticPos, false);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, &entity->mysticPos, false);
    }

    for (int i = 0; i < 3; ++i) {
        drawPos.x         = entity->position.x + entity->cupPos[i];
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->inkEffect = INK_ALPHA;
        entity->alpha     = entity->cupAlpha[i];
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->inkEffect = INK_NONE;
        if (i != entity->correctCup)
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }
}

void ERZMystic_State_Unknown1(void)
{
    RSDK_THIS(ERZMystic);

    entity->mysticPos.x = entity->position.x;
    entity->velocity.y  = 0x40000;
    entity->field_70    = -0x80000;
    entity->mysticPos.y = entity->position.y;
    entity->cupY        = entity->position.y;
    entity->state       = ERZMystic_State_Unknown2;
}

void ERZMystic_State_Unknown2(void)
{
    RSDK_THIS(ERZMystic);

    entity->field_70 += 0x3800;
    entity->mysticPos.y += entity->field_70;
    entity->velocity.y -= 0x2800;
    entity->cupY += entity->velocity.y;

    if (entity->cupY <= entity->position.y) {
        entity->velocity.y = 0;
        entity->cupY       = entity->position.y;
    }

    entity->cupPos[0] += (-0x800000 - entity->cupPos[0]) >> 4;
    entity->cupPos[2] += ((0x800000 - entity->cupPos[2]) >> 4);

    if (entity->rotation < 256) {
        entity->rotation += 16;
    }

    if (++entity->timer == 60) {
        entity->cupPos[0] = -0x800000;
        entity->cupPos[2] = 0x800000;
        entity->timer     = 0;
        entity->state     = ERZMystic_State_Unknown3;
    }
}

void ERZMystic_State_Unknown3(void)
{
    RSDK_THIS(ERZMystic);

    entity->field_70 += 0x3800;
    entity->mysticPos.y += entity->field_70;
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x380000, true))
        entity->velocity.y = 0;

    if (entity->mysticPos.y >= entity->position.y - 0x180000) {
        entity->mysticPos.y = 0;
        entity->field_70    = 0;
        entity->onGround    = false;
        entity->state       = ERZMystic_State_Unknown4;
    }
}

void ERZMystic_State_Unknown4(void)
{
    RSDK_THIS(ERZMystic);

    entity->velocity.y += 0x4000;
    entity->position.y += entity->velocity.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x380000, true)) {
        if (!entity->onGround)
            Camera_ShakeScreen(0, 0, 4);
        entity->velocity.y = 0;
        entity->onGround   = true;
    }

    if (entity->rotation >= 512) {
        entity->rotation = 0;
        entity->state    = ERZMystic_State_Unknown5;
    }
    else {
        entity->rotation += 16;
    }
}

void ERZMystic_State_Unknown5(void)
{
    RSDK_THIS(ERZMystic);

    entity->velocity.y += 0x4000;
    entity->cupY += entity->velocity.y;

    if (entity->cupY >= entity->position.y) {
        Camera_ShakeScreen(0, 0, 4);
        entity->velocity.y = 0;
        entity->cupY       = entity->position.y;
        entity->stateDraw  = ERZMystic_StateDraw_Unknown2;
        entity->state      = ERZMystic_State_Unknown6;
    }
}

void ERZMystic_State_Unknown6(void)
{
    RSDK_THIS(ERZMystic);

    ++entity->timer;
    if (entity->cupAlpha[0] < 144) {
        entity->cupAlpha[0] += 4;
        entity->cupAlpha[2] += 4;
    }

    if (entity->timer == 30) {
        entity->timer  = 0;
        entity->timer2 = 6;
        ERZMystic_HandleCups();
        entity->state = ERZMystic_State_Unknown7;
    }
}

void ERZMystic_State_Unknown7(void)
{
    RSDK_THIS(ERZMystic);

    if (abs(entity->field_C0 - entity->field_BC) <= 0x800000)
        entity->timer += 16;
    else
        entity->timer += 8;

    int cup1 = entity->field_B4;
    int cup2 = entity->field_B8;

    int pos1 = entity->field_BC;
    if (entity->timer > 0) {
        if (entity->timer < 256)
            pos1 += ((entity->field_C0 - pos1) >> 8) * ((RSDK.Sin512(entity->timer + 384) >> 2) + 128);
        else
            pos1 = entity->field_C0;
    }
    entity->cupPos[cup1] = pos1;

    int pos2 = entity->field_C0;
    if (entity->timer > 0) {
        if (entity->timer < 256) {
            pos2 += ((entity->field_BC - pos2) >> 8) * ((RSDK.Sin512(entity->timer + 384) >> 2) + 128);
        }
        else {
            pos2 = entity->field_BC;
        }
    }
    entity->cupPos[cup2] = pos2;

    entity->cupAlpha[cup1] = abs(entity->cupPos[cup1]) / 0xE38E;
    entity->cupAlpha[cup2] = abs(entity->cupPos[cup2]) / 0xE38E;
    if (entity->timer == 0x100) {
        entity->timer = 0;
        if (entity->timer2 <= 1) {
            entity->cupY -= 0x600000;
            entity->mysticPos.x = entity->position.x + entity->cupPos[entity->correctCup];
            entity->mysticPos.y = entity->position.y;
            entity->field_60.x  = entity->mysticPos.x;
            entity->field_60.y  = entity->mysticPos.y;
            entity->velocity.y  = -0x10000;
            entity->state       = ERZMystic_State_Unknown8;
        }
        else {
            entity->timer2--;
            ERZMystic_HandleCups();
        }
    }
}

void ERZMystic_State_Unknown8(void)
{
    RSDK_THIS(ERZMystic);

    entity->velocity.y -= 0x3800;
    entity->position.y += entity->velocity.y;
    entity->cupAlpha[0] -= entity->cupAlpha[0] >> 4;
    entity->cupAlpha[1] -= entity->cupAlpha[1] >> 4;
    entity->cupAlpha[2] -= entity->cupAlpha[2] >> 4;
    if (entity->position.y <= entity->cupY) {
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 3, &entity->animator5, true, 0);
        entity->velocity.y = 0;
        entity->position.y = entity->cupY;
        entity->state      = ERZMystic_State_Unknown9;
    }

    entity->angle       = (entity->angle + 4) & 0xFF;
    entity->mysticPos.y = (RSDK.Sin256(entity->angle) << 11) + entity->field_60.y;

    if (entity->position.y - entity->cupY < 0x200000)
        ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_Unknown9(void)
{
    RSDK_THIS(ERZMystic);

    RSDK.ProcessAnimation(&entity->animator5);
    entity->cupAlpha[0] -= entity->cupAlpha[0] >> 4;
    entity->cupAlpha[1] -= entity->cupAlpha[1] >> 4;
    entity->cupAlpha[2] -= entity->cupAlpha[2] >> 4;
    if (entity->animator5.frameID == entity->animator5.frameCount - 1) {
        entity->field_70 = 0;
        entity->state    = ERZMystic_State_Unknown10;
    }

    entity->angle       = (entity->angle + 4) & 0xFF;
    entity->mysticPos.y = (RSDK.Sin256(entity->angle) << 11) + entity->field_60.y;
    ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_Unknown10(void)
{
    RSDK_THIS(ERZMystic);

    entity->cupPos[0] += ((entity->cupPos[entity->correctCup] - entity->cupPos[0]) >> 3);
    entity->cupPos[1] += ((entity->cupPos[entity->correctCup] - entity->cupPos[1]) >> 3);
    entity->cupPos[2] += ((entity->cupPos[entity->correctCup] - entity->cupPos[2]) >> 3);
    entity->field_70 -= 0x3000;
    entity->mysticPos.y += entity->field_70;

    if (entity->mysticPos.y < entity->position.y)
        entity->mysticPos.y = entity->position.y;

    ERZMystic_CheckPlayerCollisions();
    if (++entity->timer == 60) {
        foreach_all(ERZKing, king)
        {
            king->active     = ACTIVE_NORMAL;
            king->position.x = entity->mysticPos.x;
        }
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void ERZMystic_EditorDraw(void)
{
    RSDK_THIS(ERZMystic);
    entity->mysticPos   = entity->position;
    entity->cupY        = entity->position.y;
    entity->cupAlpha[0] = 0x60;
    entity->cupAlpha[1] = 0;
    entity->cupAlpha[2] = 0x60;
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &entity->animator3, true, 1);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &entity->animator4, true, 0);

    ERZMystic_StateDraw_Unknown1();
}

void ERZMystic_EditorLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }
#endif

void ERZMystic_Serialize(void) {}
