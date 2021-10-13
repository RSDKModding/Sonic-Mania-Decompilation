#include "SonicMania.h"

ObjectBuzzSaw *BuzzSaw;

void BuzzSaw_Update(void)
{
    RSDK_THIS(BuzzSaw);
    StateMachine_Run(entity->state);
}

void BuzzSaw_LateUpdate(void) {}

void BuzzSaw_StaticUpdate(void) {}

void BuzzSaw_Draw(void)
{
    RSDK_THIS(BuzzSaw);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void BuzzSaw_Create(void *data)
{
    RSDK_THIS(BuzzSaw);

    entity->drawFX = FX_ROTATE;
    if (!entity->type)
        entity->rotation = entity->angle;
    else
        entity->inkEffect = INK_ADD;

    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BuzzSaw->aniFrames, entity->type, &entity->animator, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = (abs(entity->speed * entity->amplitude.x) + 64) << 17;
        entity->updateRange.y = (abs(entity->speed * entity->amplitude.y) + 64) << 17;
        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->startPos  = entity->position;

        if (entity->type == 0)
            entity->state = BuzzSaw_State_OnArm;
        else
            entity->state = BuzzSaw_State_FreeMove_Waiting;
    }
}

void BuzzSaw_StageLoad(void)
{
    BuzzSaw->aniFrames     = RSDK.LoadSpriteAnimation("MMZ/BuzzSaw.bin", SCOPE_STAGE);
    BuzzSaw->field_10      = 0x200000;
    BuzzSaw->field_14      = 0x200000;
    BuzzSaw->hitbox.left   = -24;
    BuzzSaw->hitbox.top    = -24;
    BuzzSaw->hitbox.right  = 24;
    BuzzSaw->hitbox.bottom = 24;

    Soundboard_LoadSFX("MMZ/SawSus.wav", true, BuzzSaw_CheckCB, BuzzSaw_UpdateCB);
}

bool32 BuzzSaw_CheckCB(void)
{
    int32 worldX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    int32 worldY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

    int32 count = 0;

    foreach_all(BuzzSaw, saw)
    {
        int32 x    = abs(worldX - saw->position.x);
        int32 y    = abs(worldY - saw->position.y);
        int32 dist = MathHelpers_Unknown6((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) << 16;
        if (dist > 0x2800000)
            count++;
    }

    return count > 0;
}

void BuzzSaw_UpdateCB(int32 sfx)
{
    int32 dist   = 0x7FFF0000;
    int32 worldX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    int32 worldY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

    foreach_all(BuzzSaw, saw)
    {
        int32 x = abs(worldX - saw->position.x);
        int32 y = abs(worldY - saw->position.y);
        dist  = minVal(MathHelpers_Unknown6((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) << 16, dist);
    }

    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfx], 1.0 - (minVal(dist >> 16, 640) / 640.0), 0.0, 1.0);
}

void BuzzSaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuzzSaw);
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &BuzzSaw->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }
    }
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
}

void BuzzSaw_State_OnArm(void)
{
    RSDK_THIS(BuzzSaw);
    entity->drawPos.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + entity->startPos.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer) + entity->startPos.y;
    RSDK.ProcessAnimation(&entity->animator);
    BuzzSaw_CheckPlayerCollisions();
}

void BuzzSaw_State_FreeMove_Waiting(void)
{
    RSDK_THIS(BuzzSaw);

    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;

    foreach_active(Player, player)
    {
        int32 angle = RSDK.ATan2((player->position.x - entity->position.x) >> 16, (player->position.y - entity->position.y) >> 16);

        int32 rx = (abs(player->position.x - entity->position.x) >> 16) * (abs(player->position.x - entity->position.x) >> 16);
        int32 ry = (abs(player->position.y - entity->position.y) >> 16) * (abs(player->position.y - entity->position.y) >> 16);
        if (angle + 32 - (entity->angle & 0xFF) < 0x40 && (rx + ry) - 0x4000 < 0x5000) {
            entity->active     = ACTIVE_NORMAL;
            entity->velocity.x = 0x600 * RSDK.Cos256(entity->angle);
            entity->velocity.y = 0x600 * RSDK.Sin256(entity->angle);
            entity->state      = BuzzSaw_State_FreeMove_Released;
        }
    }
}

void BuzzSaw_State_FreeMove_Released(void)
{
    RSDK_THIS(BuzzSaw);

    if (entity->alpha < 256)
        entity->alpha += 16;
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator);
    BuzzSaw_CheckPlayerCollisions();
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->drawPos.x  = entity->startPos.x;
        entity->drawPos.y  = entity->startPos.y;
        entity->alpha      = 0;
        entity->state      = BuzzSaw_State_FreeMove_Reset;
    }
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
}

void BuzzSaw_State_FreeMove_Reset(void)
{
    RSDK_THIS(BuzzSaw);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->state = BuzzSaw_State_FreeMove_Waiting;
        BuzzSaw_Create(NULL);
    }
}

void BuzzSaw_EditorDraw(void) {}

void BuzzSaw_EditorLoad(void) {}

void BuzzSaw_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, speed);
}
