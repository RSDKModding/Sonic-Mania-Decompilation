#include "../SonicMania.h"

ObjectStickyPlatform *StickyPlatform;

void StickyPlatform_Update(void)
{
    RSDK_THIS(StickyPlatform);
    entity->active = ACTIVE_NORMAL;
    StateMachine_Run(entity->state);
    entity->position.x = entity->amplitude.x * RSDK.Cos1024(entity->angle) + entity->internalPos.x;
    entity->position.y = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->internalPos.y;
    StickyPlatform_Interact();
    RSDK.ProcessAnimation(&entity->animator);
    int animState = entity->animator.animationID % 3;
    if (animState) {
        if (animState == 2 && entity->animator.frameID == entity->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (entity->type >> 1), &entity->animator, false, 0);
            entity->animator.animationSpeed = 0;
        }
    }
    else if (entity->animator.frameID == entity->animator.frameCount - 3) {
        RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (entity->type >> 1) + 1, &entity->animator, false, 0);
    }
    if (!RSDK.CheckOnScreen(entity, 0) && !RSDK.CheckPosOnScreen(&entity->internalPos, &entity->updateRange)) {
        entity            = entity;
        entity->position  = entity->internalPos;
        entity->angle     = 0;
        entity->playerBits = 0;
        entity->state     = entity->oscillate ? StickyPlatform_AddSpeed : StickyPlatform_HandleMovement;
    }
}

void StickyPlatform_LateUpdate(void) {}

void StickyPlatform_StaticUpdate(void) {}

void StickyPlatform_Draw(void)
{
    RSDK_THIS(StickyPlatform);
    RSDK.DrawSprite(&entity->animator, 0, false);
}

void StickyPlatform_Create(void *data)
{
    RSDK_THIS(StickyPlatform);
    entity->drawFX = INK_BLEND;
    if (RSDK_sceneInfo->inEditor != true) {
        entity->visible       = true;
        entity->active        = ACTIVE_BOUNDS;
        entity->drawOrder     = Zone->drawOrderHigh - 2;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        short left, top, right, bottom;
        RSDK.SetSpriteAnimation(StickyPlatform->animID, entity->type > 1 ? 3 : 0, &entity->animator, true, 0);
        switch (entity->type) {
            case 0:
                entity->direction = FLIP_NONE;
                entity->rotation  = 0;

                left   = -16;
                top    = -16;
                right  = 16;
                bottom = 8;
                break;
            case 1:
                entity->direction = FLIP_Y;
                entity->rotation  = 0x80;

                left   = -16;
                top    = -8;
                right  = 16;
                bottom = 16;
                break;
            case 2:
                entity->direction = FLIP_NONE;
                entity->rotation  = 0x40;

                left   = -8;
                top    = -16;
                right  = 16;
                bottom = 16;
                break;
            case 3:
                entity->direction = FLIP_X;
                entity->rotation  = 0xC0;

                left   = -16;
                top    = -16;
                right  = 8;
                bottom = 16;
                break;
            default: break;
        }
        entity->hitbox.left   = left;
        entity->hitbox.top    = top;
        entity->hitbox.bottom = bottom;
        entity->hitbox.right  = right;

        entity->animator.animationSpeed = 0;

        entity->internalPos.x = entity->position.x;
        entity->position.x += entity->amplitude.x;
        entity->internalPos.y = entity->position.y;
        entity->position.y += entity->amplitude.y;
        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;

        if (!entity->speed)
            entity->speed = 4;

        entity->state = entity->oscillate ? StickyPlatform_AddSpeed : StickyPlatform_HandleMovement;
    }
}

void StickyPlatform_StageLoad(void)
{
    StickyPlatform->animID = RSDK.LoadSpriteAnimation("CPZ/StickyPlatform.bin", SCOPE_STAGE);
    StickyPlatform->sfxID  = RSDK.GetSFX("Puyo/Land.wav");
}

void StickyPlatform_EditorDraw(void) {}

void StickyPlatform_EditorLoad(void) {}

void StickyPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(StickyPlatform, VAR_BOOL, oscillate);
}

void StickyPlatform_Interact(void)
{
    RSDK_THIS(StickyPlatform);
    if (Player->playerCount > 0) {
        for (int i = 0; i < Player->playerCount; i++) {
            EntityPlayer *player = RSDK.GetEntityByID(i);
            if (Player_CheckValidState(player) && player->interaction) {
                if (((1 << i) & entity->playerBits)) {
                    if (player->state == Player_State_None) {
                        if (player->jumpPress) {
                            player->angle     = entity->rotation;
                            player->groundVel = 0;
                            Player_StartJump(player);
                            player->velocity.x = (320 * player->velocity.x) >> 8;
                            player->velocity.y = (320 * player->velocity.y) >> 8;
                            entity->playerBits &= ~(1 << i);
                            entity->cooldowns[i] = 16;
                            RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (entity->type >> 1) + 2, &entity->animator, false, 0);
                        }
                        else {
                            Hitbox *playerbox = Player_GetHitbox(player);
                            ushort animID     = 0;
                            switch (entity->type) {
                                case 0:
                                    player->position.x = entity->position.x;
                                    player->position.y = entity->position.y - (playerbox->bottom << 16);
                                    animID             = 10;
                                    break;
                                case 1:
                                    player->position.x = entity->position.x;
                                    player->position.y = entity->position.y - (playerbox->top << 16);
                                    animID             = 10;
                                    break;
                                case 2:
                                    player->position.y = entity->position.y;
                                    player->position.x = entity->position.x + ((8 - playerbox->left) << 16);
                                    player->direction  = FLIP_NONE;
                                    animID             = 36;
                                    break;
                                case 3:
                                    player->position.y = entity->position.y;
                                    player->position.x = entity->position.x - ((playerbox->right + 8) << 16);
                                    player->direction  = FLIP_X;
                                    animID             = 36;
                                    break;
                                default: break;
                            }
                            RSDK.SetSpriteAnimation(player->spriteIndex, animID, &player->playerAnimator, false, 0);
                        }
                    }
                    else
                        entity->playerBits &= ~(1 << i);
                }
                else {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || entity->cooldowns[i]) {
                        if (entity->cooldowns[i])
                            entity->cooldowns[i]--;
                        continue;
                    }

                    entity->playerBits |= 1 << i;
                    RSDK.SetSpriteAnimation(StickyPlatform->animID, 3 * (entity->type >> 1), &entity->animator, true, 0);
                    entity->animator.animationSpeed = 1;
                    player->state                   = Player_State_None;
                    player->velocity.x              = 0;
                    player->velocity.y              = 0;
                    player->groundVel               = 0;
                    player->rotation                = 0;
                    RSDK.PlaySFX(StickyPlatform->sfxID, 0, 255);
                }
            }
        }
    }
}

void StickyPlatform_HandleMovement(void)
{
    RSDK_THIS(StickyPlatform);
    if (entity->angle) {
        if (!entity->playerBits)
            entity->state = StickyPlatform_MoveBackForth;
    }
    else if (entity->playerBits > 0) {
        entity->state = StickyPlatform_MoveBack;
    }
}

void StickyPlatform_MoveBack(void)
{
    RSDK_THIS(StickyPlatform);
    entity->angle += entity->speed;
    if (entity->angle >= 0x200) {
        entity->angle = 0x200;
        entity->state = StickyPlatform_HandleMovement;
    }
}

void StickyPlatform_MoveBackForth(void)
{
    RSDK_THIS(StickyPlatform);
    entity->angle += entity->speed;
    if (entity->angle >= 0x400) {
        entity->angle = 0;
        entity->state = StickyPlatform_HandleMovement;
    }
}

void StickyPlatform_AddSpeed(void)
{
    RSDK_THIS(StickyPlatform);
    entity->angle = (entity->angle + entity->speed) & 0x3FF;
}