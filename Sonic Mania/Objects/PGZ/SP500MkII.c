#include "SonicMania.h"

ObjectSP500MkII *SP500MkII;

void SP500MkII_Update(void)
{
    RSDK_THIS(SP500MkII);
    entity->pos2.x = entity->position.x + entity->field_98;
    entity->pos2.y = entity->position.y + entity->field_9C;

    StateMachine_Run(entity->state);

    entity->field_74 = false;
    entity->field_70 = false;
    if (entity->field_7C && entity->field_68 < entity->printRows)
        entity->field_70 = true;
    if (entity->field_78 && !entity->field_7C && entity->state != SP500MkII_Unknown8 && entity->field_68 < entity->printRows)
        entity->field_74 = true;
    RSDK.ProcessAnimation(&entity->animator);
}

void SP500MkII_LateUpdate(void) {}

void SP500MkII_StaticUpdate(void) {}

void SP500MkII_Draw(void)
{
    SP500MkII_Unknown3();
    SP500MkII_Unknown2();
    SP500MkII_Unknown1();
}

void SP500MkII_Create(void *data)
{
    RSDK_THIS(SP500MkII);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->pos           = entity->position;
    entity->drawFX        = FX_FLIP;
    entity->visible       = true;
    entity->updateRange.x = (entity->length + 16) << 19;
    entity->updateRange.y = (entity->printRows + 4) << 21;
    entity->state         = SP500MkII_Unknown5;
}

void SP500MkII_StageLoad(void)
{
    SP500MkII->aniFrames      = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE);
    SP500MkII->hitbox1.top    = -24;
    SP500MkII->hitbox1.left   = -32;
    SP500MkII->hitbox1.right  = 32;
    SP500MkII->hitbox1.bottom = 23;
    SP500MkII->hitbox2.top    = -38;
    SP500MkII->hitbox2.left   = -32;
    SP500MkII->hitbox2.right  = 32;
    SP500MkII->hitbox2.bottom = -24;
    SP500MkII->hitbox3.top    = 24;
    SP500MkII->hitbox3.left   = -23;
    SP500MkII->hitbox3.right  = 22;
    SP500MkII->hitbox3.bottom = 39;
}

void SP500MkII_Unknown1(void)
{
    RSDK_THIS(SP500MkII);
    if (RSDK_sceneInfo->inEditor || DebugMode->debugActive) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;
        entity->position.x += entity->field_98;
        entity->position.y += entity->field_9C;
        DrawHelpers_DrawHitboxOutline(0xFFFF00, entity->direction, entity->position.x, entity->position.y, &SP500MkII->hitbox2);
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, entity->direction, entity->position.x, entity->position.y, &SP500MkII->hitbox1);
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, entity->direction, entity->position.x, entity->position.y, &SP500MkII->hitbox3);
        entity->position.x = storeX;
        entity->position.y = storeY;
    }
}

void SP500MkII_Unknown2(void)
{
    RSDK_THIS(SP500MkII);
    int storeX = entity->position.x;
    int storeY = entity->position.y;
    entity->position.x += entity->field_98;
    entity->position.y += entity->field_9C;

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->field_84 + entity->position.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 1, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.y -= entity->field_84;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 0, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    if (entity->field_70) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 255;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 2, &entity->animator, true, 0);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->inkEffect = INK_NONE;
    }
    if (entity->field_74) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 255;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 3, &entity->animator, true, 0);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->inkEffect = INK_NONE;
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void SP500MkII_Unknown3(void)
{
    RSDK_THIS(SP500MkII);

    Vector2 drawPos;
    drawPos.x = entity->position.x - 0x280000;
    drawPos.y = entity->field_9C + entity->position.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 5, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x += 0x80000;
    entity->animator.frameID = 1;
    for (int l = 0; l < entity->length + 8; ++l) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }

    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x += 0x80000;
    entity->direction = FLIP_X;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->direction = FLIP_NONE;
}

void SP500MkII_Unknown4(void)
{
    RSDK_THIS(SP500MkII);

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->pos2.x;
    entity->position.y = entity->pos2.y;
    int top            = SP500MkII->hitbox2.top << 16;

    int pos = 0;
    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        int bottom   = player->position.y + (Player_GetHitbox(player)->bottom << 16);
        Player_CheckCollisionBox(player, entity, &SP500MkII->hitbox3);
        int side        = Player_CheckCollisionBox(player, entity, &SP500MkII->hitbox1);
        bool32 collided = side == C_TOP;
        bool32 touched  = Player_CheckCollisionTouch(player, entity, &SP500MkII->hitbox2);

        if (collided) {
            player->position.x += entity->field_A0;
            player->position.y += entity->field_A4;
        }

        if (touched) {
            int v12 = minVal(bottom - entity->position.y + top, 0xD0000);

            if (pos <= v12) {
                pos = v12;
            }
            if (collided)
                entity->activePlayers |= (1 << playerID);
            else
                entity->activePlayers &= ~(1 << playerID);
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }
    }

    entity->field_84   = pos;
    entity->field_78   = entity->activePlayers != 0;
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void SP500MkII_Unknown5(void)
{
    RSDK_THIS(SP500MkII);
    entity->field_98 = 0;
    entity->field_9C = 0;
    if (entity->start == 1)
        entity->field_98 = entity->length << 19;
    else
        entity->field_98 = 0;
    entity->timer         = 0;
    entity->activePlayers = 0;
    entity->field_84      = 0;
    entity->field_78      = 0;
    entity->field_7C      = 0;
    entity->field_6C      = 2 * (entity->start == 0) - 1;
    entity->field_70      = false;
    entity->field_74      = false;
    entity->state         = SP500MkII_Unknown6;
}

void SP500MkII_Unknown6(void)
{
    RSDK_THIS(SP500MkII);
    entity->field_A0 = 0;
    entity->field_A4 = 0;
    SP500MkII_Unknown4();

    if (entity->activePlayers & 1) {
        ++entity->timer;
    }
    else {
        if (entity->field_7C) {
            if (entity->field_68 < entity->printRows)
                entity->state = SP500MkII_Unknown7;
        }
        entity->timer = 0;
    }
    if (entity->timer == 30)
        entity->field_7C = 1;
}

void SP500MkII_Unknown7(void)
{
    RSDK_THIS(SP500MkII);
    entity->field_A0 = entity->field_98;
    entity->field_A4 = entity->field_9C;

    if (entity->field_6C <= 0) {
        if (entity->field_98 > 0)
            entity->field_98 -= 0x40000;
        if (entity->field_98 < 0)
            entity->field_98 = 0;

        if (!entity->field_98) {
            entity->field_6C = 1;
            ++entity->field_68;
            entity->state    = SP500MkII_Unknown8;
            entity->field_7C = 0;
        }
    }
    else {
        if (entity->field_98 < entity->length << 19)
            entity->field_98 += 0x40000;
        if (entity->field_98 > entity->length << 19)
            entity->field_98 = entity->length << 19;

        if (entity->field_98 == entity->length << 19) {
            entity->field_6C = -1;
            ++entity->field_68;
            entity->state    = SP500MkII_Unknown8;
            entity->field_7C = 0;
        }
    }

    entity->field_A0 = entity->field_98 - entity->field_A0;
    entity->field_A4 = 0;
    SP500MkII_Unknown4();
}

void SP500MkII_Unknown8(void)
{
    RSDK_THIS(SP500MkII);
    entity->field_A0 = entity->field_98;
    entity->field_A4 = entity->field_9C;

    int dist = (entity->field_68 * (2 * (entity->yDir != 0) - 1)) << 21;
    if (entity->yDir == 1) {
        if (entity->field_9C < dist)
            entity->field_9C += 0x40000;

        if (entity->field_9C > dist)
            entity->field_9C = dist;
    }
    else {
        if (entity->field_9C > dist)
            entity->field_9C -= 0x40000;

        if (entity->field_9C < dist)
            entity->field_9C = dist;
    }

    if (entity->field_9C == dist)
        entity->state = SP500MkII_Unknown6;
    entity->field_A0 = 0;
    entity->field_A4 = entity->field_9C - entity->field_A4;
    SP500MkII_Unknown4();
}

void SP500MkII_EditorDraw(void)
{
    SP500MkII_Unknown3();
    SP500MkII_Unknown2();
    SP500MkII_Unknown1();
}

void SP500MkII_EditorLoad(void) { SP500MkII->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE); }

void SP500MkII_Serialize(void)
{
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, start);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, yDir);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, printRows);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, length);
}
