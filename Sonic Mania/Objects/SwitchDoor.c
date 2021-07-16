#include "../SonicMania.h"

ObjectSwitchDoor *SwitchDoor;

void SwitchDoor_Update(void)
{
    RSDK_THIS(SwitchDoor);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &SwitchDoor->hitbox)) {
            if (entity->go) {
                FBZSetup_BGSwitchCB_B();
                if (entity->reversible)
                    entity->go = false;
                entity->field_60 = true;
            }
            else {
                FBZSetup_BGSwitchCB_A();
                if (entity->reversible) {
                    entity->go = true;
                }
                entity->field_60 = true;
            }
        }
        else if (entity->field_60)
            entity->field_60 = false;
    }

    entity->visible = DebugMode->debugActive;
}

void SwitchDoor_LateUpdate(void) {}

void SwitchDoor_StaticUpdate(void) {}

void SwitchDoor_Draw(void) { SwitchDoor_DrawSprites(); }

void SwitchDoor_Create(void *data)
{
    RSDK_THIS(SwitchDoor);
    RSDK.SetSpriteAnimation(SwitchDoor->aniFrames, 0, &entity->animator, true, 0);
    if (RSDK_sceneInfo->inEditor != true) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x140000;
        entity->field_60      = 0;
        entity->visible       = false;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void SwitchDoor_StageLoad(void)
{
    SwitchDoor->aniFrames     = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
    SwitchDoor->hitbox.left   = 0;
    SwitchDoor->hitbox.top    = 0;
    SwitchDoor->hitbox.right  = 0;
    SwitchDoor->hitbox.bottom = 32;
}

void SwitchDoor_DrawSprites(void)
{
    RSDK_THIS(SwitchDoor);
    int id = 0;
    Vector2 drawPos;
    int yOffset              = 0;
    entity->animator.frameID = entity->go ? 2 : 0;
    for (int i = 0; i < 8; ++i) {
        drawPos.y = entity->position.y + yOffset;
        drawPos.x = entity->position.x + (id << 20);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        if (++id == 2) {
            yOffset += 0x100000;
            id = 0;
        }
    }
}

void SwitchDoor_EditorDraw(void) {}

void SwitchDoor_EditorLoad(void) {}

void SwitchDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_ENUM, go);
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_BOOL, reversible);
}
