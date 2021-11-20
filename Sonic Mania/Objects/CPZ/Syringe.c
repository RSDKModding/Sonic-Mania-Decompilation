#include "SonicMania.h"

ObjectSyringe *Syringe;

void Syringe_Update(void)
{
    RSDK_THIS(Syringe);
    if (self->activated) {
        if (self->offsetY < 0x280000) {
            self->offsetY += 0x8000;
            if (!(Zone->timer & 3))
                CREATE_ENTITY(Reagent, intToVoid(self->type), self->position.x, self->position.y + 0x680000);
        }
        self->active = ACTIVE_NORMAL;
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active  = ACTIVE_BOUNDS;
        self->offsetY = 0;
    }

    foreach_active(Player, player)
    {
        Player_CheckCollisionPlatform(player, self, &Syringe->bodyHitbox);
        self->position.y += self->offsetY;
        if (Player_CheckCollisionPlatform(player, self, &Syringe->handleHitbox)) {
            self->activated = true;
            player->position.y += 0x10000;
            EntityCamera *camera = player->camera;
            if (camera) {
                if (camera->lookPos.y < 96) {
                    camera->lookPos.y += 8;
                    camera->lookPos.y = minVal(camera->lookPos.y, 80);
                }
            }
        }
        self->position.y -= self->offsetY;
    }
}

void Syringe_LateUpdate(void) {}

void Syringe_StaticUpdate(void) {}

void Syringe_Draw(void)
{
    RSDK_THIS(Syringe);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->offsetY + self->position.y;
    RSDK.DrawRect(drawPos.x - 0x100000, drawPos.y, 0x200000, 0x4E0000 - self->offsetY, self->colour, 0xC0, INK_ALPHA, false);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Syringe_Create(void *data)
{
    RSDK_THIS(Syringe);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        switch (self->type) {
            case 0: self->colour = 0x0008C0; break;
            case 1: self->colour = 0x189000; break;
            case 2: self->colour = 0x0080B0; break;
        }

        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->animator1, true, 1);
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
    switch (self->type) {
        case 0: self->colour = 0x0008C0; break;
        case 1: self->colour = 0x189000; break;
        case 2: self->colour = 0x0080B0; break;
    }

    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->animator1, true, 1);

    Syringe_Draw();
}

void Syringe_EditorLoad(void) { Syringe->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE); }
#endif

void Syringe_Serialize(void)
{
    RSDK_EDITABLE_VAR(Syringe, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Syringe, VAR_UINT8, tag);
}
