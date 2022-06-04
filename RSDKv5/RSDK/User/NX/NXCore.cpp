#if RETRO_REV02

uint8 messageLoopThreadStack[16384];
Thread messageLoopThread;

void MessageLoopThreadCB(SKU::NXCore *core)
{
    uint32 msg     = 0;
    uint32 unknown = 0;

    while (true) {
        appletGetMessage(&msg);

        switch (msg) {
            case AppletMessage_ExitRequest: unknown = 1; break;

            case AppletMessage_FocusStateChanged:
                core->focusState = appletGetFocusState();
                if (engine.inFocus)
                    engine.inFocus = (core->focusState == AppletFocusState_InFocus) ? 1 : 2;
                break;

            case AppletMessage_Resume:
                // sub_71002813BC();
                break;

            case AppletMessage_OperationModeChanged:
                // if (appletGetOperationMode == AppletOperationMode_Console)
                //     sub_71002813BC();
                break;

            case AppletMessage_PerformanceModeChanged: break;

            default: PrintLog(PRINT_NORMAL, "Unhandled message = 0x%08x\n", v2); break;
        }
    }
}

SKU::NXCore *InitNXCore()
{
    // Initalize API subsystems
    NXCore *core = new NXCore;

    if (achievements)
        delete achievements;
    achievements = new NXAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new NXLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new NXRichPresence;

    if (stats)
        delete stats;
    stats = new NXStats;

    if (userStorage)
        delete userStorage;
    userStorage = new NXUserStorage;

    // Setup default values

    memset(core->values, 0, sizeof(core->values));
    core->values[0]  = false;
    core->valueCount = 1;

    core->focusState = = appletGetFocusState();
    if (engine.inFocus) 
        engine.inFocus = (core->focusState == AppletFocusState_InFocus) ? 1 : 2;
    appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);
    // nn::oe::SetResumeNotificationEnabled(true);
    // nn::oe::SetOperationModeChangedNotificationEnabled(true);

    // if (nn::os::CreateThread(&messageLoopThread, MessageLoopThreadCB, userCore, messageLoopThreadStack, sizeof(messageLoopThreadStack), 0))
    //     nn::diag::detail::OnAssertionFailure(2, &blankString, &blankString, &blankString, 0);
    // nn::os::StartThread(&messageLoopThread);

    // TODO: remove
    leaderboards->userRank = 0;
    leaderboards->isUser   = false;

    achievements->enabled      = true;
    leaderboards->status       = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_STATUS", 0));
    stats->enabled             = true;
    userStorage->authStatus    = STATUS_NONE;
    userStorage->storageStatus = STATUS_NONE;
    userStorage->saveStatus    = STATUS_NONE;
    userStorage->noSaveActive  = false;

    return core;
}
#endif
