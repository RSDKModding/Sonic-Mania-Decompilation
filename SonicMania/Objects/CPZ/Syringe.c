// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Syringe Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSyringe *Syringe;

void Syringe_Update(void)
{
    RSDK_THIS(Syringe);

    if (self->activated) {
        if (self->offsetY < 0x280000) {
            self->offsetY += 0x8000;
            if (!(Zone->timer & 3))
                CREATE_ENTITY(Reagent, INT_TO_VOID(self->type), self->position.x, self->position.y + 0x680000);
        }

        self->active = ACTIVE_NORMAL;
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active  = ACTIVE_BOUNDS;
        self->offsetY = 0;
    }

    foreach_active(Player, player)
    {
        Player_CheckCollisionPlatform(player, self, &Syringe->hitboxBody);
        self->position.y += self->offsetY;

        if (Player_CheckCollisionPlatform(player, self, &Syringe->hitboxHandle)) {
            self->activated = true;
            player->position.y += 0x10000;

            EntityCamera *camera = player->camera;
            if (camera) {
                if (camera->lookPos.y < 96) {
                    camera->lookPos.y += 8;
                    camera->lookPos.y = MIN(camera->lookPos.y, 80);
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
    drawPos.y = self->position.y + self->offsetY;
    RSDK.DrawRect(drawPos.x - 0x100000, drawPos.y, 0x200000, 0x4E0000 - self->offsetY, self->color, 0xC0, INK_ALPHA, false);
    RSDK.DrawSprite(&self->handleAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->containerAnimator, NULL, false);
}

void Syringe_Create(void *data)
{
    RSDK_THIS(Syringe);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        switch (self->type) {
            case CHEMICALPOOL_BLUE: self->color = 0x0008C0; break;
            case CHEMICALPOOL_GREEN: self->color = 0x189000; break;
            case CHEMICALPOOL_CYAN: self->color = 0x0080B0; break;
        }

        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->handleAnimator, true, 0);
        RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->containerAnimator, true, 1);
    }
}

void Syringe_StageLoad(void)
{
    Syringe->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);

    Syringe->hitboxBody.left   = -16;
    Syringe->hitboxBody.top    = 0;
    Syringe->hitboxBody.right  = 16;
    Syringe->hitboxBody.bottom = 8;

    Syringe->hitboxHandle.left   = -24;
    Syringe->hitboxHandle.top    = -49;
    Syringe->hitboxHandle.right  = 24;
    Syringe->hitboxHandle.bottom = -40;

    Syringe->sfxBloop = RSDK.GetSfx("Stage/Bloop.wav");
}

#if GAME_INCLUDE_EDITOR
void Syringe_EditorDraw(void)
{
    RSDK_THIS(Syringe);

    switch (self->type) {
        case CHEMICALPOOL_BLUE: self->color = 0x0008C0; break;
        case CHEMICALPOOL_GREEN: self->color = 0x189000; break;
        case CHEMICALPOOL_CYAN: self->color = 0x0080B0; break;
    }

    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->handleAnimator, true, 0);
    RSDK.SetSpriteAnimation(Syringe->aniFrames, 0, &self->containerAnimator, true, 1);

    Syringe_Draw();
}

void Syringe_EditorLoad(void)
{
    Syringe->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Syringe, type);
    RSDK_ENUM_VAR("Blue", CHEMICALPOOL_BLUE);
    RSDK_ENUM_VAR("Green", CHEMICALPOOL_GREEN);
    RSDK_ENUM_VAR("Cyan", CHEMICALPOOL_CYAN);
}
#endif

void Syringe_Serialize(void)
{
    RSDK_EDITABLE_VAR(Syringe, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Syringe, VAR_UINT8, tag);
}
