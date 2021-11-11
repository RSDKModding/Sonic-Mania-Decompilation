#include "SonicMania.h"

ObjectLRZConvControl *LRZConvControl;

void LRZConvControl_Update(void)
{
    RSDK_THIS(LRZConvControl);

    switch (entity->triggerMode) {
        case LRZCONVCTRL_TRIGGER_PLAYER: LRZConvControl_HandlePlayerTrigger(); break;
        case LRZCONVCTRL_TRIGGER_BUTTON: LRZConvControl_HandleButtonTrigger(); break;
    }

    entity->visible = DebugMode->debugActive;
}

void LRZConvControl_LateUpdate(void) {}

void LRZConvControl_StaticUpdate(void) {}

void LRZConvControl_Draw(void)
{
    RSDK_THIS(LRZConvControl);

    RSDK.SetSpriteAnimation(LRZConvControl->aniFrames, 0, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    if (entity->triggerMode == LRZCONVCTRL_TRIGGER_PLAYER) {
        entity->hitbox.left   = -entity->hitboxSize.x >> 17;
        entity->hitbox.top    = -entity->hitboxSize.y >> 17;
        entity->hitbox.right  = entity->hitboxSize.x >> 17;
        entity->hitbox.bottom = entity->hitboxSize.y >> 17;
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, entity->direction, entity->position.x, entity->position.y, &entity->hitbox);
    }
}

void LRZConvControl_Create(void *data)
{
    RSDK_THIS(LRZConvControl);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    switch (entity->triggerMode) {
        case LRZCONVCTRL_TRIGGER_PLAYER:
            entity->hitbox.left   = -entity->hitboxSize.x >> 17;
            entity->hitbox.top    = -entity->hitboxSize.y >> 17;
            entity->hitbox.right  = entity->hitboxSize.x >> 17;
            entity->hitbox.bottom = entity->hitboxSize.y >> 17;
            entity->updateRange.x += entity->hitboxSize.x;
            entity->updateRange.y += entity->hitboxSize.y;
            break;
        case LRZCONVCTRL_TRIGGER_BUTTON: entity->taggedButton = LRZ2Setup_HandleTagSetup(entity->buttonTag, (Entity *)entity); break;
    }
}

void LRZConvControl_StageLoad(void) { LRZConvControl->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvControl.bin", SCOPE_STAGE); }

void LRZConvControl_HandlePlayerTrigger(void)
{
    RSDK_THIS(LRZConvControl);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        int playerID    = RSDK.GetEntityID(player);
        bool32 collided = Player_CheckCollisionTouch(player, entity, &entity->hitbox);

        if ((1 << playerID) & entity->activePlayers) {
            if (!collided)
                entity->activePlayers &= ~(1 << playerID);
        }
        else {
            if (collided) {
                if (!player->sidekick)
                    flag = true;
                entity->activePlayers |= (1 << playerID);
            }
        }
    }

    if (flag) {
        switch (entity->behavior) {
            case LRZCONVCTRL_BEHAVIOR_ONOFF_SET: LRZ2Setup->conveyorOff = entity->flipVal; break;
            case LRZCONVCTRL_BEHAVIOR_ONOFF_SWAP: LRZ2Setup->conveyorOff = !LRZ2Setup->conveyorOff; break;
            case LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SET: LRZ2Setup->conveyorDir = entity->flipVal; break;
            case LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SWAP: LRZ2Setup->conveyorDir = !LRZ2Setup->conveyorDir; break;
            default: break;
        }
    }
}

void LRZConvControl_HandleButtonTrigger(void)
{
    RSDK_THIS(LRZConvControl);

    if (entity->buttonTag) {
        EntityButton *button = entity->taggedButton;
        if (button) {
            switch (entity->behavior) {
                case LRZCONVCTRL_BEHAVIOR_ONOFF_SET:
                    if (button->field_64)
                        LRZ2Setup->conveyorOff = entity->flipVal;
                    break;
                case LRZCONVCTRL_BEHAVIOR_ONOFF_SWAP:
                    if (entity->field_75 != button->field_68)
                        LRZ2Setup->conveyorOff = !LRZ2Setup->conveyorOff;
                    entity->field_75 = button->field_68;
                    break;
                case LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SET:
                    if (button->field_64)
                        LRZ2Setup->conveyorDir = entity->flipVal;
                    break;
                case LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SWAP:
                    if (button->field_68 != entity->field_75)
                        LRZ2Setup->conveyorDir = !LRZ2Setup->conveyorDir;
                    entity->field_75 = button->field_68;
                    break;
                default: break;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZConvControl_EditorDraw(void) { LRZConvControl_Draw(); }

void LRZConvControl_EditorLoad(void)
{
    LRZConvControl->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvControl.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LRZConvControl, triggerMode);
    RSDK_ENUM_VAR("Player", LRZCONVCTRL_TRIGGER_PLAYER);
    RSDK_ENUM_VAR("Button", LRZCONVCTRL_TRIGGER_BUTTON);

    RSDK_ACTIVE_VAR(LRZConvControl, behavior);
    RSDK_ENUM_VAR("On/Off (Set)", LRZCONVCTRL_BEHAVIOR_ONOFF_SET);
    RSDK_ENUM_VAR("On/Off (Swap)", LRZCONVCTRL_BEHAVIOR_ONOFF_SWAP);
    RSDK_ENUM_VAR("Change Dir (Set)", LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SET);
    RSDK_ENUM_VAR("Change Dir (Swap)", LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SWAP);
}
#endif

void LRZConvControl_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZConvControl, VAR_UINT8, triggerMode);
    RSDK_EDITABLE_VAR(LRZConvControl, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(LRZConvControl, VAR_VECTOR2, hitboxSize);
    RSDK_EDITABLE_VAR(LRZConvControl, VAR_UINT8, behavior);
    RSDK_EDITABLE_VAR(LRZConvControl, VAR_UINT8, flipVal);
}
