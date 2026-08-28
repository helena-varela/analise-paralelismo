#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 8) {
		cout << "Erro. Uso correto: ./varredor <A> <B> <LIMIAR> <W> <modo> <particao> <arquivo_saida>" << endl;
		return 1;
	}

	unsigned long long A = stoull(argv[1]);
	unsigned long long B = stoull(argv[2]);
	unsigned long long LIMIAR = stoull(argv[3]);
	int W = stoi(argv[4]);
	string modo = argv[5];
	string particao = argv[6];
	string arquivo_saida = argv[7];
	
	unsigned long long L = B - A; 

	vector<ResultadoParcial> resultados_finais(W);

	auto inicio_total = chrono::steady_clock::now();

	if (modo == "thread") {
		resultados_threads.resize(W);
		vector<thread> pool_threads;

		for (int i = 0; i < W; i++) {
			pool_threads.push_back(thread(rotina_thread, i, W, A, B, LIMIAR, particao));
		}

		for (int i = 0; i < W; i++) {
			pool_threads[i].join();
		}
			
		resultados_finais = resultados_threads;

	} 
	else if (modo == "processo") {
		for (int i = 0; i < W; i++) {
			pid_t pid = fork();
			if (pid == 0) {
				ResultadoParcial res = executar_trabalho(i, W, A, B, LIMIAR, particao);
						
				string nome_arquivo = "parcial_" + to_string(i) + ".txt";
				ofstream arquivo_parcial(nome_arquivo);
				arquivo_parcial << res.qtd_acima_limiar << " " << res.recorde_n << " " << res.recorde_s << " " << res.tempo_worker << endl;
				arquivo_parcial.close();
				exit(0);
			}
		}

		for (int i = 0; i < W; i++) {
			wait(NULL);
		}

		for (int i = 0; i < W; i++) {
			string nome_arquivo = "parcial_" + to_string(i) + ".txt";
			ifstream arquivo_parcial(nome_arquivo);
			arquivo_parcial >> resultados_finais[i].qtd_acima_limiar >> resultados_finais[i].recorde_n >> resultados_finais[i].recorde_s >> resultados_finais[i].tempo_worker;
			arquivo_parcial.close();
		}
	}

	auto inicio_agregacao = chrono::steady_clock::now();

	unsigned long long QTD_total = 0;
	unsigned long long RECORDE_N_geral = 0;
	unsigned long long RECORDE_S_geral = 0;
	double tempo_max_worker = -1.0;
	double tempo_min_worker = 9999999.0;

	for (int i = 0; i < W; i++) {
		QTD_total += resultados_finais[i].qtd_acima_limiar;

		if (resultados_finais[i].recorde_s > RECORDE_S_geral) {
			RECORDE_S_geral = resultados_finais[i].recorde_s;
			RECORDE_N_geral = resultados_finais[i].recorde_n;
		} else if (resultados_finais[i].recorde_s == RECORDE_S_geral && resultados_finais[i].recorde_n < RECORDE_N_geral) {
			RECORDE_N_geral = resultados_finais[i].recorde_n;
		}

		if (resultados_finais[i].tempo_worker > tempo_max_worker) tempo_max_worker = resultados_finais[i].tempo_worker;
		if (resultados_finais[i].tempo_worker < tempo_min_worker) tempo_min_worker = resultados_finais[i].tempo_worker;
	}

	auto fim_total = chrono::steady_clock::now();
	chrono::duration<double> agregacao_decorrido = fim_total - inicio_agregacao;
	chrono::duration<double> total_decorrido = fim_total - inicio_total;

	ofstream csv(arquivo_saida, ios::app);
	
	csv << modo << "," << particao << "," << W << "," << L << ",";
	
	csv << scientific << setprecision(2); 
	csv << total_decorrido.count() << "," << tempo_max_worker << "," << tempo_min_worker << "," << agregacao_decorrido.count() << ",";
	
	csv << fixed << QTD_total << "," << RECORDE_N_geral << "," << RECORDE_S_geral << endl;

	csv.close();

	return 0;
}