#!/bin/bash
echo "Compilando o projeto..."
make clean
make

rm -f resultados.csv

echo "Iniciando bateria do Checkpoint (20 testes com W=1)..."

W=1
for modo in thread processo; do
  for particao in bloco ciclico; do
    for rep in 1 2 3 4 5; do
      echo "Rodando Checkpoint: W=$W, $modo, $particao (Repetição $rep/5)..."
      ./varredor 100033713 8100067426 213 $W $modo $particao resultados.csv
    done
  done
done

echo "Checkpoint concluido"