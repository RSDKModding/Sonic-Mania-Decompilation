#include "../SonicMania.h"

ObjectZipLine *ZipLine;

void ZipLine_Update(void)
{
    RSDK_THIS(ZipLine);
    StateMachine_Run(entity->state);
}

void ZipLine_LateUpdate(void) {}

void ZipLine_StaticUpdate(void) {}

void ZipLine_Draw(void)
{
    RSDK_THIS(ZipLine);
    RSDK.DrawLine(entity->startPos.x, entity->startPos.y, entity->endPos.x, entity->endPos.y, 0x6060A0, 0x00, INK_NONE, false);
    RSDK.DrawLine(entity->startPos.x, entity->startPos.y + 0x10000, entity->endPos.x, entity->endPos.y + 0x10000, 0x303070, 0x00, INK_NONE, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &entity->handlePos, false);
    entity->animator.frameID = 1;

    RSDK.DrawSprite(&entity->animator, &entity->startPos, false);
    RSDK.DrawSprite(&entity->animator, &entity->endPos, false);
}

void ZipLine_Create(void *data)
{
    RSDK_THIS(ZipLine);
    entity->active  = ACTIVE_BOUNDS;
    entity->visible = true;
    entity->drawFX  = FX_FLIP;
    RSDK.SetSpriteAnimation(ZipLine->aniFrames, 0, &entity->animator, true, 0);
    entity->drawOrder   = Zone->playerDrawLow;
    entity->handlePos.x = entity->position.x;
    entity->handlePos.y = entity->position.y;
    entity->startPos.x  = entity->position.x;
    entity->startPos.y  = entity->position.y;
    entity->endPos.x    = entity->position.x + (entity->length << 8) * RSDK.Cos256(entity->angle);
    entity->endPos.y    = entity->position.y + (entity->length << 8) * RSDK.Sin256(entity->angle);

    if (!RSDK_sceneInfo->inEditor) {
        entity->position.x += (entity->endPos.x - entity->startPos.x) >> 1;
        entity->position.y += (entity->endPos.y - entity->startPos.y) >> 1;
        entity->updateRange.x = (abs(entity->endPos.x - entity->startPos.x) >> 1) + 0x400000;
        entity->updateRange.y = (abs(entity->endPos.y - entity->startPos.y) >> 1) + 0x400000;
        // entity->field_98.x         = ZipLine_Unknown3();
        // entity->field_98.y         = v12;
    }
}

void ZipLine_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        ZipLine->aniFrames = RSDK.LoadSpriteAnimation("GHZ/ZipLine.bin", SCOPE_STAGE);
    ZipLine->hitbox.top    = 0;
    ZipLine->hitbox.left   = -8;
    ZipLine->hitbox.bottom = 24;
    ZipLine->hitbox.right  = 8;

    Zone_AddCallback(ZipLine_Unknown1);
}

void ZipLine_Unknown1(void)
{
    foreach_active(ZipLine, zipline)
    {
        if ((1 << Zone->playerID) & zipline->activePlayers)
            Zone->playerFlags[Zone->playerID] = false;
    }
}

void ZipLine_Unknown2(void)
{
    RSDK_THIS(ZipLine);

    foreach_active(Player, player)
    {
        int pid = RSDK.GetEntityID(player);
        if ((1 << pid) & entity->activePlayers) {
            entity->grabDelay[pid] = 60;
            player->velocity.y     = entity->velocity.y;
            player->velocity.x     = entity->velocity.x;
            player->groundVel      = player->velocity.x;
            entity->activePlayers &= ~(1 << pid);
            player->onGround       = false;
            player->groundedStore  = false;
            player->tileCollisions = true;
            player->state          = Player_State_Air;
        }
    }
}

void ZipLine_EditorDraw(void) {}

void ZipLine_EditorLoad(void) {}

void ZipLine_Serialize(void)
{
    RSDK_EDITABLE_VAR(ZipLine, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(ZipLine, VAR_ENUM, length);
}
