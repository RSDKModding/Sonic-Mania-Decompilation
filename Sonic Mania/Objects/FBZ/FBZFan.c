#include "SonicMania.h"

ObjectFBZFan *FBZFan;

void FBZFan_Update(void)
{
    RSDK_THIS(FBZFan);

    FBZFan->fanHitbox.top    = (RSDK.Sin256(2 * Zone->timer) >> 5) - 80;
    FBZFan->fanHitbox.bottom = FBZFan->fanHitbox.top + 96;
    entity->hitbox.top       = -96;
    entity->hitbox.bottom    = -8;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        bool32 flag  = false;
        Player_CheckCollisionBox(player, entity, &FBZFan->solidHitbox);

        if (Player_CheckValidState(player) && player->playerAnimator.animationID != ANI_HURT
            && RSDK.CheckObjectCollisionTouchBox(entity, &FBZFan->fanHitbox, player, &FBZFan->playerHitbox)) {
            flag = true;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
            player->state    = Player_State_Air;
            player->onGround = false;
            int vel          = (entity->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 4;
            if (player->velocity.y <= vel
                || (player->velocity.y += ((entity->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 6)
                                          + ((entity->position.y + (FBZFan->fanHitbox.top << 16) - player->position.y) >> 5),
                    player->velocity.y < vel)) {
                player->velocity.y = vel;
            }

            if (!((1 << playerID) & FBZFan->activePlayers)) {
                if (player->velocity.y > -0x40000 && player->velocity.y < 0)
                    player->velocity.x += (32 * player->velocity.x / 31) >> 5;
                FBZFan->activePlayers |= 1 << playerID;
            }
        }

        if (RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox, player, &FBZFan->playerHitbox)) {
            if (!((1 << playerID) & entity->activePlayers) && flag) {
                RSDK.PlaySFX(FBZFan->sfxFan, false, 255);
                entity->activePlayers |= (1 << playerID);
            }
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }
    }
}

void FBZFan_LateUpdate(void) {}

void FBZFan_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&FBZFan->animator2);
    RSDK.ProcessAnimation(&FBZFan->animator3);
    FBZFan->activePlayers = 0;
}

void FBZFan_Draw(void)
{
    RSDK.DrawSprite(&FBZFan->animator1, 0, false);
    RSDK.DrawSprite(&FBZFan->animator2, 0, false);
    RSDK.DrawSprite(&FBZFan->animator3, 0, false);
}

void FBZFan_Create(void *data)
{
    RSDK_THIS(FBZFan);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow + 1;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->hitbox.left   = -64;
        entity->hitbox.right  = 64;
    }
}

void FBZFan_StageLoad(void)
{
    FBZFan->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZFan.bin", SCOPE_STAGE);
    FBZFan->sfxFan    = RSDK.GetSFX("FBZ/FBZFan.wav");
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 0, &FBZFan->animator1, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 1, &FBZFan->animator2, true, 0);
    RSDK.SetSpriteAnimation(FBZFan->aniFrames, 2, &FBZFan->animator3, true, 0);
    FBZFan->solidHitbox.left    = -64;
    FBZFan->solidHitbox.top     = -16;
    FBZFan->solidHitbox.right   = 64;
    FBZFan->solidHitbox.bottom  = 16;
    FBZFan->fanHitbox.left      = -64;
    FBZFan->fanHitbox.right     = 64;
    FBZFan->playerHitbox.left   = -1;
    FBZFan->playerHitbox.top    = -1;
    FBZFan->playerHitbox.right  = 1;
    FBZFan->playerHitbox.bottom = 1;
}

void FBZFan_EditorDraw(void) {}

void FBZFan_EditorLoad(void) {}

void FBZFan_Serialize(void) {}
