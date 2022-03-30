#include <bits/stdc++.h>
#include <chrono>
using namespace std;

int n, m, k;

// Imprime o estado do tabuleiro
void print_board(int *board){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      cout << board[i*m + j] << " ";
    }
    cout << endl;
  }
}

// verifica se o jogo acabou
bool board_solved(int *board){
  int c1 = board[0];
  for(int i = 1 ;i < m*n; i++){
    if(board[i] != c1) return false;
  }
  return true;
}

// Ação do problema: Escolher uma cor
// A heuristica é implementada aqui
int select_color(int *board){
  int *frequency = new int[k];
  for(int i = 0; i < m*n; i++)frequency[board[i]]++;
  int highest = frequency[0];
  int index = 0;
  for(int i = 1; i < k; i++){
    if(frequency[k] > highest){
       index = i;
       highest = frequency[i];
    }
  }

  return((rand() % k) + 1);
  //return index + 1;
}

// Atualiza a matriz de cores de acordo com a decisão
// da heuristica implementada.
// Faz a atualização de maneira recursiva baseado no algoritmo 'Flood Fill'
void update_flood(int *board, int old_color, int choice, int i, int j){

  if(i == n || j == m || board[i*m + j] != old_color){
    return;
  }
  else{
    board[i*m + j] = choice;
    update_flood(board, old_color, choice, i + 1, j);
    update_flood(board, old_color, choice, i, j + 1);
  }

}

// Função que resolve o problema
void solve(int *board){

  vector<int> past_choices;
  int choice;

  // Enquanto o problema não está resolvido
  while(!board_solved(board)){

    // Garante que a cor escolhida não pode ser a mesma
    // que a anterior
    do{
      choice = select_color(board);
    } while(choice == board[0]);

    // Faz um push para guardar as escolhas
    past_choices.push_back(choice);

    // Atualiza o tabuleiro de acordo com a escolha
    update_flood(board, board[0], choice, 0, 0);

  }

  // Formata a saída
  cout << past_choices.size() << endl;
  for(int i = 0; i < past_choices.size(); i++) cout << past_choices[i] << " ";
  cout << endl;
}

int main(){



  // Lê os parametros n, m e k
  cin >> n >> m >> k;

  // pra melhorar performance
  // acessar uma célula como board[i][j] = board[i*m + j]
  int *board = new int[n*m];

  // Lê o tabuleiro
  for(int i = 0; i < n*m; i++) cin >> board[i];

  // Resolve o problema
  solve(board);

  // Imprime a solução
  print_board(board);

  return 0;

}
