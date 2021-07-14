#include "../SonicMania.h"

ObjectCutsceneHBH *CutsceneHBH;

void CutsceneHBH_Update(void)
{
    RSDK_THIS(CutsceneHBH);
    if (entity->oscillate)
        entity->position.y += RSDK.Sin256(4 * (entity->field_68 + entity->dword84)) << 7;
    if (entity->timerFlag)
        entity->field_68 = (entity->field_68 + 1) & 0x7FFF;
    else
        entity->field_68 = Zone->timer;

    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->data);
    RSDK.ProcessAnimation(&entity->altData);
}

void CutsceneHBH_LateUpdate(void) {}

void CutsceneHBH_StaticUpdate(void) {}

void CutsceneHBH_Draw(void)
{
    RSDK_THIS(CutsceneHBH);
    if (entity->paletteFlag)
        CutsceneHBH_SetupPalettes();
    RSDK.DrawSprite(&entity->altData, NULL, false);
    RSDK.DrawSprite(&entity->data, NULL, false);
    if (entity->paletteFlag)
        CutsceneHBH_RestorePalette();
}

void CutsceneHBH_Create(void *data)
{
    RSDK_THIS(CutsceneHBH);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->visible       = !entity->hiddenAtStart;
    entity->spriteIndex   = 0;
    entity->dword70       = entity->position.x;
    entity->dword74       = entity->position.y;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->dword84       = RSDK.Rand(0, 256);
    CutsceneHBH_LoadSprites();
    CutsceneHBH_SetupColours();
    if (RSDK.CheckStageFolder("MSZ"))
        entity->timerFlag = true;
    else
        entity->paletteFlag = true;
}

void CutsceneHBH_StageLoad(void) { CutsceneHBH_StorePalette(); }

void CutsceneHBH_SetupColours(void)
{
    RSDK_THIS(CutsceneHBH);
    switch (entity->characterID) {
        case HBH_GUNNER: {
            entity->colourSet = 0;
            colour data[0x80] = { 0x200020, 0x400040, 0x500050, 0x700070, 0x880088, 0xB000B0, 0xC800C8, 0xF000F0, 0x300040, 0x480070, 0x5000A0,
                                  0x4828F0, 0x3850F0, 0x5898F0, 0x98C8F0, 0xD8E8F0, 0x080078, 0x1000C0, 0x0028F0, 0x0040F0, 0x00A8F0, 0x40F0F0,
                                  0xC8F0F0, 0xF0F0F0, 0x280068, 0x280098, 0x3010D0, 0x2830F0, 0x1880F0, 0x48B8F0, 0x98E0F0, 0xD0F0F0, 0xA80810,
                                  0xE04830, 0xF08840, 0xF0B850, 0xF0D860, 0xF0E870, 0xF0F090, 0x300060, 0x480068, 0x580070, 0x700078, 0x880880,
                                  0xB00880, 0xD81870, 0xF030F0, 0xF048F0, 0x082888, 0x1848D0, 0x2070E0, 0x3890F0, 0x50B0F0, 0x68C8F0, 0x90D8F0,
                                  0xC8E8F0, 0x200060, 0x280068, 0x300080, 0x4008A0, 0x080010, 0x100038, 0x100068, 0x080878, 0x003820, 0x005840,
                                  0x008040, 0x00A058, 0x08C080, 0x30F0B8, 0x081828, 0x182850, 0x204078, 0x3868A8, 0x0080B8, 0x10A0E8, 0xE8F040,
                                  0xE8F098, 0x78E8F0, 0x78F0F0, 0x100038, 0x181038, 0x181048, 0x182868, 0x204090, 0x4878B0, 0x58A8D0, 0x78D0E0,
                                  0x200038, 0x400038, 0x881038, 0xC03010, 0xC86020, 0xC08038, 0xE00058, 0xE89850, 0x100060, 0x280070, 0x400088,
                                  0x5800C8, 0x8810C8, 0xC830C8, 0x98F068, 0xA8F070, 0x100848, 0x101048, 0x102050, 0x102858, 0x180838, 0x201038,
                                  0x282038, 0x302838, 0x0070E0, 0x00A0C0, 0x80D810, 0x28C010, 0xF04040, 0xF05050, 0x48A0E8, 0x60B0F0, 0x380028,
                                  0x580030, 0x780830, 0x981830, 0xB83030, 0x000000, 0x000000, 0xFFFFFF };
            memcpy(entity->colours, data, sizeof(entity->colours));
            break;
        }
        case HBH_SHINOBI: {
            entity->colourSet = 1;
            colour data[0x80] = { 0x000000, 0x083048, 0x005848, 0x107858, 0x309060, 0x50A868, 0x68B868, 0x80D068, 0xA8E088, 0xC8E8A0, 0x481878,
                                  0x6020B0, 0x9830C0, 0xC050E8, 0xF080F0, 0xF0A8F0, 0x506870, 0x607080, 0x887868, 0xA09078, 0xB8B090, 0xD0D0A8,
                                  0xE0E0C0, 0xF0F0D8, 0x602030, 0x781818, 0x983810, 0xB05010, 0xC07818, 0xD89840, 0xF0C078, 0xF0E0B8, 0x282060,
                                  0x500880, 0x880090, 0xB80888, 0xD80848, 0xE83850, 0xE86880, 0x000000, 0x284058, 0x385078, 0x5068A0, 0x6878B0,
                                  0x8890C0, 0xA0A8D8, 0xB8C0E0, 0x000000, 0x204888, 0x4070C0, 0x5898D0, 0x78B8E0, 0xA0D8F0, 0xB8F0F0, 0xD8F0F0,
                                  0xF0F0F0, 0x081840, 0x103058, 0x305870, 0x487080, 0x589088, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFFFF };
            memcpy(entity->colours, data, sizeof(entity->colours));
            break;
        }
        case HBH_MYSTIC:
        case HBH_ROUGESA:
        case HBH_ROUGESB:
        case HBH_ROUGESC: {
            entity->colourSet = 2;
            colour data[0x80] = { 0x200000, 0x300010, 0x400030, 0x600038, 0x800030, 0x901038, 0xA81038, 0xB82020, 0xB83828, 0xC04020, 0xC85020,
                                  0xD06020, 0xD88020, 0xE0A028, 0xE0C030, 0xE8D038, 0x004000, 0x086000, 0x087800, 0x209000, 0x38A800, 0x60C000,
                                  0x80E008, 0xE0E0A0, 0x780080, 0xA00880, 0xC018A0, 0xE040C0, 0xE068D0, 0xE0A0E0, 0x000000, 0x000000, 0x201030,
                                  0x182050, 0x203880, 0x2850A0, 0x2878B8, 0x30B8F0, 0x60E8F0, 0x88F0F0, 0xB0F0F0, 0x402000, 0x603000, 0x804000,
                                  0xA05000, 0xC06000, 0xD07810, 0x3060A0, 0x1090D8, 0x3098E8, 0x58A0E8, 0x78A0F0, 0x98A8F0, 0xB8B0F0, 0xD0C0F0,
                                  0xF0D8F0, 0xF0F0F0, 0x801860, 0x982070, 0xA83070, 0xB83868, 0xD05068, 0x000000, 0x000000, 0xD86858, 0xE88858,
                                  0xE8B060, 0xF0D860, 0xF0E878, 0xF0F0A8, 0x505828, 0x587028, 0x788030, 0x909030, 0xA89038, 0xC0A040, 0x180080,
                                  0x5000A8, 0x6000D8, 0x8820F0, 0x000000, 0x000080, 0x0038C0, 0x0068F0, 0x1888F0, 0x30A0F0, 0x68D0F0, 0xF0F000,
                                  0xF0B000, 0x182050, 0x2828A8, 0x4038D0, 0x5848E8, 0x8868F0, 0xB8B0F0, 0x80E008, 0xFF00FF, 0xFF00FF, 0xFF00FF,
                                  0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,
                                  0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,
                                  0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFFFFFF };
            memcpy(entity->colours, data, sizeof(entity->colours));
            break;
        }
        case HBH_RIDER:
        case HBH_KING:
        case HBH_KINGTMZ2: {
            entity->colourSet = 3;
            colour data[0x80] = { 0x000000, 0x202020, 0x202040, 0x302070, 0x4020A0, 0x4030B0, 0x4040C0, 0x6060D1, 0x8080E0, 0x90A0E0, 0xA0C0E0,
                                  0xB0D0E0, 0xC0E0E0, 0x783810, 0xA06040, 0xC07020, 0x203060, 0x204080, 0x2050A0, 0x2060C0, 0x3080C0, 0x40A0C0,
                                  0x50C0D0, 0x60E0E0, 0x205060, 0x206040, 0x308060, 0x40A080, 0x60C0A0, 0x80E0C0, 0x404000, 0xE0E000, 0x302060,
                                  0x402080, 0x4030A0, 0x4040C0, 0x7030D0, 0xA020E0, 0xC060E0, 0xE0A0E0, 0xD0C0E0, 0x202030, 0x302040, 0x302050,
                                  0x402060, 0x303070, 0x000000, 0x000000, 0x501010, 0x800000, 0xB00000, 0xE00000, 0xE04000, 0xE08000, 0xE09000,
                                  0xE0A000, 0xE0C070, 0xE0E0E0, 0xA00000, 0xE00000, 0xE06000, 0xE0A000, 0xE0E000, 0x000000, 0x606080, 0x8080A0,
                                  0xA0A0C0, 0xB0B0D0, 0xC0C0E0, 0x202020, 0x301818, 0x381810, 0x482010, 0x482810, 0x584008, 0x806010, 0x907828,
                                  0xB89848, 0xD8C070, 0xE0E0C0, 0x202020, 0x383040, 0x484868, 0x587090, 0x80A0B0, 0x98C0C8, 0xB0D8D8, 0xC8E8E0,
                                  0x402040, 0x582040, 0x802060, 0xA020A0, 0xE14FB1, 0xE02080, 0xE080C0, 0xE0A0E0, 0xE00000, 0xE06000, 0xE0A000,
                                  0xE0E000, 0x202040, 0x302060, 0x402080, 0x503070, 0x604080, 0x8050A0, 0xA060C0, 0xB080D0, 0xC0A0E0, 0xD0C0E0,
                                  0xE0E0E0, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFFFF };
            memcpy(entity->colours, data, sizeof(entity->colours));
            break;
        }
        case HBH_KINGDAMAGED:
        case HBH_PILE: {
            entity->colourSet  = 4;
            colour data[0x80] = { 0x883801, 0xB86801, 0xE09060, 0xF0B090, 0xA06040, 0xE08008, 0xE0A008, 0xE0C070, 0x085848, 0x107858, 0x309060,
                                  0x50A868, 0x68B868, 0x80D068, 0xA8E088, 0xC8E8A0, 0x400101, 0x900101, 0xE00101, 0xE04830, 0xF08840, 0xF0D860,
                                  0xF0E870, 0xF0F090, 0x302060, 0x402080, 0x4030A0, 0x7030D0, 0xA020E0, 0xC060E0, 0xE0A0E0, 0x013048, 0xA09078,
                                  0xB8B090, 0xD0D0A8, 0xF0F0D8, 0x2850A0, 0x2878B8, 0x30B8F0, 0x60E8F0, 0x603001, 0x804001, 0xA05001, 0x584008,
                                  0x806010, 0x907828, 0xB89848, 0x082888, 0x1848D0, 0x2070E0, 0x3890F0, 0x50B0F0, 0x68C8F0, 0xC8E8F0, 0xC06001,
                                  0xD07810, 0xE0A028, 0xE8B048, 0xE0C030, 0xE8D038, 0xD8C070, 0xE0E0C0, 0x301801, 0x482001, 0x704008, 0xB07008,
                                  0xD09818, 0xE0C020, 0xF0E060, 0x380118, 0x580120, 0x800110, 0xA80101, 0xC02001, 0xE85820, 0x000000, 0x000000,
                                  0x000000, 0x000000, 0x000000, 0x400168, 0x700178, 0x900188, 0xB80188, 0xF00198, 0xF060B0, 0xF0A0C8, 0xB00801,
                                  0xD82001, 0xF03001, 0xF06818, 0xF0D050, 0xF0E888, 0xF0F0E8, 0xC8D0F0, 0xD0F0F0, 0x181001, 0x301808, 0x502010,
                                  0x803018, 0xB05030, 0xD08850, 0xE8B858, 0xF0D898, 0x480808, 0x701001, 0xA82008, 0xD03808, 0xE86818, 0xF0A020,
                                  0xF0D068, 0xF0E0B8, 0x100828, 0x180840, 0x201050, 0x281068, 0x381880, 0x682098, 0x803098, 0xB038A8, 0xC858B8,
                                  0x500120, 0x800120, 0xB80110, 0xE01001, 0xB8A0E0, 0xC0B8E8, 0xFFFFFF };
            memcpy(entity->colours, data, sizeof(entity->colours));
            break;
        }
        default: break;
    }
}

void CutsceneHBH_SetupPalettes(void)
{
    RSDK_THIS(CutsceneHBH);
    for (int c = 0; c < 0x80; ++c) {
        RSDK.SetPaletteEntry(0, c + 0x80, entity->colours[c]);
    }
}
void CutsceneHBH_StorePalette(void)
{
    for (int c = 0; c < 0x80; ++c) {
        CutsceneHBH->paletteColours[c] = RSDK.GetPaletteEntry(0, c + 0x80);
    }
}
void CutsceneHBH_RestorePalette(void)
{
    for (int c = 0; c < 0x80; ++c) {
        RSDK.SetPaletteEntry(0, c + 0x80, CutsceneHBH->paletteColours[c]);
    }
}

void CutsceneHBH_LoadSprites(void)
{
    RSDK_THIS(CutsceneHBH);
    switch (entity->characterID) {
        case HBH_GUNNER:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("SPZ1/Boss.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 5, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_SHINOBI:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("PSZ2/Shinobi.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 5, &entity->altData, true, 0);
            break;
        case HBH_MYSTIC:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_RIDER:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("LRZ3/HeavyRider.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 3, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_KING:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 7, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 16, &entity->altData, true, 0);
            break;
        case HBH_ROUGESA:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_ROUGESB:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 5, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_ROUGESC:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 12, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        case HBH_KINGDAMAGED:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("Cutscene/DamagedKing.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->altData, true, 0);
        case HBH_PILE:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("Cutscene/HBHPile.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
        case HBH_KINGTMZ2:
            if (!entity->spriteIndex)
                entity->spriteIndex = RSDK.LoadSpriteAnimation("Cutscene/KingTMZ2.bin", SCOPE_STAGE);
            RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
            break;
        default: break;
    }
}

EntityCutsceneHBH *CutsceneHBH_GetEntity(char charID)
{
    EntityCutsceneHBH *result = NULL;
    foreach_all(CutsceneHBH, entity)
    {
        if (entity->characterID == charID)
            result = entity;
    }
    return result;
}

void CutsceneHBH_Unknown7(void)
{
    EntityCutsceneHBH *entity = CutsceneHBH_GetEntity(HBH_SHINOBI);
    if (entity) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 3, &entity->data, true, 0);
        entity->velocity.x = 0x10000;
        entity->velocity.y = -0x60000;
        entity->direction ^= FLIP_X;
        entity->drawOrder    = Zone->drawOrderHigh + 2;
        entity->state        = CutsceneHBH_Unknown14;
        EntityFXTrail *trail = (EntityFXTrail *)RSDK.CreateEntity(FXTrail->objectID, entity, entity->position.x, entity->position.y);
        trail->dataPtr       = &entity->data;
    }
}

void CutsceneHBH_Unknown8(void)
{
    EntityCutsceneHBH *entity = CutsceneHBH_GetEntity(HBH_SHINOBI);
    if (entity) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->data, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
        entity->state = CutsceneHBH_Unknown13;
    }
}

void CutsceneHBH_Unknown9(void)
{
    EntityCutsceneHBH *entity = CutsceneHBH_GetEntity(HBH_RIDER);
    if (entity) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
        entity->timer      = 0;
        entity->originPos.x   = entity->position.x;
        entity->originPos.y   = entity->position.y;
        entity->velocity.x = 0x30000;
        entity->state      = CutsceneHBH_Unknown16;
    }
}

void CutsceneHBH_Unknown10(void)
{
    EntityCutsceneHBH *entity = CutsceneHBH_GetEntity(HBH_KING);
    if (entity) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
        RSDK.SetSpriteAnimation(entity->spriteIndex, 9, &entity->altData, true, 0);
    }
}

void CutsceneHBH_Unknown11(void)
{
    EntityCutsceneHBH *entity = CutsceneHBH_GetEntity(HBH_KINGTMZ2);
    if (entity) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->data, true, 0);
        entity->timer      = 0;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = CutsceneHBH_Unknown20;
    }
}

void CutsceneHBH_Unknown12(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->velocity.y -= 0x2000;
    entity->position.y += entity->velocity.y;

    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
}

void CutsceneHBH_Unknown13(void)
{
    RSDK_THIS(CutsceneHBH);
    if (entity->timer <= 0) {
        entity->velocity.y += 0x2000;
        entity->position.y += entity->velocity.y;
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x2E0000, true)) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, 1, &entity->data, true, 0);
            entity->velocity.y = -0x20000;
            entity->timer      = 3;
        }
    }
    else {
        entity->timer--;
    }

    foreach_active(PhantomRuby, phantomRuby)
    {
        if (phantomRuby->velocity.y >= 0 && phantomRuby->position.y > entity->position.y - 0x200000) {
            phantomRuby->velocity.y = -(5 * abs(entity->velocity.y) >> 1);
        }
    }
}

void CutsceneHBH_Unknown14(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->velocity.y += 0x4000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
}

void CutsceneHBH_Unknown15(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->velocity.x += 0x4000;
    entity->position.x += entity->velocity.x;
    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
}

void CutsceneHBH_Unknown16(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->position.x += entity->velocity.x;
    if (!entity->timer) {
        if (entity->velocity.x < 0x30000)
            entity->velocity.x += 0x4000;

        if (entity->direction == FLIP_NONE) {
            if (entity->position.x - entity->originPos.x > 0x300000) {
                entity->timer = 1;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 2, &entity->data, true, 0);
            }
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x300000, 8);
        }
        else {
            if (entity->data.frameID == entity->data.frameCount - 1) {
                entity->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            }
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x300000, 8);
        }
    }
    else {
        if (entity->velocity.x > -0x30000)
            entity->velocity.x -= 0x4000;

        if (entity->direction != FLIP_X) {
            if (entity->data.frameID == entity->data.frameCount - 1) {
                entity->direction = FLIP_X;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
            }
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x300000, 8);
        }
        else {
            if (entity->position.x - entity->originPos.x < -0x300000) {
                entity->timer = 0;
                RSDK.SetSpriteAnimation(entity->spriteIndex, 2, &entity->data, true, 0);
            }
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x300000, 8);
        }
    }
}

void CutsceneHBH_Unknown17(void)
{
    RSDK_THIS(CutsceneHBH);

    entity->position.x += entity->velocity.x;
    if (entity->velocity.x < 0x60000)
        entity->velocity.x += 0x4000;
    if (entity->direction == FLIP_X && entity->data.frameID == entity->data.frameCount - 1) {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
    }
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x300000, 8);
    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
}

void CutsceneHBH_Unknown18(void)
{
    RSDK_THIS(CutsceneHBH);
    if (++entity->timer == 16) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 3, &entity->data, true, 0);
    }
    else if (entity->timer > 16 && entity->data.frameID > 4) {
        entity->velocity.x = 0x28000;
        entity->velocity.y = -0x60000;
        entity->state      = CutsceneHBH_Unknown19;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->altData, true, 0);
    }
}

void CutsceneHBH_Unknown19(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, false);
}

void CutsceneHBH_Unknown20(void)
{
    RSDK_THIS(CutsceneHBH);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, 0x2C0000, true)) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 2, &entity->data, true, 0);
        entity->velocity.y = 0;
        entity->state      = CutsceneHBH_Unknown21;
        Camera_ShakeScreen(0, 0, 4);
    }
}

void CutsceneHBH_Unknown21(void)
{
    RSDK_THIS(CutsceneHBH);

    if (entity->data.frameID != entity->data.frameCount - 1) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->data, true, 0);
        entity->state = StateMachine_None;
    }
}

void CutsceneHBH_EditorDraw(void) {}

void CutsceneHBH_EditorLoad(void) {}

void CutsceneHBH_Serialize(void)
{
    RSDK_EDITABLE_VAR(CutsceneHBH, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(CutsceneHBH, VAR_UINT8, characterID);
    RSDK_EDITABLE_VAR(CutsceneHBH, VAR_BOOL, oscillate);
    RSDK_EDITABLE_VAR(CutsceneHBH, VAR_BOOL, hiddenAtStart);
}
