#include "SonicMania.h"

ObjectDirectorChair *DirectorChair;

void DirectorChair_Update(void) { Platform_Update(); }

void DirectorChair_LateUpdate(void) {}

void DirectorChair_StaticUpdate(void) {}

void DirectorChair_Draw(void)
{
    RSDK_THIS(DirectorChair);
    Vector2 drawPos;

    int sin = RSDK.Sin512(entity->rotation);

    drawPos                  = entity->centerPos;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
    RSDK.DrawSprite(&entity->animator, &entity->centerPos, false);

    drawPos.x += -0x10000 - (RSDK.Cos512(entity->rotation) << 11);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->direction = FLIP_X;
    entity->rotation  = 0x100 - entity->rotation;
    drawPos.y += sin << 12;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->direction = FLIP_NONE;
    entity->rotation  = 0x100 - entity->rotation;

    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_X;
        entity->rotation  = 0x100 - entity->rotation;
        drawPos.y += sin << 12;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        entity->rotation  = 0x100 - entity->rotation;
    }
}

void DirectorChair_Create(void *data)
{
    RSDK_THIS(DirectorChair);

    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(DirectorChair->aniFrames, 1, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->centerPos     = entity->position;
        entity->drawPos.x     = entity->position.x;
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawFX        = FX_ROTATE | FX_FLIP;
        entity->drawPos.y     = entity->position.y;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (entity->size + 1);
        entity->drawOrder     = Zone->drawOrderLow;
        entity->field_E8      = (entity->size >> 2) - 16;
        if (!entity->type)
            entity->rotation = entity->field_E8;
        else
            entity->rotation = -96;
        entity->angle        = entity->rotation << 8;
        entity->state        = DirectorChair_Unknown2;
        entity->stateCollide = DirectorChair_StateCollide_Chair;
        entity->speed <<= 7;
        entity->field_EC = maxVal(32 - 2 * entity->size, 1);
    }
}

void DirectorChair_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        DirectorChair->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/DirectorChair.bin", SCOPE_STAGE);
    DirectorChair->hitbox1.top    = -18;
    DirectorChair->hitbox1.left   = -18;
    DirectorChair->hitbox1.right  = 18;
    DirectorChair->hitbox1.bottom = -4;
    DirectorChair->hitbox2.top    = -35;
    DirectorChair->hitbox2.left   = -24;
    DirectorChair->hitbox2.right  = -16;
    DirectorChair->hitbox2.bottom = -3;
    DirectorChair->hitbox3.top    = -35;
    DirectorChair->hitbox3.left   = 16;
    DirectorChair->hitbox3.right  = 24;
    DirectorChair->hitbox3.bottom = -3;
    DirectorChair->field_3        = 0;
    DirectorChair->field_4        = 0;
    DirectorChair->sfxUnravel     = RSDK.GetSFX("Stage/Unravel.wav");
    DirectorChair->sfxExtend      = RSDK.GetSFX("Stage/Extend.wav");
    DirectorChair->sfxRetract     = RSDK.GetSFX("Stage/Retract.wav");
}

void DirectorChair_StateCollide_Chair(void)
{
    RSDK_THIS(DirectorChair);

    int playerID         = 0;
    entity->stoodPlayers = 0;
    foreach_active(Player, player)
    {
        bool32 prevOnGround = player->onGround;
        if (Player_CheckCollisionPlatform(player, entity, &DirectorChair->hitbox1)) {
#if RETRO_USE_PLUS
            if (entity->state == DirectorChair_Unknown2 && player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif
            entity->collapseDelay = 0;
            if (!prevOnGround) {
                entity->stoodPlayers |= 1 << playerID;
                entity->stood = true;
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
        if (Player_CheckCollisionBox(player, entity, &DirectorChair->hitbox2) == C_TOP) {
            entity->collapseDelay = 0;
            if (!prevOnGround) {
                entity->stoodPlayers |= 1 << playerID;
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
        if (Player_CheckCollisionBox(player, entity, &DirectorChair->hitbox3) == C_TOP) {
            entity->collapseDelay = 0;
            if (!prevOnGround) {
                entity->stoodPlayers |= 1 << playerID;
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }

        Hitbox hitbox;
        hitbox.top    = 0;
        hitbox.bottom = (entity->centerPos.y - entity->drawPos.y) >> 16;
        hitbox.right  = (RSDK.Cos512(entity->rotation) >> 5) + 8;
        hitbox.left   = -hitbox.right;
        Player_CheckCollisionBox(player, entity, &hitbox);
        ++playerID;
    }
}

void DirectorChair_Unknown2(void)
{
    RSDK_THIS(DirectorChair);

    int sin           = RSDK.Sin512(entity->rotation);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y + (sin << 8) + (sin << 12) * (entity->size + 1);

    if (entity->stood) {
        entity->active   = ACTIVE_NORMAL;
        entity->field_E4 = 512;
        if (!entity->type) {
            RSDK.PlaySfx(DirectorChair->sfxUnravel, false, 255);
            entity->state = DirectorChair_Unknown3;
        }
        else {
            ++DirectorChair->field_4;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 255);
            entity->state = DirectorChair_Unknown6;
        }
    }
}

void DirectorChair_Unknown3(void)
{
    RSDK_THIS(DirectorChair);

    entity->angle += entity->field_E4;
    entity->field_E4 -= entity->field_EC;
    entity->rotation = entity->angle >> 8;
    if (entity->rotation >= 0) {
        entity->rotation = 0;
        entity->angle    = 0;
        entity->field_E4 = -entity->field_EC;
        ++DirectorChair->field_3;
        RSDK.PlaySfx(DirectorChair->sfxExtend, false, 255);
        entity->state = DirectorChair_Unknown4;
    }

    int sin           = RSDK.Sin512(entity->rotation);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y + (sin << 8) + (sin << 12) * (entity->size + 1);
}

void DirectorChair_Unknown4(void)
{
    RSDK_THIS(DirectorChair);

    entity->angle -= entity->field_E4;
    if (entity->field_E4 != entity->speed) {
        if (entity->field_E4 >= entity->speed) {
            entity->speed -= 32;
            if (entity->field_E4 < entity->speed)
                entity->field_E4 = entity->speed;
        }
        else {
            entity->field_E4 += 0x20;
            if (entity->field_E4 > entity->speed)
                entity->field_E4 = entity->speed;
        }
    }

    entity->rotation = entity->angle >> 8;
    if (entity->rotation <= -96) {
        entity->rotation = -96;
        if (!--DirectorChair->field_3)
            RSDK.StopSFX(DirectorChair->sfxExtend);
        if (entity->type) {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = DirectorChair_Unknown2;
        }
        else {
            entity->state = DirectorChair_Unknown5;
        }
    }

    int sin           = RSDK.Sin512(entity->rotation);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y + (sin << 8) + (sin << 12) * (entity->size + 1);
}

void DirectorChair_Unknown5(void)
{
    RSDK_THIS(DirectorChair);

    if (++entity->collapseDelay >= 60) {
        entity->collapseDelay = 0;
        entity->field_E4      = 0;
        if (!entity->type) {
            ++DirectorChair->field_4;
            RSDK.PlaySfx(DirectorChair->sfxRetract, false, 255);
            entity->state = DirectorChair_Unknown6;
        }
        else {
            ++DirectorChair->field_3;
            RSDK.PlaySfx(DirectorChair->sfxExtend, false, 255);
            entity->state = DirectorChair_Unknown4;
        }
    }

    int sin           = RSDK.Sin512(entity->rotation);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y + (sin << 8) + (sin << 12) * (entity->size + 1);
}

void DirectorChair_Unknown6(void)
{
    RSDK_THIS(DirectorChair);

    entity->angle += entity->field_E4;

    if (entity->field_E4 != entity->speed) {
        if (entity->field_E4 >= entity->speed) {
            entity->field_E4 -= 32;
            if (entity->field_E4 < entity->speed)
                entity->field_E4 = entity->speed;
        }
        else {
            entity->field_E4 += 0x20;
            if (entity->field_E4 > entity->speed)
                entity->field_E4 = entity->speed;
        }
    }

    entity->rotation = entity->angle >> 8;
    if (entity->rotation >= entity->field_E8) {
        if (!--DirectorChair->field_4)
            RSDK.StopSFX(DirectorChair->sfxRetract);
        entity->rotation = entity->field_E8;
        if (!entity->type) {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = DirectorChair_Unknown2;
        }
        else {
            entity->state = DirectorChair_Unknown5;
        }
    }

    int sin           = RSDK.Sin512(entity->rotation);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y + (sin << 8) + (sin << 12) * (entity->size + 1);
}

#if RETRO_INCLUDE_EDITOR
void DirectorChair_EditorDraw(void)
{
    RSDK_THIS(DirectorChair);
    entity->centerPos     = entity->position;
    entity->drawPos.x     = entity->position.x;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->drawPos.y     = entity->position.y;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = ((RSDK.Sin512(96) + 0x8000) << 8) + (RSDK.Sin512(96) << 12) * (entity->size + 1);
    entity->drawOrder     = Zone->drawOrderLow;
    entity->field_E8      = (entity->size >> 2) - 16;
    if (!entity->type)
        entity->rotation = entity->field_E8;
    else
        entity->rotation = -96;
    entity->angle = entity->rotation << 8;
    entity->field_EC = maxVal(32 - 2 * entity->size, 1);

    DirectorChair_Draw();
}

void DirectorChair_EditorLoad(void)
{
    DirectorChair->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/DirectorChair.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DirectorChair, type);
    RSDK_ENUM_VAR("Closed", DIRECTORCHAIR_CLOSED);
    RSDK_ENUM_VAR("Stretched Out", DIRECTORCHAIR_STRETCHED);
}
#endif

void DirectorChair_Serialize(void)
{
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(DirectorChair, VAR_ENUM, size);
}
