#if RETRO_REV02
void DummyRichPresence::SetPresence(int32 id, String *message)
{
    char buffer[0xFF];
    GetCString(buffer, message);
    if (message->chars[message->length - 1] == '\r')
        buffer[message->length - 1] = 0;

    curID = id;

    std::string str = __FILE__;
    str += ": SetPresence() # Set Steam rich presence string to ";
    str += buffer;
    str += "\r\n";
    PrintLog(PRINT_NORMAL, str.c_str());
}
#endif
