@echo off
ECHO "%~1"
ECHO "%~dpn1"

del "%~dpn1".vso

"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe" /D RETRO_REV02=1 /T vs_3_0 /Fe log.txt /Zi /E VSMain /Fo "%~dpn1".vso "%~1"