@echo off
cls
SET root= < Enter root here >
SET appname=latest
SET source=%root%\src\*.c
SET include=-IC:\MinGW\include\SDL2 -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

IF NOT "%1" == "" ( 
	
	SET source = %root%\src\%1%
	
	IF NOT "%2" == "" ( 
		SET appname=%2%
	)
)
SET filename=%root%\bin\%appname%\%appname%.exe
@echo on
rd %root%\bin\%appname%\ /q /s
md %root%\bin\%appname%
xcopy %root%\res %root%\bin\%appname%\res /I /q
gcc %source% %include% -o %filename% -mwindows