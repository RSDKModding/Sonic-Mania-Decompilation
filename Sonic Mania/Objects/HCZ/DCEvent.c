#include "SonicMania.h"

ObjectDCEvent *DCEvent;

void DCEvent_Update(void)
{
    RSDK_THIS(DCEvent);
    StateMachine_Run(entity->state);
}

void DCEvent_LateUpdate(void) {}

void DCEvent_StaticUpdate(void) {}

void DCEvent_Draw(void)
{
    RSDK_THIS(DCEvent);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void DCEvent_Create(void *data)
{
    RSDK_THIS(DCEvent);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;

        int type = 2;
        if (entity->type != 2)
            type = voidToInt(data);

        switch (type) {
            case 0:
                entity->visible       = false;
                entity->drawOrder     = Zone->playerDrawLow + 2;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = 0x800000;
                entity->field_64      = entity->position.y;
                entity->timer2        = 2;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 0, &entity->animator, true, 0);
                if (entity->type) {
                    entity->active = ACTIVE_BOUNDS;
                    entity->state  = DCEvent_State1_Unknown1;
                }
                else {
                    entity->active = ACTIVE_XBOUNDS;
                    entity->state  = DCEvent_State_Unknown1;
                }
                break;
            case 1:
                entity->active    = ACTIVE_NORMAL;
                entity->drawOrder = Zone->playerDrawLow + 1;
                entity->timer     = 480;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 5, &entity->animator, true, 0);
                entity->state = DCEvent_State2_Unknown1;
                break;
            case 2: {
                EntityWater *water = (EntityWater *)entity;
                int x              = entity->position.x;
                int y              = entity->position.y;
                RSDK.ResetEntityPtr(water, Water->objectID, intToVoid(WATER_BUBBLE));
                water->position.x = x;
                water->field_68   = x;
                water->position.y = y;
                water->childPtr   = 0;
                water->speed      = -1;
                RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &water->animator, true, 0);
                break;
            }
        }
    }
}

void DCEvent_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ")) {
        DCEvent->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE);
        DCEvent->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ1.bin", SCOPE_STAGE);
    }
    DCEvent->finished = false;

    DCEvent->hitbox1.left   = -8;
    DCEvent->hitbox1.top    = -8;
    DCEvent->hitbox1.right  = 8;
    DCEvent->hitbox1.bottom = 8;

    DCEvent->hitbox2.left   = -8;
    DCEvent->hitbox2.top    = -8;
    DCEvent->hitbox2.right  = 8;
    DCEvent->hitbox2.bottom = 8;

    DCEvent->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    DCEvent->sfxRumble    = RSDK.GetSFX("Stage/Rumble.wav");
    DCEvent->sfxImpact6   = RSDK.GetSFX("Stage/Impact6.wav");
    DCEvent->sfxImpact4   = RSDK.GetSFX("Stage/Impact4.wav");
}

void DCEvent_State_Collapse(void)
{
    RSDK_THIS(DCEvent);

    TileLayer *move = RSDK.GetSceneLayer(Zone->moveLayer);
    move->scrollPos -= 0x8000;

    foreach_active(Player, player)
    {
        player->collisionLayers |= Zone->moveID;
        player->moveOffset.x = move->scrollInfo[0].scrollPos;
        player->moveOffset.y = move->scrollPos;
    }

    int slot = RSDK_sceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.y += 0x8000;
    }

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 2);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(DCEvent->sfxRumble, false, 255);

    ++entity->timer;
    if (entity->timer >= 0x880 || DCEvent->finished) {
        RSDK.PlaySfx(DCEvent->sfxImpact4, false, 255);
        destroyEntity(entity);
    }
}

void DCEvent_State_Unknown1(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&entity->animator);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > entity->position.x - 0x100000)
        player1->stateInput = DCEvent_StateInput_Unknown1;

    if (player1->position.x > entity->position.x) {
        player1->velocity.x      = 0;
        player1->velocity.y      = 0;
        player1->groundVel       = 0;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        player1->stateInput      = DCEvent_StateInput_Unknown2;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &entity->animator, true, 0);

        TileLayer *moveLayer    = RSDK.GetSceneLayer(Zone->moveLayer);
        moveLayer->drawLayer[0] = 6;

        entity->position.x -= 0x1000000;
        entity->timer      = 172;
        entity->direction  = FLIP_X;
        entity->velocity.x = 0x10000;
        entity->active     = ACTIVE_NORMAL;
        entity->visible    = true;
        entity->state      = DCEvent_State_Unknown2;
    }
}

void DCEvent_StateInput_Unknown1(void)
{
    RSDK_THIS(Player);

    Player_ProcessP1Input();
    entity->up        = false;
    entity->down      = false;
    entity->left      = false;
    entity->right     = true;
    entity->jumpPress = false;
    entity->jumpHold  = false;
}

void DCEvent_StateInput_Unknown2(void)
{
    RSDK_THIS(Player);

    Player_ProcessP1Input();
    entity->airTimer  = 0;
    entity->up        = false;
    entity->down      = true;
    entity->left      = false;
    entity->right     = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    if (entity->onGround)
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimator, false, 1);
}

void DCEvent_State_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->camera;
    if (camera->lookPos.y >= 96)
        entity->state = DCEvent_State_Unknown3;
}

void DCEvent_State_Unknown3(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;

    if (entity->velocity.y > 0) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x400;
    }
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }

    if (--entity->timer <= 0) {
        switch (entity->timer2) {
            case 0:
                DCEvent->finished = true;
                entity->state     = DCEvent_State_Unknown5;
                break;
            case 1:
                entity->timer2--;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 4, &entity->animator, false, 0);
                entity->state = DCEvent_State_Unknown4;
                break;
            case 2:
                entity->direction = FLIP_NONE;
                entity->timer2--;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 4, &entity->animator, false, 0);
                entity->state = DCEvent_State_Unknown4;
                break;
        }
    }
}

void DCEvent_State_Unknown4(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID >= entity->animator.frameCount - 1) {
        EntityDCEvent *child = CREATE_ENTITY(DCEvent, intToVoid(1), entity->position.x, entity->position.y + 0x20000);
        if (entity->direction)
            child->position.x += 0x1A0000;
        else
            child->position.x -= 0x1A0000;

        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &entity->animator, false, 0);

        if (entity->timer2 == 1)
            entity->direction = FLIP_X;
        entity->timer = 160;
        entity->state = DCEvent_State_Unknown3;
    }
}

void DCEvent_State_Unknown5(void)
{
    RSDK_THIS(DCEvent);

    if (!DCEvent->finished) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->stateInput   = Player_ProcessP1Input;

        Water->targetWaterLevel = 0x8DC0000;
        Water->waterMoveSpeed   = 0x8000;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        entity->state = DCEvent_State_Collapse;
    }
}

void DCEvent_State1_Unknown1(void)
{
    RSDK_THIS(DCEvent);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&entity->animator);

    if (player1->position.y < entity->position.y) {
        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &entity->animator, true, 0);
        entity->timer      = 900;
        entity->direction  = FLIP_X;
        entity->velocity.y = -0x8000;
        entity->active     = ACTIVE_NORMAL;
        entity->visible    = true;
        entity->state      = DCEvent_State1_Unknown2;
    }
}

void DCEvent_State1_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&entity->animator);

    entity->position.y += entity->velocity.y;
    if (--entity->timer <= 0)
        destroyEntity(entity);
}

void DCEvent_State2_Unknown1(void)
{
    RSDK_THIS(DCEvent);

    if (entity->velocity.x) {
        if (entity->velocity.x <= 0)
            entity->velocity.x += 0x2000;
        else
            entity->velocity.x -= 0x2000;
    }

    if (entity->velocity.y > 0) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x400;
    }

    RSDK.ProcessAnimation(&entity->animator);
    if (DCEvent->finished) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(DCEvent->sfxImpact6, false, 255);
        RSDK.PlaySfx(DCEvent->sfxExplosion, false, 255);
        Camera_ShakeScreen(4, 0, 0);

        EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), entity->position.x, entity->position.y);
        water->velocity.y  = -0x8800;
        water->childPtr    = 0;
        water->angle       = 2 * RSDK.Rand(0, 256);
        water->field_68    = water->position.x;
        RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &water->animator, true, 0);

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (entity->position.x >= player1->position.x)
            entity->position.x += 0x260000;
        else
            entity->position.x -= 0x260000;

        for (int i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
            debris->position.x += RSDK.Rand(-8, 8) << 16;
            debris->position.y += RSDK.Rand(-8, 8) << 16;
            debris->velocity.x = RSDK.Rand(-8, 8) << 16;
            debris->velocity.y = RSDK.Rand(-8, 8) << 16;
            if (entity->position.x >= player1->position.x)
                debris->velocity.x = RSDK.Rand(-8, 0) << 15;
            else
                debris->velocity.x = RSDK.Rand(1, 9) << 15;
            debris->velocity.y = RSDK.Rand(-8, 5) << 16;
            debris->direction  = RSDK.Rand(0, 4);
            debris->drawFX     = FX_FLIP;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
            RSDK.CopyTileLayer(Zone->moveLayer, 1038, 146, Zone->moveLayer, 1068, 8, 20, 6);
        }

        entity->state = DCEvent_State2_Unknown2;
    }
}

void DCEvent_State2_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    if (!(Zone->timer & 3)) {
        entity->position.y -= 0x100000;
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), (RSDK.Rand(-16, 17) << 17) + entity->position.x, entity->position.y)->drawOrder =
            Zone->drawOrderHigh;
        RSDK.PlaySfx(DCEvent->sfxExplosion, false, 255);
        if (++entity->timer2 == 16) {
            DCEvent->finished = false;
            destroyEntity(entity);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void DCEvent_EditorDraw(void)
{
    RSDK_THIS(DCEvent);

    switch (entity->type) {
        case 0:
            entity->drawOrder = Zone->playerDrawLow + 2;
            RSDK.SetSpriteAnimation(DCEvent->aniFrames, 0, &entity->animator, true, 0);
            break;
        case 1:
            entity->drawOrder = Zone->playerDrawLow + 1;
            RSDK.SetSpriteAnimation(DCEvent->aniFrames, 5, &entity->animator, true, 0);
            break;
        case 2:
            if (Water)
                RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &entity->animator, true, 0);
            break;
    }

    DCEvent_Draw();
}

void DCEvent_EditorLoad(void)
{
    DCEvent->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE);
    DCEvent->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ1.bin", SCOPE_STAGE);
}
#endif

void DCEvent_Serialize(void)
{
    RSDK_EDITABLE_VAR(DCEvent, VAR_UINT8, numChildren);
    RSDK_EDITABLE_VAR(DCEvent, VAR_UINT8, type);
}
