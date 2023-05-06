// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SDashWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSDashWheel *SDashWheel;

void SDashWheel_Update(void)
{
    RSDK_THIS(SDashWheel);

    Hitbox hitboxSolid;
    hitboxSolid.left  = -32;
    hitboxSolid.right = 32;

    self->down            = false;
    self->currentlyActive = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SDashWheel->hitboxWheel) && player->animator.animationID == ANI_SPINDASH) {
            if (!self->wasActivated) {
                self->toggled ^= true;
                self->currentlyActive = true;
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);
            }

            self->wasActivated = true;
            self->down         = true;
            self->activated    = true;
            self->cooldown     = 60;
            self->rotateOffset = player->direction == FLIP_NONE ? -32 : 32;
        }

        int32 stoodPos = MIN(abs(self->position.x - player->position.x) >> 16, 31);

        hitboxSolid.top    = SDashWheel->heightTable[stoodPos] - 36;
        hitboxSolid.bottom = -4 - hitboxSolid.top;
        if (Player_CheckCollisionBox(player, self, &hitboxSolid) == C_TOP) {
            player->position.y += 0x40000;

            if (player->animator.animationID == ANI_SPINDASH || self->cooldown > 0) {
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);

                if (player->animator.animationID == ANI_SPINDASH) {
                    self->cooldown     = 60;
                    self->rotateOffset = player->direction == FLIP_NONE ? -32 : 32;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                }

                int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                player->velocity.x = 0x700 * RSDK.Cos256(angle);
                player->velocity.y = 0x700 * RSDK.Sin256(angle);
                player->onGround   = false;
            }
        }
    }

    if (!self->down)
        self->wasActivated = false;

    if (self->cooldown > 0) {
        self->cooldown--;

        if (!self->cooldown) {
            RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->shineAnimator, true, 0);
        }
        else {
            self->rotation = (self->rotation + self->rotateOffset) & 0xFF;

            RSDK.ProcessAnimation(&self->shineAnimator);
        }
    }
}

void SDashWheel_LateUpdate(void) {}

void SDashWheel_StaticUpdate(void) {}

void SDashWheel_Draw(void)
{
    RSDK_THIS(SDashWheel);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->knobAnimator, NULL, false);
    RSDK.DrawSprite(&self->shineAnimator, NULL, false);
}

void SDashWheel_Create(void *data)
{
    RSDK_THIS(SDashWheel);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->shineAnimator, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 2, &self->knobAnimator, true, 0);

        // ideally use 'h' (for "high")
        self->drawGroup = RSDK.GetFrameID(&self->mainAnimator) ? Zone->objectDrawGroup[1] : Zone->objectDrawGroup[0];
    }
}

void SDashWheel_StageLoad(void)
{
    SDashWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);

    SDashWheel->hitboxWheel.top    = -33;
    SDashWheel->hitboxWheel.left   = -33;
    SDashWheel->hitboxWheel.right  = 33;
    SDashWheel->hitboxWheel.bottom = 0;

    SDashWheel->sfxBumper = RSDK.GetSfx("Stage/Bumper3.wav");

    if (RSDK.CheckSceneFolder("SSZ1"))
        Soundboard_LoadSfx("SSZ1/MGZDoor.wav", true, SDashWheel_SfxCheck_MGZDoor, SDashWheel_SfxUpdate_MGZDoor);
}

bool32 SDashWheel_SfxCheck_MGZDoor(void)
{
    int32 count = 0;
    foreach_active(PlatformControl, control)
    {
        if (control->taggedButton && control->isActive && control->speed > 0)
            ++count;
    }

    return count > 0;
}

void SDashWheel_SfxUpdate_MGZDoor(int32 sfx)
{
    if (!(Soundboard->sfxPlayingTimer[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

#if GAME_INCLUDE_EDITOR
void SDashWheel_EditorDraw(void)
{
    RSDK_THIS(SDashWheel);

    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->shineAnimator, false, 1);
    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 2, &self->knobAnimator, false, 0);

    // ideally use 'h' (for "high")
    self->drawGroup = RSDK.GetFrameID(&self->mainAnimator) ? Zone->objectDrawGroup[1] : Zone->objectDrawGroup[0];

    SDashWheel_Draw();
}

void SDashWheel_EditorLoad(void)
{
    SDashWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SDashWheel, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void SDashWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, direction);
}
