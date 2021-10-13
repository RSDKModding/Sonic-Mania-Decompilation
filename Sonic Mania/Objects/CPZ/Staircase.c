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

    int32 add             = (entity->direction) ? -0x200000 : 0x200000;
    entity->blocks[0].x = entity->position.x;
    entity->blocks[0].y = entity->position.y;
    entity->blocks[1].x = entity->position.x + 1 * add;
    entity->blocks[1].y = entity->position.y;
    entity->blocks[2].x = entity->position.x + 2 * add;
    entity->blocks[2].y = entity->position.y;
    entity->blocks[3].x = entity->position.x + 3 * add;
    entity->blocks[3].y = entity->position.y;

    RSDK.SetSpriteAnimation(Staircase->animID, 0, &entity->animator, true, 0);

    entity->state     = Staircase_MainState;
    entity->drawState = Staircase_DrawBlocks;
}

void Staircase_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        Staircase->animID = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE);
        Staircase->sfxID  = RSDK.GetSFX("CPZ/CPZ2HitBlocksStop.wav");
        Soundboard_LoadSFX("CPZ/CPZ2HitBlocks.wav", true, Staircase_SFXCallback, NULL);
    }
}

void Staircase_EditorDraw(void) {}

void Staircase_EditorLoad(void) {}

void Staircase_Serialize(void)
{
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, type);
    // fun fact: why is this never used
    RSDK_EDITABLE_VAR(Staircase, VAR_BOOL, bumpable);
}

bool32 Staircase_SFXCallback(void)
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
            int32 col          = Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox);
            if (col == 4) {
                entity->active    = ACTIVE_NORMAL;
                entity->timer     = 59;
                entity->state     = Staircase_Wait;
                entity->drawState = Staircase_DrawShake;
                if (player->onGround)
                    player->hurtFlag = 1;
            }
            else if (col == 1) {
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
        for (int32 i = 0; i < 4; i++) {
            entity->position = entity->blocks[i];
            Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox);
        }
    }
    entity->position = oldpos;

    if (--entity->timer < 0) {
        entity->timer = 128;
        if (!RSDK.IsSFXPlaying(Staircase->sfxID))
            RSDK.PlaySfx(Staircase->sfxID, 0, 255);
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
        for (int32 i = 0; i < 4; i++) {
            entity->position = entity->blocks[i];
            if (Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox) == 1)
                player->position.y += add * (i + 1);
        }
    }
    entity->position = oldpos;

    entity->blocks[0].y += 1 * add;
    entity->blocks[1].y += 2 * add;
    entity->blocks[2].y += 3 * add;
    entity->blocks[3].y += 4 * add;
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
        for (int32 i = 0; i < 4; i++) {
            entity->position = entity->blocks[i];
            if (Player_CheckCollisionBox(player, entity, &Staircase->blockHitbox) == 4 && player->onGround)
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
    for (int32 i = 0; i < 4; i++) {
        RSDK.DrawSprite(&entity->animator, &entity->blocks[i], false);
    }
}
void Staircase_DrawShake(void) {
    RSDK_THIS(Staircase);
    for (int32 i = 0; i < 4; i++) {
        Vector2 drawPos = entity->blocks[i];
        drawPos.y += ((entity->timer - i * 4) << 14) & 0x10000;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}