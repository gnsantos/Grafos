#include <stdlib.h>
#include <stdio.h>

#define MAX 100

int *degrees;
int** adj;

/*fila de prioridade*/
int leafs[MAX];
int *code;
int place = 0; 
int ini, fim;
/**************** FUNCOES DA FILA DE PRIORIDADE ************************/
void inicializaFila(){
  int i;
  fim = ini = -1;
  for(i = 0; i < MAX; i++)
    leafs[i] = -1;
}
void check(int data){
  int i,j;
  for (i = 0; i <= fim; i++){
    if (data <= leafs[i]){
      for (j = fim + 1; j > i; j--){
	leafs[j] = leafs[j - 1];
      }
      leafs[i] = data;
      return;
    }
  }
  leafs[i] = data;
}

void priorityInsert(int l){
  if((ini == -1) && (fim == -1)){
    ini++; fim++;
    leafs[fim] = l;
    return;
  }
  else
    check(l);
  fim++;
}

void priorityDelete(int data){
  int i;
  for (i = 0; i <= fim; i++){
    if (data == leafs[i]){
      for (; i < fim; i++){
	leafs[i] = leafs[i + 1];
      }
 
      leafs[i] = -99;
      fim--;
 
      if (fim == -1) 
	ini = -1;
      return;
    }
  }
}

int isEmpty(){
  return ((fim == -1) && (ini == -1));
} 

/******************** FUNCOES PARA FOLHAS ***********************/

void getLeafs(int numVertex){
  int i;
  for(i = ini; i < numVertex; i++)
    if(degrees[i] == 1)
      priorityInsert(i);
}


int getParent(int leaf, int v){
  int i;
  for(i = 0; i < v; i++)
    if(adj[leaf][i] != 0)
      break;
  adj[leaf][i] = adj[i][leaf] = 0;
  degrees[i]--;
  return i;
}

void removeLeaf(int v){
  int folha = leafs[ini];
  int parent;
  priorityDelete(folha);
  parent = getParent(folha, v);
  code[place++] = parent;
  if( degrees[parent] == 1) priorityInsert(parent);
}

/******************************* CONSTROI GRAFO ****************************/

int readGraph(FILE* in){
  int numVertex;
  int i, u, v;
  fscanf(in, "%d", &numVertex);
  /*inicializa matriz de adjacencia*/
  for(i = 0; i < numVertex; i++)
    adj = malloc(numVertex*sizeof(int*));
  for(i = 0; i < numVertex; i++)
    adj[i] = calloc(numVertex,sizeof(int));

  /*inicializa o vetor de graus*/
  degrees = calloc(numVertex, sizeof(int));
  code = calloc(numVertex - 2, sizeof(int));
  
  /*identifica as arestas*/
  for(i = 1; i < numVertex; i++){
    fscanf(in, "%d %d", &u, &v);
    adj[u][v] = adj[v][u] = 1;
    degrees[u]++;
    degrees[v]++;
  }  
  
  getLeafs(numVertex);
  fclose(in);

  return numVertex;
}


/************************ INVERTE O PROCESSO *******************************/

int isPresent(int num, int size){
  int i = 0;
  for(; i < size; i++)
    if(code[i] == num) return 1;
  return 0;
}

int notPreset(int* code, int size){
  int i;
  for(i = 0; i < size+2; i++)
    if( !isPresent(i, size) )
      return i;
  return -1;
}

int getLast(int size, int f){
 int i;
  for(i = 0; i < size+2; i++)
    if(i != f)
      if( !isPresent(i, size) )
	return i;
  return -1;
}

int readCode(FILE *in){
  int i = 0, size;
  fscanf(in, "%d", &size);
  code = calloc(size, sizeof(int));
  for(; i < size; i++)
    fscanf(in, "%d", &code[i]);
  return size;
}

void codeToTree(int size){
  int vertex, parent, numVertex = size+2, i;
  inicializaFila();
  place = 0;
  /*inicializa matriz de adjacencia*/
  for(i = 0; i < numVertex; i++)
    adj = malloc(numVertex*sizeof(int*));
  for(i = 0; i < numVertex; i++)
    adj[i] = calloc(numVertex,sizeof(int));
  while(place < size){
    vertex = notPreset(code, size);
    parent = code[place];
    adj[parent][vertex] = adj[vertex][parent] = 1;
    code[place++] = vertex;
  }
  vertex = notPreset(code,size);
  parent = getLast(size, vertex);
  adj[parent][vertex] = adj[vertex][parent] = 1;
}


/**************************** VISUALIZACAO **********************************/
void showLeafs(){
  int i;
  if(isEmpty()) return;
  for(i = ini; i <= fim; i++)
    printf("%d ", leafs[i]);
}

void showGraph(int numVertex){
  int i, j;
  for(i = 0; i < numVertex; i++)
    for(j = i+1; j < numVertex; j++)
      if(adj[i][j] == 1) printf("%d %d\n", i, j);
}

void showDegrees(int numVertex){
  int i;
  for(i = 0; i < numVertex; i++)
    printf("%d: %d\n", i, degrees[i]);
}

void showCode(int v){
  int i = 0;
  for(i = 0; i < v; i++)
    printf("%d ", code[i]);
  printf("\n");
}


/********************************* MAIN ************************************/

int main(int argc, char** argv){
  int v, c = 0, size;
  FILE* in = fopen(argv[1], "r");
  int op = atoi(argv[2]);
  switch(op){
  case 1:
    inicializaFila();
    v = readGraph(in);
    while(c++ < v -2){
      removeLeaf(v);
    }
    showCode(v);
    printf("\n");
    break;
  
  case 2:
    size = readCode(in);
    showCode(size);
    codeToTree(size);
    showGraph(size+2);
    break;
  }
  return 0;
}

