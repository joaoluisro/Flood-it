#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#define MAX_INF 2147483647;

int n, m, k;

int visitado[100*100];

// Guardar a borda
// Calcula cada bloco adjacente a borda
// Calcular a menor distancia euclidiana de cada bloco
// Expande no bloco de menor distancia até dividir o tabuleiro (n-1, m-1)
// BFS

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

// V =
// v1: [(1,0), (1,1)]
// v2 : [(0,0)]

int dist_euclidiana(int i, int j){
  return((n - i)*(n - i) + (m - j)*(m - j));
}

int calcula_distancia(int *board, int cor, int i, int j){

  if(i == n || j == m || board[i*m + j] != cor){
     return MAX_INF;
  }
  else{
    return min(dist_euclidiana(i,j),
           min(calcula_distancia(board,cor,i+1, j),
           calcula_distancia(board, cor, i, j + 1)));
  }
}

int escolhe_cor_diagonal(int *board){
  vector<pair<int,int>> fronteira;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(visitado[i*m + j] && (!visitado[i*m + (j + 1)] || !visitado[(i + 1)*m + j])){
        fronteira.push_back(make_pair(i,j));
        //cout << i << " " <<j << endl;
      }
    }

  }

  int cor1, cor2, dist1, dist2, min_cor;
  int min_dist = MAX_INF;
  int curr_i, curr_j;
  for(int i = 0; i < fronteira.size(); i++){
    curr_i = fronteira[i].first;
    curr_j = fronteira[i].second;
    cor1 = board[curr_i*m + (curr_j + 1)];
    cor2 = board[(curr_i + 1)*m + curr_j];
    dist1 = calcula_distancia(board, cor1, curr_i, curr_j + 1);
    dist2 = calcula_distancia(board, cor2, curr_i + 1, curr_j);
    if(min_dist > dist1 ){
       min_dist = dist1;
       min_cor = cor1;
    }else if(min_dist > dist2){
      min_dist = dist2;
      min_cor = cor2;
    }
  }
  return min_cor;
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

  if(i == n || j == m){
    return;
  }
  if(board[i*m + j] == old_color || board[i*m + j] == choice){
    visitado[i*m + j] = 1;
  }

  if(i == n || j == m || board[i*m + j] != old_color){

    return;
  }
  else{
    board[i*m + j] = choice;
    visitado[i*m + j] = 1;
    update_flood(board, old_color, choice, i + 1, j);
    update_flood(board, old_color, choice, i, j + 1);
  }
}


// Função que resolve o problema
void solve(int *board){

  vector<int> past_choices;
  int choice;

  do{
    choice = select_color(board);
  } while(choice == board[0]);
  update_flood(board, board[0], choice, 0, 0);

  print_board(visitado);
  cout << endl;
  // Enquanto o problema não está resolvido
  while(!visitado[m*n]){

    // Garante que a cor escolhida não pode ser a mesma
    // que a anterior


    choice = escolhe_cor_diagonal(board);
    cout << "c " << choice << endl;

    // Faz um push para guardar as escolhas
    past_choices.push_back(choice);

    // Atualiza o tabuleiro de acordo com a escolha
    update_flood(board, board[0], choice, 0, 0);

    print_board(visitado);
    cout << endl;
    //print_board(visitado);
    //cout << endl;
  }

  for(int i = 0; i < past_choices.size() - 1; i++){
    if(past_choices[i] == past_choices[i+1]){
      past_choices.erase(past_choices.begin() + i);
    }
  }

  // Formata a saída
  cout << past_choices.size() << endl;
  for(int i = 0; i < past_choices.size(); i++){
    cout << past_choices[i] << " ";
  }
  cout << endl;


  print_board(board);
  cout << endl;
}


void search_tree(int *board){



  while(!board_solved(board)){

  }
}


int main(){

  // Lê os parametros n, m e k
  cin >> n >> m >> k;

  // pra melhorar performance
  // acessar uma célula como board[i][j] = board[i*m + j]
  int *board = new int[n*m];

  // Lê o tabuleiro
  for(int i = 0; i < n*m; i++) cin >> board[i];
  for(int i = 0; i < n*m; i++) visitado[i] = 0;
  // Resolve o problema
  solve(board);

  // Imprime a solução
  //print_board(board);

  return 0;

}
