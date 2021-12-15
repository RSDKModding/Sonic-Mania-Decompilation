// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DCEvent Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectDCEvent *DCEvent;

void DCEvent_Update(void)
{
    RSDK_THIS(DCEvent);
    StateMachine_Run(self->state);
}

void DCEvent_LateUpdate(void) {}

void DCEvent_StaticUpdate(void) {}

void DCEvent_Draw(void)
{
    RSDK_THIS(DCEvent);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void DCEvent_Create(void *data)
{
    RSDK_THIS(DCEvent);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->visible = true;

        int type = 2;
        if (self->type != 2)
            type = voidToInt(data);

        switch (type) {
            case 0:
                self->visible       = false;
                self->drawOrder     = Zone->playerDrawLow + 2;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                self->field_64      = self->position.y;
                self->timer2        = 2;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 0, &self->animator, true, 0);
                if (self->type) {
                    self->active = ACTIVE_BOUNDS;
                    self->state  = DCEvent_State1_Unknown1;
                }
                else {
                    self->active = ACTIVE_XBOUNDS;
                    self->state  = DCEvent_State_Unknown1;
                }
                break;
            case 1:
                self->active    = ACTIVE_NORMAL;
                self->drawOrder = Zone->playerDrawLow + 1;
                self->timer     = 480;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 5, &self->animator, true, 0);
                self->state = DCEvent_State2_Unknown1;
                break;
            case 2: {
                EntityWater *water = (EntityWater *)self;
                int x              = self->position.x;
                int y              = self->position.y;
                RSDK.ResetEntityPtr(water, Water->objectID, intToVoid(WATER_BUBBLE));
                water->position.x = x;
                water->bubbleX   = x;
                water->position.y = y;
                water->childPtr   = 0;
                water->speed      = -1;
                RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);
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

    DCEvent->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    DCEvent->sfxRumble    = RSDK.GetSfx("Stage/Rumble.wav");
    DCEvent->sfxImpact6   = RSDK.GetSfx("Stage/Impact6.wav");
    DCEvent->sfxImpact4   = RSDK.GetSfx("Stage/Impact4.wav");
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

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.y += 0x8000;
    }

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 2);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(DCEvent->sfxRumble, false, 255);

    ++self->timer;
    if (self->timer >= 0x880 || DCEvent->finished) {
        RSDK.PlaySfx(DCEvent->sfxImpact4, false, 255);
        destroyEntity(self);
    }
}

void DCEvent_State_Unknown1(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&self->animator);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > self->position.x - 0x100000)
        player1->stateInput = DCEvent_StateInput_Unknown1;

    if (player1->position.x > self->position.x) {
        player1->velocity.x      = 0;
        player1->velocity.y      = 0;
        player1->groundVel       = 0;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        player1->stateInput      = DCEvent_StateInput_Unknown2;
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &self->animator, true, 0);

        TileLayer *moveLayer    = RSDK.GetSceneLayer(Zone->moveLayer);
        moveLayer->drawLayer[0] = 6;

        self->position.x -= 0x1000000;
        self->timer      = 172;
        self->direction  = FLIP_X;
        self->velocity.x = 0x10000;
        self->active     = ACTIVE_NORMAL;
        self->visible    = true;
        self->state      = DCEvent_State_Unknown2;
    }
}

void DCEvent_StateInput_Unknown1(void)
{
    RSDK_THIS(Player);

    Player_ProcessP1Input();
    self->up        = false;
    self->down      = false;
    self->left      = false;
    self->right     = true;
    self->jumpPress = false;
    self->jumpHold  = false;
}

void DCEvent_StateInput_Unknown2(void)
{
    RSDK_THIS(Player);

    Player_ProcessP1Input();
    self->airTimer  = 0;
    self->up        = false;
    self->down      = true;
    self->left      = false;
    self->right     = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    if (self->onGround)
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_CROUCH, &self->animator, false, 1);
}

void DCEvent_State_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->camera;
    if (camera->lookPos.y >= 96)
        self->state = DCEvent_State_Unknown3;
}

void DCEvent_State_Unknown3(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;

    if (self->velocity.y > 0) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x400;
    }
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)
        || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    if (--self->timer <= 0) {
        switch (self->timer2) {
            case 0:
                DCEvent->finished = true;
                self->state     = DCEvent_State_Unknown5;
                break;
            case 1:
                self->timer2--;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 4, &self->animator, false, 0);
                self->state = DCEvent_State_Unknown4;
                break;
            case 2:
                self->direction = FLIP_NONE;
                self->timer2--;
                RSDK.SetSpriteAnimation(DCEvent->aniFrames, 4, &self->animator, false, 0);
                self->state = DCEvent_State_Unknown4;
                break;
        }
    }
}

void DCEvent_State_Unknown4(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID >= self->animator.frameCount - 1) {
        EntityDCEvent *child = CREATE_ENTITY(DCEvent, intToVoid(1), self->position.x, self->position.y + 0x20000);
        if (self->direction)
            child->position.x += 0x1A0000;
        else
            child->position.x -= 0x1A0000;

        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &self->animator, false, 0);

        if (self->timer2 == 1)
            self->direction = FLIP_X;
        self->timer = 160;
        self->state = DCEvent_State_Unknown3;
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
        self->state = DCEvent_State_Collapse;
    }
}

void DCEvent_State1_Unknown1(void)
{
    RSDK_THIS(DCEvent);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&self->animator);

    if (player1->position.y < self->position.y) {
        RSDK.SetSpriteAnimation(DCEvent->aniFrames, 1, &self->animator, true, 0);
        self->timer      = 900;
        self->direction  = FLIP_X;
        self->velocity.y = -0x8000;
        self->active     = ACTIVE_NORMAL;
        self->visible    = true;
        self->state      = DCEvent_State1_Unknown2;
    }
}

void DCEvent_State1_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    if (--self->timer <= 0)
        destroyEntity(self);
}

void DCEvent_State2_Unknown1(void)
{
    RSDK_THIS(DCEvent);

    if (self->velocity.x) {
        if (self->velocity.x <= 0)
            self->velocity.x += 0x2000;
        else
            self->velocity.x -= 0x2000;
    }

    if (self->velocity.y > 0) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x400;
    }

    RSDK.ProcessAnimation(&self->animator);
    if (DCEvent->finished) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(DCEvent->sfxImpact6, false, 255);
        RSDK.PlaySfx(DCEvent->sfxExplosion, false, 255);
        Camera_ShakeScreen(0, 4, 0);

        EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), self->position.x, self->position.y);
        water->velocity.y  = -0x8800;
        water->childPtr    = 0;
        water->angle       = 2 * RSDK.Rand(0, 256);
        water->bubbleX    = water->position.x;
        RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (self->position.x >= player1->position.x)
            self->position.x += 0x260000;
        else
            self->position.x -= 0x260000;

        for (int i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
            debris->position.x += RSDK.Rand(-8, 8) << 16;
            debris->position.y += RSDK.Rand(-8, 8) << 16;
            debris->velocity.x = RSDK.Rand(-8, 8) << 16;
            debris->velocity.y = RSDK.Rand(-8, 8) << 16;
            if (self->position.x >= player1->position.x)
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

        self->state = DCEvent_State2_Unknown2;
    }
}

void DCEvent_State2_Unknown2(void)
{
    RSDK_THIS(DCEvent);

    if (!(Zone->timer & 3)) {
        self->position.y -= 0x100000;
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), (RSDK.Rand(-16, 17) << 17) + self->position.x, self->position.y)->drawOrder =
            Zone->drawOrderHigh;
        RSDK.PlaySfx(DCEvent->sfxExplosion, false, 255);
        if (++self->timer2 == 16) {
            DCEvent->finished = false;
            destroyEntity(self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void DCEvent_EditorDraw(void)
{
    RSDK_THIS(DCEvent);

    switch (self->type) {
        case 0:
            self->drawOrder = Zone->playerDrawLow + 2;
            RSDK.SetSpriteAnimation(DCEvent->aniFrames, 0, &self->animator, true, 0);
            break;
        case 1:
            self->drawOrder = Zone->playerDrawLow + 1;
            RSDK.SetSpriteAnimation(DCEvent->aniFrames, 5, &self->animator, true, 0);
            break;
        case 2:
            if (Water)
                RSDK.SetSpriteAnimation(Water->aniFrames, 3, &self->animator, true, 0);
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
