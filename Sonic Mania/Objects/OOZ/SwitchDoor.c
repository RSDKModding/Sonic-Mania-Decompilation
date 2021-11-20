#include "SonicMania.h"

ObjectSwitchDoor *SwitchDoor;

void SwitchDoor_Update(void)
{
    RSDK_THIS(SwitchDoor);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SwitchDoor->hitbox)) {
            if (self->go) {
                FBZSetup_BGSwitchCB_ShowInside2();
                if (self->reversible)
                    self->go = false;
                self->field_60 = true;
            }
            else {
                FBZSetup_BGSwitchCB_ShowInside1();
                if (self->reversible) {
                    self->go = true;
                }
                self->field_60 = true;
            }
        }
        else if (self->field_60)
            self->field_60 = false;
    }

    self->visible = DebugMode->debugActive;
}

void SwitchDoor_LateUpdate(void) {}

void SwitchDoor_StaticUpdate(void) {}

void SwitchDoor_Draw(void) { SwitchDoor_DrawSprites(); }

void SwitchDoor_Create(void *data)
{
    RSDK_THIS(SwitchDoor);
    RSDK.SetSpriteAnimation(SwitchDoor->aniFrames, 0, &self->animator, true, 0);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x140000;
        self->field_60      = 0;
        self->visible       = false;
        self->drawOrder     = Zone->drawOrderLow;
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
    int32 id = 0;
    Vector2 drawPos;
    int32 yOffset              = 0;
    self->animator.frameID = self->go ? 2 : 0;
    for (int32 i = 0; i < 8; ++i) {
        drawPos.y = self->position.y + yOffset;
        drawPos.x = self->position.x + (id << 20);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        if (++id == 2) {
            yOffset += 0x100000;
            id = 0;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void SwitchDoor_EditorDraw(void)
{
    RSDK_THIS(SwitchDoor);
    self->updateRange.x = 0x100000;
    self->updateRange.y = 0x140000;

    SwitchDoor_DrawSprites();
}

void SwitchDoor_EditorLoad(void) {}
#endif

void SwitchDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_ENUM, go);
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_BOOL, reversible);
}
