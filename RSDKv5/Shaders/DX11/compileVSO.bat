@echo off
ECHO "%~1"
ECHO "%~dpn1"

del "%~dpn1".vso

"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x86\fxc.exe" /D RETRO_REV02=1 /T vs_5_0 /Fe log.txt /Zi /E VSMain /Fo "%~dpn1".vso "%~1"