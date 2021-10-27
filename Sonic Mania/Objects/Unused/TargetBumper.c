#include "SonicMania.h"

ObjectTargetBumper *TargetBumper = NULL;

void TargetBumper_Update(void)
{
    RSDK_THIS(TargetBumper);
    StateMachine_Run(entity->state);
}

void TargetBumper_LateUpdate(void) {}

void TargetBumper_StaticUpdate(void) {}

void TargetBumper_Draw(void)
{
    RSDK_THIS(TargetBumper);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TargetBumper_Create(void *data)
{
    RSDK_THIS(TargetBumper);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(TargetBumper->aniFrames, entity->type, &entity->animator, true, 0);
    entity->animator.frameID = entity->hitCount;
    entity->state            = TargetBumper_Unknown4;
}

void TargetBumper_StageLoad(void)
{
    TargetBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/TargetBumper.bin", SCOPE_STAGE);
    DEBUGMODE_ADD_OBJ(TargetBumper);
}

void TargetBumper_DebugSpawn(void)
{
    RSDK_THIS(TargetBumper);
    CREATE_ENTITY(TargetBumper, NULL, entity->position.x, entity->position.y);
}

void TargetBumper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TargetBumper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void TargetBumper_Unknown3(void)
{
    RSDK_THIS(TargetBumper);

    Hitbox hitbox;
    switch (entity->type) {
        case 0:
            hitbox.left          = -14;
            hitbox.top           = -4;
            hitbox.right         = 14;
            hitbox.bottom        = 4;
            break;
        case 1:
            hitbox.left          = -4;
            hitbox.top           = -14;
            hitbox.right         = 4;
            hitbox.bottom        = 14;
            break;
        case 2:
            hitbox.left   = -8;
            hitbox.top    = -8;
            hitbox.right  = 8;
            hitbox.bottom  = 8;
            break;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &hitbox) && player->playerAnimator.animationID != ANI_HURT) {
            entity->curPos = entity->startPos;
            entity->state  = TargetBumper_Unknown5;
            entity->active = ACTIVE_NORMAL;

            switch (entity->type) {
                case 0:
                    if (player->position.y <= entity->position.y) {
                        player->velocity.y = -0x70000;
                        entity->curPos.y += 0x20000;
                    }
                    else {
                        player->velocity.y = 0x70000;
                        entity->curPos.y -= 0x20000;
                    }
                    break;
                case 1:
                    if (player->position.x <= entity->position.x) {
                        player->velocity.x = -0x70000;
                        entity->curPos.x += 0x20000;
                    }
                    else {
                        player->velocity.x = 0x70000;
                        entity->curPos.x -= 0x20000;
                    }
                    break;
                case 2: {
                    int32 angle = 96;
                    if (entity->direction)
                        angle = 32;

                    int32 ang  = 0;
                    int32 ang2 = 0;
                    if (RSDK.ATan2(player->velocity.x, player->velocity.y) - angle >= 0) {
                        ang  = RSDK.ATan2(player->velocity.x, player->velocity.y) - angle;
                        ang2 = ang;
                    }
                    else {
                        ang  = RSDK.ATan2(player->velocity.x, player->velocity.y) - angle;
                        ang2 = -ang;
                    }

                    if (ang2 < 0x40) {
                        if (ang2 < 0x38) {
                            angle -= ang;
                            angle &= 0xFF;
                        }
                        if ((entity->direction & 1))
                            entity->curPos.x += 0x20000;
                        else
                            entity->curPos.x -= 0x20000;
                        entity->curPos.y += 0x20000;
                    }
                    else {
                        angle += 0x80;
                        if ((0x80 - ang2) < 0x38) {
                            angle -= ang;
                            angle &= 0xFF;
                        }
                        if ((entity->direction & 1))
                            entity->curPos.x -= 0x20000;
                        else
                            entity->curPos.x += 0x20000;
                        entity->curPos.y -= 0x20000;
                    }
                    player->velocity.x = -0x700 * RSDK.Cos256(angle);
                    player->velocity.y = -0x700 * RSDK.Sin256(angle);
                    break;
                }
            }

            if (player->state == Player_State_FlyCarried)
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

            int32 anim = player->playerAnimator.animationID;
            if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED && player->state != Player_State_TailsFlight) {
                player->state = Player_State_Air;
                if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                    player->playerAnimator.animationID = ANI_WALK;
            }
            if (player->playerAnimator.animationID != ANI_FLY)
                player->groundVel = player->velocity.x;
            player->onGround       = false;
            player->tileCollisions = true;
            if (entity->hitCount < 3) {
                entity->field_74        = 0;
                EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y);
                bonus->animator.frameID = 16;
                Player_GiveScore(player, 10);
                if (++entity->hitCount < 3)
                    entity->animator.frameID = entity->hitCount;
            }
        }
    }
}

void TargetBumper_Unknown4(void) { TargetBumper_Unknown3(); }

void TargetBumper_Unknown5(void)
{
    RSDK_THIS(TargetBumper);
    TargetBumper_Unknown3();

    if ((entity->field_74 & 4)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
    }
    else {
        entity->position.x = entity->curPos.x;
        entity->position.y = entity->curPos.y;
    }

    entity->field_74++;
    if (entity->field_74 == 12) {
        if (entity->hitCount < 3) {
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            entity->field_74   = 0;
            entity->state      = TargetBumper_Unknown4;
        }
        else {
            destroyEntity(entity);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TargetBumper_EditorDraw(void) {}

void TargetBumper_EditorLoad(void) {}
#endif

void TargetBumper_Serialize(void)
{
    RSDK_EDITABLE_VAR(TargetBumper, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TargetBumper, VAR_ENUM, hitCount);
    RSDK_EDITABLE_VAR(TargetBumper, VAR_UINT8, direction);
}
