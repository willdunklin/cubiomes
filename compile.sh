make clean
make libcubiomes
gcc -pthread get_seeds_here.c -L. -lcubiomes -lm
./a.out