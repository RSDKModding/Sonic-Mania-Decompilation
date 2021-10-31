#include "SonicMania.h"

ObjectStaircase *Staircase;

void Staircase_Update(void)
{
    RSDK_THIS(Staircase);
    StateMachine_Run(entity->state);
}

void Staircase_LateUpdate(void) {}

void Staircase_StaticUpdate(void) {}

void Staircase_Draw(void)
{
    RSDK_THIS(Staircase);
    StateMachine_Run(entity->drawState);
}

void Staircase_Create(void *data)
{
    RSDK_THIS(Staircase);

    entity->visible               = true;
    entity->drawOrder             = Zone->drawOrderLow;
    entity->active                = ACTIVE_BOUNDS;
    entity->updateRange.x         = 0x1000000;
    entity->updateRange.y         = 0x1000000;
    Staircase->blockHitbox.top    = -16;
    Staircase->blockHitbox.left   = -16;
    Staircase->blockHitbox.bottom = 16;
    Staircase->blockHitbox.right  = 16;

    int32 add = (entity->direction) ? -0x200000 : 0x200000;

    for (int i = 0; i < Starcase_StairCount; ++i) {
        entity->blocks[i].x = entity->position.x + i * add;
        entity->blocks[i].y = entity->position.y;
    }

    RSDK.SetSpriteAnimation(Staircase->animID, 0, &entity->animator, true, 0);

    entity->state     = Staircase_MainState;
    entity->drawState = Staircase_DrawBlocks;
}

void Staircase_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        Staircase->animID = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE);
        Staircase->sfxID  = RSDK.GetSFX("CPZ/CPZ2HitBlocksStop.wav");
        Soundboard_LoadSFX("CPZ/CPZ2HitBlocks.wav", true, Staircase_CheckCB, NULL);
    }
}

bool32 Staircase_CheckCB(void)
{
    foreach_active(Staircase, entity)
    {
        if (entity->drawState == Staircase_DrawShake)
            return true;
    }
    return false;
}

void Staircase_MainState(void)
{
    RSDK_THIS(Staircase);
    Vector2 oldpos = entity->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < 4; i++) {
            entity->position = entity->blocks[i];
            int32 side          = Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox);
            if (side == C_BOTTOM) {
                //bumpable would prolly be used here :D
                entity->active    = ACTIVE_NORMAL;
                entity->timer     = 59;
                entity->state     = Staircase_Wait;
                entity->drawState = Staircase_DrawShake;
                if (player->onGround)
                    player->hurtFlag = 1;
            }
            else if (side == C_TOP) {
                entity->active = ACTIVE_NORMAL;
                entity->timer  = 32;
                entity->state  = Staircase_Wait;
            }
        }
    }
    entity->position = oldpos;
}

void Staircase_Wait(void)
{
    RSDK_THIS(Staircase);
    Vector2 oldpos = entity->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            entity->position = entity->blocks[i];
            Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox);
        }
    }
    entity->position = oldpos;

    if (--entity->timer < 0) {
        entity->timer = 128;
        if (!RSDK.IsSFXPlaying(Staircase->sfxID))
            RSDK.PlaySfx(Staircase->sfxID, false, 255);
        entity->state     = Staircase_MoveBlocks;
        entity->drawState = Staircase_DrawBlocks;
    }
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        Staircase_Create(NULL);
}

void Staircase_MoveBlocks(void)
{
    RSDK_THIS(Staircase);
    int32 add        = (entity->type) ? -0x4000 : 0x4000;
    Vector2 oldpos = entity->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            entity->position = entity->blocks[i];
            if (Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox) == 1)
                player->position.y += add * (i + 1);
        }
    }
    entity->position = oldpos;

    for (int i = 0; i < Starcase_StairCount; ++i) entity->blocks[i].y += (i + 1) * add;

    if (--entity->timer <= 0)
        entity->state = Staircase_BasicCollision;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        Staircase_Create(NULL);
}

void Staircase_BasicCollision(void)
{
    RSDK_THIS(Staircase);
    Vector2 oldpos = entity->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            entity->position = entity->blocks[i];
            if (Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox) == C_BOTTOM && player->onGround)
                player->hurtFlag = 1;
        }
    }
    entity->position = oldpos;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        Staircase_Create(NULL);
}

void Staircase_DrawBlocks(void)
{
    RSDK_THIS(Staircase);
    for (int32 i = 0; i < Starcase_StairCount; i++) {
        RSDK.DrawSprite(&entity->animator, &entity->blocks[i], false);
    }
}
void Staircase_DrawShake(void) {
    RSDK_THIS(Staircase);
    for (int32 i = 0; i < Starcase_StairCount; i++) {
        Vector2 drawPos = entity->blocks[i];
        drawPos.y += ((entity->timer - i * 4) << 14) & 0x10000;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void Staircase_EditorDraw(void)
{
    RSDK_THIS(Staircase);

    int32 add = (entity->direction) ? -0x200000 : 0x200000;

    for (int i = 0; i < Starcase_StairCount; ++i) {
        entity->blocks[i].x = entity->position.x + i * add;
        entity->blocks[i].y = entity->position.y;
    }

    Staircase_DrawBlocks();
}

void Staircase_EditorLoad(void) { Staircase->animID = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE); }
#endif

void Staircase_Serialize(void)
{
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, type);
    // fun fact: why is this never used
    RSDK_EDITABLE_VAR(Staircase, VAR_BOOL, bumpable);
}