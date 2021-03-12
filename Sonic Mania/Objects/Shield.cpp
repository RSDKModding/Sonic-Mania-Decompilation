#include "../SonicMania.hpp"

ObjectShield *Shield;

void Shield_Update()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
    EntityPlayer *player = entity->player;
    if (player) {
        int drawOrder = player->drawOrder;
        if (entity->frameID & -4 <= 0)
            --drawOrder;
        entity->drawOrder = drawOrder;
        entity->visible   = entity->field_6C & player->visible;
        entity->field_6C  = 1;
    }
}

void Shield_LateUpdate()
{

}

void Shield_StaticUpdate()
{

}

void Shield_Draw()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
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
        Hitbox *playerHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
        if (playerHitbox) {
            if (player->direction & FLIP_X)
                entity->position.x += (playerHitbox->left << 15) - (playerHitbox->right << 15) - (playerHitbox->left << 16);
            else
                entity->position.x += ((playerHitbox->right + 2 * playerHitbox->left) << 15) - (playerHitbox->left << 15);
            if (player->direction & 2 || player->invertGravity)
                entity->position.y += (playerHitbox->top << 15) - (playerHitbox->bottom << 15) - (playerHitbox->top << 16);
            else
                entity->position.y += ((playerHitbox->bottom + 2 * playerHitbox->top) << 15) - (playerHitbox->top << 15);
        }
    }

    if (entity->type == SHIELD_BUBBLE) {
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->altData, NULL, false);
        entity->inkEffect = INK_BLEND;
    }

    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Shield_Create(void* data)
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    entity->active  = ACTIVE_NORMAL;
    entity->visible = 1;
    if (data) {
        EntityPlayer *player = (EntityPlayer *)data;
        if (player->objectID == Player->objectID)
            entity->type = player->shield;
        entity->player = player;
    }

    entity->drawFX = FX_FLIP;
    entity->state       = Shield_State_Update;
    switch (entity->type - 1) {
        case SHIELD_NONE:
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 0, &entity->data, true, 0);
            entity->inkEffect = INK_ADD;
            entity->alpha     = 0x80;
            break;
        case SHIELD_BLUE:
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 5, &entity->altData, true, 0);
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 6, &entity->data, true, 0);
            entity->alpha = 0x100;
            break;
        case SHIELD_BUBBLE: RSDK.SetSpriteAnimation(Shield->spriteIndex, 1, &entity->data, true, 0); break;
        case SHIELD_FIRE: RSDK.SetSpriteAnimation(Shield->spriteIndex, 3, &entity->data, true, 0); break;
        default: return;
    }
}

void Shield_StageLoad()
{
    Shield->spriteIndex         = RSDK.LoadAnimation("Global/Shields.bin", SCOPE_STAGE);
    Shield->sfx_BlueShield      = RSDK.GetSFX("Global/BlueShield.wav");
    Shield->sfx_BubbleShield    = RSDK.GetSFX("Global/BubbleShield.wav");
    Shield->sfx_FireShield      = RSDK.GetSFX("Global/FireShield.wav");
    Shield->sfx_LightningShield = RSDK.GetSFX("Global/LightningShield.wav");
    Shield->sfx_InstaShield     = RSDK.GetSFX("Global/InstaShield.wav");
    Shield->sfx_BubbleBounce    = RSDK.GetSFX("Global/BubbleBounce.wav");
    Shield->sfx_FireDash        = RSDK.GetSFX("Global/FireDash.wav");
    Shield->sfx_LightningJump   = RSDK.GetSFX("Global/LightningJump.wav");
}

void Shield_State_Update()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    RSDK.ProcessAnimation(&entity->altData);
    int id                     = RSDK.GetFrameID(&entity->data) & 7;
    entity->frameID        = id;
    entity->direction     = id & 3;
}

void Shield_State_Bubble()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    RSDK.ProcessAnimation(&entity->altData);
    int id                 = RSDK.GetFrameID(&entity->data) & 7;
    entity->frameID        = id;
    entity->direction      = id & 3;
    if (entity->altData.frameID == entity->altData.frameCount - 1) {
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 9, &entity->altData, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &entity->altData, true, 0);
        entity->state = Shield_State_BubbleAlt;
    }
}

void Shield_State_BubbleAlt()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    RSDK.ProcessAnimation(&entity->altData);
    int id            = RSDK.GetFrameID(&entity->data) & 7;
    entity->frameID   = id;
    entity->direction = id & 3;
    if (entity->altData.frameID == entity->altData.frameCount - 1) {
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 5, &entity->altData, true, 0);
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 6, &entity->altData, true, 0);
        entity->state = Shield_State_Update;
    }
}

void Shield_State_Fire()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    entity->frameID = RSDK.GetFrameID(&entity->data) & 7;
    ++entity->timer;
    if (entity->timer > 24) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(Shield->spriteIndex, 1, &entity->altData, true, 0);
        entity->state = Shield_State_Update;
    }
}

void Shield_State_Lightning()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->altData);
    int id            = RSDK.GetFrameID(&entity->altData);
    entity->frameID   = id & 7;
    entity->direction = entity->frameID & 3;
    if (entity->player) {
        entity->position.x = entity->player->position.x;
        entity->position.y = entity->player->position.y;
    }

    EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer        = 22;
    debris->velocity.x   = -0x20000;
    debris->velocity.y   = -0x20000;
    debris->drawOrder    = Zone->uiDrawLow;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->data, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = 0x20000;
    debris->velocity.y = -0x20000;
    debris->drawOrder  = Zone->uiDrawLow;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->data, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = -0x20000;
    debris->velocity.y = 0x20000;
    debris->drawOrder  = Zone->uiDrawLow;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->data, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }

    debris             = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, (void*)Debris_State_LightningSpark, entity->position.x, entity->position.y);
    debris->timer      = 22;
    debris->velocity.x = 0x20000;
    debris->velocity.y = 0x20000;
    debris->drawOrder  = Zone->uiDrawLow;
    RSDK.SetSpriteAnimation(Shield->spriteIndex, 4, &debris->data, true, 0);
    if (entity->drawFX & FX_SCALE) {
        debris->drawFX |= FX_SCALE;
        debris->scale.x = entity->scale.x;
        debris->scale.y = entity->scale.y;
    }
    entity->state = Shield_State_Update;
}

void Shield_State_Insta()
{
    EntityShield *entity = (EntityShield *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    EntityPlayer *player = entity->player;
    if (player)
        player->invincibleTimer = 1;

    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}

void Shield_EditorDraw()
{

}

void Shield_EditorLoad()
{

}

void Shield_Serialize()
{

}

