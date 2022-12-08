gcc -o build/terminal resources/resources.res Main.c src/terminal.c -Iinclude -Wall -static -static-libgcc 
C:\"Program Files (x86)"\"Microsoft SDKs"\ClickOnce\SignTool\signtool sign /fd SHA256 /p "JamDev_Licence1" /f "build/legal.pfx" build/terminal.exe
start .\build\terminal.exe