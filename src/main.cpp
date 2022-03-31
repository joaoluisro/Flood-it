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

// Verifica se o jogo acabou
// Ele acaba quando todas as cores do tabuleiro forem iguais
bool board_solved(int *board){
  int c1 = board[0];
  for(int i = 1 ;i < m*n; i++){
    if(board[i] != c1) return false;
  }
  return true;
}

// Distancia euclidiana (sem a raiz para performance) de dois pontos distintos do tabuleiro
// dist_euclidiana = (x-i)^2 + (y - j)^2
int dist_euclidiana(int i, int j, int corner1, int corner2){
  return((corner1 - i)*(corner1 - i) + (corner2 - j)*(corner2 - j));
}

// Função um pouco confusa mas extremamente elegante
// Calcula a menor distancia euclidiana de um bloco que inicia-se na posição (i,j)
// Faz uma comparação entre a distancia da celula e de seus vizinhos (i + 1, j) e (i, j + 1)

int calcula_distancia(int *board, int cor, int i, int j, int corner1, int corner2){

  // Caso base: a cor da célula não é a mesma do bloco original OU
  //            a posição é invalida no tabuleiro 'encostou na parede'
  if(i == n || j == m || board[i*m + j] != cor){
     return MAX_INF;
  }
  else{
    // Calcule a minima distancia entre a célula atual e todas as outras
    // células do mesmo bloco (contém a mesma cor)
    return min(dist_euclidiana(i,j, corner1, corner2),
           min(calcula_distancia(board,cor,i+1, j, corner1, corner2),
           calcula_distancia(board, cor, i, j + 1, corner1, corner2)));
  }
}


// Função que retorna a escolha gananciosa
int bfs(int *board){

  // Calcula a frequencia de cada cor no tabuleiro
  int *frequency = new int[k + 1];
  for(int i =0; i < k + 1; i++) frequency[i] = 0;

  for(int i = 0; i < m*n; i++){
    if(!visitado[i]){
      frequency[board[i]]++;
    }
  }

  // retorna aquela que tiver o maior valor
  int index, highest = 0;
  for(int i = 1; i <= k; i++){
    if(frequency[i] > highest && i != board[0]){
       index = i;
       highest = frequency[i];
    }
  }

  return index;
}


// Função heuristica que retorna uma cor na tentativa de 'dividir o tabuleiro'
// 'corner1' e 'corner2' são valores de possiveis cantos do tabuleiro:
//  inferior direito, superior direito e inferior esquerdo.

// Ela procura encontrar blocos adjacentes a fronteira do flood que possuem
// a menor distancia euclidiana do canto escolhido.
// Selecionando a cor deste bloco como escolha para a heuristica.
int escolhe_cor_diagonal(int *board, int corner1, int corner2){

  // Inicializa a 'fronteira' do flood: valores que estão entre o 'interior' e a
  // região não explorada do tabuleiro.
  vector<pair<int,int>> fronteira;
  int old_color = board[0];
  bool is_right, is_down;

  // Calcula a 'fronteira': valores que foram visitados e possuem 0's adjacentes.
  for(int i = 0; i < n - 1; i++){
    for(int j = 0; j < m - 1; j++){
      is_right = !visitado[i*m + (j + 1)];
      is_down = !visitado[(i + 1)*m + j];
      if(visitado[i*m + j] && (is_right || is_down)) {
        if(is_right) fronteira.push_back(make_pair(i,j + 1));
        if(is_down) fronteira.push_back(make_pair(i + 1,j));
      }
    }
  }

  // Itera sobre cada elemento da fronteira, identificando 'blocos' de cores.
  // um 'bloco' é um conjunto conectado de cores, e cada elemento da fronteira
  // possui um ou dois blocos adjacentes. Procuramos saber qual a distancia euclidiana
  // minima de cada bloco, isto é, a distancia minima entre todas as celulas do bloco.

  int curr_cor, min_cor = board[0], dist;
  int min_dist = MAX_INF;
  int curr_i, curr_j;
  for(int i = 0; i < fronteira.size(); i++){
    // Pega o elemento adjacente da fronteira, identificado por 'curr_i' e 'curr_j'
    curr_i = fronteira[i].first;
    curr_j = fronteira[i].second;
    curr_cor = board[curr_i * m + curr_j];

    // Se a cor não for igual a antiga
    if(curr_cor != old_color){
      // Calcula a distancia desse bloco em relação ao canto escolhido
      dist = calcula_distancia(board, curr_cor, curr_i, curr_j, corner1, corner2);

      // Retorne o menor valor, e escolha a cor que é mais promissora.
      if(min_dist > dist){
         min_dist = dist;
         min_cor = curr_cor;
      }
    }

  }

  return min_cor;
}


// Atualiza a matriz de cores de acordo com a decisão da heuristica implementada.
// Faz a atualização de maneira recursiva baseado no algoritmo 'Flood Fill'
void update_flood(int *board, int old_color, int choice, int i, int j){

  // Caso de borda
  if(i == n || j == m || i < 0 || j < 0){
    return;
  }
  // Atualizamos a matriz de visitados
  if(board[i*m + j] == old_color || board[i*m + j] == choice){
    visitado[i*m + j] = 1;
  }
  // Caso no qual a cor não fara parte do novo flood
  if(board[i*m + j] != old_color){
    return;
  }
  // Recursão que atualiza todas as outras células adjacentes
  else{
    board[i*m + j] = choice;
    visitado[i*m + j] = 1;
    update_flood(board, old_color, choice, i + 1, j);
    update_flood(board, old_color, choice, i, j + 1);
    update_flood(board, old_color, choice, i - 1, j);
    update_flood(board, old_color, choice, i, j - 1);
  }
}


// Função que resolve o problema
void solve(int *board){

  // past_choices: escolhas feitas pela heuristica
  // choice      : cor atual escolhida
  vector<int> past_choices;
  int choice;


  // Escolhe a primeira cor a direita da celula inicial (0,0)
  choice = board[1];
  // Caso no qual a cor a direita é igual, escolhe a de baixo (1,0)
  if(choice == board[0]) choice = board[m];

  // Faz um push para guardar a escolha
  past_choices.push_back(choice);
  // Atualiza o tabuleiro de acordo com a escolha
  update_flood(board, board[0], choice, 0, 0);

  // Enquanto o problema não está resolvido
  while(!board_solved(board)){

    // Se o canto inferior direito ainda não foi visitado, Tente alcança-lo
    if(!visitado[(m*n) - 1]){
      choice = escolhe_cor_diagonal(board, n, m);
    }
    // Se o canto superior direito ainda não foi visitado, Tente alcança-lo
    else if(!visitado[(m - 1)]){

      choice = escolhe_cor_diagonal(board, 0, m );
    }
    // Se o canto inferior esquerdo ainda não foi visitado, Tente alcança-lo
    else if(!visitado[(n - 1)*m + 1]){

      choice = escolhe_cor_diagonal(board, n, 0);
    }
    // Todos os cantos foram visitados, escolha a decisão gananciosa (best-first)
    else{
      choice = bfs(board);
    }

    // Faz um push para guardar as escolhas
    past_choices.push_back(choice);

    // Atualiza o tabuleiro de acordo com a escolha
    update_flood(board, board[0], choice, 0, 0);

  }

  // Formata a saída
  cout << past_choices.size() << endl;
  for(int i = 0; i < past_choices.size(); i++){
    cout << past_choices[i] << " ";
  }
  cout << endl;

}




int main(){

  // Lê os parametros n, m e k
  cin >> n >> m >> k;

  // pra melhorar performance
  // acessar uma célula como board[i][j] = board[i*m + j]
  int *board = new int[n*m];

  // Lê o tabuleiro e inicializa a matriz de visitados
  for(int i = 0; i < n*m; i++) cin >> board[i];
  for(int i = 0; i < n*m; i++) visitado[i] = 0;

  // Resolve o problema
  solve(board);

  return 0;

}
