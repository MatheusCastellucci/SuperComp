#include <iostream> // Inclui a biblioteca de entrada e saída padrão
#include <vector>   // Inclui a biblioteca para usar vetores
#include <string>   // Inclui a biblioteca para manipulação de strings

struct Item {
    std::string nome;
    int quantidade;
};

// Função para adicionar um item ao estoque
void adicionarItem(std::vector<Item>& estoque) {
    Item novoItem;
    std::cout << "Digite o nome do item: ";
    std::cin.ignore(); // Limpa o buffer do cin
    std::getline(std::cin, novoItem.nome);
    std::cout << "Digite a quantidade do item: ";
    std::cin >> novoItem.quantidade;
    
    estoque.push_back(novoItem);
    std::cout << "Item adicionado com sucesso!" << std::endl;
}

// Função para remover um item do estoque
void removerItem(std::vector<Item>& estoque) {
    std::string nomeItem;
    std::cout << "Digite o nome do item a ser removido: ";
    std::cin.ignore(); // Limpa o buffer do cin
    std::getline(std::cin, nomeItem);

    for (auto it = estoque.begin(); it != estoque.end(); ++it) {
        if (it->nome == nomeItem) {
            estoque.erase(it);
            std::cout << "Item removido com sucesso!" << std::endl;
            return;
        }
    }
    
    std::cout << "Item nao encontrado!" << std::endl;
}

// Função para visualizar o estoque atual
void visualizarEstoque(const std::vector<Item>& estoque) {
    if (estoque.empty()) {
        std::cout << "O estoque esta vazio!" << std::endl;
        return;
    }
    
    std::cout << "Estoque atual:" << std::endl;
    for (const auto& item : estoque) {
        std::cout << "Nome: " << item.nome << ", Quantidade: " << item.quantidade << std::endl;
    }
}

int main() {
    std::vector<Item> estoque; // Declara um vetor para armazenar os itens do estoque
    int opcao;

    do {
        std::cout << "\nSistema de Controle de Estoque\n";
        std::cout << "1. Adicionar item\n";
        std::cout << "2. Remover item\n";
        std::cout << "3. Visualizar estoque\n";
        std::cout << "4. Sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                adicionarItem(estoque);
                break;
            case 2:
                removerItem(estoque);
                break;
            case 3:
                visualizarEstoque(estoque);
                break;
            case 4:
                std::cout << "Saindo do sistema..." << std::endl;
                break;
            default:
                std::cout << "Opcao invalida! Tente novamente." << std::endl;
                break;
        }
    } while (opcao != 4);

    return 0; // Indica que o programa terminou com sucesso
}
