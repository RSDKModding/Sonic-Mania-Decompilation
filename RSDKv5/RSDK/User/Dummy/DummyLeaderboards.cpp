#include <string>

#if RETRO_REV02
void DummyLeaderboards::FillLeaderboardEntries(RSDK::SKU::LeaderboardLoadInfo *info)
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

    UserLeaderboards *leaderboards = info->parent->parent;
    info->status                   = STATUS_OK;

    for (int32 e = 0; e < info->avail.length; ++e) {
        LeaderboardEntry *entry = &info->entries[e];

        if (leaderboards->isUser) {
            entry->status     = STATUS_OK;
            entry->globalRank = info->avail.start + e;
            entry->score      = (4 * entry->globalRank + 2400) % 59999;
            entry->isUser     = entry->globalRank == leaderboards->userRank;

            memset(&entry->username, 0, sizeof(entry->username));
            if (entry->isUser) {
                GetUsername(&entry->username);
            }
            else {
                InitString(&entry->username, (char *)"", 0);
                SetString(&entry->username, (char *)dummyNames[e % 100]);
            }

            memset(&entry->userID, 0, sizeof(entry->userID));
            InitString(&entry->userID, (char *)"DUMMY_USER_ID", 0);
        }
        else {
            entry->status     = STATUS_OK;
            entry->globalRank = info->avail.start + e;
            entry->score      = (4 * entry->globalRank + 2400) % 59999;
            entry->isUser     = false;

            memset(&entry->username, 0, sizeof(entry->username));
            InitString(&entry->username, (char *)"", 0);
            SetString(&entry->username, (char *)dummyNames[e % 100]);

            memset(&entry->userID, 0, sizeof(entry->userID));
            InitString(&entry->userID, (char *)"DUMMY_USER_ID", 0);
        }
    }
}

void DummyLeaderboards::FinishLeaderboardFetch(DummyLeaderboardCallback *callback)
{
    this->status = SKU::GetAPIValue(SKU::GetAPIValueID("SYSTEM_LEADERBOARD_STATUS", 0));

    if (this->status == STATUS_OK) {
        int32 startIndex = 1;
        if (callback->isUser) {
            startIndex = this->userRank - 10;
            if (this->userRank == 10)
                startIndex = 1;
        }

        LeaderboardLoadInfo *info = this->loadList.InitLoadInfo();
        info->avail.start         = startIndex;
        info->avail.length        = 20;

        FillLeaderboardEntries(info);

        this->loadList.Clear();
        this->loadList.count = 1;
        this->loadList.prev  = info;
        this->loadList.next  = info;
        this->loadList.last  = info;
        if (info->avail.start == 1)
            info->disableLoadPrev = true;

        this->entryInfo.Setup();
        leaderboards->entryInfo.loadSize.start  = this->entryInfo.viewSize.start;
        leaderboards->entryInfo.loadSize.length = this->entryInfo.viewSize.length;
    }
}

void DummyLeaderboards::FinishLeaderboardLoad(DummyLeaderboardCallback *callback)
{
    FillLeaderboardEntries(callback->info);
    entryInfo.Setup();
}

void DummyLeaderboards::FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser)
{
    if (!leaderboard)
        return;

    if (status == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": FetchLeaderboard() # ERROR Attemped to fetch leaderboard when fetch currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
    else {
        PrintLog(PRINT_NORMAL, "DUMMY FetchLeaderboard(%s, %s)", leaderboard->idPC, isUser ? "true" : "false");

        this->currentLeaderboard = leaderboard;
        this->loadList.Clear();
        this->entryInfo.Setup();

        this->isUser   = isUser;
        this->userRank = 0;
        if (isUser)
            userRank = rand() % 9999 + 1;

        status = STATUS_CONTINUE;

        DummyLeaderboardCallback *cb = callbackList.Append();
        cb->type                     = 0;
        cb->isUser                   = isUser;
        cb->loadTime                 = SKU::GetAPIValue(SKU::GetAPIValueID("SYSTEM_LEADERBOARD_LOAD_TIME", 0));
    }
}

void DummyLeaderboards::LoadLeaderboards(RSDK::SKU::LeaderboardLoadInfo *info)
{
    for (int32 i = 0; i < info->avail.length; ++i) {
        info->entries[i].status     = STATUS_CONTINUE;
        info->entries[i].globalRank = info->avail.start + i;
    }

    DummyLeaderboardCallback *cb = callbackList.Append();
    cb->type                     = 1;
    cb->loadTime                 = SKU::GetAPIValue(SKU::GetAPIValueID("SYSTEM_LEADERBOARD_LOAD_TIME", 0));
    cb->info                     = info;
}

void DummyLeaderboards::TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank))
{
    if (!leaderboard)
        return;

    std::string str = __FILE__;
    str += ": TrackScore() # TrackScore ";
    str += std::to_string(score);
    str += " \r\n";
    PrintLog(PRINT_NORMAL, str.c_str());

    DummyLeaderboardCallback *cb = callbackList.Append();
    cb->type                     = 2;
    cb->loadTime                 = SKU::GetAPIValue(SKU::GetAPIValueID("SYSTEM_LEADERBOARD_LOAD_TIME", 0));
    cb->trackRank                = rand() % 61 + 30;
    cb->trackScore               = rand() % 9999 + 1;
    cb->trackCB                  = callback;
}
#endif
