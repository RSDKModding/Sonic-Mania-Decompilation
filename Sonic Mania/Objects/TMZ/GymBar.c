#include "SonicMania.h"

ObjectGymBar *GymBar;

void GymBar_Update(void) { GymBar_HandlePlayerInteractions(); }

void GymBar_LateUpdate(void) {}

void GymBar_StaticUpdate(void) {}

void GymBar_Draw(void)
{
    RSDK_THIS(GymBar);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->type) {
        drawPos.y                = entity->position.y - (entity->size << 18);
        entity->animator.frameID = 0;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->animator.frameID = 1;
        for (int i = 0; i < entity->size; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.y += 0x80000;
        }
    }
    else {
        drawPos.x                = entity->position.x - (entity->size << 18);
        entity->animator.frameID = 0;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->animator.frameID = 1;
        for (int i = 0; i < entity->size; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.x += 0x80000;
        }
    }
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void GymBar_Create(void *data)
{
    RSDK_THIS(GymBar);

    entity->drawFX        = FX_FLIP;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    if (!entity->type) {
        entity->updateRange.x = (entity->size << 18) + 0x400000;
        entity->hitbox.left   = (-4 * entity->size);
        entity->hitbox.top    = 0;
        entity->hitbox.right  = 4 * entity->size - 8;
        entity->hitbox.bottom = 8;
        entity->field_68.x    = entity->position.x - (entity->size << 18) + 0x50000;
        entity->field_68.y    = (entity->size << 18) + entity->position.x - 0xC0000;
    }
    else {
        entity->updateRange.y = (entity->size + 16) << 18;
        entity->hitbox.left   = 0;
        entity->hitbox.top    = (8 - (entity->size << 2));
        entity->hitbox.right  = 0;
        entity->hitbox.bottom = (entity->size << 2) - 16;
    }
    RSDK.SetSpriteAnimation(GymBar->aniFrames, entity->type, &entity->animator, true, 0);
}

void GymBar_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        GymBar->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/GymBar.bin", SCOPE_STAGE);
    GymBar->sfxBumper = RSDK.GetSFX("Stage/Bumper3.wav");

    DEBUGMODE_ADD_OBJ(GymBar);
}

void GymBar_DebugSpawn(void)
{
    RSDK_THIS(GymBar);

    CREATE_ENTITY(GymBar, NULL, entity->position.x, entity->position.y);
}

void GymBar_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(GymBar->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void GymBar_HandlePlayerInteractions(void)
{
    RSDK_THIS(GymBar);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (player->playerAnimator.animationID != ANI_HURT && player->state != Player_State_FlyCarried) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                if (!entity->playerTimers[playerID]) {
                    player->onGround = false;
                    if (entity->type) {
                        if (abs(player->velocity.x) >= 0x40000) {
                            player->position.x = entity->position.x;
                            player->direction  = FLIP_NONE;
                            if (player->velocity.x <= 0)
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGV, &player->playerAnimator, false, 2);
                            else
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGV, &player->playerAnimator, false, 9);
                            player->playerAnimator.animationSpeed = ((abs(player->velocity.x) - 0x40000) >> 12) + 224;
                            player->abilityValues[0]              = 0;
                            player->abilityValues[1]              = player->playerAnimator.frameID;
                            player->abilityValues[2]              = player->velocity.x;
                            player->state                         = GymBar_PlayerState_SwingH;
                        }
                    }
                    else {
                        player->rotation   = 0;
                        player->position.x = clampVal(player->position.x, entity->field_68.x, entity->field_68.y);
                        player->position.y = entity->position.y;
                        if (abs(player->velocity.y) < 0x40000 || entity->noSwing) {
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, false, 0);
                            player->playerAnimator.animationSpeed = 0;
                            player->abilityPtrs[0]                = entity;
                            player->state                         = GymBar_PlayerState_Hang;
                        }
                        else {
                            if (player->velocity.y <= 0)
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, false, 2);
                            else
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, false, 9);
                            player->playerAnimator.animationSpeed = ((abs(player->velocity.y) - 0x40000) >> 12) + 256;
                            player->abilityValues[0]              = 0;
                            player->abilityValues[1]              = player->playerAnimator.frameID;
                            player->abilityValues[2]              = player->velocity.y;
                            player->state                         = GymBar_PlayerState_SwingV;
                        }
                    }
                    if (player->state == GymBar_PlayerState_Hang || player->state == GymBar_PlayerState_SwingH
                        || player->state == GymBar_PlayerState_SwingV) {
                        entity->playerTimers[playerID] = 16;
                        RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        player->jumpAbility     = 0;
                    }
                }
            }
            else if (entity->playerTimers[playerID])
                entity->playerTimers[playerID]--;
        }
    }
}

void GymBar_HandleSwingJump(void)
{
    RSDK_THIS(Player);

    if (!entity->down)
        entity->velocity.y = -0x50000;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    if (entity->characterID == ID_TAILS) {
        entity->playerAnimator.animationSpeed = 120;
    }
    else {
        entity->playerAnimator.animationSpeed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;
    }
    if (entity->playerAnimator.animationSpeed > 0xF0)
        entity->playerAnimator.animationSpeed = 0xF0;
    entity->jumpAbility      = 1;
    entity->jumpAbilityTimer = 1;
    entity->abilityPtrs[0]   = NULL;
    entity->abilityValues[0] = 0;
    entity->abilityValues[1] = 0;
    entity->abilityValues[3] = 0;
    entity->state            = Player_State_Air;
}

void GymBar_PlayerState_SwingV(void)
{
    RSDK_THIS(Player);

    if (entity->jumpPress)
        GymBar_HandleSwingJump();

    int frame = 11;
    if (entity->abilityValues[2] <= 0)
        frame = 4;
    if (entity->playerAnimator.frameID == frame && entity->abilityValues[1] != frame)
        ++entity->abilityValues[0];

    entity->abilityValues[1] = entity->playerAnimator.frameID;
    if (entity->abilityValues[0] >= 2) {
        if (entity->abilityValues[2] <= 0)
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPRINGDIAGONAL, &entity->playerAnimator, false, 0);
        else
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 0);

        entity->velocity.y =
            entity->abilityValues[2] + (entity->abilityValues[2] >> ((abs(entity->abilityValues[2]) >> 18) + (abs(entity->abilityValues[2]) >> 20)));
        entity->abilityPtrs[0]   = NULL;
        entity->abilityValues[0] = 0;
        entity->abilityValues[1] = 0;
        entity->abilityValues[2] = 0;
        entity->state            = Player_State_Air;
    }
}

void GymBar_PlayerState_Hang(void)
{
    RSDK_THIS(Player);

    EntityGymBar *gymbar = entity->abilityPtrs[0];
    if (entity->left) {
        if (entity->position.x > gymbar->field_68.x)
            entity->position.x -= 0x10000;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SHIMMYMOVE, &entity->playerAnimator, false, 0);
        entity->direction = FLIP_X;
    }
    else if (entity->right) {
        if (entity->position.x < gymbar->field_68.y)
            entity->position.x += 0x10000;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SHIMMYMOVE, &entity->playerAnimator, false, 0);
        entity->direction = FLIP_NONE;
    }
    else {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SHIMMYIDLE, &entity->playerAnimator, false, 0);
    }
    if (entity->jumpPress)
        GymBar_HandleSwingJump();
}

void GymBar_PlayerState_SwingH(void)
{
    RSDK_THIS(Player);

    if (entity->jumpPress)
        GymBar_HandleSwingJump();

    int frame = 11;
    if (entity->abilityValues[2] <= 0)
        frame = 4;
    if (entity->playerAnimator.frameID == frame && entity->abilityValues[1] != frame)
        ++entity->abilityValues[0];

    entity->abilityValues[1] = entity->playerAnimator.frameID;
    if (entity->abilityValues[0] >= 2) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 0);
        entity->velocity.x       = entity->abilityValues[2] + (entity->abilityValues[2] >> (abs(entity->abilityValues[2]) >> 18));
        entity->groundVel        = entity->velocity.x;
        entity->direction        = abs(entity->velocity.x >> 31);
        entity->abilityPtrs[0]   = NULL;
        entity->abilityValues[0] = 0;
        entity->abilityValues[1] = 0;
        entity->abilityValues[2] = 0;
        entity->state            = Player_State_Air;
    }
}

#if RETRO_INCLUDE_EDITOR
void GymBar_EditorDraw(void)
{
    RSDK_THIS(GymBar);
    if (!entity->type) {
        entity->updateRange.x = (entity->size << 18) + 0x400000;
        entity->field_68.x    = entity->position.x - (entity->size << 18) + 0x50000;
        entity->field_68.y    = (entity->size << 18) + entity->position.x - 0xC0000;
    }
    else {
        entity->updateRange.y = (entity->size + 16) << 18;
    }
    RSDK.SetSpriteAnimation(GymBar->aniFrames, entity->type, &entity->animator, true, 0);

    GymBar_Draw();
}

void GymBar_EditorLoad(void) { GymBar->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/GymBar.bin", SCOPE_STAGE); }
#endif

void GymBar_Serialize(void)
{
    RSDK_EDITABLE_VAR(GymBar, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(GymBar, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(GymBar, VAR_BOOL, noSwing);
}
