#ifndef USERDATA_H
#define USERDATA_H

extern bool settingsChanged;

int GetSettingsValue(int id);
void SetSettingsValue(int id, int val);

void readSettings();
void writeSettings(bool writeToFile);

#endif
