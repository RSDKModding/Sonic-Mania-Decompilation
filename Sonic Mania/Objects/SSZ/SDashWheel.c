#include "SonicMania.h"

ObjectSDashWheel *SDashWheel;

void SDashWheel_Update(void)
{
    RSDK_THIS(SDashWheel);

    Hitbox hitbox;
    hitbox.left       = -32;
    hitbox.right      = 32;
    entity->field_64  = false;
    entity->activated = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &SDashWheel->hitbox) && player->playerAnimator.animationID == ANI_SPINDASH) {
            if (!entity->field_74) {
                entity->field_68 ^= 1;
                entity->activated = true;
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);
            }
            entity->field_74 = true;
            entity->field_64 = true;
            entity->field_70 = true;
            entity->cooldown = 60;
            if (player->direction == FLIP_NONE)
                entity->rotateOffset = -32;
            else
                entity->rotateOffset = 32;
        }
        int dist = minVal(abs(entity->position.x - player->position.x) >> 16, 31);

        hitbox.top    = SDashWheel->heightArray[dist] - 36;
        hitbox.bottom = -4 - hitbox.top;
        if (Player_CheckCollisionBox(player, entity, &hitbox) == C_TOP) {
            player->position.y += 0x40000;
            if (player->playerAnimator.animationID == 15 || entity->cooldown > 0) {
                RSDK.PlaySfx(SDashWheel->sfxBumper, false, 255);
                if (player->playerAnimator.animationID == ANI_SPINDASH) {
                    entity->cooldown = 60;

                    if (player->direction == FLIP_NONE)
                        entity->rotateOffset = -32;
                    else
                        entity->rotateOffset = 32;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                }

                int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

                player->velocity.x = 0x700 * RSDK.Cos256(angle);
                player->velocity.y = 0x700 * RSDK.Sin256(angle);
                player->onGround   = false;
            }
        }
    }

    if (!entity->field_64)
        entity->field_74 = false;

    if (entity->cooldown > 0) {
        entity->cooldown--;
        if (!entity->cooldown) {
            RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &entity->animator2, true, 0);
        }
        else {
            entity->rotation = (entity->rotation + entity->rotateOffset) & 0xFF;
            RSDK.ProcessAnimation(&entity->animator2);
        }
    }
}

void SDashWheel_LateUpdate(void) {}

void SDashWheel_StaticUpdate(void) {}

void SDashWheel_Draw(void)
{
    RSDK_THIS(SDashWheel);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void SDashWheel_Create(void *data)
{
    RSDK_THIS(SDashWheel);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawFX        = FX_ROTATE | FX_FLIP;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(SDashWheel->aniFrames, 2, &entity->animator3, true, 0);
        if (RSDK.GetFrameID(&entity->animator1))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
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
    int count = 0;
    foreach_active(PlatformControl, control)
    {
        if (control->controller && control->field_6C && control->speed > 0)
            ++count;
    }
    return count > 0;
}

void SDashWheel_UpdateCB(int sfx)
{
    if (!(Soundboard->sfxUnknown6[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

void SDashWheel_EditorDraw(void) {}

void SDashWheel_EditorLoad(void) {}

void SDashWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(SDashWheel, VAR_UINT8, direction);
}
