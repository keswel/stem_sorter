$env:PATH = "C:\msys64\ucrt64\bin;$env:PATH"
gcc main.c -o main.exe -lregex
if ($?) { ./main.exe }
