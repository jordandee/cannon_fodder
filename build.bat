REM build for Win7 x86 VS2013 SDL2 2.0.3 SDL2_image 2.0.0 SDL2_ttf 2.0.12

cl main.cpp gameengine.cpp title.cpp level.cpp terrain.cpp math.cpp resources.cpp globals.cpp timer.cpp cannon.cpp ball.cpp score.cpp obstacle.cpp options.cpp buttons.cpp /EHsc /I C:\vs_sdl\include /link C:\vs_sdl\lib\x86\SDL2.lib  C:\vs_sdl\lib\x86\SDL2main.lib C:\vs_sdl\lib\x86\SDL2_image.lib  C:\vs_sdl\lib\x86\SDL2_ttf.lib /SUBSYSTEM:WINDOWS /NODEFAULTLIB:msvcrtd.lib /NODEFAULTLIB:libcmtd.lib
