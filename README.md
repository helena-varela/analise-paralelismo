# Varredor de Carga: Análise de Paralelismo em C++

Este projeto é uma ferramenta de linha de comando desenvolvida em C++ para medir o desempenho de execução paralela utilizando **Processos (`fork`)** e **Threads (`std::thread`)**. O caso de estudo é o cálculo massivo de uma variante da Conjectura de Collatz sobre bilhões de números, desenhado especificamente para testar gargalos de CPU, limites de escalabilidade (Lei de Amdahl) e estratégias de balanceamento de carga.

Esse trabalho foi desenvolvido no contexto prático da disciplina de Sistemas Operacionais (IMD0036) da UFRN.

## 🚀 Funcionalidades

- **Duas abordagens de concorrência:** 
  - `thread`: Compartilhamento de memória utilizando instâncias no mesmo processo, com escritas disjuntas em vetores globais para evitar exclusão mútua.
  - `processo`: Isolamento de memória onde cada filho cria um arquivo parcial `.txt` no disco, processado posteriormente pelo escalonador pai via `wait()`.
- **Estratégias de particionamento (Balanceamento de Carga):**
  - `bloco`: Divide o intervalo de números em fatias contínuas exatas para cada trabalhador.
  - `ciclico`: Intercala a distribuição dos números, garantindo que o peso computacional (altamente variável) seja distribuído de forma mais igualitária entre os núcleos da CPU.
- **Temporização de Alta Precisão:** Medição em notação científica utilizando `std::chrono::steady_clock`.

## ⚙️ O Problema Matemático

O programa calcula a quantidade de passos $s(n)$ necessários para que um número $n$ chegue a 1, seguindo a regra:
- Se $n$ for par: $n = n / 2$
- Se $n$ for ímpar: $n = 3n + 1$

A carga de trabalho é propositalmente irregular. Variáveis vizinhas podem exigir 30 passos e 250 passos, respectivamente, causando saltos gigantescos que exigem o uso estrito de inteiros de 64 bits (`unsigned long long`) para evitar *overflow*.

## 🛠️ Como Compilar

O projeto inclui um `Makefile` automatizado com as *flags* de otimização necessárias para aferição de desempenho real.

Para compilar, abra o terminal na raiz do projeto e execute:
```bash
make
```
Para limpar os binários gerados:

```bash
make clean
```

## 💻 Como Executar
O programa não interage com o usuário durante a execução. Ele deve receber exatamente 7 parâmetros via linha de comando:

```bash
./varredor <A> <B> <LIMIAR> <W> <modo> <particao> <arquivo_saida>
```

Exemplo de uso testando 8 bilhões de inteiros com 6 threads (Partição Cíclica):

```bash
./varredor 100123456 8100246912 256 6 thread ciclico resultados.csv
```

Parâmetros:
```bash
<A> e <B>: Início e fim do intervalo de números inteiros a serem calculados.

<LIMIAR>: Quantidade mínima de passos para que o número seja contabilizado.

<W>: Número de unidades de execução (ex: 1, 2, 4, 8).

<modo>: Escolha entre thread ou processo.

<particao>: Escolha entre bloco ou ciclico.

<arquivo_saida>: Caminho do arquivo CSV de saída (gerado em modo append).
```
## 📊 Formato de Saída (CSV)
Após a execução, o programa salva uma única linha no CSV especificado contendo os agregados finais formatados:

```bash
modo, particao, W, L, tempo_total, tempo_max_worker, tempo_min_worker, tempo_agregacao, QTD, RECORDE_N, RECORDE_S
```

Os tempos são registrados em segundos usando notação científica com duas casas decimais.