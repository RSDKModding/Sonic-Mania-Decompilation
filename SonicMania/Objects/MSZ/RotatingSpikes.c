// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RotatingSpikes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRotatingSpikes *RotatingSpikes;

void RotatingSpikes_Update(void)
{
    RSDK_THIS(RotatingSpikes);

    RSDK.ProcessAnimation(&self->pivotAnimator);
    RSDK.ProcessAnimation(&self->spikeBallAnimator);

    self->angle = (self->angle + self->rotSpeed) & 0x3FF;

    Hitbox *hitbox = RSDK.GetHitbox(&self->spikeBallAnimator, 0);
    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;

    foreach_active(Player, player)
    {
        int32 radius = self->spikeRadius + self->pivotRadius;
        for (int32 i = 0; i < self->spikeCount; ++i) {
            self->position.x = storeX + ((radius * RSDK.Cos1024(self->angle + self->angleOffset)) << 6);
            self->position.y = storeY + ((radius * RSDK.Sin1024(self->angle + self->angleOffset)) << 6);
            if (Player_CheckCollisionTouch(player, self, hitbox)) {
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
            }
            radius += 2 * self->spikeRadius;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void RotatingSpikes_LateUpdate(void) {}

void RotatingSpikes_StaticUpdate(void) {}

void RotatingSpikes_Draw(void)
{
    RSDK_THIS(RotatingSpikes);

    int32 radius = self->spikeRadius + self->pivotRadius;
    RSDK.DrawSprite(&self->pivotAnimator, NULL, false);

    for (int32 i = 0; i < self->spikeCount; ++i) {
        Vector2 drawPos;
        drawPos.x = ((radius * RSDK.Cos1024(self->angleOffset + self->angle)) << 6) + self->position.x;
        drawPos.y = ((radius * RSDK.Sin1024(self->angleOffset + self->angle)) << 6) + self->position.y;
        RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);

        radius += self->spikeRadius << 1;
    }
}

void RotatingSpikes_Create(void *data)
{
    RSDK_THIS(RotatingSpikes);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->angleOffset &= 0x3FF;
        self->drawGroup = self->priority == ROTSPIKES_PRIO_LOW ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];

        RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 0, &self->pivotAnimator, true, 0);
        RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 1, &self->spikeBallAnimator, true, 0);
    }
}

void RotatingSpikes_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MSZ"))
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RotatingSpikes.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ1"))
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/RotatingSpikes.bin", SCOPE_STAGE);
}

#if GAME_INCLUDE_EDITOR
void RotatingSpikes_EditorDraw(void)
{
    RSDK_THIS(RotatingSpikes);

    RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 0, &self->pivotAnimator, true, 0);
    RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 1, &self->spikeBallAnimator, true, 0);

    self->drawGroup = self->priority == ROTSPIKES_PRIO_LOW ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];

    RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 0, &self->pivotAnimator, true, 0);
    RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 1, &self->spikeBallAnimator, true, 0);

    int32 radius = self->spikeRadius + self->pivotRadius;
    RSDK.DrawSprite(&self->pivotAnimator, NULL, false);

    for (int32 i = 0; i < self->spikeCount; ++i) {
        Vector2 drawPos;
        drawPos.x = ((radius * RSDK.Cos1024((self->angleOffset & 0x3FF) + self->angle)) << 6) + self->position.x;
        drawPos.y = ((radius * RSDK.Sin1024((self->angleOffset & 0x3FF) + self->angle)) << 6) + self->position.y;
        RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);

        radius += self->spikeRadius << 1;
    }
}

void RotatingSpikes_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("MSZ"))
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RotatingSpikes.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ1"))
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/RotatingSpikes.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(RotatingSpikes, priority);
    RSDK_ENUM_VAR("Low", ROTSPIKES_PRIO_LOW);
    RSDK_ENUM_VAR("High", ROTSPIKES_PRIO_HIGH);
}
#endif

void RotatingSpikes_Serialize(void)
{
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, pivotRadius);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, spikeRadius);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, spikeCount);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, rotSpeed);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, angleOffset);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, priority);
}
