#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct Item {
    int weight;
    int value;
    bool used; // Indica se o item foi usado na solução
};

int computeWeight(const std::vector<Item>& items, const std::vector<bool>& solution) {
    int totalWeight = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) {
            totalWeight += items[i].weight;
        }
    }
    return totalWeight;
}

int computeValue(const std::vector<Item>& items, const std::vector<bool>& solution) {
    int totalValue = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) {
            totalValue += items[i].value;
        }
    }
    return totalValue;
}

void adjustSolution(std::vector<Item>& items, std::vector<bool>& solution, int& currentWeight, int& currentValue, int maxWeight) {
    while (currentWeight > maxWeight) {
        for (int i = solution.size() - 1; i >= 0; --i) {
            if (solution[i]) {
                solution[i] = false;
                currentWeight -= items[i].weight;
                currentValue -= items[i].value;
                items[i].used = false;
                if (currentWeight <= maxWeight) break;
            }
        }
    }
}

int main() {
    // Inicializa o gerador de números aleatórios
    std::srand(std::time(0));

    // Inicia a medição do tempo
    auto start = std::chrono::high_resolution_clock::now();

    int N, W;
    std::cin >> N >> W;

    std::vector<Item> items(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> items[i].weight >> items[i].value;
        items[i].used = false;
    }

    // Solução inicial aleatória (vector<bool>)
    std::vector<bool> solution(N);
    int currentWeight = 0;
    int currentValue = 0;
    for (int i = 0; i < N; ++i) {
        solution[i] = std::rand() % 2;
        if (solution[i]) {
            items[i].used = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }

    // Ajusta se a solução inicial ultrapassa a capacidade
    adjustSolution(items, solution, currentWeight, currentValue, W);

    // Armazena os valores iniciais
    int initialWeight = currentWeight;
    int initialValue = currentValue;

    // Fase da Mochila Cheia
    for (int i = 0; i < N; ++i) {
        if (!items[i].used && currentWeight + items[i].weight <= W) {
            currentWeight += items[i].weight;
            currentValue += items[i].value;
            items[i].used = true;
            solution[i] = true;
        }
    }

    // Ajusta se a solução após Mochila Cheia ultrapassa a capacidade
    adjustSolution(items, solution, currentWeight, currentValue, W);

    int afterFullWeight = currentWeight;
    int afterFullValue = currentValue;

    // Fase de Substituição de Objeto
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < N; ++i) {
            if (!items[i].used) { // Objeto não usado
                for (int j = 0; j < N; ++j) {
                    if (items[j].used && items[i].weight <= items[j].weight && 
                        currentValue - items[j].value + items[i].value > currentValue) {
                        // Substitui o objeto j pelo objeto i
                        currentWeight = currentWeight - items[j].weight + items[i].weight;
                        currentValue = currentValue - items[j].value + items[i].value;
                        items[j].used = false;
                        items[i].used = true;
                        solution[j] = false;
                        solution[i] = true;
                        improved = true;
                        break; // Recomeça o processo
                    }
                }
            }
        }
    }

    // Ajusta se a solução após Substituição de Objeto ultrapassa a capacidade
    adjustSolution(items, solution, currentWeight, currentValue, W);

    int afterSubWeight = currentWeight;
    int afterSubValue = currentValue;

    // Fase de Hill Climbing
    improved = true;
    while (improved) {
        improved = false;
        std::vector<bool> bestNeighbor = solution;

        for (int i = 0; i < N; ++i) {
            // Gera um vizinho ao inverter um bit
            std::vector<bool> neighbor = solution;
            neighbor[i] = !neighbor[i];

            int neighborWeight = computeWeight(items, neighbor);
            int neighborValue = computeValue(items, neighbor);

            // Verifica se o vizinho é válido e melhor
            if (neighborWeight <= W && neighborValue > currentValue) {
                bestNeighbor = neighbor;
                currentWeight = neighborWeight;
                currentValue = neighborValue;
                improved = true;
            }
        }

        if (improved) {
            solution = bestNeighbor;
        }
    }

    // Ajusta se a solução após Hill Climbing ultrapassa a capacidade
    adjustSolution(items, solution, currentWeight, currentValue, W);

    // Exibe os resultados
    std::cout << "Peso total antes da Mochila Cheia: " << initialWeight << std::endl;
    std::cout << "Valor total antes da Mochila Cheia: " << initialValue << std::endl;

    std::cout << "Peso total depois da Mochila Cheia: " << afterFullWeight << std::endl;
    std::cout << "Valor total depois da Mochila Cheia: " << afterFullValue << std::endl;

    std::cout << "Peso total depois da Substituição de Objeto: " << afterSubWeight << std::endl;
    std::cout << "Valor total depois da Substituição de Objeto: " << afterSubValue << std::endl;

    std::cout << "Peso total depois do Hill Climbing: " << currentWeight << std::endl;
    std::cout << "Valor total depois do Hill Climbing: " << currentValue << std::endl;

    // Finaliza a medição do tempo
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
