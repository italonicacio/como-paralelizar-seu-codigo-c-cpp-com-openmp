#include <iostream>
#include <chrono>

std::size_t Soma(std::size_t numero_maximo_de_iteracoes) {

    std::size_t reducao = 0;
    for(std::size_t iter = 0; iter < numero_maximo_de_iteracoes; ++iter) {
        reducao += 1;
    }

    return reducao;
}

std::size_t SomaParalela(std::size_t  numero_maximo_de_iteracoes) {

    std::size_t reducao = 0;
    #pragma omp parallel for reduction(+:reducao)
    for(std::size_t iter = 0; iter < numero_maximo_de_iteracoes; ++iter) {
        reducao += 1;
    }

    return reducao;
}

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

int main(int argc, char** argv) {
    if(argc < 4) {
        std::cerr << "Necessário passar um como parâmetro para o programa um número máximo de iterações e a quantidade de iterações do segundo teste para cada loop" << "\n";
        return 1;
    }



    std::size_t numero_maximo_iteracoes = std::stoi(argv[1]);
    std::size_t maximo_iteracoes_loop1 = std::stoi(argv[2]);
    std::size_t maximo_iteracoes_loop2 = std::stoi(argv[2]);



    std::cout << "Fazendo testes!!!" << "\n";
    std::cout << "Número máximo de iterações da soma (teste 1): " << numero_maximo_iteracoes << "\n";
    std::cout << "Número de iterações 1 (teste 2): " << maximo_iteracoes_loop1 << "\n";
    std::cout << "Número de iterações 2 (teste 2): " << maximo_iteracoes_loop2 << "\n";
    

    // Somas
    auto inicio = std::chrono::high_resolution_clock::now();
    std::size_t resultado = Soma(numero_maximo_iteracoes);
    auto fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracao = fim - inicio;

    inicio = std::chrono::high_resolution_clock::now();
    std::size_t resultadoParalelo = SomaParalela(numero_maximo_iteracoes);
    fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracaoParalelo = fim - inicio;

    // Somas duplas

    inicio = std::chrono::high_resolution_clock::now();
    std::size_t resultadoLoopDuplo = LoopDuplo(maximo_iteracoes_loop1, maximo_iteracoes_loop2);
    fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracaoLoopDuplo = fim - inicio;

    inicio = std::chrono::high_resolution_clock::now();
    std::size_t resultadoLoopDuploParalelo = LoopDuploParalelo(maximo_iteracoes_loop1, maximo_iteracoes_loop2);
    fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracaoLoopDuploParalelo = fim - inicio;

    std::cout << "Testes finalizados!!!" << "\n";

    std::cout << "Somas: " << "\n";
    std::cout << "  Soma: " << "\n";
    std::cout << "      resultado: " << resultado << "\n";
    std::cout << "      tempo(ms): " << duracao/std::chrono::milliseconds(1) << "\n";


    std::cout << "  Soma Paralela: " << "\n";
    std::cout << "      resultado: " << resultadoParalelo << "\n";
    std::cout << "      tempo(ms): " << duracaoParalelo/std::chrono::milliseconds(1)  << "\n";

    std::cout << "  Diferenca dos Resultados: " << resultado - resultadoParalelo << "\n\n";


    std::cout << "Somas Dupla: " << "\n";
    std::cout << "  Soma Dupla: " << "\n";
    std::cout << "      resultado: " << resultadoLoopDuplo << "\n";
    std::cout << "      tempo(ms): " << duracaoLoopDuplo/std::chrono::milliseconds(1) << "\n";


    std::cout << "  Soma Dupla Paralela: " << "\n";
    std::cout << "      resultado: " << resultadoLoopDuploParalelo << "\n";
    std::cout << "      tempo(ms): " << duracaoLoopDuploParalelo/std::chrono::milliseconds(1)  << "\n";

    std::cout << "  Diferenca dos Resultados: " << resultadoLoopDuplo - resultadoLoopDuploParalelo << "\n\n";
    

    return 0;
}