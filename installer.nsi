; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

!include "LogicLib.nsh"
!include "nsProcess.nsh"

;--------------------------------

; The name of the installer
Name "PuTTY Launchy Plugin"

; The file to write
OutFile "PuTTY-Plugin-v2.X.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Launchy

;--------------------------------

;wait for Launchy to exit
Function WaitForLaunchy
  ${Do}
    ${nsProcess::FindProcess} "Launchy.exe" $R0
    ${If} $R0 = 0
      MessageBox MB_ABORTRETRYIGNORE "Launchy is currently running.  Please quit Launchy first to install this plugin." IDABORT canceled IDIGNORE ignored
    ${EndIf}
  ${LoopUntil} $R0 != 0
ignored:
  ${nsProcess::Unload}
  Return
canceled:
  Quit
FunctionEnd

; Pages

PageEx license
  LicenseText "README"
  LicenseData README.txt
PageExEnd
Page directory WaitForLaunchy
;Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\Plugins
  
  ; Put file there
  File release\PuTTY.dll

  SetOutPath $INSTDIR\Plugins\icons

  File PuTTY\putty.png
  
SectionEnd ; end the section
