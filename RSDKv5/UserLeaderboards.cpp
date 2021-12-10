#include "RetroEngine.hpp"
#include <string>

// Start custom leaderboard code
// this is added because we don't have access to any store APIs that would otherwise use this feature
std::vector<LeaderboardInfo> leaderboardList;
// End custom leaderboard code

#if RETRO_REV02
DummyLeaderboards *leaderboards = NULL;

void FillDummyLeaderboardEntries()
{
    const char *dummyNames[] = { "ORCIHILLARY124",      "AUCTORJOLIE521",       "SENECTUSFLORENCE789", "MAGNAAVRAM503",       "SITVERNON320",
                                 "DUICHRISTEN429",      "NULLAKERMIT649",       "INTEGERGEORGE708",    "HENDRERITDREW443",    "UTULYSSES507",
                                 "ACCUMSANBRUCE276",    "BANANAARON804",        "MAURISSILAS372",      "ETCALVIN641",         "UTGALENA780",
                                 "FEUGIATSHAY665",      "ORCIJULIET388",        "ETJENNA318",          "LIBEROIDOLA420",      "ATVIELKA576",
                                 "PHASELLUSJENETTE627", "CONSECTETUERCALEB550", "SITVICTOR359",        "PURUSDARRYL50",       "DONECAVRAM56",
                                 "PENATIBUSSYBILL30",   "MAURISBARCLAY563",     "CONGUECOLLEEN811",    "MORBIDESTINY655",     "SEMPERIONA579",
                                 "RISUSKEANE807",       "QUISQUEMARYAM927",     "COMMODOSTEPHANIE54",  "MALESUADABURTON350",  "VIVAMUSROBIN330",
                                 "QUISQUEWHILEMINA413", "VESTIBULUMRYDER890",   "LACUSINDIRA112",      "NEQUEJESCIE446",      "EGESTASMORGAN390",
                                 "ETODYSSEUS95",        "EGETHERMIONE127",      "ENIMPASCALE599",      "ODIOWHOOPI328",       "DOLORANGELICA718",
                                 "ODIOSLOANE426",       "DUIZELDA319",          "METUSPORTER906",      "NONUMMYSHANNON166",   "IPSUMANTHONY906",
                                 "ACCUMSANRAPHAEL296",  "ORNAREHU604",          "MOLESTIERYDER304",    "FACILISILAITH900",    "MIYVONNE287",
                                 "CRASHEDY110",         "TEMPUSRAE486",         "ORCICOLBY173",        "ULTRICESJADEN480",    "LECTUSBEAU552",
                                 "LIGULAJESSAMINE824",  "ORCISOLOMON19",        "VARIUSDARRYL437",     "EUISMODKATELL668",    "LACUSSTEVEN302",
                                 "ALIQUETRICHARD807",   "PEDEKAI692",           "VIVAMUSCHARLOTTE167", "MAURISLEWIS101",      "NULLANELLE11",
                                 "MAECENASKAMEKO602",   "LOREMHALEY545",        "PHASELLUSPAUL954",    "ERATGLORIA527",       "DIAMBURKE290",
                                 "SEMYOKO792",          "ANTEMICHELLE769",      "SEMPERDOMINIC873",    "CONDIMENTUMNYSSA826", "PEDEDYLAN675",
                                 "ETCRUZ380",           "VELJAQUELYN95",        "NONDACEY190",         "UTCAIN386",           "NULLANOMLANGA941",
                                 "QUISQUESIERRA659",    "MATTISABDUL346",       "PHASELLUSKNOX183",    "NEQUEXYLA92",         "ORNAREGAY372",
                                 "FACILISISIVY303",     "TELLUSHARLAN339",      "METUSSOPOLINE75",     "NUNCKYLYNN927",       "PROINHERMIONE775",
                                 "TEMPORFITZGERALD656", "VELVELMA504",          "FAUCIBUSTAMEKAH272",  "PORTTITORWHOOPI881",  "EUPETER41" };

    leaderboards->unknown.loadStatus = STATUS_OK;
    for (int e = 0; e < leaderboards->unknown.entryCount; ++e) {
        LeaderboardEntry *entry = &leaderboards->unknown.entries[e];

        entry->status     = STATUS_OK;
        entry->globalRank = leaderboards->unknown.field_8 + e;
        entry->score      = (4 * entry->globalRank + 2400) % 59999;
        entry->isUser     = leaderboards->isUser && e == leaderboards->userRank;
        if (entry->isUser) {
            GetUserName(&entry->username);
        }
        else {
            SetText(&entry->username, (char *)"", 0);
            PrependText(&entry->username, (char *)dummyNames[e % 100]);
        }
        SetText(&entry->userID, (char *)"DUMMY_USER_ID", 0);
    }
}

void FetchLeaderboard(const char *name, bool32 isUser)
{
    PrintLog(PRINT_NORMAL, "DUMMY FetchLeaderboard(%s, %s)", name, isUser ? "true" : "false");
    leaderboards->isUser   = isUser;
    leaderboards->userRank = 0;
    if (isUser)
        leaderboards->userRank = rand() % 9999 + 1;
    leaderboards->status = STATUS_OK;

    leaderboards->unknown.entryStart  = 1;
    leaderboards->unknown.entryLength = 20;
    leaderboards->unknown.field_8     = 1;
    leaderboards->unknown.entryCount  = 20;

    FillDummyLeaderboardEntries();
}
void TrackScore(const char *name, int score, void (*callback)(int status, int rank))
{
    int id = -1;
    for (int i = 0; i < leaderboardList.size(); ++i) {
        if (std::string(leaderboardList[i].name) == name) {
            id = i;
            break;
        }
    }

    if (id == -1) {
        LeaderboardInfo info;
        sprintf(info.name, "%s", name);
        info.score = 0x7FFFFFFF;
        id         = leaderboardList.size();
        leaderboardList.push_back(info);
    }

    leaderboardList[id].score = score;
    saveUserData();

    if (callback)
        callback(true, 1);
}
Vector2 LeaderboardEntryLength()
{
    Vector2 value;
    value.x = leaderboards->unknown.entryCount;
    value.y = leaderboards->unknown.field_10;
    return value;
}

Vector2 LeaderboardEntryCount()
{
    Vector2 value;
    value.x = leaderboards->unknown.entryStart;
    value.y = leaderboards->unknown.entryLength;
    return value;
}

void LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type)
{
    switch (type) {
        default:
            leaderboards->unknown2.loadType       = type;
            leaderboards->unknown2.loadStartIndex = start;
            leaderboards->unknown2.loadEndIndex   = end;
            break;
        case 1:
            leaderboards->unknown2.loadType = type;
            if (!start)
                start = 1;
            leaderboards->unknown2.loadStartIndex = start;
            if (end > 200)
                end = 200;
            leaderboards->unknown2.loadEndIndex = end;
            break;
        case 2: {
            int32 startPos = start;
            int32 endPos   = start + end - 1;
            leaderboards->unknown2.loadType = 2;
            if (start < (endPos - 199))
                startPos = endPos - 199;
            leaderboards->unknown2.loadStartIndex = startPos;
            leaderboards->unknown2.loadEndIndex   = start + end - startPos;
            break;
        }
    }
}

void ClearLeaderboardInfo()
{
    leaderboards->status             = STATUS_NONE;
    leaderboards->currentLeaderboard = NULL;
    // ClearLeaderboardsUnknown2(&leaderboards->unknown2);
    // SetupLeaderboardEntries(&leaderboards->unknown);
}

LeaderboardEntry *ReadLeaderboardEntry(int entryID)
{
    if (entryID < leaderboards->unknown.entryStart || entryID >= leaderboards->unknown.entryStart + leaderboards->unknown.entryLength)
        return NULL;
    else
        return &leaderboards->unknown.entries[entryID - leaderboards->unknown.entryStart];
}
#endif
