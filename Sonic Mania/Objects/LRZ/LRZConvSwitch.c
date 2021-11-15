#include "SonicMania.h"

ObjectLRZConvSwitch *LRZConvSwitch;

void LRZConvSwitch_Update(void)
{
    RSDK_THIS(LRZConvSwitch);

    if ((LRZ2Setup->conveyorDir != entity->conveyorDir) != entity->calibration)
        LRZConvSwitch_Calibrate();

    int ty1 = entity->position.x;
    int ty2 = entity->position.y - 0x180000;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (entity->playerPositions[playerID].x || entity->playerPositions[playerID].y) {
            int dir = 0;
            if (player->position.x <= entity->playerPositions[playerID].x) {
                if (player->position.x < entity->playerPositions[playerID].x)
                    dir = -1;
            }
            else {
                dir = 1;
            }

            int val = MathHelpers_Unknown12(player->position.x, player->position.y, entity->playerPositions[playerID].x,
                                            entity->playerPositions[playerID].y, entity->position.x, entity->position.y, ty1, ty2);
            if (!entity->dir && !player->sidekick) {
                if (dir < 0) {
                    if (val && !entity->conveyorDir) {
                        RSDK.PlaySfx(LRZConvSwitch->sfxClack, false, 255);
                        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 1, &entity->animator, true, 0);
                        entity->conveyorDir    = 1;
                        LRZ2Setup->conveyorDir = entity->calibration ^ 1;
                        entity->dir            = 1;
                    }
                }
                if (dir > 0 && val && entity->conveyorDir == 1) {
                    RSDK.PlaySfx(LRZConvSwitch->sfxClack, false, 255);
                    RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 3, &entity->animator, true, 0);
                    entity->conveyorDir    = 0;
                    LRZ2Setup->conveyorDir = entity->calibration;
                    entity->dir            = 1;
                }
            }
        }

        entity->playerPositions[playerID].x = player->position.x;
        entity->playerPositions[playerID].y = player->position.y;
    }

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.animationID == 1 && entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 2, &entity->animator, true, 0);
        entity->dir = 0;
    }
    if (entity->animator.animationID == 3 && entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 0, &entity->animator, true, 0);
        entity->dir = 0;
    }
}

void LRZConvSwitch_LateUpdate(void) {}

void LRZConvSwitch_StaticUpdate(void) {}

void LRZConvSwitch_Draw(void)
{
    RSDK_THIS(LRZConvSwitch);

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void LRZConvSwitch_Create(void *data)
{
    RSDK_THIS(LRZConvSwitch);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    LRZConvSwitch_Calibrate();
}

void LRZConvSwitch_StageLoad(void)
{
    LRZConvSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvSwitch.bin", SCOPE_STAGE);
    LRZConvSwitch->sfxClack  = RSDK.GetSFX("Stage/Clack.wav");
}

void LRZConvSwitch_Calibrate(void)
{
    RSDK_THIS(LRZConvSwitch);

    entity->conveyorDir = entity->calibration ^ LRZ2Setup->conveyorDir;
    if (!entity->conveyorDir)
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 0, &entity->animator, true, 0);
    else if (entity->conveyorDir == 1)
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 2, &entity->animator, true, 0);
}

#if RETRO_INCLUDE_EDITOR
void LRZConvSwitch_EditorDraw(void)
{
    LRZConvSwitch_Calibrate();
    LRZConvSwitch_Draw();
}

void LRZConvSwitch_EditorLoad(void)
{
    LRZConvSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvSwitch.bin", SCOPE_STAGE);
    RSDK_ACTIVE_VAR(LRZConvSwitch, calibration);
    RSDK_ENUM_VAR("Right", LRZCONVSWITCH_RIGHT);
    RSDK_ENUM_VAR("Left", LRZCONVSWITCH_LEFT);
}
#endif

void LRZConvSwitch_Serialize(void) { RSDK_EDITABLE_VAR(LRZConvSwitch, VAR_UINT8, calibration); }
