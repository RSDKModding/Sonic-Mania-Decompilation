#include "SonicMania.h"

ObjectSDashWheel *SDashWheel;

void SDashWheel_Update(void)
{
    RSDK_THIS(SDashWheel);

    Hitbox hitbox;
    hitbox.left       = -32;
    hitbox.right      = 32;
    self->field_64  = false;
    self->activated = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SDashWheel->hitbox) && player->playerAnimator.animationID == ANI_SPINDASH) {
            if (!self->field_74) {
                self->field_68 ^= 1;
                self->activated = true;
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);
            }
            self->field_74 = true;
            self->field_64 = true;
            self->field_70 = true;
            self->cooldown = 60;
            if (player->direction == FLIP_NONE)
                self->rotateOffset = -32;
            else
                self->rotateOffset = 32;
        }
        int32 dist = minVal(abs(self->position.x - player->position.x) >> 16, 31);

        hitbox.top    = SDashWheel->heightArray[dist] - 36;
        hitbox.bottom = -4 - hitbox.top;
        if (Player_CheckCollisionBox(player, self, &hitbox) == C_TOP) {
            player->position.y += 0x40000;
            if (player->playerAnimator.animationID == 15 || self->cooldown > 0) {
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);
                if (player->playerAnimator.animationID == ANI_SPINDASH) {
                    self->cooldown = 60;

                    if (player->direction == FLIP_NONE)
                        self->rotateOffset = -32;
                    else
                        self->rotateOffset = 32;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                }

                int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                player->velocity.x = 0x700 * RSDK.Cos256(angle);
                player->velocity.y = 0x700 * RSDK.Sin256(angle);
                player->onGround   = false;
            }
        }
    }

    if (!self->field_64)
        self->field_74 = false;

    if (self->cooldown > 0) {
        self->cooldown--;
        if (!self->cooldown) {
            RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->animator2, true, 0);
        }
        else {
            self->rotation = (self->rotation + self->rotateOffset) & 0xFF;
            RSDK.ProcessAnimation(&self->animator2);
        }
    }
}

void SDashWheel_LateUpdate(void) {}

void SDashWheel_StaticUpdate(void) {}

void SDashWheel_Draw(void)
{
    RSDK_THIS(SDashWheel);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
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
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 2, &self->animator3, true, 0);
        if (RSDK.GetFrameID(&self->animator1))
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;
    }
}

void SDashWheel_StageLoad(void)
{
    SDashWheel->aniFrames     = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);
    SDashWheel->hitbox.top    = -33;
    SDashWheel->hitbox.left   = -33;
    SDashWheel->hitbox.right  = 33;
    SDashWheel->hitbox.bottom = 0;
    SDashWheel->sfxBumper     = RSDK.GetSFX("Stage/Bumper3.wav");
    if (RSDK.CheckStageFolder("SSZ1"))
        Soundboard_LoadSFX("SSZ1/MGZDoor.wav", true, SDashWheel_CheckCB, SDashWheel_UpdateCB);
}

bool32 SDashWheel_CheckCB(void)
{
    int32 count = 0;
    foreach_active(PlatformControl, control)
    {
        if (control->ControllerInfo && control->field_6C && control->speed > 0)
            ++count;
    }
    return count > 0;
}

void SDashWheel_UpdateCB(int32 sfx)
{
    if (!(Soundboard->sfxUnknown6[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

#if RETRO_INCLUDE_EDITOR
void SDashWheel_EditorDraw(void)
{
    RSDK_THIS(SDashWheel);
    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 2, &self->animator3, false, 0);
    if (RSDK.GetFrameID(&self->animator1))
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;

    SDashWheel_Draw();
}

void SDashWheel_EditorLoad(void) { SDashWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE); }
#endif

void SDashWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, direction);
}
