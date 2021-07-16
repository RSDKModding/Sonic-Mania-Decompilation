#include "../SonicMania.h"

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
    RSDK.DrawSprite(&entity->animator, 0, false);
}

void TubeSpring_Create(void *data)
{
    RSDK_THIS(TubeSpring);
    Spring_Create(NULL);
    RSDK.SetSpriteAnimation(TubeSpring->animID, 0, &entity->animator, true, 0);
    entity->drawOrder = Zone->drawOrderHigh;
    entity->velocity.y = !entity->type ? -0x100000 : 0xA8000;
    entity->type = 0xFF;
    entity->animator.animationSpeed = 0;
    entity->state = TubeSpring_Interact;
}

void TubeSpring_StageLoad(void) {
    if (RSDK.CheckStageFolder("CPZ"))
        TubeSpring->animID = RSDK.LoadSpriteAnimation("CPZ/TubeSpring.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("FBZ"))
        TubeSpring->animID = RSDK.LoadSpriteAnimation("FBZ/TubeSpring.bin", SCOPE_STAGE);
    TubeSpring->sfxID         = RSDK.GetSFX("Tube/Exit.wav");
    TubeSpring->hitbox.left = -16;
    TubeSpring->hitbox.top = 0xB;
    TubeSpring->hitbox.right = 0x10;
    TubeSpring->hitbox.bottom = 0x30;
}

void TubeSpring_EditorDraw(void) {}

void TubeSpring_EditorLoad(void) {}

void TubeSpring_Serialize(void) {
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TubeSpring, VAR_ENUM, flipFlag);
}

void TubeSpring_Interact(void) {
    TubeSpring_Spring(true);
}

void TubeSpring_Springing(void) {
    RSDK_THIS(TubeSpring);
    if (TubeSpring_Spring(false) && entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TubeSpring->animID, 2, &entity->animator, true, 0);
        entity->state = TubeSpring_Pullback;
        TubeSpring_Pullback();
    }
}
void TubeSpring_Pullback(void) {
    RSDK_THIS(TubeSpring);
    TubeSpring_Spring(false);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
    {
        RSDK.SetSpriteAnimation(TubeSpring->animID, 0, &entity->animator, true, 0);
        entity->state = TubeSpring_Interact;
    }
}

bool32 TubeSpring_Spring(bool32 interact) {
    RSDK_THIS(TubeSpring);
    Spring_State_Vertical();
    bool32 sprung = false;
    for (int i = 0; i < Player->playerCount; i++) {
        EntityPlayer* player = RSDK.GetEntityByID(i);
        if (Player_CheckValidState(player)) {
            bool32 oldinter = player->interaction;
            player->interaction = true;
            if (!Player_CheckCollisionTouch(player, entity, &TubeSpring->hitbox)) {
                player->interaction = oldinter;
                continue;
            }
            if (interact) {
                RSDK.SetSpriteAnimation(TubeSpring->animID, 1, &entity->animator, true, 0);
                RSDK.PlaySFX(TubeSpring->sfxID, 0, 255);
                entity->state = TubeSpring_Springing;
            }
            sprung = true;
            player->drawOrder = Zone->playerDrawLow;
            player->tileCollisions = true;
            player->interaction = true;
            player->jumpAbility = true;
            player->state = Player_State_Air;
            if (player->velocity.y < -0x80000)
                player->velocity.y = -0x80000;
        }
    }
    return sprung;
}
