#include "SonicMania.h"

ObjectSyringe *Syringe;

void Syringe_Update(void)
{
    RSDK_THIS(Syringe);
    if (entity->activated) {
        if (entity->offsetY < 0x280000) {
            entity->offsetY += 0x8000;
            if (!(Zone->timer & 3))
                RSDK.CreateEntity(Reagent->objectID, intToVoid(entity->type), entity->position.x, entity->position.y + 0x680000);
        }
        entity->active = ACTIVE_NORMAL;
    }
    else if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->active  = ACTIVE_BOUNDS;
        entity->offsetY = 0;
    }

    foreach_active(Player, player)
    {
        Player_CheckCollisionPlatform(player, entity, &Syringe->bodyHitbox);
        entity->position.y += entity->offsetY;
        if (Player_CheckCollisionPlatform(player, entity, &Syringe->handleHitbox)) {
            entity->activated = true;
            player->position.y += 0x10000;
            EntityCamera *camera = player->camera;
            if (camera) {
                if (camera->lookPos.y < 96) {
                    camera->lookPos.y += 8;
                    camera->lookPos.y = minVal(camera->lookPos.y, 80);
                }
            }
        }
        entity->position.y -= entity->offsetY;
    }
}

void Syringe_LateUpdate(void) {}

void Syringe_StaticUpdate(void) {}

void Syringe_Draw(void)
{
    RSDK_THIS(Syringe);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->offsetY + entity->position.y;
    RSDK.DrawRect(drawPos.x - 0x100000, drawPos.y, 0x200000, 0x4E0000 - entity->offsetY, entity->colour, 0xC0, INK_ALPHA, false);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Syringe_Create(void *data)
{
    RSDK_THIS(Syringe);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;

        switch (entity->type) {
            case 0: entity->colour = 0x0008C0; break;
            case 1: entity->colour = 0x189000; break;
            case 2: entity->colour = 0x0080B0; break;
        }

        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &entity->animator1, true, 1);
    }
}

void Syringe_StageLoad(void)
{
    Syringe->aniFrames           = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);
    Syringe->bodyHitbox.left     = -0x10;
    Syringe->bodyHitbox.top      = 0x00;
    Syringe->bodyHitbox.right    = 0x10;
    Syringe->bodyHitbox.bottom   = 0x08;
    Syringe->handleHitbox.left   = -0x18;
    Syringe->handleHitbox.top    = -0x31;
    Syringe->handleHitbox.right  = 0x18;
    Syringe->handleHitbox.bottom = -0x27;
    Syringe->sfxBloop            = RSDK.GetSFX("Stage/Bloop.wav");
}

#if RETRO_INCLUDE_EDITOR
void Syringe_EditorDraw(void)
{
    RSDK_THIS(Syringe);
    switch (entity->type) {
        case 0: entity->colour = 0x0008C0; break;
        case 1: entity->colour = 0x189000; break;
        case 2: entity->colour = 0x0080B0; break;
    }

    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &entity->animator1, true, 1);

    Syringe_Draw();
}

void Syringe_EditorLoad(void) { Syringe->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE); }
#endif

void Syringe_Serialize(void)
{
    RSDK_EDITABLE_VAR(Syringe, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Syringe, VAR_UINT8, tag);
}
