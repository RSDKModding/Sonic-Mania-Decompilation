#include "../SonicMania.h"

ObjectChemicalBall *ChemicalBall;

void ChemicalBall_Update(void)
{
    RSDK_THIS(ChemicalBall);
    if (globals->gameMode == MODE_TIMEATTACK && TimeAttackGate /*&& TimeAttackGate->field_1C*/)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
    StateMachine_Run(entity->state);
}

void ChemicalBall_LateUpdate(void) {}

void ChemicalBall_StaticUpdate(void) {}

void ChemicalBall_Draw(void)
{
    RSDK_THIS(ChemicalBall);
    RSDK.DrawSprite(&entity->animator, &entity->startPos, false);
}

void ChemicalBall_Create(void *data)
{
    RSDK_THIS(ChemicalBall);

    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos2.x   = entity->position.x;
    entity->startPos2.y   = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    if (entity->interval == 0)
        entity->interval = -128;
    entity->startPos.x = entity->position.x;
    entity->startPos.y = entity->position.y;
    if (entity->direction == FLIP_NONE)
        entity->position.x += 0x320000;
    else
        entity->position.x -= 0x320000;

    if (entity->type == 1)
        entity->position.y -= 0x480000;
    RSDK.SetSpriteAnimation(ChemicalBall->aniFrames, 0, &entity->animator, true, 0);
    if (!entity->type)
        entity->state = ChemicalBall_MoveType0;
    else
        entity->state = ChemicalBall_MoveType1;
}

void ChemicalBall_StageLoad(void)
{
    ChemicalBall->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/ChemicalBall.bin", SCOPE_STAGE);
    ChemicalBall->hitbox.left   = -8;
    ChemicalBall->hitbox.top    = -8;
    ChemicalBall->hitbox.right  = 8;
    ChemicalBall->hitbox.bottom = 8;
    ChemicalBall->sfxBloop      = RSDK.GetSFX("Stage/Bloop.wav");
}

void ChemicalBall_CheckHit(void)
{
    RSDK_THIS(ChemicalBall);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, &entity->startPos, &ChemicalBall->hitbox)) {
            Player_CheckHit(player, entity);
        }
    }
}

void ChemicalBall_MoveType0(void)
{
    RSDK_THIS(ChemicalBall);

    int val = (Zone->timer + entity->intervalOffset) % (2 * entity->interval);
    if (val < entity->interval == (entity->direction != FLIP_NONE)) {
        if (!entity->direction)
            val -= entity->interval;
        if (val >= 96) {
            entity->startPos.x = ChemicalBall->array1[0];
            entity->startPos.y = ChemicalBall->array1[1];
        }
        else {
            entity->startPos.x = ChemicalBall->array1[2 * (97 - val)];
            entity->startPos.y = ChemicalBall->array1[2 * (97 - val) + 1];
        }
    }
    else {
        if (entity->direction == FLIP_X)
            val -= entity->interval;
        if (val >= 97) {
            entity->startPos.x = ChemicalBall->array1[194];
            entity->startPos.y = ChemicalBall->array1[195];
        }
        else {
            entity->startPos.x = ChemicalBall->array1[2 * val];
            entity->startPos.y = ChemicalBall->array1[2 * val + 1];
        }
    }

    entity->startPos.x += entity->position.x;
    entity->startPos.y += entity->position.y;
    if (!val || val == entity->interval)
        RSDK.PlaySFX(ChemicalBall->sfxBloop, 0, 255);
    ChemicalBall_CheckHit();
}

void ChemicalBall_MoveType1(void)
{
    RSDK_THIS(ChemicalBall);
    int val = (Zone->timer + entity->intervalOffset) % entity->interval;
    if (val < 98) {
        if (val >= 49) {
            if (!entity->direction)
                entity->startPos.x = -0x320000;
            else
                entity->startPos.x = 0x2E0000;
            entity->startPos.y = ChemicalBall->array2[97 - val];
        }
        else {
            if (!entity->direction)
                entity->startPos.x = 0x2E0000;
            else
                entity->startPos.x = -0x320000;
            entity->startPos.y = ChemicalBall->array2[val];
        }
    }
    entity->startPos.x += entity->position.x;
    entity->startPos.y += entity->position.y;
    if (val == 27 || val == 48)
        RSDK.PlaySFX(ChemicalBall->sfxBloop, 0, 255);
    ChemicalBall_CheckHit();
}

void ChemicalBall_EditorDraw(void) {}

void ChemicalBall_EditorLoad(void) {}

void ChemicalBall_Serialize(void) {}
