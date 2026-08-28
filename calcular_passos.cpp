#include <iostream>

unsigned long long calcular_passos(unsigned long long n){
  unsigned long long passos = 0;
  unsigned long long atual = n;

  while(atual > 1){
    if(atual % 2 == 0){
      atual = atual/2;
    } else {
      atual = (3 * atual) + 1;
    }
    passos++;
  }
  return passos;
}