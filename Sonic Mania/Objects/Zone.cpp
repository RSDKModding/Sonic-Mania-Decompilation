#include "../SonicMania.hpp"

ObjectZone *Zone;

void Zone_Update() {}

void Zone_LateUpdate() {}

void Zone_StaticUpdate()
{
    ++Zone->timer;
    Zone->timer &= 0x7FFFu;
    ++Zone->timer2;
    Zone->timer2 &= 0x7FFFu;
    if (!(Zone->timer & 1)) {
        ++Zone->ringFrame;
        Zone->ringFrame &= 0xFu;
    }

    int zone = Zone_GetZoneID();
    if (zone >= 13) {
        zone = 13;
    }
    else {
        if (zone == -1)
            return;
    }

    int act = Zone->actID;
    if (act >= 3)
        act = 0;
    int pos = act + 2 * zone;
    if (pos >= 0 && RSDK_sceneInfo->timeEnabled && options->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM[pos + 0x22];
}

void Zone_Draw()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->screenID >= 4 || entity->screenID == RSDK_sceneInfo->currentScreenID) {
        if (entity->stateDraw)
            entity->stateDraw();
    }
}

void Zone_Create(void *data)
{
    EntityZone *entity               = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->active = ACTIVE_ALWAYS;
    if (!entity->stateDraw) {
        entity->visible   = 0;
        entity->drawOrder = -1;
    }
}

void Zone_StageLoad()
{

    // TODO: this junk here

    Zone->timer          = 0;
    Zone->field_154      = 0;
    Zone->ringFrame      = 0;
    Zone->field_15C      = 0;
    Zone->field_50       = 0;
    Zone->fgLayerLow     = 0;
    Zone->drawOrderLow   = 2;
    Zone->playerDrawLow  = 4;
    Zone->playerDrawHigh = 6;
    Zone->drawOrderHigh  = 8;
    Zone->uiDrawLow      = 12;
    Zone->uiDrawHigh     = 14;

    Zone->fgLow        = RSDK.GetSceneLayerID("FG Low");
    Zone->fgHigh       = RSDK.GetSceneLayerID("FG High");
    Zone->moveLayer    = RSDK.GetSceneLayerID("Move");
    Zone->scratchLayer = RSDK.GetSceneLayerID("Scratch");

    if (Zone->fgLowID) {
        Zone->fgLowID = 1 << Zone->fgLowID;
    }

    if (Zone->fgHigh) {
        Zone->fgHighID = 1 << Zone->fgHigh;
    }

    if (Zone->moveLayer) {
        Zone->moveID = 1 << Zone->moveLayer;
    }
    Zone->fgLayers = 1 << Zone->fgLow;
    Zone->fgLayers |= 1 << Zone->fgHigh;
    Vector2 layerSize;
    RSDK.GetLayerSize(Zone->fgLow, &layerSize, true);

    if (!options->initCoolBonus) {
        options->coolBonus[0]  = 10000;
        options->coolBonus[1]  = 10000;
        options->coolBonus[2]  = 10000;
        options->coolBonus[3]  = 10000;
        options->initCoolBonus = true;
    }

    for (EntityZone *Ent = NULL; RSDK.GetObjects(Zone->objectID, (Entity **)&Ent); RSDK.DestroyEntity(Ent, 0, 0)) {
    }

    RSDK.ResetEntity(SLOT_ZONE, Zone->objectID, 0);
    if (options->gameMode = MODE_COMPETITION) {
        if (RSDK.CheckStageFolder("Puyo")) {
            if (options->gameMode == MODE_COMPETITION) {
                RSDK.SetSettingsValue(12, 1);
            }
            else {
                /// Competition_Unknown2();
                RSDK.SetSettingsValue(12, 1);
            }
        }
        else {
            if (options->competitionSession[23] >= 2) {
                if (options->competitionSession[23] > 4)
                    options->competitionSession[23] = 4;
                RSDK.SetSettingsValue(12, options->competitionSession[23]);
            }
            else {
                options->competitionSession[23] = 2;
                RSDK.SetSettingsValue(12, options->competitionSession[23]);
            }
        }
    }
    else {
        // Competition_Unknown2();
        RSDK.SetSettingsValue(12, 1);
    }
    switch (options->gameMode) {
        case MODE_MANIA:
            // Localization_Unknown1(&textInfo, 0x3Fu);
            // User.PossiblyGetStrings(2, &textInfo);
            break;
        case MODE_ENCORE:
            // Localization_Unknown1(&textInfo, 0x40u);
            // User.PossiblyGetStrings(3, &textInfo);
            break;
        case MODE_TIMEATTACK:
            // Localization_Unknown1(&textInfo, 0x41u);
            // User.PossiblyGetStrings(4, &textInfo);
            break;
        case MODE_COMPETITION:
            // Localization_Unknown1(&textInfo, 0x42u);
            // User.PossiblyGetStrings(5, &textInfo);
            break;
        default: break;
    }
    Zone->sfx_fail = RSDK.GetSFX("Stage/Fail.wav");
}

int Zone_GetZoneID()
{
    if (RSDK.CheckStageFolder("GHZ"))
        return 0;
    if (RSDK.CheckStageFolder("CPZ"))
        return 1;
    if (RSDK.CheckStageFolder("SPZ1") || RSDK.CheckStageFolder("SPZ2"))
        return 2;
    if (RSDK.CheckStageFolder("FBZ"))
        return 3;
    if (RSDK.CheckStageFolder("PSZ1") || RSDK.CheckStageFolder("PSZ2"))
        return 4;
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        return 5;
    if (RSDK.CheckStageFolder("HCZ"))
        return 6;
    if (RSDK.CheckStageFolder("MSZ"))
        return 7;
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        return 8;
    if (RSDK.CheckStageFolder("LRZ1") || RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        return 9;
    if (RSDK.CheckStageFolder("MMZ"))
        return 10;
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2") || RSDK.CheckStageFolder("TMZ3"))
        return 11;
    if (RSDK.CheckStageFolder("ERZ"))
        return 12;
    if (RSDK.CheckStageFolder("AIZ") && options->gameMode == MODE_ENCORE)
        return 13;
    return -1;
}

void Zone_StoreEntities(int xOffset, int yOffset)
{
    int count      = 0;
    Entity *entity = NULL;
    if (RSDK.GetActiveObjects(Player->objectID, &entity)) {
        int pos = 0;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            int id                        = RSDK.GetEntityID(entity);
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveObjects(Player->objectID, &entity));
    }

    entity = NULL;
    if (RSDK.GetActiveObjects(SignPost->objectID, &entity)) {
        int pos = count << 11;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            int id                        = RSDK.GetEntityID(entity);
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveObjects(SignPost->objectID, &entity));
    }

    entity = NULL;
    if (RSDK.GetActiveObjects(ItemBox->objectID, &entity)) {
        int pos = count << 11;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            int id                        = RSDK.GetEntityID(entity);
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveObjects(ItemBox->objectID, &entity));
    }

    EntityPlayer *player     = (EntityPlayer *)RSDK.GetObjectByID(SLOT_PLAYER1);
    options->restartLives[0] = player->lives;
    options->restartScore    = player->score;
    options->restartShield   = player->shield;
    options->atlEntityCount  = count;
    options->atlEnabled      = true;
}

void Zone_Unknown1(int fade1, int fade2)
{
    EntityZone *zone = (EntityZone *)RSDK.GetObjectByID(SLOT_ZONE);
    zone->fade2      = fade2;
    zone->fade1      = fade1;
    zone->screenID   = 4;
    zone->timer      = 0;
    zone->state      = Zone_Unknown13;
    zone->stateDraw  = Zone_Unknown12;
    zone->visible    = true;
    zone->drawOrder  = 15;
}

void Zone_Unknown2()
{
    EntityZone *entity = (EntityZone *)RSDK.GetObjectByID(SLOT_ZONE);
    entity->screenID   = 4;
    entity->timer      = 0;
    entity->fade1      = 10;
    entity->fade2      = 0;
    entity->state      = Zone_Unknown13;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    // if (Music->ActiveTrack != 8) {
    //    RSDK.DestroyEntity(music, Music->objectID, 0);
    //    EntityMusic *music = (EntityMusic *)RSDK.GetObjectByID(SLOT_MUSIC);
    //    music->state    = Music_Unknown14;
    //    music->field_88 = 1.0;
    //    music->field_8C = 0.025;
    //}
}

void Zone_Unknown3(Entity *entity, Vector2 *pos, int angle)
{
    int v5 = (pos->x - entity->position.x) >> 8;
    int v6 = (pos->y - entity->position.y) >> 8;
    int v7 = v6 * RSDK.Sin256(angle);
    pos->x = v7 + v5 * RSDK.Cos256(angle) + entity->position.x;
    int v8 = v6 * RSDK.Cos256(angle);
    int v9 = v8 - v5 * RSDK.Sin256(angle);
    pos->y = v9 + entity->position.y;
}

void Zone_Unknown4(int screen)
{
    EntityZone *entity = (EntityZone *)RSDK.SpawnEntity(Zone->objectID, 0, 0, 0);
    entity->screenID   = screen;
    entity->timer      = 640;
    entity->fade1      = 16;
    entity->fade2      = 0xF0F0F0;
    if (options->gameMode != MODE_ENCORE || EncoreIntro) {
        entity->state     = Zone_Unknown18;
        entity->stateDraw = Zone_Unknown12;
        entity->visible   = true;
        entity->drawOrder = 15;
    }
    else {
        entity->state     = Zone_Unknown17;
        entity->stateDraw = Zone_Unknown12;
        entity->visible   = true;
        entity->drawOrder = 15;
    }
}

void Zone_Unknown5()
{
    EntityZone *entity = (EntityZone *)RSDK.SpawnEntity(Zone->objectID, 0, 0, 0);
    entity->screenID   = 4;
    entity->timer      = 640;
    entity->fade1      = 16;
    entity->fade2      = 0xF0F0F0;
    entity->state      = Zone_Unknown20;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    Zone->field_4724   = 1;
}

bool32 Zone_Unknown8()
{
    if (RSDK.CheckStageFolder("GHZ") && Zone->actID == 1 || RSDK.CheckStageFolder("CPZ") && Zone->actID == 1 || RSDK.CheckStageFolder("SPZ2")
        || RSDK.CheckStageFolder("FBZ") && Zone->actID == 1 || RSDK.CheckStageFolder("PSZ2")) {
        return true;
    }
    if (RSDK.CheckStageFolder("SSZ2")) {
        if (RSDK.GetSceneLayerID("Tower") < LAYER_COUNT)
            return true;
    }
    else if (RSDK.CheckStageFolder("HCZ") && Zone->actID == 1 || RSDK.CheckStageFolder("MSZ") && Zone->actID == 1 || RSDK.CheckStageFolder("OOZ2")
             || RSDK.CheckStageFolder("LRZ3") || RSDK.CheckStageFolder("MMZ") && Zone->actID == 1 || RSDK.CheckStageFolder("TMZ3")
             || RSDK.CheckStageFolder("ERZ")) {
        return true;
    }
    return false;
}

int Zone_Unknown9()
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Mania Mode", textBuffer);
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Encore Mode", textBuffer);
    int eOff = RSDK_sceneInfo->listPos;

    int listPos = RSDK_sceneInfo->listPos;

    int pos2 = 0;
    if (mOff >= 15) {
        if (mOff == 15 || mOff == 16) {
            pos2 = listPos + 15;
        }
        else {
            --listPos;
            pos2 = mOff + listPos;
        }
    }
    else {
        pos2 = mOff + listPos;
    }
    RSDK_sceneInfo->listPos = pos;
    Game_Print("Mania Mode offset %d, pos %d -> Encore Mode offset %d, pos %d", mOff, pos, pos2 - eOff, pos2);
    return pos2;
}

void Zone_Unknown12()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(entity->fade2, entity->fade1, entity->fade1 - 0x80, entity->fade1 - 0x100);
}

void Zone_Unknown13()
{
    // TODO
}

void Zone_Unknown14()
{
    EntityZone *entity          = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->timeEnabled = true;
    if (entity->timer <= 0) {
        options->suppressAutoMusic = false;
        options->suppressTitlecard = false;
        RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fade1;
    }
}

void Zone_Unknown15()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    entity->timer += entity->fade1;
    if (entity->timer > 1024) {
        options->competitionSession[options->competitionSession[24] + 31] = 1;
        options->competitionSession[27]                                   = options->competitionSession[93] + 1;
        RSDK.LoadScene("Presentation", "Menu");
        RSDK.SetSettingsValue(12, 1);
        RSDK.InitSceneLoad();
    }
}

void Zone_Unknown16()
{
    Entity *entity              = RSDK_sceneInfo->entity;
    RSDK_sceneInfo->timeEnabled = true;
    // SaveGame_Unknown9();
    // v1 = Music->field_254;
    // if (Music->ActiveTrack != Music->field_254)
    //    Music_Unknown9(Music->field_254, 0.039999999);
    EntityZone *entityZone     = (EntityZone *)RSDK.SpawnEntity(Zone->objectID, 0, 0, 0);
    entityZone->screenID       = 0;
    entityZone->timer          = 640;
    entityZone->fade1          = 16;
    entityZone->fade2          = 0xF0F0F0;
    entityZone->state          = Zone_Unknown18;
    entityZone->stateDraw      = Zone_Unknown12;
    entityZone->visible        = true;
    options->suppressTitlecard = 0;
    entityZone->drawOrder      = 15;
    // TitleCard->pfuncC      = NULL;
    Player->rings = 0;
    RSDK.DestroyEntity(entity, 0, 0);
}

void Zone_Unknown17()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK.GetObjectByID(SLOT_PLAYER1);
    StarPost->storedMinutes    = RSDK_sceneInfo->minutes;
    StarPost->storedSeconds    = RSDK_sceneInfo->seconds;
    StarPost->storedMS         = RSDK_sceneInfo->milliseconds;
    options->suppressAutoMusic = true;
    options->suppressTitlecard = true;
    // TitleCard->pfuncC             = Zone_Unknown16;
    // SaveGame_Unknown8();
    Player->rings = entity->rings;
    RSDK.InitSceneLoad();
}

void Zone_Unknown18()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0)
        RSDK.DestroyEntity(entity, 0, 0);
    else
        entity->timer -= entity->fade1;
}

void Zone_Unknown19()
{
    // TODO
}

void Zone_Unknown20()
{
    // TODO
}

void Zone_Unknown21()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0) {
        Zone->field_4724 = false;
        RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fade1;
        Zone->field_4724 = true;
    }
}

void Zone_EditorDraw() {}

void Zone_EditorLoad() {}

void Zone_Serialize() {}
