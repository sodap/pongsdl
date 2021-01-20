@echo off
cls
SET root=W:\PONG
SET appname=latest
SET source=%root%\src\*.c
REM (WITHOUT SDL IMAGE EXTENSION) ----> SET include=-IC:\MinGW\include\SDL2 -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 
REM (WITHOUT CONSOLE OUTPUT) -----> SET include=-IC:\MinGW\include\SDL2 -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
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
gcc %source% %include% -o %filename% -Wall
