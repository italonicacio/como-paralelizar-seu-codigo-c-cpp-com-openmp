# Executando o projeto

## Requisitos
- Linux
- Compilador C++ (Nesse projeto usei o [g++](https://gcc.gnu.org))
- OpenMP

```
Obs.: Talvez a distro Linux que você vai utilizar já venha com o g++ e o OpenMP.
```

## Variáveis Ambientes

Só precisamos de apenas uma variável ambiente que pode ser inicializada com o comando abaixo.

```bash
export OMP_NUM_THREADS=4 
```


## Compilando o projeto
```bash
g++ main.cpp -fopenmp -o main 
```


## Executando o projeto

```bash
./main <numero-maximo-iteracoes-teste-soma> <maximo-iteracoes-loop1-teste-loop-duplo> <maximo-iteracoes-loop1-teste-loop-duplo>
```