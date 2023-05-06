// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SwitchDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSwitchDoor *SwitchDoor;

void SwitchDoor_Update(void)
{
    RSDK_THIS(SwitchDoor);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SwitchDoor->hitbox)) {
            if (self->go) {
                FBZSetup_BGSwitch_ShowInside2();

                if (self->reversible)
                    self->go = SWITCHDOOR_GO_INSIDE1;

                self->activated = true;
            }
            else {
                FBZSetup_BGSwitch_ShowInside1();

                if (self->reversible)
                    self->go = SWITCHDOOR_GO_INSIDE2;

                self->activated = true;
            }
        }
        else {
            if (self->activated)
                self->activated = false;
        }
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
        self->activated     = false;
        self->visible       = false;
        self->drawGroup     = Zone->objectDrawGroup[0];
    }
}

void SwitchDoor_StageLoad(void)
{
    SwitchDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    SwitchDoor->hitbox.left   = 0;
    SwitchDoor->hitbox.top    = 0;
    SwitchDoor->hitbox.right  = 0;
    SwitchDoor->hitbox.bottom = 32;
}

void SwitchDoor_DrawSprites(void)
{
    RSDK_THIS(SwitchDoor);

    self->animator.frameID = self->go ? SWITCHDOOR_GO_INSIDE2_ALT : SWITCHDOOR_GO_INSIDE1;

    int32 id      = 0;
    int32 yOffset = 0;
    for (int32 i = 0; i < 8; ++i) {
        Vector2 drawPos;
        drawPos.x = self->position.x + (id << 20);
        drawPos.y = self->position.y + yOffset;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        if (++id == 2) {
            yOffset += 0x100000;
            id = 0;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void SwitchDoor_EditorDraw(void)
{
    RSDK_THIS(SwitchDoor);

    self->updateRange.x = 0x100000;
    self->updateRange.y = 0x140000;

    SwitchDoor_DrawSprites();
}

void SwitchDoor_EditorLoad(void)
{
    SwitchDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SwitchDoor, go);
    RSDK_ENUM_VAR("Inside 1 BG", SWITCHDOOR_GO_INSIDE1);
    RSDK_ENUM_VAR("Inside 2 BG", SWITCHDOOR_GO_INSIDE2);
    RSDK_ENUM_VAR("Inside 2 BG (Alt Editor Sprites)", SWITCHDOOR_GO_INSIDE2_ALT);
}
#endif

void SwitchDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_ENUM, go);
    RSDK_EDITABLE_VAR(SwitchDoor, VAR_BOOL, reversible);
}
