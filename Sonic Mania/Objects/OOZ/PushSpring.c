#include "SonicMania.h"

ObjectPushSpring *PushSpring;

void PushSpring_Update(void)
{
    RSDK_THIS(PushSpring);
    StateMachine_Run(entity->state);
    entity->field_78 = false;
    StateMachine_Run(entity->stateLate);
}

void PushSpring_LateUpdate(void) {}

void PushSpring_StaticUpdate(void) {}

void PushSpring_Draw(void)
{
    RSDK_THIS(PushSpring);
    if (entity->pushOffset) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        entity->animator.frameID = 0;
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void PushSpring_Create(void *data)
{
    RSDK_THIS(PushSpring);
    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PushSpring->aniFrames, entity->type, &entity->animator, true, 0);
        if (entity->type) {
            if (entity->direction) {
                entity->stateDraw = PushSpring_StateDraw_Left;
                entity->stateLate = PushSpring_HandlePlayerCollisions_Left;
            }
            else {
                entity->stateDraw = PushSpring_StateDraw_Right;
                entity->stateLate = PushSpring_HandlePlayerCollisions_Right;
            }
            entity->groundVel     = 0x4000;
            entity->hitbox.left   = -20;
            entity->hitbox.top    = -16;
            entity->hitbox.right  = 20;
            entity->hitbox.bottom = 16;
            entity->state         = PushSpring_Unknown9;
        }
        else {
            if (entity->direction) {
                entity->direction = FLIP_Y;
                entity->stateDraw = PushSpring_StateDraw_Bottom;
                entity->stateLate = PushSpring_HandlePlayerCollisions_Bottom;
            }
            else {
                entity->stateDraw = PushSpring_StateDraw_Top;
                entity->stateLate = PushSpring_HandlePlayerCollisions_Top;
            }
            entity->groundVel     = 0x20000;
            entity->hitbox.left   = -16;
            entity->hitbox.top    = -20;
            entity->hitbox.right  = 16;
            entity->hitbox.bottom = 20;
            entity->state         = PushSpring_Unknown9;
        }
    }
}

void PushSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE);
    PushSpring->sfxPush   = RSDK.GetSFX("Stage/Push.wav");
    PushSpring->sfxSpring = RSDK.GetSFX("Global/Spring.wav");
}

void PushSpring_HandlePlayerCollisions_Top(void)
{
    RSDK_THIS(PushSpring);
    entity->hitbox.top = (entity->pushOffset >> 16) - 22;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_TOP) {
            player->position.y += 0x20000;
            entity->field_78 |= 1;
            if (entity->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = -0xA0000;
                int32 anim              = player->playerAnimator.animationID;
                if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                    player->storedAnim = player->playerAnimator.animationID;
                else
                    player->storedAnim = ANI_WALK;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                entity->state = PushSpring_Unknown11;
            }
        }
    }
}

void PushSpring_HandlePlayerCollisions_Bottom(void)
{
    RSDK_THIS(PushSpring);
    entity->hitbox.top = (entity->pushOffset >> 16) - 22;

    foreach_active(Player, player)
    {
        int32 yvel = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_BOTTOM) {
            if (yvel < 0) {
                player->velocity.y = yvel + 0x3800;
                player->position.y -= 0x20000;
                entity->field_78 |= 1;
            }
            if (entity->pushOffset >= 0x120000) {
                player->collisionMode = 0;
                player->onGround      = false;
                player->state         = Player_State_Air;
                player->velocity.y    = 0xA0000;
                RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                entity->state = PushSpring_Unknown11;
            }
        }
    }
}

void PushSpring_HandlePlayerCollisions_Left(void)
{
    RSDK_THIS(PushSpring);
    entity->hitbox.right = 22 - (entity->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_LEFT) {
            if (player->direction == FLIP_NONE)
                player->position.x += 0x20000;
            if (player->right == true)
                entity->field_78 |= 1u;
            if (entity->state == PushSpring_Unknown11) {
                if (entity->pushOffset > 0x10000) {
                    player->groundVel     = -12 * entity->pushOffset / 18;
                    player->velocity.x    = player->groundVel;
                    player->collisionMode = 0;
                    player->controlLock   = 16;
                    player->pushing       = false;
                    player->direction     = entity->direction;
                    player->state         = Player_State_Ground;
                    RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                }
            }
        }
    }
}

void PushSpring_HandlePlayerCollisions_Right(void)
{
    RSDK_THIS(PushSpring);
    entity->hitbox.right = 22 - (entity->pushOffset >> 16);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_RIGHT) {
            if (player->direction == FLIP_X)
                player->position.x -= 0x20000;
            if (player->left == true)
                entity->field_78 |= 1;
            if (entity->state == PushSpring_Unknown11) {
                if (entity->pushOffset > 0x10000) {
                    player->groundVel     = 12 * entity->pushOffset / 18;
                    player->velocity.x    = player->groundVel;
                    player->collisionMode = 0;
                    player->controlLock   = 16;
                    player->pushing       = false;
                    player->direction     = entity->direction;
                    player->state         = Player_State_Ground;
                    RSDK.PlaySfx(PushSpring->sfxSpring, false, 255);
                }
            }
        }
    }
}

void PushSpring_StateDraw_Top(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y += entity->pushOffset;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.y                = entity->position.y + 2 * entity->pushOffset / 3;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.y                = entity->position.y + entity->pushOffset / 3;
    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 4;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PushSpring_StateDraw_Bottom(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->pushOffset;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
    drawPos.y                = entity->position.y - 2 * entity->pushOffset / 3;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
    drawPos.y                = entity->position.y - entity->pushOffset / 3;
    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
    entity->animator.frameID = 4;
    RSDK.DrawSprite(&entity->animator, 0, false);
}

void PushSpring_StateDraw_Left(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->pushOffset;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x                = entity->position.x + 2 * entity->pushOffset / 3;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x                = entity->position.x + entity->pushOffset / 3;
    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 4;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PushSpring_StateDraw_Right(void)
{
    RSDK_THIS(PushSpring);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->pushOffset;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x                = entity->position.x - 2 * entity->pushOffset / 3;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x                = entity->position.x - entity->pushOffset / 3;
    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 4;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PushSpring_Unknown9(void)
{
    RSDK_THIS(PushSpring);
    if (entity->field_78) {
        entity->pushTimer = 0;
        entity->state     = PushSpring_Unknown10;
    }
}

void PushSpring_Unknown10(void)
{
    RSDK_THIS(PushSpring);
    if (entity->field_78) {
        entity->pushOffset += entity->groundVel;
        if (entity->pushOffset < 0x120000) {
            if (!(entity->pushTimer % 10))
                RSDK.PlaySfx(PushSpring->sfxPush, false, 255);
            ++entity->pushTimer;
        }
        else {
            ++entity->pushTimer;
            entity->pushOffset = 0x120000;
        }
    }
    else {
        entity->state = PushSpring_Unknown11;
    }
}

void PushSpring_Unknown11(void)
{
    RSDK_THIS(PushSpring);
    if (entity->field_78) {
        entity->pushTimer = 0;
        entity->state     = PushSpring_Unknown10;
    }
    else {
        entity->pushOffset -= 0x20000;
        if (entity->pushOffset <= 0) {
            entity->pushOffset = 0;
            entity->state      = PushSpring_Unknown9;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PushSpring_EditorDraw(void)
{
    RSDK_THIS(PushSpring);

    int dir = entity->direction;

    RSDK.SetSpriteAnimation(PushSpring->aniFrames, entity->type, &entity->animator, false, 0);
    if (!entity->type  &&entity->direction) {
        entity->direction = FLIP_Y;
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->direction = dir;
}

void PushSpring_EditorLoad(void) { PushSpring->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PushSpring.bin", SCOPE_STAGE); }
#endif

void PushSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(PushSpring, VAR_UINT8, direction);
}
