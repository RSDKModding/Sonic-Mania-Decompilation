@echo off
ECHO "%~1"
ECHO "%~dpn1"

del "%~dpn1".fso

"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x86\fxc.exe" /D RETRO_REV02=1 /T ps_5_0 /Fe log.txt /Zi /E PSMain /Fo "%~dpn1".fso "%~1"