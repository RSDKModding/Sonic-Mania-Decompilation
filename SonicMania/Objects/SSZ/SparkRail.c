// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SparkRail Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSparkRail *SparkRail;

void SparkRail_Update(void)
{
    RSDK_THIS(SparkRail);

    int32 posY = self->position.y + (self->size.y >> 1);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround) {
            if (abs(player->groundVel) > 0x80000) {
                RSDK.PlaySfx(SparkRail->sfxPon, false, 255);

                EntityDebris *spark = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x, self->position.y);
                RSDK.SetSpriteAnimation(SparkRail->aniFrames, 0, &spark->animator, true, 0);
                spark->drawFX     = FX_FLIP;
                spark->direction  = player->direction != FLIP_NONE;
                spark->drawGroup  = Zone->objectDrawGroup[0];
                spark->position.x = player->position.x;
                spark->position.y = posY;
                spark->timer      = 30;
            }
        }
    }
}

void SparkRail_LateUpdate(void) {}

void SparkRail_StaticUpdate(void) {}

void SparkRail_Draw(void) {}

void SparkRail_Create(void *data)
{
    RSDK_THIS(SparkRail);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = false;

    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.y >> 17;

    if (!self->size.x)
        self->size.x = 128 << 16;

    if (!self->size.y)
        self->size.y = 32 << 16;
}

void SparkRail_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Spark.bin", SCOPE_STAGE);
    else
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Spark.bin", SCOPE_STAGE);

    SparkRail->sfxPon = RSDK.GetSfx("Stage/Pon.wav");
}

#if GAME_INCLUDE_EDITOR
void SparkRail_EditorDraw(void)
{
    RSDK_THIS(SparkRail);

    Animator animator;
    RSDK.SetSpriteAnimation(SparkRail->aniFrames, 0, &animator, true, 0);

    RSDK.DrawSprite(&animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SparkRail_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Spark.bin", SCOPE_STAGE);
    else
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Spark.bin", SCOPE_STAGE);
}
#endif

void SparkRail_Serialize(void) { RSDK_EDITABLE_VAR(SparkRail, VAR_VECTOR2, size); }
