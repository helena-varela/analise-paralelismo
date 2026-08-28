#include "header.h"
#include <chrono>

std::vector<ResultadoParcial> resultados_threads;

ResultadoParcial executar_trabalho(int id_worker, int W, unsigned long long A, unsigned long long B, unsigned long long LIMIAR, std::string particao) {
  auto inicio_worker = std::chrono::steady_clock::now();
  ResultadoParcial res;
  
  unsigned long long total_elementos = (B - A) + 1; 

  if (particao == "bloco") {
    unsigned long long tamanho_bloco = total_elementos / W;
    unsigned long long inicio = A + (id_worker * tamanho_bloco);
    unsigned long long fim;
    
    if (id_worker == W - 1) {
      fim = B;
    } else {
      fim = inicio + tamanho_bloco - 1;
    }

    for (unsigned long long n = inicio; n <= fim; n++) {
      unsigned long long passos = calcular_passos(n);
        
      if (passos > LIMIAR) res.qtd_acima_limiar++;
        
      if (passos > res.recorde_s) {
        res.recorde_s = passos;
        res.recorde_n = n;
      } else if (passos == res.recorde_s && n < res.recorde_n) {
        res.recorde_n = n;
      }
    }
  } 
  else if (particao == "ciclico") {
    unsigned long long inicio = A + id_worker;
    for (unsigned long long n = inicio; n <= B; n += W) {
      unsigned long long passos = calcular_passos(n);
        
      if (passos > LIMIAR) res.qtd_acima_limiar++;
        
      if (passos > res.recorde_s) {
        res.recorde_s = passos;
        res.recorde_n = n;
      } else if (passos == res.recorde_s && n < res.recorde_n) {
        res.recorde_n = n;
      }
    }
  }

  auto fim_worker = std::chrono::steady_clock::now();
  std::chrono::duration<double> tempo = fim_worker - inicio_worker;
  res.tempo_worker = tempo.count();

  return res;
}

void rotina_thread(int id_worker, int W, unsigned long long A, unsigned long long B, unsigned long long LIMIAR, std::string particao) {
  resultados_threads[id_worker] = executar_trabalho(id_worker, W, A, B, LIMIAR, particao);
}