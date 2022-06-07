#if RETRO_REV02

// release switch has the stack and other stuff predefined for the loop thread to use
// issue is it MUST be page-aligned (& 0xFFF of location must be 0), which we don't know how to do
// so bc we're using libNX we can pass null for it to do it for us
// uint8 messageLoopThreadStack[16384];

// Thread messageLoopThread;

// normally this func looks a little different
// but libNX sucks and won't properly thread things for us
// 4 am note: this literally doesn't work anyway fuck libNX we do a hack at the bottom
void MessageLoopThreadCB(SKU::NXCore *core)
{
    uint32 msg     = 0;
    uint32 unknown = 0;

    while (R_SUCCEEDED(appletGetMessage(&msg))) {
        appletProcessMessage(msg);
        PrintLog(PRINT_NORMAL, "yo whatup %d", msg);

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

            default: PrintLog(PRINT_NORMAL, "Unhandled message = 0x%08x\n", msg); break;
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

    core->focusState = appletGetFocusState();
    if (engine.inFocus)
        engine.inFocus = (core->focusState == AppletFocusState_InFocus) ? 1 : 2;
    appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);
    // nn::oe::SetResumeNotificationEnabled(true);
    // nn::oe::SetOperationModeChangedNotificationEnabled(true);

    // int32 rc = threadCreate(&messageLoopThread, MessageLoopThreadCB, userCore, NULL, 0x4000, 28, -2);
    // PrintLog(PRINT_NORMAL, "%d %d", R_MODULE(rc), R_DESCRIPTION(rc));

    // threadStart(&messageLoopThread);

    return core;
}

void NXCore::FrameInit()
{
    MessageLoopThreadCB(this);

    // hack to ensure focus state
    this->focusState = appletGetFocusState();
    if (engine.inFocus)
        engine.inFocus = (this->focusState == AppletFocusState_InFocus) ? 1 : 2;

    UserCore::FrameInit();
}
#endif
