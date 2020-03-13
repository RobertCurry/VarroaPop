# Microsoft Developer Studio Generated NMAKE File, Based on VarroaPop.dsp
!IF "$(CFG)" == ""
CFG=VarroaPop - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VarroaPop - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VarroaPop - Win32 Release" && "$(CFG)" != "VarroaPop - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VarroaPop.mak" CFG="VarroaPop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VarroaPop - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VarroaPop - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VarroaPop - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\Release\VarroaPop.hlp" "$(OUTDIR)\VarroaPop.exe"


CLEAN :
	-@erase "$(INTDIR)\Adult.obj"
	-@erase "$(INTDIR)\Bee.obj"
	-@erase "$(INTDIR)\Brood.obj"
	-@erase "$(INTDIR)\Colony.obj"
	-@erase "$(INTDIR)\ColonyDataPage.obj"
	-@erase "$(INTDIR)\CreateWeatherHdr.obj"
	-@erase "$(INTDIR)\Egg.obj"
	-@erase "$(INTDIR)\ImmigrationDlg.obj"
	-@erase "$(INTDIR)\Larva.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MyPropSheet.obj"
	-@erase "$(INTDIR)\picclip.obj"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\Queen.obj"
	-@erase "$(INTDIR)\SimulationPage.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\VarroaPop.obj"
	-@erase "$(INTDIR)\VarroaPop.pch"
	-@erase "$(INTDIR)\VarroaPop.res"
	-@erase "$(INTDIR)\VarroaPopDoc.obj"
	-@erase "$(INTDIR)\VarroaPopView.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WeatherEvents.obj"
	-@erase "$(INTDIR)\WeatherPage.obj"
	-@erase "$(OUTDIR)\VarroaPop.exe"
	-@erase ".\Release\VarroaPop.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\VarroaPop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VarroaPop.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VarroaPop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\VarroaPop.pdb" /machine:I386 /out:"$(OUTDIR)\VarroaPop.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Adult.obj" \
	"$(INTDIR)\Bee.obj" \
	"$(INTDIR)\Brood.obj" \
	"$(INTDIR)\Colony.obj" \
	"$(INTDIR)\ColonyDataPage.obj" \
	"$(INTDIR)\CreateWeatherHdr.obj" \
	"$(INTDIR)\Egg.obj" \
	"$(INTDIR)\ImmigrationDlg.obj" \
	"$(INTDIR)\Larva.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyPropSheet.obj" \
	"$(INTDIR)\picclip.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\Queen.obj" \
	"$(INTDIR)\SimulationPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VarroaPop.obj" \
	"$(INTDIR)\VarroaPopDoc.obj" \
	"$(INTDIR)\VarroaPopView.obj" \
	"$(INTDIR)\WeatherEvents.obj" \
	"$(INTDIR)\WeatherPage.obj" \
	"$(INTDIR)\VarroaPop.res"

"$(OUTDIR)\VarroaPop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\Debug\VarroaPop.hlp" "$(OUTDIR)\VarroaPop.exe" "$(OUTDIR)\VarroaPop.bsc"


CLEAN :
	-@erase "$(INTDIR)\Adult.obj"
	-@erase "$(INTDIR)\Adult.sbr"
	-@erase "$(INTDIR)\Bee.obj"
	-@erase "$(INTDIR)\Bee.sbr"
	-@erase "$(INTDIR)\Brood.obj"
	-@erase "$(INTDIR)\Brood.sbr"
	-@erase "$(INTDIR)\Colony.obj"
	-@erase "$(INTDIR)\Colony.sbr"
	-@erase "$(INTDIR)\ColonyDataPage.obj"
	-@erase "$(INTDIR)\ColonyDataPage.sbr"
	-@erase "$(INTDIR)\CreateWeatherHdr.obj"
	-@erase "$(INTDIR)\CreateWeatherHdr.sbr"
	-@erase "$(INTDIR)\Egg.obj"
	-@erase "$(INTDIR)\Egg.sbr"
	-@erase "$(INTDIR)\ImmigrationDlg.obj"
	-@erase "$(INTDIR)\ImmigrationDlg.sbr"
	-@erase "$(INTDIR)\Larva.obj"
	-@erase "$(INTDIR)\Larva.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MyPropSheet.obj"
	-@erase "$(INTDIR)\MyPropSheet.sbr"
	-@erase "$(INTDIR)\picclip.obj"
	-@erase "$(INTDIR)\picclip.sbr"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\picture.sbr"
	-@erase "$(INTDIR)\Queen.obj"
	-@erase "$(INTDIR)\Queen.sbr"
	-@erase "$(INTDIR)\SimulationPage.obj"
	-@erase "$(INTDIR)\SimulationPage.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\VarroaPop.obj"
	-@erase "$(INTDIR)\VarroaPop.pch"
	-@erase "$(INTDIR)\VarroaPop.res"
	-@erase "$(INTDIR)\VarroaPop.sbr"
	-@erase "$(INTDIR)\VarroaPopDoc.obj"
	-@erase "$(INTDIR)\VarroaPopDoc.sbr"
	-@erase "$(INTDIR)\VarroaPopView.obj"
	-@erase "$(INTDIR)\VarroaPopView.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WeatherEvents.obj"
	-@erase "$(INTDIR)\WeatherEvents.sbr"
	-@erase "$(INTDIR)\WeatherPage.obj"
	-@erase "$(INTDIR)\WeatherPage.sbr"
	-@erase "$(OUTDIR)\VarroaPop.bsc"
	-@erase "$(OUTDIR)\VarroaPop.exe"
	-@erase "$(OUTDIR)\VarroaPop.ilk"
	-@erase "$(OUTDIR)\VarroaPop.pdb"
	-@erase ".\Debug\VarroaPop.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\VarroaPop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VarroaPop.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VarroaPop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Adult.sbr" \
	"$(INTDIR)\Bee.sbr" \
	"$(INTDIR)\Brood.sbr" \
	"$(INTDIR)\Colony.sbr" \
	"$(INTDIR)\ColonyDataPage.sbr" \
	"$(INTDIR)\CreateWeatherHdr.sbr" \
	"$(INTDIR)\Egg.sbr" \
	"$(INTDIR)\ImmigrationDlg.sbr" \
	"$(INTDIR)\Larva.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MyPropSheet.sbr" \
	"$(INTDIR)\picclip.sbr" \
	"$(INTDIR)\picture.sbr" \
	"$(INTDIR)\Queen.sbr" \
	"$(INTDIR)\SimulationPage.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VarroaPop.sbr" \
	"$(INTDIR)\VarroaPopDoc.sbr" \
	"$(INTDIR)\VarroaPopView.sbr" \
	"$(INTDIR)\WeatherEvents.sbr" \
	"$(INTDIR)\WeatherPage.sbr"

"$(OUTDIR)\VarroaPop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\VarroaPop.pdb" /debug /machine:I386 /out:"$(OUTDIR)\VarroaPop.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Adult.obj" \
	"$(INTDIR)\Bee.obj" \
	"$(INTDIR)\Brood.obj" \
	"$(INTDIR)\Colony.obj" \
	"$(INTDIR)\ColonyDataPage.obj" \
	"$(INTDIR)\CreateWeatherHdr.obj" \
	"$(INTDIR)\Egg.obj" \
	"$(INTDIR)\ImmigrationDlg.obj" \
	"$(INTDIR)\Larva.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyPropSheet.obj" \
	"$(INTDIR)\picclip.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\Queen.obj" \
	"$(INTDIR)\SimulationPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VarroaPop.obj" \
	"$(INTDIR)\VarroaPopDoc.obj" \
	"$(INTDIR)\VarroaPopView.obj" \
	"$(INTDIR)\WeatherEvents.obj" \
	"$(INTDIR)\WeatherPage.obj" \
	"$(INTDIR)\VarroaPop.res"

"$(OUTDIR)\VarroaPop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VarroaPop.dep")
!INCLUDE "VarroaPop.dep"
!ELSE 
!MESSAGE Warning: cannot find "VarroaPop.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VarroaPop - Win32 Release" || "$(CFG)" == "VarroaPop - Win32 Debug"
SOURCE=.\Adult.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Adult.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Adult.obj"	"$(INTDIR)\Adult.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Bee.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Bee.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Bee.obj"	"$(INTDIR)\Bee.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Brood.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Brood.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Brood.obj"	"$(INTDIR)\Brood.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Colony.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Colony.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Colony.obj"	"$(INTDIR)\Colony.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\ColonyDataPage.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\ColonyDataPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\ColonyDataPage.obj"	"$(INTDIR)\ColonyDataPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\CreateWeatherHdr.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\CreateWeatherHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\CreateWeatherHdr.obj"	"$(INTDIR)\CreateWeatherHdr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Egg.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Egg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Egg.obj"	"$(INTDIR)\Egg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\ImmigrationDlg.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\ImmigrationDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\ImmigrationDlg.obj"	"$(INTDIR)\ImmigrationDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Larva.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Larva.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Larva.obj"	"$(INTDIR)\Larva.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\MyPropSheet.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\MyPropSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\MyPropSheet.obj"	"$(INTDIR)\MyPropSheet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\picclip.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\picclip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\picclip.obj"	"$(INTDIR)\picclip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\picture.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\picture.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\picture.obj"	"$(INTDIR)\picture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\Queen.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\Queen.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\Queen.obj"	"$(INTDIR)\Queen.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\SimulationPage.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\SimulationPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\SimulationPage.obj"	"$(INTDIR)\SimulationPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\VarroaPop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VarroaPop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\VarroaPop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\VarroaPop.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VarroaPop.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\VarroaPop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\VarroaPop.obj"	"$(INTDIR)\VarroaPop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\hlp\VarroaPop.hpj

!IF  "$(CFG)" == "VarroaPop - Win32 Release"

OutDir=.\Release
ProjDir=.
TargetName=VarroaPop
InputPath=.\hlp\VarroaPop.hpj
USERDEP__VARRO="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	

"$(INTDIR)\VarroaPop.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VARRO)
	<<tempfile.bat 
	@echo off 
	call "$(ProjDir)\makehelp.bat"
<< 
	

!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"

OutDir=.\Debug
ProjDir=.
TargetName=VarroaPop
InputPath=.\hlp\VarroaPop.hpj
USERDEP__VARRO="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	

"$(INTDIR)\VarroaPop.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VARRO)
	<<tempfile.bat 
	@echo off 
	call "$(ProjDir)\makehelp.bat"
<< 
	

!ENDIF 

SOURCE=.\VarroaPop.rc

"$(INTDIR)\VarroaPop.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\VarroaPopDoc.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\VarroaPopDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\VarroaPopDoc.obj"	"$(INTDIR)\VarroaPopDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\VarroaPopView.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\VarroaPopView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\VarroaPopView.obj"	"$(INTDIR)\VarroaPopView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\WeatherEvents.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\WeatherEvents.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\WeatherEvents.obj"	"$(INTDIR)\WeatherEvents.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 

SOURCE=.\WeatherPage.cpp

!IF  "$(CFG)" == "VarroaPop - Win32 Release"


"$(INTDIR)\WeatherPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ELSEIF  "$(CFG)" == "VarroaPop - Win32 Debug"


"$(INTDIR)\WeatherPage.obj"	"$(INTDIR)\WeatherPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VarroaPop.pch"


!ENDIF 


!ENDIF 

