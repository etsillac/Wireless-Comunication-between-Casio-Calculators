@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.

if exist debug\*.obj  del debug\*.obj
if exist WMCP.G1A  del WMCP.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Users\calli\fx-9860G SDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Users\calli\fx-9860G SDK\Tools\MakeAddinHeader363.exe" "C:\Users\calli\Documents\Github\Wireless-Comunication-between-Casio-Calculators\Casio Application"
if not exist WMCP.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

