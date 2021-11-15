#include "SonicMania.h"

ObjectShield *Shield;

void Shield_Update(void)
{
    RSDK_THIS(Shield);
    StateMachine_Run(entity->state);
    EntityPlayer *player = entity->player;
    if (player) {
        int32 drawOrder = player->drawOrder;
        if ((entity->frameID & -4) <= 0)
            --drawOrder;
        entity->drawOrder = drawOrder;
        entity->visible   = entity->flag & player->visible;
        entity->flag      = true;
    }
}

void Shield_LateUpdate(void) {}

void Shield_StaticUpdate(void) {}

void Shield_Draw(void)
{
    RSDK_THIS(Shield);
    EntityPlayer *player = (EntityPlayer *)entity->player;
    if (player) {
        if (player->isChibi) {
            entity->drawFX |= FX_SCALE;
            entity->scale.x = 0x100;
            entity->scale.y = 0x100;
        }
        else {
            if (player->drawFX & FX_SCALE)
                entity->drawFX |= FX_SCALE;
            else
                entity->drawFX &= ~FX_SCALE;
            entity->scale.x = player->scale.x;
            entity->scale.y = player->scale.y;
        }
        entity->position.x   = player->position.x;
        entity->position.y   = player->position.y;
        Hitbox *playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
        if (playerHitbox) {
            if (player->direction & FLIP_X)
                entity->position.x += (playerHitbox->left << 15) - (playerHitbox->right << 15) - (playerHitbox->left << 16);
            else
                entity->position.x += ((playerHitbox->right + 2 * playerHitbox->left) << 15) - (playerHitbox->left << 15);
            if ((player->direction & FLIP_Y) || player->invertGravity)
                entity->position.y += (playerHitbox->top << 15) - (playerHitbox->bottom << 15) - (playerHitbox->top << 16);
            else
                entity->position.y += ((playerHitbox->bottom + 2 * playerHitbox->top) << 15) - (playerHitbox->top << 15);
        }
    }

    if (entity->type == SHIELD_BUBBLE) {
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->inkEffect = INK_BLEND;
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Shield_Create(void *data)
{
    RSDK_THIS(Shield);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    if (data) {
        EntityPlayer *player = (EntityPlayer *)data;
        if (player->objectID == Player->objectID)
            entity->type = player->shield;
        entity->player = player;
    }

    entity->drawFX = FX_FLIP;
    entity->state  = Shield_State_Generic;
    switch (entity->type) {
        case SHIELD_BLUE:
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 0, &entity->animator, true, 0);
            entity->inkEffect = INK_ADD;
            entity->alpha     = 0x80;
            break;
        case SHIELD_BUBBLE:
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 5, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 6, &entity->animator, true, 0);
            entity->alpha = 0x100;
            break;
        case SHIELD_FIRE: RSDK.SetSpriteAnimation(Shield->spriteIndex, 1, &entity->animator, true, 0); break;
        case SHIELD_LIGHTNING: RSDK.SetSpriteAnimation(Shield->spriteIndex, 3, &entity->animator, true, 0); break;
        default: break;
    }
}

void Shield_StageLoad(void)
{
    Shield->spriteIndex         = RSDK.LoadSpriteAnimation("Global/Shields.bin", SCOPE_STAGE);
    Shield->sfx_BlueShield      = RSDK.GetSFX("Global/BlueShield.wav");
    Shield->sfx_BubbleShield    = RSDK.GetSFX("Global/BubbleShield.wav");
    Shield->sfx_FireShield      = RSDK.GetSFX("Global/FireShield.wav");
    Shield->sfx_LightningShield = RSDK.GetSFX("Global/LightningShield.wav");
    Shield->sfx_InstaShield     = RSDK.GetSFX("Global/InstaShield.wav");
    Shield->sfx_BubbleBounce    = RSDK.GetSFX("Global/BubbleBounce.wav");
    Shield->sfx_FireDash        = RSDK.GetSFX("Global/FireDash.wav");
    Shield->sfx_LightningJump   = RSDK.GetSFX("Global/LightningJump.wav");
}

void Shield_State_Generic(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->frameID   = RSDK.GetFrameID(&entity->animator) & 7;
    entity->direction = entity->frameID & 3;
}

void Shield_State_Bubble(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->frameID   = RSDK.GetFrameID(&entity->animator) & 7;
    entity->direction = entity->frameID & 3;
    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 9, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator, true, 0);
        entity->state = Shield_State_BubbleAlt;
    }
}

void Shield_State_BubbleAlt(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->frameID   = RSDK.GetFrameID(&entity->animator) & 7;
    entity->direction = entity->frameID & 3;
    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 5, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 6, &entity->animator, true, 0);
        entity->state = Shield_State_Generic;
    }
}

void Shield_State_Fire(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator);
    entity->frameID = RSDK.GetFrameID(&entity->animator) & 7;
    ++entity->timer;
    if (entity->timer > 24) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 1, &entity->animator, true, 0);
        entity->state = Shield_State_Generic;
    }
}

void Shield_State_Lightning(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->frameID   = RSDK.GetFrameID(&entity->animator) & 7;
    entity->direction = entity->frameID & 3;
    if (entity->player) {
        entity->position.x = entity->player->position.x;
        entity->position.y = entity->player->position.y;
    }

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer        = 22;
    debris->velocity.x   = -0x20000;
    debris->velocity.y   = -0x20000;
    debris->drawOrder    = Zone->playerDrawHigh;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->animator, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = 0x20000;
    debris->velocity.y = -0x20000;
    debris->drawOrder  = Zone->playerDrawHigh;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->animator, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = -0x20000;
    debris->velocity.y = 0x20000;
    debris->drawOrder  = Zone->playerDrawHigh;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->animator, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = 0x20000;
    debris->velocity.y = 0x20000;
    debris->drawOrder  = Zone->playerDrawHigh;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->animator, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }
    entity->state = Shield_State_Generic;
}

void Shield_State_Insta(void)
{
    RSDK_THIS(Shield);
    RSDK.ProcessAnimation(&entity->animator);
    EntityPlayer *player = entity->player;
    if (player)
        player->invincibleTimer = 1;

    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Shield_EditorDraw(void)
{
    RSDK_THIS(Shield);
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 0, &entity->animator, false, 0);
    entity->inkEffect = INK_ADD;
    entity->alpha     = 0x80;

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Shield_EditorLoad(void) { Shield->spriteIndex = RSDK.LoadSpriteAnimation("Global/Shields.bin", SCOPE_STAGE); }
#endif

void Shield_Serialize(void) {}
