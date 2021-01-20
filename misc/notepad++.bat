@echo off
SET root="W:\PONG"
PUSHD "C:\Program Files (x86)\Notepad++\"
START notepad++ -openFoldersAsWorkspace "%root%"
POPD