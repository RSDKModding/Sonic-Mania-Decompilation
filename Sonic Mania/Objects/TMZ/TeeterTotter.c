#include "SonicMania.h"

ObjectTeeterTotter *TeeterTotter;

void TeeterTotter_Update(void)
{
    RSDK_THIS(TeeterTotter);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
}

void TeeterTotter_LateUpdate(void) {}

void TeeterTotter_StaticUpdate(void) {}

void TeeterTotter_Draw(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = entity->position.x;
    int32 y       = entity->position.y;
    int32 offsetX = 0x100000 - (entity->length << 21) + x;

    for (int32 i = 0; i < 2 * entity->length; ++i) {
        if (!((1 << i) & entity->inactiveSegments)) {
            Vector2 drawPos;
            drawPos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            drawPos.y = (entity->field_90[i] + y) & 0xFFFF0000;
            RSDK.SetSpriteAnimation(TeeterTotter->aniFrames, 0, &entity->animator, true, (i + entity->color) % 2);
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
        }
    }
}

void TeeterTotter_Create(void *data)
{
    RSDK_THIS(TeeterTotter);

    if (entity->length >= 0x10 || entity->length) {
        if (entity->length > 0x10)
            entity->length = 0x10;
    }
    else {
        entity->length = 1;
    }

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->origin        = entity->position;
    entity->updateRange.x = (entity->length + 2) << 22;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.y = (entity->length + 4) << 21;

    entity->hitbox.left   = -16;
    entity->hitbox.top    = -16;
    entity->hitbox.right  = 16;
    entity->hitbox.bottom = 16;
    entity->state         = TeeterTotter_State_Setup;
}

void TeeterTotter_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE);
}

int32 TeeterTotter_CheckPlayerCollisions(void)
{
    RSDK_THIS(TeeterTotter);
    int32 id = -1;

    int32 ids[] = { -1, -1, -1, -1 };

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    int32 x = 0x100000 - (entity->length << 21) + entity->position.x;

    for (int32 i = 0; i < 2 * entity->length; ++i) {
        if (!((1 << id) & entity->inactiveSegments)) {
            entity->position.x = (x + (i << 21)) & 0xFFFF0000;
            entity->position.y = (entity->field_90[i] + storeY) & 0xFFFF0000;
            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntityID(player);
                if (entity->playerIDs[playerID] == i) {
                    player->position.y += entity->field_110[i];
                    player->position.y += 0x10000;
                }
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_TOP) {
                    if (!player->sidekick)
                        id = i;
                    ids[playerID] = i;
                }
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;

    for (int32 i = 0; i < PLAYER_MAX; ++i) entity->playerIDs[i] = ids[i];

    return id;
}

void TeeterTotter_ProcessSegmentGravity(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = entity->position.x;
    int32 y       = entity->position.y;
    int32 offsetX = (0x100000 - (entity->length << 21)) + x;

    for (int32 i = 0; i < 2 * entity->length; ++i) {
        if (!((1 << i) & entity->inactiveSegments)) {
            entity->field_110[i] += 0x3800;
            entity->field_90[i] += entity->field_110[i];

            Vector2 pos;
            pos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            pos.y = (entity->field_90[i] + y) & 0xFFFF0000;
            if (!RSDK.CheckPosOnScreen(&pos, &entity->updateRange))
                entity->inactiveSegments |= 1 << i;
        }
    }
}

void TeeterTotter_Unknown3(void)
{
    RSDK_THIS(TeeterTotter);

    uint8 len = entity->length;
    for (int32 i = 0; i < 2 * entity->length; ++i) {
        int8 val = i - len + 1;
        if (i - len < 0)
            val = i - len;
        entity->field_90[i] = (entity->field_84 >> 1) * (int8)(2 * val + 2 * ((int8)(2 * val) <= 0) - 1);
    }
}

void TeeterTotter_State_Setup(void)
{
    RSDK_THIS(TeeterTotter);

    entity->field_8C = 0;
    entity->field_84 = 0;
    entity->field_88 = 0;

    for (int32 i = 0; i < 2 * entity->length; ++i) {
        entity->field_90[i]  = 0;
        entity->field_110[i] = 0;
    }
    entity->inactiveSegments = 0;

    entity->visible = true;
    entity->active  = ACTIVE_NORMAL;
    entity->state   = TeeterTotter_State_Unknown1;
}

void TeeterTotter_State_Unknown1(void)
{
    RSDK_THIS(TeeterTotter);

    int32 prevVal[32];
    for (int32 i = 0; i < 2 * entity->length; ++i) {
        prevVal[i] = entity->field_90[i];
    }

    TeeterTotter_Unknown3();

    for (int32 i = 0; i < 2 * entity->length; ++i) {
        entity->field_110[i] = entity->field_90[i] - prevVal[i];
    }

    int32 val = TeeterTotter_CheckPlayerCollisions();
    if (val > -1) {
        int32 id = val - entity->length;
        if (id >= 0)
            ++id;
        entity->field_88 += 32 * id;
    }
    entity->field_84 += entity->field_88;

    if (abs(entity->field_84) > 0x200000)
        entity->state = TeeterTotter_State_Unknown2;
}

void TeeterTotter_State_Unknown2(void)
{
    RSDK_THIS(TeeterTotter);
    TeeterTotter_ProcessSegmentGravity();

    bool32 flag = true;
    for (int32 i = 0; i < 2 * entity->length; ++i) {
        flag &= ((1 << i) & entity->inactiveSegments) != 0;
    }

    if (flag || !(2 * entity->length)) {
        if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->origin, &entity->updateRange)) {
            entity->position.x = entity->origin.x;
            entity->position.y = entity->origin.y;
            entity->visible    = false;
            entity->active     = ACTIVE_BOUNDS;
            entity->state      = TeeterTotter_State_Setup;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TeeterTotter_EditorDraw(void)
{
    RSDK_THIS(TeeterTotter);

    if (entity->length >= 0x10 || entity->length) {
        if (entity->length > 0x10)
            entity->length = 0x10;
    }
    else {
        entity->length = 1;
    }

    entity->origin        = entity->position;
    entity->updateRange.x = (entity->length + 2) << 22;
    entity->updateRange.y = (entity->length + 4) << 21;

    TeeterTotter_Draw();
}

void TeeterTotter_EditorLoad(void) { TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE); }
#endif

void TeeterTotter_Serialize(void)
{
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT8, color);
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT32, length);
}
