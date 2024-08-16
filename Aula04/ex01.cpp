#include <iostream> // Inclui a biblioteca para entrada e saída de dados

// mains do C++ sempre precisa ser um int, logo sempre precisa retornar algum INT
int main() {
    std::cout << "Ola, Mundo!" << std::endl; // Exibe a mensagem na tela    (endl serve para quebrar linha)
    return 0; // Indica que o programa terminou com sucesso
}


// Comando para compilar: g++ ex1.cpp -o ex1 -> ./ex1 

// outro jeito de fazer
/*
using namespace std;

int main ()
{
  cout << "Olá mundo!"; 
  return 0;
}
*/