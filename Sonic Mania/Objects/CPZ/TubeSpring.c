#include "SonicMania.h"

ObjectTubeSpring *TubeSpring;

void TubeSpring_Update(void)
{
    RSDK_THIS(TubeSpring);
    if (entity->timer > 0)
        entity->timer--;
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
}

void TubeSpring_LateUpdate(void) {}

void TubeSpring_StaticUpdate(void) {}

void TubeSpring_Draw(void)
{
    RSDK_THIS(TubeSpring);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TubeSpring_Create(void *data)
{
    RSDK_THIS(TubeSpring);
    Spring_Create(NULL);
    RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 0, &entity->animator, true, 0);
    entity->drawOrder               = Zone->drawOrderHigh;
    entity->velocity.y              = !entity->type ? -0x100000 : -0xA8000;
    entity->type                    = 0xFF;
    entity->animator.animationSpeed = 0;
    entity->state                   = TubeSpring_Interact;
}

void TubeSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TubeSpring.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("FBZ"))
        TubeSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TubeSpring.bin", SCOPE_STAGE);
    TubeSpring->sfxExit       = RSDK.GetSFX("Tube/Exit.wav");
    TubeSpring->hitbox.left   = -16;
    TubeSpring->hitbox.top    = 12;
    TubeSpring->hitbox.right  = 16;
    TubeSpring->hitbox.bottom = 48;
}

void TubeSpring_EditorDraw(void) {}

void TubeSpring_EditorLoad(void) {}

void TubeSpring_Interact(void) { TubeSpring_Spring(true); }

void TubeSpring_Springing(void)
{
    RSDK_THIS(TubeSpring);
    if (TubeSpring_Spring(false) && entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 2, &entity->animator, true, 0);
        entity->state = TubeSpring_Pullback;
        TubeSpring_Pullback();
    }
}
void TubeSpring_Pullback(void)
{
    RSDK_THIS(TubeSpring);
    TubeSpring_Spring(false);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 0, &entity->animator, true, 0);
        entity->state = TubeSpring_Interact;
    }
}

bool32 TubeSpring_Spring(bool32 interact)
{
    RSDK_THIS(TubeSpring);
    Spring_State_Vertical();
    bool32 sprung = false;
    for (int32 i = 0; i < Player->playerCount; i++) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        if (Player_CheckValidState(player)) {
            bool32 oldinter     = player->interaction;
            player->interaction = true;
            if (!Player_CheckCollisionTouch(player, entity, &TubeSpring->hitbox)) {
                player->interaction = oldinter;
                continue;
            }

            if (interact) {
                RSDK.SetSpriteAnimation(TubeSpring->aniFrames, 1, &entity->animator, true, 0);
                RSDK.PlaySfx(TubeSpring->sfxExit, 0, 255);
                entity->state = TubeSpring_Springing;
            }

            sprung = true;
            if (player->state == Player_State_None || player->state == Player_Unknown7) {
                player->drawOrder      = Zone->playerDrawLow;
                player->tileCollisions = true;
                player->interaction    = true;
                player->jumpAbility    = 0;
                player->state          = Player_State_Air;
                if (player->velocity.y < -0x80000)
                    player->velocity.y = -0x80000;
            }
        }
    }
    return sprung;
}

void TubeSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, flipFlag);
}
