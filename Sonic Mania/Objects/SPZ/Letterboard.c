#include "SonicMania.h"

ObjectLetterboard *Letterboard;

void Letterboard_Update(void)
{
    RSDK_THIS(Letterboard);
    StateMachine_Run(entity->state);
}

void Letterboard_LateUpdate(void) {}

void Letterboard_StaticUpdate(void) {}

void Letterboard_Draw(void)
{
    RSDK_THIS(Letterboard);

    entity->scale.x = abs(RSDK.Cos512(entity->angle));
    if (RSDK.Cos512(entity->angle) >= 0)
        RSDK.DrawSprite(&entity->animatorBack, NULL, false);
    else
        RSDK.DrawSprite(&entity->animatorFront, NULL, false);
}

void Letterboard_Create(void *data)
{
    RSDK_THIS(Letterboard);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Letterboard->aniFrames, 0, &entity->animatorBack, true, 0);
        RSDK.SetSpriteAnimation(Letterboard->aniFrames, 1, &entity->animatorFront, true, 0);

        if (entity->ControllerInfo || !entity->letterID) {
            if (entity->ControllerInfo)
                entity->state = Letterboard_State_Controller;
        }
        else {
            entity->state                 = Letterboard_State_CheckPlayerSpin;
            entity->animatorFront.frameID = entity->letterID - 1;
        }

        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void Letterboard_StageLoad(void)
{
    Letterboard->aniFrames     = RSDK.LoadSpriteAnimation("SPZ2/Letterboard.bin", SCOPE_STAGE);
    Letterboard->hitbox.left   = -12;
    Letterboard->hitbox.top    = -12;
    Letterboard->hitbox.right  = 12;
    Letterboard->hitbox.bottom = 12;
    Letterboard->sfxLetterTurn = RSDK.GetSFX("Stage/LetterTurn.wav");
    Letterboard->sfxWin        = RSDK.GetSFX("Stage/Win.wav");
}

void Letterboard_State_Controller(void)
{
    RSDK_THIS(Letterboard);

    entity->active = ACTIVE_BOUNDS;

    bool32 flag = true;
    int count   = 0;
    int slot    = SceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->letterID; ++i) {
        EntityLetterboard *letterboard = RSDK_GET_ENTITY(slot + i, Letterboard);
        if (letterboard->state)
            flag = false;

        if (letterboard->state == Letterboard_State_Spun)
            entity->active = ACTIVE_NORMAL;
    }
    flag = count == entity->letterID;

    if (flag) {
        RSDK.PlaySfx(Letterboard->sfxWin, false, 255);
        entity->active = ACTIVE_BOUNDS;
        entity->state  = StateMachine_None;
        if (globals->gameMode != MODE_COMPETITION)
            Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 100 * entity->letterID);
    }
}

void Letterboard_State_CheckPlayerSpin(void)
{
    RSDK_THIS(Letterboard);

    foreach_active(Player, player)
    {
        entity->spinSpeed = (abs(player->velocity.x) + abs(player->velocity.y)) >> 14;
        if (entity->spinSpeed > 0) {
            if (Player_CheckCollisionTouch(player, entity, &Letterboard->hitbox)) {
                entity->drawFX = FX_SCALE;
                entity->state  = Letterboard_State_Spun;
                if (entity->spinSpeed > 16)
                    entity->spinSpeed = 16;
                if (entity->spinSpeed < 8)
                    entity->spinSpeed = 8;

                entity->timer = 2;
                int slot      = SceneInfo->entitySlot;

                EntityLetterboard *letterboard = entity;
                while (slot >= 0) {
                    letterboard = RSDK_GET_ENTITY(slot--, Letterboard);
                    if (letterboard->state == Letterboard_State_Controller)
                        break;
                }

                letterboard->active = ACTIVE_NORMAL;
                entity->active      = ACTIVE_NORMAL;
                RSDK.PlaySfx(Letterboard->sfxLetterTurn, false, 255);
                foreach_break;
            }
        }
    }
}

void Letterboard_State_Spun(void)
{
    RSDK_THIS(Letterboard);

    int prevAngle = entity->angle;
    entity->angle = (entity->angle + entity->spinSpeed) & 0x1FF;
    if (entity->angle > 255 && prevAngle < 256 && --entity->timer <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->angle  = 256;
        entity->state  = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void Letterboard_EditorDraw(void)
{
    RSDK_THIS(Letterboard);
    RSDK.SetSpriteAnimation(Letterboard->aniFrames, 0, &entity->animatorBack, true, 0);
    RSDK.SetSpriteAnimation(Letterboard->aniFrames, 1, &entity->animatorFront, true, 0);

    if (!entity->ControllerInfo && entity->letterID) {
        entity->animatorFront.frameID = entity->letterID - 1;
        RSDK.DrawSprite(&entity->animatorFront, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animatorBack, NULL, false);
    }
}

void Letterboard_EditorLoad(void) { Letterboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Letterboard.bin", SCOPE_STAGE); }
#endif

void Letterboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(Letterboard, VAR_BOOL, ControllerInfo);
    RSDK_EDITABLE_VAR(Letterboard, VAR_UINT8, letterID);
}
