#include "SonicMania.h"

ObjectBarStool *BarStool;

void BarStool_Update(void)
{
    RSDK_THIS(BarStool);

    if (entity->spinSpeed <= 0) {
        if (!entity->activePlayers)
            entity->spinSpeed -= (entity->spinSpeed >> 4);
        if (entity->spinSpeed < -0x100000)
            entity->spinSpeed = -0x100000;
    }
    else {
        if (!entity->activePlayers)
            entity->spinSpeed -= (entity->spinSpeed >> 4);
        if (entity->spinSpeed > 0x100000)
            entity->spinSpeed = 0x100000;
    }

    entity->activePlayers = 0;
    if (abs(entity->spinSpeed) > 0x10000 && !(Zone->timer & 0xF))
        RSDK.PlaySfx(BarStool->sfxSpin, false, 255);

    int playerID = 0;
    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionPlatform(player, entity, &BarStool->hitbox)) {
            bool32 flag = true;
            if (abs(player->velocity.x) <= 0x20000 && player->state != Player_State_None) {
                if (abs(entity->spinSpeed) <= 0x10000)
                    flag = false;
            }
#if RETRO_USE_PLUS
            flag = flag && player->state != Player_State_MightyHammerDrop;
#endif
            if (flag) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->playerAnimator, false, 0);
                if (player->state != Player_State_None) {
                    if (velY >= 0)
                        entity->spinSpeed += player->velocity.x;
                    entity->playerVal1[playerID] = 0;
                    entity->playerVal2[playerID] = (player->position.x - entity->position.x) >> 1;
                    player->state                = Player_State_None;
                    player->nextAirState         = StateMachine_None;
                    player->nextGroundState      = StateMachine_None;
                }
                player->velocity.x = 0;
                player->groundVel  = 0;

                int ang = entity->playerVal1[playerID] >> 16;

                player->position.x = clampVal((entity->playerVal2[playerID] >> 8) * RSDK.Cos256(ang), -0x100000, 0x100000);
                ang &= 0xFF;

                int frame = (ang / 21) % 12;
                if (entity->spinSpeed <= 0)
                    player->playerAnimator.frameID = 11 - frame;
                else
                    player->playerAnimator.frameID = frame;
                player->playerAnimator.animationSpeed = 0;
                player->position.x += entity->position.x;

                if (entity->playerVal2[playerID] <= 0)
                    entity->playerVal2[playerID] -= 0x2000;
                else
                    entity->playerVal2[playerID] += 0x2000;

                entity->playerVal1[playerID] += entity->spinSpeed;

                if (abs(entity->playerVal2[playerID]) > 0x100000) {
                    int speed = entity->spinSpeed;
                    bool32 flag = true;

                    if (!player->left && !player->right) {
                        if (player->position.x <= entity->position.x)
                            speed = -abs(speed);
                        else
                            speed = abs(speed);
                    }
                    else if (player->left) {
                        if (player->position.x < entity->position.x)
                            speed = -abs(speed);
                        else
                            flag = false;
                    }
                    else {
                        if (player->position.x > entity->position.x)
                            speed = abs(speed);
                        else
                            flag = false;
                    }

                    if (flag) {
                        player->velocity.x = speed;
                        player->onGround   = false;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIRWALK, &player->playerAnimator, false, 0);
                        RSDK.PlaySfx(BarStool->sfxHop, false, 0xFF);
                    }
                }
                if (player->jumpPress && player->state == Player_State_None) {
                    int speed = entity->spinSpeed;
                    if (player->position.x <= entity->position.x) {
                        speed = -abs(speed);
                    }
                    else {
                        speed = -speed;
                    }

                    player->velocity.x = speed;
                    player->onGround   = false;
                    player->velocity.y = -0x40000;
                    player->state      = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                }

                entity->activePlayers |= 1 << playerID;
            }
        }

        playerID++;
    }

    entity->spinAngle         = (entity->spinAngle + (entity->spinSpeed >> 2)) & 0x7FFFF;
    entity->animator1.frameID = entity->spinAngle >> 16;
}

void BarStool_LateUpdate(void) {}

void BarStool_StaticUpdate(void) {}

void BarStool_Draw(void)
{
    RSDK_THIS(BarStool);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void BarStool_Create(void *data)
{
    RSDK_THIS(BarStool);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &entity->animator2, true, entity->height);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void BarStool_StageLoad(void)
{
    BarStool->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE);
    BarStool->hitbox.left   = -16;
    BarStool->hitbox.top    = -24;
    BarStool->hitbox.right  = 16;
    BarStool->hitbox.bottom = -16;
    BarStool->sfxSpin       = RSDK.GetSFX("MSZ/StoolSpin.wav");
    BarStool->sfxHop        = RSDK.GetSFX("MSZ/StoolHop.wav");
}

void BarStool_EditorDraw(void)
{
    RSDK_THIS(BarStool);
    RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &entity->animator2, true, entity->height);

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void BarStool_EditorLoad(void) { BarStool->aniFrames = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE); }

void BarStool_Serialize(void) { RSDK_EDITABLE_VAR(BarStool, VAR_UINT8, height); }
