@echo off
ECHO "%~1"
ECHO "%~dpn1"

del "%~dpn1".fso

"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\fxc.exe" /D RETRO_REV02=1 /T ps_3_0 /Fe log.txt /Zi /E PSMain /Fo "%~dpn1".fso "%~1"