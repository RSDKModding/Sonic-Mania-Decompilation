#include "SonicMania.h"

ObjectChemicalPool *ChemicalPool;

void ChemicalPool_Update(void)
{
    RSDK_THIS(ChemicalPool);

    if (entity->timer > 0)
        entity->timer--;
    entity->offsetY = entity->position.y - entity->updateRange.y;
    StateMachine_Run(entity->state);
}

void ChemicalPool_LateUpdate(void) {}

void ChemicalPool_StaticUpdate(void) {}

void ChemicalPool_Draw(void)
{
    RSDK_THIS(ChemicalPool);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    Vector2 drawPos;
    Vector2 verts[4];

    int32 screenX = entity->position.x - (screen->position.x << 16);

    int32 screenY = screen->position.y << 16;
    int32 vy      = entity->position.y + entity->updateRange.y - screenY;

    Vector2 *vertPtr = entity->field_A0;
    for (int32 i = 0; i < entity->tileSizeX; ++i) {
        verts[0].x = vertPtr[0].x + screenX;
        verts[0].y = vertPtr[0].y - screenY;
        verts[1].x = vertPtr[2].x + screenX;
        verts[1].y = vertPtr[2].y - screenY;
        verts[2].x = vertPtr[3].x + screenX;
        verts[2].y = vertPtr[3].y - screenY;
        verts[3].x = vertPtr[1].x + screenX;
        verts[3].y = vertPtr[1].y - screenY;

        colour *colours = NULL;
        if (entity->timer >= 1)
            colours = ChemicalPool->colours;
        else
            colours = entity->colours;
        RSDK.DrawBlendedQuad(verts, colours, 4, 0xC0, INK_NONE);

        verts[0].x = verts[3].x;
        verts[0].y = verts[3].y;
        verts[1].x = verts[2].x;
        verts[1].y = verts[2].y;
        verts[2].y = vy;
        verts[3].y = vy;
        if (entity->timer >= 1)
            RSDK.DrawQuad(verts, 4, 0xE0, 0xE0, 0xE0, 0xC0, INK_ALPHA);
        else
            RSDK.DrawQuad(verts, 4, entity->r, entity->g, entity->b, 0xC0, INK_ALPHA);

        vertPtr += 2;
    }

    if (entity->type > 0) // da reflection
    {
        int32 x = ((entity->position.x - entity->updateRange.x) >> 16) - screen->position.x;
        int32 y = ((entity->position.y - entity->updateRange.y) >> 16) - screen->position.y;
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, x, y, x + (entity->size.x >> 16), y + (entity->size.y >> 16));

        drawPos.x = entity->position.x - ((entity->position.x - (screen->position.x << 16) - (RSDK_screens->centerX << 16)) >> 1);

        int32 offset = 0x800000 - entity->size.y;
        offset     = minVal(offset, 0x400000);
        offset     = maxVal(offset, 0);

        drawPos.y =
            offset + entity->position.y + entity->field_78 - ((entity->position.y - (screen->centerY << 16) - (screen->position.y << 16)) >> 4);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
}

void ChemicalPool_Create(void *data)
{
    RSDK_THIS(ChemicalPool);
    if (!RSDK_sceneInfo->inEditor) {
        entity->inkEffect     = INK_ADD;
        entity->visible       = true;
        entity->alpha         = 0xFF;
        entity->active        = ACTIVE_BOUNDS;
        entity->drawOrder     = Zone->drawOrderHigh - 2;
        entity->updateRange.x = entity->size.x >> 1;
        entity->updateRange.y = entity->size.y >> 1;
        entity->tileSizeX     = entity->size.x >> 20;

        Vector2 *vertPtr = entity->field_A0;
        for (int32 i = 0; i < 0x2100000; i += 0x100000) {
            int32 x        = i - (entity->size.x >> 1);
            vertPtr[0].x = x;
            vertPtr[1].x = x;

            int32 y        = entity->position.y - (entity->size.y >> 1);
            vertPtr[0].y = y;
            vertPtr[1].y = y + 0x80000;

            vertPtr += 2;
        }

        entity->dword88       = (entity->position.x - (entity->size.x >> 1)) >> 20;
        entity->dword8C       = (entity->position.x + (entity->size.x >> 1)) >> 20;
        entity->dword90       = 0x80;
        entity->dword94       = 0x99;
        entity->r             = 0x00;
        entity->g             = 0x08;
        entity->b             = 0xC0;
        entity->timer         = 0;
        entity->hitbox.left   = -(entity->size.x >> 17);
        entity->hitbox.top    = (entity->type == 0 ? 6 : 0) - (entity->size.y >> 17);
        entity->hitbox.right  = -entity->hitbox.left;
        entity->hitbox.bottom = -entity->hitbox.bottom;
        ChemicalPool_SetupColours();
        RSDK.SetSpriteAnimation(ChemicalPool->aniFrames, 0, &entity->animator, true, 0);
    }
}

void ChemicalPool_StageLoad(void)
{
    ChemicalPool->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE);
    ChemicalPool->sfxChemYellow = RSDK.GetSFX("CPZ/ChemYellow.wav");
    ChemicalPool->sfxChemRed    = RSDK.GetSFX("CPZ/ChemRed.wav");
    ChemicalPool->sfxChemChange = RSDK.GetSFX("CPZ/ChemChange.wav");
}

void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int32 newType, int32 newR, int32 newG, int32 newB)
{
    chemPool->r          = ((newR - chemPool->r) >> 2) + chemPool->r;
    chemPool->b          = chemPool->b + ((newB - chemPool->b) >> 2);
    chemPool->g          = chemPool->g + ((newG - chemPool->g) >> 2);
    chemPool->colours[2] = ((chemPool->g + (chemPool->r << 8)) << 8) + chemPool->b;
    chemPool->colours[3] = ((chemPool->g + (chemPool->r << 8)) << 8) + chemPool->b;

    uint32 val = abs(newR - chemPool->r) + abs(newG - chemPool->g) + abs(newB - chemPool->b) - 1;
    if (val <= 0x1E && chemPool->state != ChemicalPool_State_Change) {
        chemPool->type  = newType;
        chemPool->timer = 4;
        chemPool->state = ChemicalPool_State_Change;
        RSDK.PlaySfx(ChemicalPool->sfxChemChange, 0, 255);
    }
}

void ChemicalPool_ProcessDeformations(void)
{
    RSDK_THIS(ChemicalPool);

    for (int32 i = entity->dword88; i < entity->dword8C; ++i) {
        int32 val = ChemicalPool->table1[i];
        ChemicalPool->table3[i] += (-val >> 5) - (ChemicalPool->table3[i] >> 3);
        ChemicalPool->table2[i] = ChemicalPool->table3[i] + ChemicalPool->table1[i];
    }

    int32 v = entity->dword88 + 1;
    for (int32 i = entity->dword88; i < entity->dword8C; ++i) {
        int32 id2 = v - 2;
        if (id2 - 2 < entity->dword88)
            id2 = entity->dword88;
        if (v >= entity->dword8C)
            v = entity->dword8C;

        ChemicalPool->table1[i] =
            (entity->dword90 * ChemicalPool->table2[i] >> 8) + (entity->dword94 * (ChemicalPool->table2[id2] + ChemicalPool->table2[v]) >> 9);
        ++v;
    }

    entity->field_78 = 0;
    int32 id           = entity->dword88;
    Vector2 *vertPtr = entity->field_A0;
    for (int32 i = 0; i <= entity->tileSizeX; ++i) {
        if (ChemicalPool->table1[id] > entity->field_78)
            entity->field_78 = ChemicalPool->table1[id];
        vertPtr[0].y = ChemicalPool->table1[id] + entity->offsetY;

        int32 val = 2 * ChemicalPool->table1[id] + 0x80000;
        if (val < ChemicalPool->table1[id])
            val = ChemicalPool->table1[id] + 0x10000;
        ++id;
        vertPtr[1].y = val + entity->offsetY;
        vertPtr += 2;
    }
}

void ChemicalPool_SetDeform(int32 x, int32 y)
{
    int32 val = (x + 0x80000) >> 20;
    foreach_active(ChemicalPool, chemPool)
    {
        if (val > chemPool->dword88 && val < chemPool->dword8C) {
            int32 val1 = val - 1;
            if (val1 <= chemPool->dword88)
                val1 = chemPool->dword88 + 1;

            int32 val2 = minVal(val + 2, chemPool->dword8C);

            for (int32 i = 0; i < val2 - val1; ++i) {
                ChemicalPool->table3[val1++] += y;
            }
            foreach_break;
        }
    }
}

void ChemicalPool_SpawnDebris(int32 x, int32 y)
{
    RSDK_THIS(ChemicalPool);

    for (int32 i = 0; i < 6; ++i) {
#if RETRO_USE_PLUS
        EntityDebris *debris =
            CREATE_ENTITY(Debris, NULL, x + RSDK.Random(-0x80000, 0x80000, &Zone->randKey), y + RSDK.Random(0x40000, 0x140000, &Zone->randKey));
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x3800;
        debris->velocity.x   = RSDK.Random(0, 0x20000, &Zone->randKey);
        if (debris->position.x < x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Random(-0x40000, -0x10000, &Zone->randKey);
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Reagent->aniFrames, entity->type + 1, &debris->animator, true, RSDK.Random(0, 2, &Zone->randKey));
#else
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + RSDK.Rand(-0x80000, 0x80000), y + RSDK.Rand(0x40000, 0x140000));
        debris->state      = Debris_State_Fall;
        debris->gravity    = 0x3800;
        debris->velocity.x = RSDK.Rand(0, 0x20000);
        if (debris->position.x < x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Reagent->aniFrames, entity->type + 1, &debris->animator, true, RSDK.Rand(0, 2));
#endif
    }
}

void ChemicalPool_SetupColours(void)
{
    RSDK_THIS(ChemicalPool);

    switch (entity->type) {
        case 0:
        default:
            entity->colours[0] = 0x0060E0;
            entity->colours[1] = 0x0060E0;
            entity->colours[2] = 0x0008C0;
            entity->colours[3] = 0x0008C0;
            entity->g          = 8;
            entity->b          = 0xC0;
            entity->r          = 0;
            entity->state      = ChemicalPool_State_HarmfulBlue;
            entity->timer      = 3;
            break;
        case 1:
            entity->colours[0] = 0x20E020;
            entity->colours[1] = 0x20E020;
            entity->colours[2] = 0x189000;
            entity->colours[3] = 0x189000;
            entity->r          = 0x18;
            entity->g          = 0x90;
            entity->b          = 0;
            entity->state      = ChemicalPool_State_Green;
            entity->timer      = 3;
            break;
        case 2:
            entity->colours[0] = 0x00B8F0;
            entity->colours[1] = 0x00B8F0;
            entity->colours[2] = 0x0080B0;
            entity->colours[3] = 0x0080B0;
            entity->g          = 0x80;
            entity->b          = 0xB0;
            entity->r          = 0;
            entity->state      = ChemicalPool_State_Blue;
            entity->timer      = 3;
            break;
    }
}

void ChemicalPool_State_HarmfulBlue(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();
    int32 topStore = entity->hitbox.top;

    foreach_active(Player, player)
    {
        if (player->velocity.y >= 0 && Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
            ChemicalPool_SetDeform(player->position.x, (player->velocity.y >> 4) + 0x1000);
            if (player->shield != SHIELD_BUBBLE) {
                if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                    && !player->invincibleTimer && player->blinkTimer <= 0) {
                    if (player->position.x > entity->position.x)
                        player->velocity.x = 0x20000;
                    else
                        player->velocity.x = -0x20000;
                    Player_Hit(player);
                }
            }
            if (player->velocity.y < 0 && player->velocity.y > -0x40000)
                player->velocity.y = -0x40000;
        }
    }

    entity->hitbox.top = topStore;
    if (!(Zone->timer & 3)) {
        EntityChemBubble *bubble = (EntityChemBubble *)RSDK.CreateEntity(
            ChemBubble->objectID, NULL, entity->position.x + RSDK.Rand(-entity->updateRange.x, entity->updateRange.x),
            entity->position.y + entity->updateRange.y);
        bubble->parent = (Entity *)entity;
    }
}

void ChemicalPool_State_Green(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            ChemicalPool_SetDeform(player->position.x, player->velocity.y >> 1);
            ChemicalPool_SpawnDebris(player->position.x, player->position.y);
            if (player->state != Player_State_DropDash)
                player->state = Player_State_Air;

            int32 anim = player->playerAnimator.animationID;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->playerAnimator.animationID = ANI_WALK;
            player->onGround    = false;
            player->jumpAbility = 0;
            player->velocity.y  = -0x140000;
            RSDK.PlaySfx(ChemicalPool->sfxChemRed, 0, 255);
        }
    }
}

void ChemicalPool_State_Blue(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();

    foreach_active(Player, player)
    {
        if (player->velocity.y >= 0) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                ChemicalPool_SetDeform(player->position.x, player->velocity.y >> 1);
                ChemicalPool_SpawnDebris(player->position.x, player->position.y);
                if (player->state != Player_State_DropDash)
                    player->state = Player_State_Air;

                int32 anim = player->playerAnimator.animationID;
                if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                    player->playerAnimator.animationID = ANI_WALK;
                if ((-0x10000 - player->velocity.y) > -0x80000)
                    player->velocity.y = -0x10000 - player->velocity.y;
                else
                    player->velocity.y = -0x80000;
                player->onGround    = false;
                player->jumpAbility = 0;
                RSDK.PlaySfx(ChemicalPool->sfxChemYellow, 0, 255);
            }
        }
    }
}

void ChemicalPool_State_Change(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();
    if (++entity->timer2 == 16) {
        foreach_active(ChemBubble, bubble)
        {
            if (abs(bubble->position.x - entity->position.x) <= entity->updateRange.x) {
                if (abs(bubble->position.y - entity->position.y) <= entity->updateRange.y) {
                    RSDK.ResetEntityPtr(bubble, TYPE_BLANK, NULL);
                }
            }
        }
        entity->timer2 = 0;
        ChemicalPool_SetupColours();
        entity->active = ACTIVE_BOUNDS;
    }
}

#if RETRO_INCLUDE_EDITOR
void ChemicalPool_EditorDraw(void) {}

void ChemicalPool_EditorLoad(void) {}
#endif

void ChemicalPool_Serialize(void)
{
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_VECTOR2, size);
}
