#include "../SonicMania.h"

ObjectInvincibleStars *InvincibleStars;

void InvincibleStars_Update()
{
    EntityInvincibleStars *entity = (EntityInvincibleStars *)RSDK_sceneInfo->entity;
    EntityPlayer *player          = (EntityPlayer *)entity->parent;
    if (player) {
        entity->starAngle[2] = (entity->starAngle[2] + 1) % 12;
        entity->starAngle[3] = (entity->starAngle[3] + 1) % 10;

        for (int i = 8 - 1; i > 0; --i) {
            entity->starPos[i].x = entity->starPos[i - 1].x;
            entity->starPos[i].y = entity->starPos[i - 1].y;
        }
        entity->starPos[0].x = player->position.x;
        entity->starPos[0].y = player->position.y;

        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->direction & FLIP_X)
            entity->starPos[0].x += (playerHitbox->left << 15) - (playerHitbox->right << 15) - (playerHitbox->left << 16);
        else
            entity->starPos[0].x += ((playerHitbox->right + 2 * playerHitbox->left) << 15) - (playerHitbox->left << 15);
        if (player->direction & 2 && player->invertGravity != 1)
            entity->starPos[0].y += (playerHitbox->top << 15) - (playerHitbox->bottom << 15) - (playerHitbox->top << 16);
        else
            entity->starPos[0].y += ((playerHitbox->bottom + 2 * playerHitbox->top) << 15) - (playerHitbox->top << 15);
        if (player->direction) {
            entity->starAngle[0] -= 144;
            entity->starAngle[1] -= 16;
        }
        else {
            entity->starAngle[0] += 144;
            entity->starAngle[1] += 16;
        }
        entity->starAngle[0] &= 0x1FF;
        entity->starAngle[1] &= 0x1FF;

        if (player->invincibleTimer < 32)
            entity->alpha = 8 * player->invincibleTimer;
        entity->drawOrder  = player->drawOrder;
        entity->visible    = player->visible /*|| (player->state == Ice_StateFrozenPlayer)*/;
        entity->starOffset = 11;
    }
    else {
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void InvincibleStars_LateUpdate()
{

}

void InvincibleStars_StaticUpdate()
{

}

void InvincibleStars_Draw()
{
    EntityInvincibleStars *entity = (EntityInvincibleStars *)RSDK_sceneInfo->entity;
    EntityPlayer *player          = (EntityPlayer *)entity->parent;
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
    }

    Vector2 drawPos;
    drawPos.x                    = (RSDK.Cos512(entity->starAngle[1] + 116) << entity->starOffset) + entity->starPos[7].x;
    drawPos.y                    = (RSDK.Sin512(entity->starAngle[1] + 116) << entity->starOffset) + entity->starPos[7].y;
    entity->starsData[3].frameID = entity->starAngle[2];
    RSDK.DrawSprite(&entity->starsData[3], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(entity->starAngle[1] + 372) << entity->starOffset) + entity->starPos[7].x;
    drawPos.y = (RSDK.Sin512(entity->starAngle[1] + 372) << entity->starOffset) + entity->starPos[7].y;
    entity->starsData[3].frameID += 6;
    RSDK.DrawSprite(&entity->starsData[3], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(entity->starAngle[1]) << entity->starOffset) + entity->starPos[5].x;
    drawPos.y                    = (RSDK.Sin512(entity->starAngle[1]) << entity->starOffset) + entity->starPos[5].y;
    entity->starsData[2].frameID = entity->starAngle[2];
    RSDK.DrawSprite(&entity->starsData[2], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(entity->starAngle[1] + 256) << entity->starOffset) + entity->starPos[5].x;
    drawPos.y = (RSDK.Sin512(entity->starAngle[1] + 256) << entity->starOffset) + entity->starPos[5].y;
    entity->starsData[2].frameID += 6;
    RSDK.DrawSprite(&entity->starsData[2], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(entity->starAngle[1] + 168) << entity->starOffset) + entity->starPos[3].x;
    drawPos.y                    = (RSDK.Sin512(entity->starAngle[1] + 168) << entity->starOffset) + entity->starPos[3].y;
    entity->starsData[1].frameID = entity->starAngle[3];
    RSDK.DrawSprite(&entity->starsData[1], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(entity->starAngle[1] + 424) << entity->starOffset) + entity->starPos[3].x;
    drawPos.y = (RSDK.Sin512(entity->starAngle[1] + 424) << entity->starOffset) + entity->starPos[3].y;
    entity->starsData[1].frameID += 5;
    RSDK.DrawSprite(&entity->starsData[1], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(entity->starAngle[0]) << entity->starOffset) + entity->starPos[0].x;
    drawPos.y                    = (RSDK.Sin512(entity->starAngle[0]) << entity->starOffset) + entity->starPos[0].y;
    entity->starsData[0].frameID = entity->starAngle[2];
    RSDK.DrawSprite(entity->starsData, &drawPos, 0);

    drawPos.x = (RSDK.Cos512(entity->starAngle[0] + 256) << entity->starOffset) + entity->starPos[0].x;
    drawPos.y = (RSDK.Sin512(entity->starAngle[0] + 256) << entity->starOffset) + entity->starPos[0].y;
    RSDK.DrawSprite(entity->starsData, &drawPos, 0);
}

void InvincibleStars_Create(void* data)
{
    EntityInvincibleStars *entity = (EntityInvincibleStars *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        Entity *player       = (Entity *)data;
        entity->active  = ACTIVE_NORMAL;
        entity->visible = true;
        entity->parent       = player;
        for (int i = 0; i < 8; ++i) {
            entity->starPos[i].x = player->position.x;
            entity->starPos[i].y = player->position.y;
        }
        entity->drawFX       = FX_FLIP;
        entity->inkEffect    = INK_ADD;
        entity->starAngle[0] = 180;
        entity->starAngle[1] = 0;
        entity->alpha        = 255;
        RSDK.SetSpriteAnimation(InvincibleStars->spriteIndex, 0, entity->starsData, true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->spriteIndex, 1, &entity->starsData[1], true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->spriteIndex, 2, &entity->starsData[2], true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->spriteIndex, 3, &entity->starsData[3], true, 0);
    }
}

void InvincibleStars_StageLoad() { InvincibleStars->spriteIndex = RSDK.LoadAnimation("Global/Invincible.bin", SCOPE_STAGE); }

void InvincibleStars_EditorDraw()
{

}

void InvincibleStars_EditorLoad()
{

}

void InvincibleStars_Serialize()
{

}

