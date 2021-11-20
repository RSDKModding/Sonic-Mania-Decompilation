#include "SonicMania.h"

ObjectFBZ1Outro *FBZ1Outro;

void FBZ1Outro_Update(void)
{
    RSDK_THIS(FBZ1Outro);
    if (!self->activated) {
        FBZ1Outro_StartCutscene();
        self->activated = true;
    }
}

void FBZ1Outro_LateUpdate(void) {}

void FBZ1Outro_StaticUpdate(void) {}

void FBZ1Outro_Draw(void) {}

void FBZ1Outro_Create(void *data)
{
    RSDK_THIS(FBZ1Outro);
    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NEVER;
}

void FBZ1Outro_StageLoad(void)
{
    foreach_all(BigSqueeze, boss)
    {
        switch (boss->type) {
            case 2: FBZ1Outro->boss1 = boss; break;
            case 3: FBZ1Outro->boss2 = boss; break;
            case 0: FBZ1Outro->boss3 = boss; break;
        }
    }

    foreach_all(CollapsingPlatform, platform)
    {
        if (platform->eventOnly) {
            FBZ1Outro->collapsingPlatform = platform;
            foreach_break;
        }
    }

    foreach_all(Crane, crane)
    {
        if (crane->position.x == 0x33400000 && crane->position.y == 0x9100000)
            FBZ1Outro->craneP1 = crane;
        if (crane->position.x == 0x33640000 && crane->position.y == 0x9100000)
            FBZ1Outro->craneP2 = crane;
    }

    FBZ1Outro->sfxDrop = RSDK.GetSfx("Stage/Drop.wav");
}

void FBZ1Outro_StartCutscene(void)
{
    RSDK_THIS(FBZ1Outro);
    void *states[] = { FBZ1Outro_CutsceneState_Unknown1, FBZ1Outro_CutsceneState_Unknown2, FBZ1Outro_CutsceneState_Unknown3,
                       FBZ1Outro_CutsceneState_Unknown4, NULL };

    CutsceneSeq_StartSequence((Entity *)self, states);

    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_DISABLED;
}

void FBZ1Outro_Unknown2(void)
{
    foreach_active(FBZTrash, trash) { trash->velocity.y = 0; }
    foreach_active(FBZSinkTrash, sinkTrash) { sinkTrash->velocity.y = 0; }

    foreach_active(SignPost, signPost)
    {
        signPost->velocity.x = 0;
        signPost->velocity.y = 0;
        signPost->position.y = BigSqueeze->value7 - 0x180000;

        int32 dist = BigSqueeze->value4[3] - 0x180000;
        if (signPost->position.x < BigSqueeze->value4[3] - 0x180000)
            dist = signPost->position.x;

        if (BigSqueeze->value4[2] + 0x180000 <= dist) {
            dist = BigSqueeze->value4[3] - 0x180000;
            if (signPost->position.x < BigSqueeze->value4[3] - 0x180000)
                dist = signPost->position.x;
        }
        signPost->position.x = dist;
    }
}

void FBZ1Outro_DispenseTrash(void)
{
    foreach_active(FBZTrash, trash)
    {
        trash->state = 0;
        trash->position.y += trash->velocity.y;
        trash->velocity.y += 0x3800;
    }
    foreach_active(FBZSinkTrash, sinkTrash)
    {
        sinkTrash->position.y += sinkTrash->velocity.y;
        sinkTrash->velocity.y += 0x3800;
        sinkTrash->type = 2;
    }

    foreach_active(SignPost, signPost)
    {
        signPost->position.y += signPost->velocity.y;
        signPost->velocity.y += 0x3800;
        signPost->type = 2;
    }
}

bool32 FBZ1Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(FBZ1Outro);
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    EntityBigSqueeze *boss1 = FBZ1Outro->boss1;
    EntityBigSqueeze *boss2 = FBZ1Outro->boss2;
    EntityBigSqueeze *boss3 = FBZ1Outro->boss3;

    EntityCollapsingPlatform *platform = FBZ1Outro->collapsingPlatform;
    if (!host->timer) {
        player1->stateInput = StateMachine_None;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BALANCE1, &player1->animator, false, 0);
        player1->state = Player_State_None;
        CutsceneSeq_LockPlayerControl(player1);
        if (player2->objectID == Player->objectID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_BALANCE1, &player2->animator, false, 0);
            player2->state = Player_State_None;
            CutsceneSeq_LockPlayerControl(player2);
        }
    }
    player1->nextGroundState = StateMachine_None;
    player1->nextAirState    = StateMachine_None;
    if (player2->objectID == Player->objectID) {
        player2->nextGroundState = StateMachine_None;
        player2->nextAirState    = StateMachine_None;
    }

    if (BigSqueeze->value4[3] - BigSqueeze->value4[2] <= 0xB00000) {
        BigSqueeze->isRumbling = false;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_HURT, &player1->animator, false, 0);
        player1->state    = Player_State_Air;
        player1->onGround = false;
        if (player2->objectID == Player->objectID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_HURT, &player2->animator, false, 0);
            player2->state    = Player_State_Air;
            player2->onGround = false;
        }
        else {
            destroyEntity(FBZ1Outro->craneP2);
            self->grabbedPlayers |= 2;
        }
        boss1->timer2               = 0;
        boss1->state                = BigSqueeze_State3_Unknown1;
        boss2->timer2               = 0;
        boss2->state                = BigSqueeze_State3_Unknown1;
        boss3->state                = BigSqueeze_Unknown18;
        Zone->screenBoundsB1[0] = 2660;
        Zone->screenBoundsB1[1] = 2660;
        Zone->screenBoundsR1[0] = 14080;
        Zone->screenBoundsR1[1] = 14080;
        platform->playerPos.x   = 1;
        RSDK.PlaySfx(FBZ1Outro->sfxDrop, false, 255);
        return true;
    }
    else {
        BigSqueeze->isRumbling = true;
        FBZ1Outro_Unknown2();
        if (host->values[0])
            host->values[0] = 1;
        boss1->timer2 = 0;
        boss1->state  = BigSqueeze_State3_Unknown2;
        boss2->timer2 = 0;
        boss2->state  = BigSqueeze_State3_Unknown2;
    }
    return false;
}
bool32 FBZ1Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(FBZ1Outro);
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    EntityBigSqueeze *boss1 = FBZ1Outro->boss1;
    EntityBigSqueeze *boss2 = FBZ1Outro->boss2;
    boss1->timer2            = 0;
    boss1->state             = BigSqueeze_State3_Unknown1;
    boss2->timer2            = 0;
    boss2->state             = BigSqueeze_State3_Unknown1;

    EntityCrane *craneP1 = FBZ1Outro->craneP1;
    craneP1->position.x  = player1->position.x;
    if (craneP1->state == Crane_State_Unknown5) {
        craneP1->startPos.x = player1->position.x;
        craneP1->startPos.y = craneP1->position.y;
        self->grabbedPlayers |= 1;
    }
    if (player2->objectID == Player->objectID) {
        EntityCrane *craneP2 = FBZ1Outro->craneP2;
        craneP2->position.x  = player2->position.x;
        if (craneP2->state == Crane_State_Unknown5) {
            craneP2->startPos.x = craneP2->position.x;
            craneP2->startPos.y = craneP2->position.y;
            self->grabbedPlayers |= 2;
        }
    }
    if (self->grabbedPlayers == 3)
        return true;
    FBZ1Outro_DispenseTrash();
    return false;
}
bool32 FBZ1Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    FBZ1Outro_DispenseTrash();
    return player1->onGround && (player2->objectID != Player->objectID || player2->onGround);
}
bool32 FBZ1Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    if (!host->timer) {
        Zone->screenBoundsL1[0]     = 13568;
        Zone->screenBoundsL1[1]     = 13568;
        Zone->screenBoundsB1[0]     = 2660;
        Zone->screenBoundsB1[0]     = 2660;
        Zone->playerBoundActiveL[0] = true;
    }
    RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, false, 0);
    if (player2->objectID == Player->objectID)
        RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, false, 0);

    if (camera->offset.x || ScreenInfo->position.x < Zone->screenBoundsL1[0] || host->timer < 30) {
        if (ScreenInfo->position.x < Zone->screenBoundsL1[0])
            ScreenInfo->position.x++;
        if (camera->offset.x > 0)
            camera->offset.x -= 0x10000;
    }
    else {
        Zone_StoreEntities(0x35D40000, 0xA640000);
        RSDK.LoadScene();

        int32 id           = 0;
        TileLayer *layer = RSDK.GetSceneLayer(1);
        for (int32 i = 0; i < layer->scrollInfoCount; ++i) {
            globals->parallaxOffset[id++] = layer->scrollInfo[i].scrollPos + layer->scrollInfo[i].parallaxFactor * ScreenInfo->position.x;
        }

        foreach_all(ParallaxSprite, parallaxSprite)
        {
            globals->parallaxOffset[id++] = parallaxSprite->scrollPos.x + parallaxSprite->parallaxFactor.x * ScreenInfo->position.x;
        }

        return true;
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void FBZ1Outro_EditorDraw(void)
{
    RSDK_THIS(FBZ1Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void FBZ1Outro_EditorLoad(void) {}
#endif

void FBZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(FBZ1Outro, VAR_VECTOR2, size); }
