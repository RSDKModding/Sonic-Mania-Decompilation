#include "RetroEngine.hpp"

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

    leaderboards->field_4 = STATUS_OK;
    leaderboards->field_8 = 1;
    leaderboards->field_C = 20;
    for (int e = 0; e < leaderboards->field_C; ++e) {
        LeaderboardEntry *entry = leaderboards->entryPtrs[e];

        entry->status     = STATUS_OK;
        entry->globalRank = leaderboards->field_8 + e;
        entry->score      = (4 * entry->globalRank + 2400) % 59999;
        entry->isUser     = leaderboards->prevIsUser && e == leaderboards->prevRank;
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

void FetchLeaderboard(const char *name, int id)
{
    PrintLog(PRINT_NORMAL, "DUMMY FetchLeaderboard(%s, %d)", name, id);
    leaderboards->entryStart  = 1;
    leaderboards->entryLength = 20;
    leaderboards->rankScore   = 0;
    // leaderboards->leaderboardEntryCount = RSDK.Rand(start, end);
    leaderboards->prevIsUser = id == 0;
    if (id == 0) {
        leaderboards->rankScore = GetRandomValue(1000, 9999);
        leaderboards->prevRank  = GetRandomValue(0, (leaderboards->entryStart + leaderboards->entryLength) - 1);
    }
    leaderboards->status = STATUS_OK;

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
Vector2 LeaderboardEntryCount()
{
    Vector2 value;
    value.x = leaderboards->entryStart;
    value.y = leaderboards->entryLength;
    return value;
}
LeaderboardEntry *ReadLeaderboardEntry(int entryID)
{
    if (entryID < leaderboards->entryStart || entryID >= leaderboards->entryStart + leaderboards->entryLength)
        return NULL;
    else
        return leaderboards->entryPtrs[entryID - leaderboards->entryStart];
}
#endif
