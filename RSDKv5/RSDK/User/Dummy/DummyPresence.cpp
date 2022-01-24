#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02
void RSDK::SKU::DummyRichPresence::SetPresence(byte id, TextInfo *info)
{
    char buffer[0xFF];
    GetCString(buffer, info);
    if (info->text[info->length - 1] == '\r')
        buffer[info->length - 1] = 0;

    richPresence->curID = id;

    std::string str = __FILE__;
    str += ": SetPresence() # Set Steam rich presence string to ";
    str += buffer;
    str += "\r\n";
    PrintLog(PRINT_NORMAL, str.c_str());
}
#endif
