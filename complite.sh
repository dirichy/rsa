clang -c gint.c rsabw.c
clang -Wl,-stack_size -Wl,0x1000000 -o rsabw gint.o rsabw.o
rm gint.o rsabw.o
