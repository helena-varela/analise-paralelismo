echo "Compilando o projeto..."
make clean
make

rm -f resultados.csv

echo "Iniciando bateria da Entrega Final (80 testes)"

for W in 1 2 4 8; do
  for modo in thread processo; do
    for particao in bloco ciclico; do
      for rep in 1 2 3 4 5; do
        echo "Rodando Entrega Final: W=$W, $modo, $particao (Repetição $rep/5)..."
        ./varredor 100033713 8100067426 213 $W $modo $particao resultados.csv
      done
    done
  done
done

echo "Entrega Final concluida, 80 resultados estao no resultados.csv"