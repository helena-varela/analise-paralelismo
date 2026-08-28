#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>

struct ResultadoParcial {
	unsigned long long qtd_acima_limiar = 0;
	unsigned long long recorde_n = 0;
	unsigned long long recorde_s = 0;
	double tempo_worker = 0.0;
};

extern std::vector<ResultadoParcial> resultados_threads;

unsigned long long calcular_passos(unsigned long long n);

ResultadoParcial executar_trabalho(int id_worker, int W, unsigned long long A, unsigned long long B, unsigned long long LIMIAR, std::string particao);

void rotina_thread(int id_worker, int W, unsigned long long A, unsigned long long B, unsigned long long LIMIAR, std::string particao);

#endif