#include "SonicMania.h"

ObjectJawz *Jawz;

void Jawz_Update(void)
{
    RSDK_THIS(Jawz);
    StateMachine_Run(entity->state);
}

void Jawz_LateUpdate(void) {}

void Jawz_StaticUpdate(void) {}

void Jawz_Draw(void)
{
    RSDK_THIS(Jawz);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Jawz_Create(void *data)
{
    RSDK_THIS(Jawz);
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos      = entity->position;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->velocity.x    = 0;
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &entity->animator, true, 0);
    entity->state = Jawz_CheckPlayerTrigger;
}

void Jawz_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        Jawz->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jawz.bin", SCOPE_STAGE);
    Jawz->hitbox.left   = -20;
    Jawz->hitbox.top    = -5;
    Jawz->hitbox.right  = 20;
    Jawz->hitbox.bottom = 5;
    DEBUGMODE_ADD_OBJ(Jawz);
}

void Jawz_DebugSpawn(void)
{
    RSDK_THIS(Jawz);
    CREATE_ENTITY(Jawz, NULL, entity->position.x, entity->position.y);
}

void Jawz_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Jawz_CheckPlayerInteractions(void)
{
    RSDK_THIS(Jawz);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Jawz->hitbox) && !Player_CheckBadnikBreak(entity, player, true)) {
            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySFX(Explosion->sfx_Destroy, false, 255);
            destroyEntity(entity);
        }
    }
}

void Jawz_CheckPlayerTrigger(void)
{
    RSDK_THIS(Jawz);

    EntityPlayer *player = NULL;
    if (Player->playerCount <= 1 || RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->sidekick)
        player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    else
        player = Player_Unknown3();

    if (player) {
        if (abs(player->position.x - entity->position.x) < 0x1800000) {

            int playerID = RSDK.GetEntityID(player);
            switch (entity->triggerDir) {
                case 0:
                    if (player->position.x > entity->position.x) {
                        entity->direction  = FLIP_X;
                        entity->velocity.x = 0x20000;
                        entity->position.x = (RSDK_screens[playerID].position.x - 64) << 16;
                    }
                    else if (player->position.x <= entity->position.x) {
                        entity->direction  = FLIP_NONE;
                        entity->velocity.x = -0x20000;
                        entity->position.x = (RSDK_screens[playerID].position.x + RSDK_screens[playerID].width + 64) << 16;
                    }
                    break;
                case 1:
                    if (player->position.x < entity->position.x) {
                        entity->direction  = FLIP_NONE;
                        entity->velocity.x = -0x20000;
                        entity->position.x = (RSDK_screens[playerID].position.x + RSDK_screens[playerID].width + 64) << 16;
                    }
                    break;
                case 2:
                    if (player->position.x > entity->position.x) {
                        entity->direction  = FLIP_X;
                        entity->velocity.x = 0x20000;
                        entity->position.x = (RSDK_screens[playerID].position.x - 64) << 16;
                    }
                    break;
            }
        }
    }

    if (entity->velocity.x) {
        entity->active  = ACTIVE_NORMAL;
        entity->visible = 1;
        entity->state   = Jawz_State_Main;
        Jawz_State_Main();
    }
    else {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = 0;
    }
}

void Jawz_State_Main(void)
{
    RSDK_THIS(Jawz);
    entity->position.x += entity->velocity.x;
    RSDK.ProcessAnimation(&entity->animator);
    Jawz_CheckPlayerInteractions();

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        Jawz_Create(NULL);
    }
}

void Jawz_EditorDraw(void) {}

void Jawz_EditorLoad(void) {}

void Jawz_Serialize(void) { RSDK_EDITABLE_VAR(Jawz, VAR_UINT8, triggerDir); }
