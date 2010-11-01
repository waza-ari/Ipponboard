; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include "defines.def"

;#define MyAppVersion GetFileVersion(AddBackslash(SourcePath) + "MyApplication.exe")
#define MyAppVersion GetEnv("IPPONBOARD_VERSION")
#if len(MyAppVersion) < 1
  #error IPPONBOARD_VERSION not defined
#endif

#define MySimpleAppVersion SimpleVersion(MyAppVersion)

#pragma message "*** Version info ***
#pragma message "Detailed version info: " + MyAppVersion
#pragma message "Simple version info:   " + MySimpleAppVersion

#define MyAppName "Ipponboard"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1E604DB1-15D5-43E9-9FEE-B3A5C8387449}
AppName={#MyAppName}
AppVersion={#MySimpleAppVersion}
AppVerName={#MyAppName} {#MySimpleAppVersion}
AppPublisher=Florian M�cke
AppPublisherURL=http://ipponboard.origo.ethz.ch/
AppSupportURL=http://ipponboard.origo.ethz.ch/
AppUpdatesURL=http://ipponboard.origo.ethz.ch/
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=true
OutputBaseFilename={#MyAppName} {#MySimpleAppVersion} setup
;OutputBaseFilename={#MyAppName} {#MyAppVersion} setup
Compression=lzma/ultra
SolidCompression=true
InternalCompressLevel=ultra
MinVersion=,5.01.2600sp1
ShowLanguageDialog=yes
UninstallDisplayIcon={app}\Ipponboard.exe
;WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardImageFile=images\install_bg.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp

[Languages]
;Name: "en"; MessagesFile: "compiler:Default.isl,compiler:MyMessages.isl"
Name: "en"; MessagesFile: "compiler:Default.isl"; LicenseFile: "License-Default.rtf"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"; LicenseFile: "License-German.rtf"

[CustomMessages]
de.ViewProgram=%1 �ffnen
en.ViewProgram=Open %1
de.Survey=Meinungsumfrage
en.Survey=Feedback/Survey

[Tasks]
Name: "desktopicon"; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: "C:\dev\ipponboard\branch-0.4\_build\build_output\~tmp\*"; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: {app}\Ipponboard.exe; WorkingDir: {app};
Name: "{group}\Anleitung"; Filename: {app}\Anleitung.pdf; WorkingDir: {app};
Name: "{group}\Manual"; Filename: {app}\manual.pdf; WorkingDir: {app}; Languages: en
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: http://ipponboard.origo.ethz.ch/
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: {uninstallexe}
Name: "{group}\Umfrage"; Filename: "http://flo.mueckeimnetz.de/ipponboard/survey_de/"; Languages: de
Name: "{group}\Online-Survey (Feedback)"; Filename: "http://flo.mueckeimnetz.de/ipponboard/survey_en/"; Languages: en
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\Ipponboard.exe"; Tasks: desktopicon; IconFilename: {app}\Ipponboard.exe; WorkingDir: {app}

[Run]
Filename: "{app}\Ipponboard.exe"; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent unchecked; WorkingDir: {app}
Filename: "{app}\Anleitung.pdf"; Description: {cm:ViewProgram,Anleitung}; Flags: shellexec nowait postinstall skipifsilent; WorkingDir: {app}; Languages: de
Filename: "{app}\Manual.pdf"; Description: {cm:ViewProgram,manual}; Flags: shellexec nowait postinstall skipifsilent; WorkingDir: {app}; Languages: en

[Registry]
Root: HKLM; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: deletekey; Languages: en de
Root: HKLM; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "InstalledVersion"; ValueData: "{#MySimpleAppVersion}"; Flags: deletekey; Languages: en de
Root: HKCU; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "Language"; ValueData: "de"; Flags: uninsdeletekeyifempty; Languages: de
Root: HKCU; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "Language"; ValueData: "en"; Flags: uninsdeletekeyifempty; Languages: en

