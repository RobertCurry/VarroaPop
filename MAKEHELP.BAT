@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by VARROAPOP.HPJ. >"hlp\VarroaPop.hm"
echo. >>"hlp\VarroaPop.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\VarroaPop.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\VarroaPop.hm"
echo. >>"hlp\VarroaPop.hm"
echo // Prompts (IDP_*) >>"hlp\VarroaPop.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\VarroaPop.hm"
echo. >>"hlp\VarroaPop.hm"
echo // Resources (IDR_*) >>"hlp\VarroaPop.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\VarroaPop.hm"
echo. >>"hlp\VarroaPop.hm"
echo // Dialogs (IDD_*) >>"hlp\VarroaPop.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\VarroaPop.hm"
echo. >>"hlp\VarroaPop.hm"
echo // Frame Controls (IDW_*) >>"hlp\VarroaPop.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\VarroaPop.hm"
REM -- Make help for Project VARROAPOP


echo Building Win32 Help files
:start /wait hcw /C /E /M "hlp\VarroaPop.hpj"
if errorlevel 1 goto :Error1
if not exist "hlp\VarroaPop.hlp" goto :Error2
if not exist "hlp\VarroaPop.cnt" goto :Error3
echo.
if exist Debug\nul copy "hlp\VarroaPop.hlp" Debug
if exist Debug\nul copy "hlp\VarroaPop.cnt" Debug
if exist Release\nul copy "hlp\VarroaPop.hlp" Release
if exist Release\nul copy "hlp\VarroaPop.cnt" Release
echo.
goto :done

:Error1
echo hlp\VarroaPop.hpj(1) : error: Problem encountered creating help file
goto :done

:Error2
echo hlp\VarroaPop.hlp does not exist
goto :done

:Error3
echo hlp\VarroaPop.cnt does not exist


:done
echo.
