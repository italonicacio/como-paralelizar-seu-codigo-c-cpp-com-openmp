# [CONTEÚDO] Como paralelizar seu código C/C++ com OpenMP

## O que é o OpenMP?

[OpenMP](https://www.openmp.org) (Open Multi-Processing) é uma ferramenta de paralelização de código Fortran, C e C++. 

## Paralelizando seu código C++

Para paralelizar seu código C++ é muito simples, basta uma linha e conseguimos extrair o máximo que seu computador pode ter. Abaixo temos um exemplo disso, temos uma função que soma repetidamente o valor 1 a uma variável de redução.

```C++
std::size_t Soma(std::size_t numero_maximo_de_iteracoes) {

    std::size_t reducao = 0;
    for(std::size_t iter = 0; iter < numero_maximo_de_iteracoes; ++iter) {
        reducao += 1;
    }

    return reducao;
}
```
E para paralelizar o código? O que precisamos fazer? Bom, essa é a parte fácil, apenas com uma linha, como mostra o código abaixo, conseguimos fazer o loop *for* ser executado em paralelo. 

```C++
#pragma omp parallel for reduction(+:reducao)
```

Adicionando essa diretiva de compilação antes do loop *for* da função *Soma*, dizermos ao compilador que queremos paralelizar esse loop.

```C++
std::size_t SomaParalela(std::size_t numero_maximo_de_iteracoes) {

    std::size_t reducao = 0;
    #pragma omp parallel for reduction(+:reducao)
    for(std::size_t iter = 0; iter < numero_maximo_de_iteracoes; ++iter) {
        reducao += 1;
    }

    return reducao;
}
```

Quando adicionamos o **pragma omp parallel** é criado uma nova região paralela, o OpenMP distribui a carga automaticamente entre as threads, nos testes a seguir coloquei para distribuir entre 4 threads a partir de uma variável ambiente chamada *OMP_NUM_THREADS*, você também consegue fazer essa manipulação da quantidade de variáveis dentro do programa, porem ela não pode ser maior que o valor de *OMP_NUM_THREADS*, por fim pedi para reduzir o valor dessa soma das threads na variável reducao com essa última parte da linha (***reduction(+:reducao)***).


Abaixo temos uma ilustração simplista de como está sendo feito o balanceamento de carga para cada thread e a redução.

![ilustracao-openmp](https://drive.google.com/uc?export=view&id=1JoGj3qPmsaWJ2lsxSjNgCwYWDCiSOVh5)

Após criar uma região paralela com a diretiva **pragma omp parallel** é possível fazer uma melhor manipulação dos dados dentro dela, mas não irei abordar aqui pois é um tópico mais avançado.

## Resultados e Tempo de execução das Somas

> Obs.: Os resultados são variados de computador para computador, dependendo do número de threads o resultado pode ser bem diferente. 

Para **1000000000** de iterações a versão paralela conseguiu executar essa soma em **485.8 milissegundos** em contrapartida a versão normal da soma executou em **1715.6 milissegundos** . A versão paralela foi **3.53** vezes mais rápido que a versão não paralelizada.

Porem note que a quantidade maxima de iterações está bastante alta, como será o resultado se baixarmos para apenas **1000** iterações? A soma não paralelizada foi finalizada em **0.001994 milissegundos** e a versão paralelizada terminou em **0.168432 milissegundos**, tendo como campeã para poucas iterações a versão não paralelizada da soma que foi **84** vezes mais rápido que a versão paralelizada. 


## E quando usar o OpenMP?


Para responder essa pergunta, trouxe outra função que faz um loop duplo e com *1000* iterações para cada loop já notamos uma diferença. Os códigos abaixo fazem uma subtração da soma do loop mais interno com o valor de redução total que está fora dos loops sendo assim o retorno da função tem que ser 0.

```C++
std::size_t LoopDuplo(std::size_t  maximo_de_iteracoes_loop1, std::size_t maximo_iteracoes_loop2) {

    std::size_t reducao_total = maximo_de_iteracoes_loop1*maximo_iteracoes_loop2;
    for(std::size_t iter1 = 0; iter1 < maximo_de_iteracoes_loop1; ++iter1) {
        std::size_t reducao = 0;
        for(std::size_t iter2 = 0; iter2 < maximo_iteracoes_loop2; ++iter2) {
            reducao += 1;
        }
        reducao_total -= reducao;
    }

    return reducao_total;
}
```

```C++
std::size_t LoopDuploParalelo(std::size_t  maximo_de_iteracoes_loop1, std::size_t maximo_iteracoes_loop2) {
    std::size_t reducao_total = maximo_de_iteracoes_loop1*maximo_iteracoes_loop2;

    #pragma omp parallel for reduction(-:reducao_total)
    for(std::size_t iter1 = 0; iter1 < maximo_de_iteracoes_loop1; ++iter1) {
        std::size_t reducao = 0;
        for(std::size_t iter2 = 0; iter2 < maximo_iteracoes_loop2; ++iter2) {
            reducao += 1;
        }
        reducao_total -= reducao;
    }

    return reducao_total;
}
```

Com *1000* iterações para cada um dos loops, conseguimos um ganho de 5 vezes para a função paralelizada. 

### E Onde mais podemos utilizar?

Podemos utilizar OpenMP para paralelizar o redimensionamento de fotos em lote, por exemplo, uma pasta contendo milhares de fotos e queremos reduzir seu tamanho em 25%, podemos utilizar paralelismo para essa tarefa, onde cada thread ficaria responsável por uma foto.

Também é muito utilizado em simulações químicas para encontrar novos medicamentos.

E como ultimo exemplo, podemos falar sobre o treinamento de IAs, temos o exemplo bastante recente ai do ChatGPT. Porem nesse caso é mais aconselhável a utilização de GPUs.

> Obs.: Para coisas simples talvez não seja vantajoso paralelizar seu código. E a partir do OpenMP 4.0 é possível também fazer a paralelização do seu código direto para a GPU.

## Boas Praticas Utilizando OpenMP

Sempre setar o número de threads igual ao número de cores do computador, por exemplo:

Se seu computador é um 6c/12t (ou seja, 6 cores físicos e 12 threads), coloque a variável ambiente *OMP_NUM_THREADS* igual a 6.

Se atentar a condições de corrida no código, o OpenMP fornece diretivas e também o uso de variáveis privadas (O OpenMP criara uma copia dessa variável para cada thread)

## Resultados Tabelados

| Função | Número de Iterações | Tempo(ms) | Número de Iterações | Tempo(ms) |
| --- | --- | --- | --- | --- |
| Soma | 1000 | 0.001994 |  1000000000 | 1715.6 |
| Soma Paralela | 1000 | 0.168432 | 1000000000 | 485.8 |



| Função | Número de Iterações dos Loops | Tempo(ms) | Número de Iterações dos Loops | Tempo(ms) |
| --- | --- | --- | --- | --- |
| Loop Duplo | 1000 | 1.51 |  10000 | 122.18 |
| Loop Duplo Paralelo | 1000 | 0.31 | 10000 | 32.22 |

**Repositório com o código dos testes:** <https://github.com/italonicacio/como-paralelizar-seu-codigo-c-cpp-com-openmp/tree/main/project>