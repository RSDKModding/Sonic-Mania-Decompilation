#include "RSDK/Core/RetroEngine.hpp"
#include <string>

// Start custom leaderboard code
// this is added because we don't have access to any store APIs that would otherwise use this feature
std::vector<RSDK::SKU::LeaderboardInfo> RSDK::SKU::leaderboardList;
// End custom leaderboard code

#if RETRO_REV02
void RSDK::SKU::FillDummyLeaderboardEntries()
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

    leaderboards->entryInfo.loadStatus = STATUS_OK;
    for (int e = 0; e < leaderboards->entryInfo.entryCount; ++e) {
        LeaderboardEntry *entry = &leaderboards->entryInfo.entries[e];

        entry->status     = STATUS_OK;
        entry->globalRank = leaderboards->entryInfo.globalRankOffset + e;
        entry->score      = (4 * entry->globalRank + 2400) % 59999;
        entry->isUser     = leaderboards->isUser && entry->globalRank == leaderboards->userRank;
        if (entry->isUser) {
            GetUsername(&entry->username);
        }
        else {
            SetText(&entry->username, (char *)"", 0);
            PrependText(&entry->username, (char *)dummyNames[e % 100]);
        }
        SetText(&entry->userID, (char *)"DUMMY_USER_ID", 0);
    }
}
void RSDK::SKU::DummyLeaderboards::FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser)
{
    if (!leaderboard)
        return;

    if (leaderboards->status == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": FetchLeaderboard() # ERROR Attemped to fetch leaderboard when fetch currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
    else {
        PrintLog(PRINT_NORMAL, "DUMMY FetchLeaderboard(%s, %s)", leaderboard->name, isUser ? "true" : "false");
        leaderboards->isUser   = isUser;
        leaderboards->userRank = 0;
        if (isUser)
            leaderboards->userRank = rand() % 9999 + 1;

        leaderboards->status   = STATUS_CONTINUE;

        API_TypeOf(leaderboards, DummyLeaderboards)->loadTime = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_LOAD_TIME", 0));
    }
}
void RSDK::SKU::DummyLeaderboards::TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank))
{
    if (!leaderboard)
        return;

    int id = -1;
    for (int i = 0; i < leaderboardList.size(); ++i) {
        if (std::string(leaderboardList[i].name) == leaderboard->name) {
            id = i;
            break;
        }
    }

    if (id == -1) {
        LeaderboardInfo info;
        sprintf(info.name, "%s", leaderboard->name);
        info.score = 0x7FFFFFFF;
        id         = leaderboardList.size();
        leaderboardList.push_back(info);
    }

    leaderboardList[id].score = score;
    saveUserData();

    std::string str = __FILE__;
    str += ": TrackScore() # TrackScore ";
    str += std::to_string(score);
    str += " \r\n";
    PrintLog(PRINT_NORMAL, str.c_str());

    if (callback) {
        API_TypeOf(leaderboards, DummyLeaderboards)->trackRank = rand() % 61 + 30;
        API_TypeOf(leaderboards, DummyLeaderboards)->trackCB   = callback;
    }
}
#endif
