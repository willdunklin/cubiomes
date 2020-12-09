@REM Set this to be your c file but leave extension off
set file=get_seeds_here

del *.o
del *.exe
gcc -c -Wall -fwrapv -D_WIN32 util.c
gcc -c -Wall -fwrapv -D_WIN32 layers.c
gcc -c -Wall -fwrapv -D_WIN32 generator.c
gcc -c -Wall -fwrapv -D_WIN32 finders.c
gcc -c -Wall -fwrapv -D_WIN32 %file%.c
gcc -o %file%.exe .\%file%.o .\layers.o .\generator.o .\finders.o -lm
%file%.exe