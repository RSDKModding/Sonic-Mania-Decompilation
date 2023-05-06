// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TubeSpring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTubeSpring *TubeSpring;

void TubeSpring_Update(void)
{
    RSDK_THIS(TubeSpring);

    if (self->timer > 0)
        self->timer--;

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);
}

void TubeSpring_LateUpdate(void) {}

void TubeSpring_StaticUpdate(void) {}

void TubeSpring_Draw(void)
{
    RSDK_THIS(TubeSpring);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TubeSpring_Create(void *data)
{
    RSDK_THIS(TubeSpring);

    Spring_Create(NULL);
    RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 0, &self->animator, true, 0);
    self->drawGroup      = Zone->objectDrawGroup[1];
    self->velocity.y     = !self->type ? -0x100000 : -0xA8000;
    self->type           = SPRING_TUBESPRING;
    self->animator.speed = 0;
    self->state          = TubeSpring_State_Idle;
}

void TubeSpring_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TubeSpring.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("FBZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TubeSpring.bin", SCOPE_STAGE);

    TubeSpring->sfxExit = RSDK.GetSfx("Tube/Exit.wav");

    TubeSpring->hitbox.left   = -16;
    TubeSpring->hitbox.top    = 12;
    TubeSpring->hitbox.right  = 16;
    TubeSpring->hitbox.bottom = 48;
}

void TubeSpring_State_Idle(void) { TubeSpring_HandleInteractions(true); }

void TubeSpring_State_Springing(void)
{
    RSDK_THIS(TubeSpring);

    if (!TubeSpring_HandleInteractions(false) && self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 2, &self->animator, true, 0);
        self->state = TubeSpring_State_Pullback;
        TubeSpring_State_Pullback();
    }
}
void TubeSpring_State_Pullback(void)
{
    RSDK_THIS(TubeSpring);

    TubeSpring_HandleInteractions(false);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 0, &self->animator, true, 0);
        self->state = TubeSpring_State_Idle;
    }
}

bool32 TubeSpring_HandleInteractions(bool32 setState)
{
    RSDK_THIS(TubeSpring);

    Spring_State_Vertical();

    bool32 sprung = false;
    for (int32 i = 0; i < Player->playerCount; i++) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        if (Player_CheckValidState(player)) {
            bool32 interactStore = player->interaction;
            player->interaction  = true;
            if (!Player_CheckCollisionTouch(player, self, &TubeSpring->hitbox)) {
                player->interaction = interactStore;
                continue;
            }

            if (setState) {
                RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 1, &self->animator, true, 0);
                RSDK.PlaySfx(TubeSpring->sfxExit, false, 255);
                self->state = TubeSpring_State_Springing;
            }

            sprung = true;
            if (player->state == Player_State_Static || player->state == Player_State_TransportTube) {
                player->drawGroup      = Zone->playerDrawGroup[0];
                player->tileCollisions = TILECOLLISION_DOWN;
                player->interaction    = true;
                player->applyJumpCap   = false;
                player->state          = Player_State_Air;
                if (player->velocity.y < -0x80000)
                    player->velocity.y = -0x80000;
            }
        }
    }

    return sprung;
}

#if GAME_INCLUDE_EDITOR
void TubeSpring_EditorDraw(void)
{
    RSDK_THIS(TubeSpring);

    Spring_Create(NULL);
    RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 0, &self->animator, true, 0);

    TubeSpring_Draw();
}

void TubeSpring_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TubeSpring.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("FBZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TubeSpring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TubeSpring, type);
    RSDK_ENUM_VAR("Strong", SPRING_VERT_YELLOW);
    RSDK_ENUM_VAR("Weak", SPRING_VERT_RED);

    // technically unused, FX_FLIP isn't ever set on the animator
    RSDK_ACTIVE_VAR(TubeSpring, flipFlag);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void TubeSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, flipFlag);
}
