#include "SonicMania.h"

ObjectClapperboard *Clapperboard;

void Clapperboard_Update(void)
{
    RSDK_THIS(Clapperboard);

    StateMachine_Run(entity->state);
    entity->rotation = entity->angle >> 1;
    entity->field_68 = 0;
    StateMachine_Run(entity->stateCollide);
}

void Clapperboard_LateUpdate(void) {}

void Clapperboard_StaticUpdate(void) {}

void Clapperboard_Draw(void)
{
    RSDK_THIS(Clapperboard);
    Vector2 drawPos;

    entity->drawFX = FX_ROTATE | FX_FLIP;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->direction == FLIP_X)
        drawPos.x -= 0x670000;
    else
        drawPos.x += 0x10000;
    entity->animator4.frameID = entity->takeID / 100;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    entity->animator4.frameID = (entity->takeID / 10) % 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    entity->animator4.frameID = entity->takeID % 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0x120000;
    entity->animator4.frameID = RSDK_sceneInfo->minutes % 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0x120000;
    entity->animator4.frameID = RSDK_sceneInfo->seconds / 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    entity->animator4.frameID = RSDK_sceneInfo->seconds % 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0x110000;
    entity->animator4.frameID = RSDK_sceneInfo->milliseconds / 10;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);
}

void Clapperboard_Create(void *data)
{
    RSDK_THIS(Clapperboard);

    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &entity->animator2, true, 1);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &entity->animator3, true, 2);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 1, &entity->animator4, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (entity->direction == FLIP_NONE) {
            entity->dword74      = -0x40000;
            entity->dword7C      = 0x6C0000;
            entity->angle        = 960;
            entity->stateCollide = Clapperboard_StateCollide_FlipX;
        }
        else {
            ++entity->animator3.frameID;
            entity->dword74      = -0x6C0000;
            entity->dword7C      = 0x40000;
            entity->angle        = 64;
            entity->stateCollide = Clapperboard_StateCollide_NoFlip;
        }
        entity->dword78   = -0x40000;
        entity->dword80   = -0x40000;
        entity->state     = Clapperboard_State_Unknown1;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
    }
}

void Clapperboard_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);
    Clapperboard->sfxWhack = RSDK.GetSFX("Stage/Whack.wav");
}

void Clapperboard_StateCollide_NoFlip(void)
{
    RSDK_THIS(Clapperboard);

    int power    = entity->dword74 >> 8;
    int negAngle = -(-entity->angle >> 2);

    int val1 = ((entity->dword78 >> 8) * RSDK.Cos256(negAngle)) - (power * RSDK.Sin256(negAngle));
    int val2 = (entity->dword80 >> 8) * RSDK.Cos256(negAngle);
    int val3 = (entity->dword7C >> 8) * RSDK.Sin256(negAngle);

    entity->hitbox.left  = ((entity->dword78 >> 8) * RSDK.Sin256(negAngle) + power * RSDK.Cos256(negAngle)) >> 16;
    entity->hitbox.right = ((entity->dword80 >> 8) * RSDK.Sin256(negAngle) + (entity->dword7C >> 8) * RSDK.Cos256(negAngle)) >> 16;
    int sizeX            = entity->hitbox.left - entity->hitbox.right;
    entity->direction ^= FLIP_X;
    bool32 clapFlag = false;

    foreach_active(Player, player)
    {
        int dist = (player->position.x - entity->position.x) >> 16;
        if (dist >= entity->hitbox.left) {
            if (dist > entity->hitbox.right)
                dist = entity->hitbox.right;
        }
        else {
            dist = entity->hitbox.left;
        }

        int angVal            = entity->hitbox.right - dist;
        int top               = ((val2 - val3) >> 16) + (((val2 - val3) >> 16) - (val1 >> 16)) * angVal / sizeX - (entity->field_64 & 0xFFFF);
        entity->hitbox.top    = top;
        entity->hitbox.bottom = top + 24;
        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (entity->field_64 + 2) << 16;
            if (angVal > (sizeX >> 2)) {
                entity->field_68 |= RSDK.GetEntityID(player) + 1;
                if (entity->state == Clapperboard_State_Unknown3 || (abs(player->groundVel) > 0x80000 && angVal > sizeX - (sizeX >> 2))) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * angVal / sizeX;
                    if (player->playerAnimator.animationID == ANI_WALK
                        || (player->playerAnimator.animationID > ANI_AIRWALK && player->playerAnimator.animationID <= ANI_DASH))
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 255);
                    clapFlag = true;
                }
                else {
                    int val = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        val = 16 * angVal / sizeX;
                    if (val > entity->field_64)
                        entity->field_64 = val;
                }
            }
        }
    }

    entity->direction ^= FLIP_X;
    if (clapFlag == true)
        ++entity->takeID;
}

void Clapperboard_StateCollide_FlipX(void)
{
    RSDK_THIS(Clapperboard);

    int negAngle = -(-entity->angle >> 2);

    int val1 = (entity->dword78 >> 8) * RSDK.Cos256(negAngle) - (entity->dword74 >> 8) * RSDK.Sin256(negAngle);
    int val2 = (val1 >> 16) - ((((entity->dword80 >> 8) * RSDK.Cos256(negAngle)) - (entity->dword7C >> 8) * RSDK.Sin256(negAngle)) >> 16);

    entity->hitbox.left  = (((entity->dword78 >> 8) * RSDK.Sin256(negAngle)) + (entity->dword74 >> 8) * RSDK.Cos256(negAngle)) >> 16;
    entity->hitbox.right = (((entity->dword80 >> 8) * RSDK.Sin256(negAngle)) + (entity->dword7C >> 8) * RSDK.Cos256(negAngle)) >> 16;
    int Angle            = entity->hitbox.right - entity->hitbox.left;
    bool32 clapFlag      = false;

    foreach_active(Player, player)
    {
        int dist = (player->position.x - entity->position.x) >> 16;
        if (dist >= entity->hitbox.left) {
            if (dist > entity->hitbox.right)
                dist = entity->hitbox.right;
        }
        else {
            dist = entity->hitbox.left;
        }

        int angVal            = dist - entity->hitbox.left;
        int top               = (val1 >> 16) + val2 * angVal / Angle - (entity->field_64 & 0xFFFF);
        entity->hitbox.top    = top;
        entity->hitbox.bottom = top + 24;

        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (entity->field_64 + 2) << 16;
            if (angVal > Angle >> 2) {
                entity->field_68 |= RSDK.GetEntityID(player) + 1;
                if (entity->state == Clapperboard_State_Unknown5 || abs(player->groundVel) > 0x80000 && angVal > Angle - (Angle >> 2)) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * angVal / Angle;
                    if (player->playerAnimator.animationID == ANI_WALK
                        || (player->playerAnimator.animationID > ANI_AIRWALK && player->playerAnimator.animationID <= ANI_DASH))
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 255);
                    clapFlag = true;
                    clapFlag = true;
                }
                else {
                    int val = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        val = 16 * angVal / Angle;
                    if (val > entity->field_64)
                        entity->field_64 = val;
                }
            }
        }
    }
    if (clapFlag)
        ++entity->takeID;
}

void Clapperboard_State_Unknown1(void)
{
    RSDK_THIS(Clapperboard);

    if (entity->field_68 > 0) {
        if (entity->direction == FLIP_NONE)
            entity->state = Clapperboard_State_Unknown4;
        else
            entity->state = Clapperboard_State_Unknown2;
    }
}

void Clapperboard_State_Unknown2(void)
{
    RSDK_THIS(Clapperboard);

    entity->angle -= entity->field_64;
    if (entity->angle <= 0) {
        entity->angle = 0;
        entity->state = Clapperboard_State_Unknown3;
    }
}

void Clapperboard_State_Unknown3(void)
{
    RSDK_THIS(Clapperboard);

    entity->angle += entity->field_64;
    if (entity->angle >= 64) {
        entity->angle    = 64;
        entity->field_64 = 0;
        entity->state    = Clapperboard_State_Unknown1;
    }
}

void Clapperboard_State_Unknown4(void)
{
    RSDK_THIS(Clapperboard);

    entity->angle += entity->field_64;
    if (entity->angle >= 1024) {
        entity->angle = 1024;
        entity->state = Clapperboard_State_Unknown5;
    }
}

void Clapperboard_State_Unknown5(void)
{
    RSDK_THIS(Clapperboard);

    entity->angle -= entity->field_64;
    if (entity->angle <= 0x3C0) {
        entity->angle    = 0x3C0;
        entity->field_64 = 0;
        entity->state    = Clapperboard_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void Clapperboard_EditorDraw(void)
{
    RSDK_THIS(Clapperboard);
    if (entity->direction == FLIP_NONE) 
        entity->animator3.frameID = 2;
    else 
        entity->animator3.frameID = 3;

    Clapperboard_Draw();
}

void Clapperboard_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);
}
#endif

void Clapperboard_Serialize(void) { RSDK_EDITABLE_VAR(Clapperboard, VAR_UINT8, direction); }
