// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HPZEmerald Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHPZEmerald *HPZEmerald;

void HPZEmerald_Update(void)
{
    RSDK_THIS(HPZEmerald);

    RSDK.ProcessAnimation(&self->emeraldAnimator);

    if (self->solid) {
        if (self->type != HPZEMERALD_MASTER) {
            foreach_active(Player, player) { Player_CheckCollisionPlatform(player, self, self->hitbox); }
        }

        foreach_active(HeavyKing, king)
        {
            if (king->bodyAnimator.animationID != 5 && !king->onGround
                && RSDK.CheckObjectCollisionPlatform(self, self->hitbox, king, &HeavyKing->hitboxBody, true)) {
                Camera_ShakeScreen(0, 0, 3);
                RSDK.PlaySfx(HeavyKing->sfxImpact2, false, 255);
            }
        }
    }

    if (!self->onGround) {
        self->velocity.y += 0x3800;
        self->position.y += self->velocity.y;

        if (self->position.y >= self->startPos.y && self->velocity.y > 0) {
            self->position.y = self->startPos.y;
            self->velocity.y = -(self->velocity.y >> 1);

            if (!self->velocity.y)
                self->onGround = true;
        }
    }
}

void HPZEmerald_LateUpdate(void) {}

void HPZEmerald_StaticUpdate(void) {}

void HPZEmerald_Draw(void)
{
    RSDK_THIS(HPZEmerald);

    self->inkEffect = INK_NONE;

    if (self->solid) {
        RSDK.DrawSprite(&self->emeraldAnimator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->emeraldAnimator, NULL, false);

        self->inkEffect = INK_ADD;
        RSDK.DrawSprite(&self->overlayAnimator, NULL, false);
    }
}

void HPZEmerald_Create(void *data)
{
    RSDK_THIS(HPZEmerald);

    if (!SceneInfo->inEditor) {
        self->visible = true;

        switch (self->type) {
            default:
            case HPZEMERALD_MASTER: self->drawGroup = 1; break;

            case HPZEMERALD_EMERALD_LOW:
                self->solid     = true;
                self->drawGroup = Zone->objectDrawGroup[0];
                break;

            case HPZEMERALD_EMERALD_HIGH:
                self->solid     = true;
                self->drawGroup = Zone->objectDrawGroup[1];
                break;
        }

        self->startPos      = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->onGround      = true;

        if (self->type != HPZEMERALD_MASTER) {
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 1, &self->emeraldAnimator, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &self->emeraldAnimator, true, 0);
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &self->overlayAnimator, true, 1);
        }

        self->hitbox = RSDK.GetHitbox(&self->emeraldAnimator, 0);
    }
}

void HPZEmerald_StageLoad(void) { HPZEmerald->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void HPZEmerald_EditorDraw(void)
{
    RSDK_THIS(HPZEmerald);

    self->solid = false;
    switch (self->type) {
        default:
        case HPZEMERALD_MASTER:
        case HPZEMERALD_EMERALD_LOW: self->solid = true; break;

        case HPZEMERALD_EMERALD_HIGH: self->solid = true; break;
    }

    if (self->type) {
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 1, &self->emeraldAnimator, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &self->emeraldAnimator, true, 0);
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &self->overlayAnimator, true, 1);
    }

    HPZEmerald_Draw();
}

void HPZEmerald_EditorLoad(void)
{
    HPZEmerald->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HPZEmerald, type);
    RSDK_ENUM_VAR("Master Emerald", HPZEMERALD_MASTER);
    RSDK_ENUM_VAR("Stone Emerald (Low Group)", HPZEMERALD_EMERALD_LOW);
    RSDK_ENUM_VAR("Stone Emerald (High Group)", HPZEMERALD_EMERALD_HIGH);
}
#endif

void HPZEmerald_Serialize(void) { RSDK_EDITABLE_VAR(HPZEmerald, VAR_UINT8, type); }
