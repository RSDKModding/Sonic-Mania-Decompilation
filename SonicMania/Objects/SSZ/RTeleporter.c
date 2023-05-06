// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RTeleporter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRTeleporter *RTeleporter;

void RTeleporter_Update(void)
{
    RSDK_THIS(RTeleporter);

    StateMachine_Run(self->state);
}

void RTeleporter_LateUpdate(void) {}

void RTeleporter_StaticUpdate(void) {}

void RTeleporter_Draw(void)
{
    RSDK_THIS(RTeleporter);

    StateMachine_Run(self->stateDraw);
}

void RTeleporter_Create(void *data)
{
    RSDK_THIS(RTeleporter);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 1, &self->electricAnimator, true, 0);

        self->originY   = self->position.y;
        self->state     = RTeleporter_State_CheckPlayerCollisions;
        self->stateDraw = RTeleporter_Draw_Active;
    }
}

void RTeleporter_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("SSZ/RTeleporter.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MMZ1") || RSDK.CheckSceneFolder("MMZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RTeleporter.bin", SCOPE_STAGE);

    RTeleporter->hitboxTeleporter.top    = -32;
    RTeleporter->hitboxTeleporter.left   = -34;
    RTeleporter->hitboxTeleporter.right  = 34;
    RTeleporter->hitboxTeleporter.bottom = 32;

    RTeleporter->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    RTeleporter->sfxGoodFuture = RSDK.GetSfx("Stage/GoodFuture.wav");
}

void RTeleporter_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->electricAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originY, 4, 9);

    foreach_active(Player, player)
    {
        if (!player->onGround) {
            if (Player_CheckCollisionTouch(player, self, &RTeleporter->hitboxTeleporter)) {
                Player_CheckItemBreak(player, self, false);
                self->state     = RTeleporter_State_Destroyed;
                self->stateDraw = RTeleporter_Draw_Exploding;
            }
        }
    }
}

void RTeleporter_State_Destroyed(void)
{
    RSDK_THIS(RTeleporter);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(RTeleporter->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Rand(-32, 32) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-32, 32) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS)), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }

    if (++self->timer == 80) {
        RSDK.PlaySfx(RTeleporter->sfxGoodFuture, false, 255);
        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 2, &self->mainAnimator, true, 0);

        self->position.y += 0x180000;
        RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8);

        self->state     = StateMachine_None;
        self->stateDraw = RTeleporter_Draw_Destroyed;
    }
}

void RTeleporter_Draw_Active(void)
{
    RSDK_THIS(RTeleporter);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->electricAnimator, NULL, false);

    drawPos.x -= 0x100000;
    RSDK.DrawSprite(&self->electricAnimator, &drawPos, false);

    drawPos.x += 0x200000;
    RSDK.DrawSprite(&self->electricAnimator, &drawPos, false);
}

void RTeleporter_Draw_Exploding(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->direction = FLIP_NONE;
}

void RTeleporter_Draw_Destroyed(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void RTeleporter_EditorDraw(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 0, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 1, &self->electricAnimator, true, 0);

    RTeleporter_Draw_Active();
}

void RTeleporter_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("SSZ/RTeleporter.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MMZ1") || RSDK.CheckSceneFolder("MMZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RTeleporter.bin", SCOPE_STAGE);
}
#endif

void RTeleporter_Serialize(void) {}
