#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectHangGlider *HangGlider;

void HangGlider_Update(void)
{
    RSDK_THIS(HangGlider);
    StateMachine_Run(entity->state);
}

void HangGlider_LateUpdate(void) {}

void HangGlider_StaticUpdate(void) {}

void HangGlider_Draw(void)
{
    RSDK_THIS(HangGlider);
    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        if (entity->playerPtr)
            RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK_sceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&entity->animator4, NULL, false);
    }
}

void HangGlider_Create(void *data)
{
    RSDK_THIS(HangGlider);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->field_60      = 4096;
        entity->state         = HangGlider_Unknown1;
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &entity->animator4, true, 1);
    }
}

void HangGlider_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangGlider.bin", SCOPE_STAGE);
    else
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/HangGlider.bin", SCOPE_STAGE);

    HangGlider->hitbox.left   = 12;
    HangGlider->hitbox.top    = 8;
    HangGlider->hitbox.right  = 24;
    HangGlider->hitbox.bottom = 128;
}

void HangGlider_Unknown1(void)
{
    RSDK_THIS(HangGlider);
    foreach_active(Player, player)
    {
        if (abs(player->position.x - entity->position.x) < 0x40000) {
            if (abs(player->position.y - entity->position.y) < 0x140000) {
                entity->playerPtr  = (Entity *)player;
                player->active     = ACTIVE_NEVER;
                player->visible    = 0;
                entity->velocity.x = (192 * player->velocity.x) >> 8;
                RSDK.SetSpriteAnimation(player->spriteIndex, 12, &entity->animator3, true, 0);
                entity->animator3.rotationFlag = 1;
                entity->rotation               = 128;
                entity->drawFX                 = FX_ROTATE;
                RSDK.PlaySFX(Player->sfx_Grab, false, 255);
                entity->state = HangGlider_Unknown2;
            }
        }
    }
}
void HangGlider_Unknown2(void)
{
    RSDK_THIS(HangGlider);

    entity->velocity.y += entity->field_60;
    if (entity->velocity.y > 0x10000)
        entity->velocity.y = 0x10000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    Entity *player = entity->playerPtr;
    if (player) {
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator3);
}

void HangGlider_EditorDraw(void) {}

void HangGlider_EditorLoad(void) {}

void HangGlider_Serialize(void) {}
#endif
