#include "RetroEngine.hpp"

#if RETRO_REV02
UserRichPresence *richPresence = NULL;

void DummyRichPresence::SetPresence(byte id, TextInfo *info)
{
    char buffer[0xFF];
    GetCString(buffer, info);
    if (info->text[info->length - 1] == '\r')
        buffer[info->length - 1] = 0;

#if RETRO_REV02
    richPresence->curID = id;

    std::string str = __FILE__;
    str += ": SetPresence() # Set Steam rich presence string to ";
    str += buffer;
    str += "\r\n";
    PrintLog(PRINT_NORMAL, str.c_str());

    // sprintf(buffer2, "DUMMY SetPresence(%d, %s) -> %s", id, buffer, (richPresence->curID != id ? "Successful Set" : "Redundant Set"));
    // PrintLog(PRINT_NORMAL, buffer2);
#else
    sprintf(buffer2, "DUMMY SetPresence(%d, %s)", id, buffer);
    PrintLog(PRINT_NORMAL, buffer2);
#endif
}
#endif
