#include "SonicMania.h"

ObjectSpikeCorridor *SpikeCorridor;

void SpikeCorridor_Update(void)
{
    RSDK_THIS(SpikeCorridor);
    StateMachine_Run(entity->state);
}

void SpikeCorridor_LateUpdate(void) {}

void SpikeCorridor_StaticUpdate(void) {}

void SpikeCorridor_Draw(void)
{
    RSDK_THIS(SpikeCorridor);
    StateMachine_Run(entity->stateDraw);
}

void SpikeCorridor_Create(void *data)
{
    RSDK_THIS(SpikeCorridor);

    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->active    = ACTIVE_BOUNDS;
    if (RSDK_sceneInfo->inEditor) {
        if (entity->colWidth < 20)
            entity->colWidth = 20;
        if (entity->rowHeight < 1)
            entity->rowHeight = 1;
        if (entity->yOffset < 64)
            entity->yOffset = 64;
    }
    entity->updateRange.x = 0x2000000;
    entity->updateRange.y = 0x2000000;
    entity->state         = SpikeCorridor_StateDropper_Setup;
}

void SpikeCorridor_StageLoad(void)
{
    SpikeCorridor->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/SpikeCorridor.bin", SCOPE_STAGE);
    SpikeCorridor->sfxDrop      = RSDK.GetSFX("Stage/Drop.wav");
    SpikeCorridor->sfxImpact    = RSDK.GetSFX("Stage/Impact2.wav");
    SpikeCorridor->sfxIndicator = RSDK.GetSFX("MMZ/Indicator.wav");
}

void SpikeCorridor_SetupHitboxes(void)
{
    RSDK_THIS(SpikeCorridor);
    int32 size = (-8 * entity->colWidth) >> 1;

    entity->hitboxes[0].left   = size;
    entity->hitboxes[0].top    = 0;
    entity->hitboxes[0].right  = size + 8 * entity->field_77;
    entity->hitboxes[0].bottom = 24;

    entity->hitboxes[1].left   = size + 8 * (entity->field_77 + 6);
    entity->hitboxes[1].top    = 0;
    entity->hitboxes[1].bottom = 24;
    entity->hitboxes[1].right  = size + 8 * entity->colWidth;
}

void SpikeCorridor_HandleDrawing(Animator *animator, int32 offsetY, int8 a3, bool32 animFlag)
{
    RSDK_THIS(SpikeCorridor);
    Vector2 drawPos;

    int32 startX = entity->position.x - (entity->colWidth << 18);
    drawPos.x  = 0;
    drawPos.y  = entity->position.y + offsetY;

    int32 anim = animFlag ? 5 : 0;

    for (int32 x = 0; x < entity->colWidth;) {
        drawPos.x = startX;

        int32 w       = 0;
        bool32 flag = false;
        if (x >= a3) {
            if (x < a3 + 6)
                flag = true;
            else
                w = entity->colWidth;
        }
        else {
            w = a3;
        }

        if (!flag) {
            if (w - x < 4) {
                switch (w - x) {
                    case 3:
                        RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 3, animator, true, 0);
                        x += 2;
                        startX += 0x100000;
                        break;
                    case 2:
                        RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 2, animator, true, 0);
                        ++x;
                        startX += 0x80000;
                        break;
                    case 1: RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 1, animator, true, 0); break;
                    case 0:
                        RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim, animator, true, 0);
                        x += 2;
                        startX += 0x100000;
                        break;
                }
            }
            else {
                RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 4, animator, true, 0);
                x += 3;
                startX += 0x180000;
            }
        }
        else {
            RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim, animator, true, 0);
            x += 2;
            startX += 0x100000;
        }
        ++x;
        RSDK.DrawSprite(animator, &drawPos, false);
        startX += 0x80000;
    }
}

void SpikeCorridor_Unknown3(void)
{
    RSDK_THIS(SpikeCorridor);

    int32 max  = (int8)(entity->colWidth - 9);
    int32 rand = RSDK.Rand(3, max);
    if (entity->field_75 <= -1) {
        entity->field_77 = rand;
        entity->field_75 = rand;
    }
    else {
        while (true) {
            if (abs(rand - entity->field_75) >= 6 || abs(rand - entity->field_75) == 3)
                break;
            rand = RSDK.Rand(3, max);
        }
        entity->field_77 = rand;
        entity->field_75 = rand;
    }
}

void SpikeCorridor_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpikeCorridor);

    foreach_active(Player, player)
    {
        for (int32 i = 0; i < 2; ++i) {
            int32 side = Player_CheckCollisionBox(player, entity, &entity->hitboxes[i]);

            if (side == C_BOTTOM) {
                Player_CheckHit(player, entity);
                player->collisionFlagV |= 2;
            }
            else if (side == C_TOP) {
                player->collisionFlagV |= 1;
            }
        }
    }
}

void SpikeCorridor_StateDropper_Setup(void)
{
    RSDK_THIS(SpikeCorridor);

    entity->hitbox.left   = -((8 * entity->colWidth) >> 1);
    entity->hitbox.top    = entity->yOffset;
    entity->hitbox.right  = (8 * entity->colWidth) >> 1;
    entity->hitbox.bottom = entity->yOffset + 24 * entity->rowHeight;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->rowID         = 0;
    entity->field_75      = -1;
    entity->timer         = 0;

    SpikeCorridor_StateDropper_CheckForPlayer();
    entity->state     = SpikeCorridor_StateDropper_CheckForPlayer;
    entity->stateDraw = StateMachine_None;
}

void SpikeCorridor_StateDropper_CheckForPlayer(void)
{
    RSDK_THIS(SpikeCorridor);

    foreach_active(Player, player)
    {
        if (player->sidekick)
            continue;

        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            if (player->position.x <= entity->position.x) {
                if (player->position.x + (playerHitbox->left << 16) >= entity->position.x + (entity->hitbox.left << 16)) {
                    SpikeCorridor_Unknown3();
                    entity->state     = SpikeCorridor_StateDropper_DropWarn;
                    entity->stateDraw = SpikeCorridor_StateDraw_DropWarn;
                }
            }
            else if (player->position.x + (playerHitbox->right << 16) <= entity->position.x + (entity->hitbox.right << 16)) {
                SpikeCorridor_Unknown3();
                entity->state     = SpikeCorridor_StateDropper_DropWarn;
                entity->stateDraw = SpikeCorridor_StateDraw_DropWarn;
            }
        }
    }
}

void SpikeCorridor_StateDropper_DropWarn(void)
{
    RSDK_THIS(SpikeCorridor);

    if (entity->timer >= 120) {
        entity->state     = SpikeCorridor_StateDropper_SpawnSpikes;
        entity->stateDraw = 0;
    }
    else {
        if (!(entity->timer % 40))
            RSDK.PlaySfx(SpikeCorridor->sfxIndicator, false, 255);
        ++entity->timer;
    }
}

void SpikeCorridor_StateDropper_SpawnSpikes(void)
{
    RSDK_THIS(SpikeCorridor);

    RSDK.PlaySfx(SpikeCorridor->sfxDrop, false, 255);

    EntitySpikeCorridor *child = CREATE_ENTITY(SpikeCorridor, entity, entity->startPos.x, entity->startPos.y);
    child->isPermanent         = true;
    child->state               = SpikeCorridor_StateSpikes_Setup;
    child->active              = ACTIVE_NORMAL;
    child->storedRowID         = entity->rowID;
    child->field_77            = entity->field_77;
    child->parent              = (Entity *)entity;
    child->drawOrder           = entity->drawOrder;
    child->colWidth            = entity->colWidth;
    child->yOffset             = 0;
    child->fallOffset          = (entity->yOffset + 24 * (entity->rowHeight - entity->rowID++ - 1)) << 16;
    entity->timer              = 0;

    if (entity->rowID >= entity->rowHeight) {
        entity->interaction = false;
        entity->state       = StateMachine_None;
    }
    else {
        entity->state = SpikeCorridor_StateDropper_DropWait;
    }
}

void SpikeCorridor_StateDropper_DropWait(void)
{
    RSDK_THIS(SpikeCorridor);

    if (entity->timer >= 60) {
        SpikeCorridor_Unknown3();
        entity->timer     = 0;
        entity->state     = SpikeCorridor_StateDropper_DropWarn;
        entity->stateDraw = SpikeCorridor_StateDraw_DropWarn;
    }
    else {
        entity->timer++;
    }
}

void SpikeCorridor_StateSpikes_Setup(void)
{
    RSDK_THIS(SpikeCorridor);
    SpikeCorridor_SetupHitboxes();
    entity->active    = ACTIVE_NORMAL;
    entity->visible   = true;
    entity->state     = SpikeCorridor_StateSpikes_Fall;
    entity->stateDraw = SpikeCorridor_StateDraw_Spikes;
    SpikeCorridor_StateSpikes_Fall();
}

void SpikeCorridor_StateSpikes_Fall(void)
{
    RSDK_THIS(SpikeCorridor);

    if (entity->yOffset >= entity->fallOffset) {
        RSDK.StopSFX(SpikeCorridor->sfxDrop);
        RSDK.PlaySfx(SpikeCorridor->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 5);
        entity->active = ACTIVE_ALWAYS;
        entity->state  = SpikeCorridor_StateSpikes_Land;
    }
    else {
        entity->velocity.y += 0x3800;
        entity->yOffset += entity->velocity.y;
        if (entity->yOffset > entity->fallOffset)
            entity->yOffset = entity->fallOffset;
        entity->position.y = entity->yOffset + ((EntitySpikeCorridor *)entity->parent)->startPos.y;
    }
    SpikeCorridor_CheckPlayerCollisions();
}

void SpikeCorridor_StateSpikes_Land(void) { SpikeCorridor_CheckPlayerCollisions(); }

void SpikeCorridor_StateDraw_DropWarn(void)
{
    Animator animator;
    memset(&animator, 0, sizeof(Animator));
    RSDK_THIS(SpikeCorridor);

    int32 yOff = 0;
    if ((RSDK_screens->position.y << 16) - entity->startPos.y > 0)
        yOff = (RSDK_screens->position.y << 16) - entity->startPos.y;
    SpikeCorridor_HandleDrawing(&animator, yOff, entity->field_77, true);
}

void SpikeCorridor_StateDraw_Spikes(void)
{
    Animator animator;
    memset(&animator, 0, sizeof(Animator));
    RSDK_THIS(SpikeCorridor);

    SpikeCorridor_HandleDrawing(&animator, 0, entity->field_77, false);
}

void SpikeCorridor_EditorDraw(void) {}

void SpikeCorridor_EditorLoad(void) {}

void SpikeCorridor_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT8, colWidth);
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT8, rowHeight);
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT32, yOffset);
}
