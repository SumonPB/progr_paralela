#include<iostream>
#include<fmt/core.h>
using  std::printf;

int main(){
    int valor  = 10;
    printf("HolaMundo\n");
    fmt::println("Hola, mundo desde fmtl:{}", valor);

    return 0;
}
