#include "SonicMania.h"

ObjectSpear *Spear;

void Spear_Update(void)
{
    RSDK_THIS(Spear);
    int32 timer = (entity->intervalOffset + Zone->timer) % entity->interval;
    if (timer >= entity->duration) {
        if (timer - entity->duration >= 6)
            entity->field_7C = 0;
        else
            entity->field_7C = 0x48000 * (6 - (timer - entity->duration));
    }
    else {
        if (timer >= 6)
            entity->field_7C = 0x1B0000;
        else
            entity->field_7C = 0x48000 * timer;
        if (entity->field_7C == 0x48000)
            RSDK.PlaySfx(Spear->sfxSpear, 0, 255);
    }
    Spear_Unknown1();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(512, player, 2, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }
    }
}

void Spear_LateUpdate(void) {}

void Spear_StaticUpdate(void) {}

void Spear_Draw(void)
{
    RSDK_THIS(Spear);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    switch (entity->orientation) {
        case 0:
            drawPos.y = entity->position.y + 0x1B0000;
            drawPos.y = entity->position.y + 0x1B0000 - entity->field_7C;
            break;
        case 1:
            drawPos.x = entity->position.x - 0x1B0000;
            drawPos.x = entity->field_7C + entity->position.x - 0x1B0000;
            break;
        case 2:
            drawPos.y = entity->position.y - 0x1B0000;
            drawPos.y = entity->field_7C + entity->position.y - 0x1B0000;
            break;
        case 3:
            drawPos.x = entity->position.x + 0x1B0000;
            drawPos.x = entity->position.x + 0x1B0000 - entity->field_7C;
            break;
        default: break;
    }
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Spear_Create(void *data)
{
    RSDK_THIS(Spear);
    if (RSDK_sceneInfo->inEditor) {
        if (!entity->interval)
            entity->interval = 120;
        if (!entity->duration)
            entity->duration = 60;
    }
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    int32 anim = 0;
    switch (entity->orientation) {
        case 0:
            entity->direction = FLIP_NONE;
            anim              = 0;
            break;
        case 1:
            entity->direction = FLIP_NONE;
            anim              = 1;
            break;
        case 2:
            entity->direction = FLIP_Y;
            anim              = 0;
            break;
        case 3:
            entity->direction = FLIP_X;
            anim              = 1;
            break;
        default: break;
    }
    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &entity->animator, true, 0);
    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &entity->animator2, true, 1);
}

void Spear_StageLoad(void)
{
    Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE);
    Spear->sfxSpear  = RSDK.GetSFX("HCZ/Spear.wav");
}

void Spear_Unknown1(void)
{
    RSDK_THIS(Spear);
    switch (entity->orientation) {
        case 0:
        case 2:
            entity->hitbox.right  = 4;
            entity->hitbox.bottom = 0;
            entity->hitbox.left   = -4;
            entity->hitbox.top    = -15 - (entity->field_7C >> 16);
            break;
        case 1:
        case 3:
            entity->hitbox.left   = 0;
            entity->hitbox.top    = 64;
            entity->hitbox.right  = (entity->field_7C >> 16) + 15;
            entity->hitbox.bottom = 4;
            break;
        default: break;
    }
}

#if RETRO_INCLUDE_EDITOR
void Spear_EditorDraw(void) { Spear_Draw(); }

void Spear_EditorLoad(void) { Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE); }
#endif

void Spear_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spear, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, interval);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, intervalOffset);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, duration);
}
