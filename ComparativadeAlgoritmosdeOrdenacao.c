#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif
// Retorna o tempo atual em milissegundos com precisão sub-milissegundo,
// utilizando compilação condicional para garantir portabilidade entre Windows e Linux.
double obterTempoMs() {
#ifdef _WIN32
    LARGE_INTEGER frequencia, contador;
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart * 1000.0 / frequencia.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
#endif
}
// Função que gera o vetor base com semente fixa (reprodutibilidade)
int* gerarVetor(int tamanho) {
	unsigned int seed_fixa = 42;
	srand(seed_fixa);
	int *vetor = (int*) malloc(tamanho * sizeof(int));
	if (vetor == NULL) return NULL;
	for (int i = 0; i < tamanho; i++) {
		unsigned int r1 = (unsigned int)(rand() & 0x7FFF);
		unsigned int r2 = (unsigned int)(rand() & 0x7FFF);
		vetor[i] = (int)(((r1 << 15) | r2) % 1000000);
	}
	return vetor;
}
// Função para clonar o vetor (garante justiça experimental)
int* copiarVetor(int *origem, int tamanho) {
	int *copia = (int*) malloc(tamanho * sizeof(int));
	if (copia == NULL) return NULL;
	for (int i = 0; i < tamanho; i++) {
		copia[i] = origem[i];
	}
	return copia;
}
// Bubble Sort O(n^2)
void bubble(int *vetor, int tamanho) {
	int aux;
	for (int i = 0; i < tamanho - 1; i++) {
		for (int j = 0; j < tamanho - 1 - i; j++) {
			if (vetor[j] > vetor[j + 1]) {
				aux = vetor[j];
				vetor[j] = vetor[j + 1];
				vetor[j + 1] = aux;
			}
		}
	}
}
// Insertion Sort O(n^2)
void insertionSort(int *vetor, int tamanho) {
	for (int i = 1; i < tamanho; i++) {
		int chave = vetor[i];
		int j = i - 1;
		while (j >= 0 && vetor[j] > chave) {
			vetor[j + 1] = vetor[j];
			j--;
		}
		vetor[j + 1] = chave;
	}
}
// Selection Sort O(n^2)
void selectionSort(int *vetor, int tamanho) {
	for (int i = 0; i < tamanho - 1; i++) {
		int indiceMinimo = i;
		for (int j = i + 1; j < tamanho; j++) {
			if (vetor[j] < vetor[indiceMinimo]) {
				indiceMinimo = j;
			}
		}
		if (indiceMinimo != i) {
			int aux = vetor[i];
			vetor[i] = vetor[indiceMinimo];
			vetor[indiceMinimo] = aux;
		}
	}
}
// Merge Sort O(n log n)
// Função de Intercalação
void merge(int *vetor, int inicio, int meio, int fim) {
	int n1 = meio - inicio + 1;
	int n2 = fim - meio;
	int *esquerda = (int*) malloc(n1 * sizeof(int));
	int *direita = (int*) malloc(n2 * sizeof(int));
	if (esquerda == NULL || direita == NULL) return;
	for (int i = 0; i < n1; i++)
		esquerda[i] = vetor[inicio + i];
	for (int j = 0; j < n2; j++)
		direita[j] = vetor[meio + 1 + j];
	int i = 0, j = 0, k = inicio;
	while (i < n1 && j < n2) {
		if (esquerda[i] <= direita[j]) {
			vetor[k] = esquerda[i];
			i++;
		} else {
			vetor[k] = direita[j];
			j++;
		}
		k++;
	}
	while (i < n1) {
		vetor[k] = esquerda[i];
		i++;
		k++;
	}
	while (j < n2) {
		vetor[k] = direita[j];
		j++;
		k++;
	}
	free(esquerda);
	free(direita);
}
// Função Principal
void mergeSort(int *vetor, int inicio, int fim) {
	if (inicio < fim) {
		int meio = inicio + (fim - inicio) / 2;
		mergeSort(vetor, inicio, meio);
		mergeSort(vetor, meio + 1, fim);
		merge(vetor, inicio, meio, fim);
	}
}
// Quick Sort O(n log n)
// Função de partição (Esquema de Lomuto)
int particao(int *vetor, int baixo, int alto) {
	int pivo = vetor[alto]; // Escolhe o último elemento como pivô
	int i = baixo - 1;      // Índice do menor elemento

	for (int j = baixo; j <= alto - 1; j++) {
		// Se o elemento atual for menor ou igual ao pivô
		if (vetor[j] <= pivo) {
			i++;
			int aux = vetor[i];
			vetor[i] = vetor[j];
			vetor[j] = aux;
		}
	}
	// Coloca o pivô na sua posição definitiva
	int aux = vetor[i + 1];
	vetor[i + 1] = vetor[alto];
	vetor[alto] = aux;

	return (i + 1);
}
// Função principal do Quick Sort O(n log n) no caso médio
void quickSort(int *vetor, int baixo, int alto) {
	if (baixo < alto) {
		int pi = particao(vetor, baixo, alto);
		// Ordena recursivamente os elementos antes e depois da partição
		quickSort(vetor, baixo, pi - 1);
		quickSort(vetor, pi + 1, alto);
	}
}
// Heap Sort O(n log n)
// Função auxiliar para manter a propriedade de Max-Heap na subárvore de raiz 'i'
void heapify(int *vetor, int tamanho, int i) {
	int maior = i;
	int esquerda = 2 * i + 1;
	int direita = 2 * i + 2;
	if (esquerda < tamanho && vetor[esquerda] > vetor[maior]) {
		maior = esquerda;
	}
	if (direita < tamanho && vetor[direita] > vetor[maior]) {
		maior = direita;
	}
	if (maior != i) {
		int aux = vetor[i];
		vetor[i] = vetor[maior];
		vetor[maior] = aux;

		heapify(vetor, tamanho, maior);
	}
}
// Função principal
void heapSort(int *vetor, int tamanho) {
	// 1. Constrói a Max-Heap de baixo para cima
	for (int i = tamanho / 2 - 1; i >= 0; i--) {
		heapify(vetor, tamanho, i);
	}
	// 2. Extrai o maior elemento (raiz) um por um para o final do vetor
	for (int i = tamanho - 1; i > 0; i--) {
		int aux = vetor[0];
		vetor[0] = vetor[i];
		vetor[i] = aux;

		heapify(vetor, i, 0);
	}
}
// Counting Sort O(n + k)
void countingSort(int *vetor, int tamanho) {
	if (tamanho <= 1) return;
	// 1. Encontra o maior elemento do vetor para definir o tamanho do array de contagem (K)
	int maior = vetor[0];
	for (int i = 1; i < tamanho; i++) {
		if (vetor[i] > maior) {
			maior = vetor[i];
		}
	}
	// 2. Aloca o vetor de contagem zerado (calloc já inicializa tudo com 0)
	int *contagem = (int*) calloc(maior + 1, sizeof(int));
	if (contagem == NULL) return;
	// 3. Conta a frequência de cada número
	for (int i = 0; i < tamanho; i++) {
		contagem[vetor[i]]++;
	}
	// 4. Reconstrói o vetor original já ordenado
	int indice = 0;
	for (int i = 0; i <= maior; i++) {
		while (contagem[i] > 0) {
			vetor[indice] = i;
			indice++;
			contagem[i]--;
		}
	}
	// Libera a memória auxiliar
	free(contagem);
}
// Radix Sort O(d * (n + k))
// Função auxiliar: Counting Sort estável baseado no dígito atual (exp = 1, 10, 100...)
void countingSortPorDigito(int *vetor, int tamanho, int exp) {
	int *saida = (int*) malloc(tamanho * sizeof(int));
	int contagem[10] = {0}; // Apenas 10 posições (dígitos de 0 a 9)
	if (saida == NULL) return;
	// 1. Conta quantas vezes cada dígito (0 a 9) aparece na casa decimal 'exp'
	for (int i = 0; i < tamanho; i++) {
		int digito = (vetor[i] / exp) % 10;
		contagem[digito]++;
	}
	// 2. Soma acumulada para descobrir a posição exata de cada dígito no vetor de saída
	for (int i = 1; i < 10; i++) {
		contagem[i] += contagem[i - 1];
	}
	// 3. Constrói o vetor de saída de trás para frente (garante ESTABILIDADE)
	for (int i = tamanho - 1; i >= 0; i--) {
		int digito = (vetor[i] / exp) % 10;
		saida[contagem[digito] - 1] = vetor[i];
		contagem[digito]--;
	}
	// 4. Copia o resultado da passada atual de volta para o vetor original
	for (int i = 0; i < tamanho; i++) {
		vetor[i] = saida[i];
	}
	free(saida);
}
// Função principal
void radixSort(int *vetor, int tamanho) {
	if (tamanho <= 1) return;
	// Encontra o maior número para saber o número máximo de dígitos
	int maior = vetor[0];
	for (int i = 1; i < tamanho; i++) {
		if (vetor[i] > maior) {
			maior = vetor[i];
		}
	}
	// Aplica o Counting Sort para cada dígito (unidades, dezenas, centenas...)
	for (int exp = 1; maior / exp > 0; exp *= 10) {
		countingSortPorDigito(vetor, tamanho, exp);
	}
}
// Bucket Sort O(n + k)
// Estrutura auxiliar para representar cada Balde
typedef struct {
	int *dados;
	int quantidade;
	int capacidade;
} Balde;
// Função auxiliar para inserir elemento no balde redimensionando se necessário
void inserirNoBalde(Balde *b, int valor) {
	if (b->quantidade == b->capacidade) {
		b->capacidade *= 2;
		b->dados = (int*) realloc(b->dados, b->capacidade * sizeof(int));
	}
	b->dados[b->quantidade++] = valor;
}
// Função principal Bucket Sort O(n + k) no caso médio
void bucketSort(int *vetor, int tamanho) {
	if (tamanho <= 1) return;
	// 1. Encontra o menor e o maior valor para mapear a proporção
	int min = vetor[0];
	int max = vetor[0];
	for (int i = 1; i < tamanho; i++) {
		if (vetor[i] < min) min = vetor[i];
		if (vetor[i] > max) max = vetor[i];
	}
	if (min == max) return; // Todos os elementos são iguais
	// Define a quantidade de baldes (ex: 1 balde para cada 10 elementos)
	int numBaldes = tamanho / 10;
	if (numBaldes < 1) numBaldes = 1;
	Balde *baldes = (Balde*) malloc(numBaldes * sizeof(Balde));
	if (baldes == NULL) return;
	for (int i = 0; i < numBaldes; i++) {
		baldes[i].quantidade = 0;
		baldes[i].capacidade = 16;
		baldes[i].dados = (int*) malloc(baldes[i].capacidade * sizeof(int));
	}
	// 2. Distribui cada elemento no seu respectivo balde
	double intervalo = (double)(max - min + 1);
	for (int i = 0; i < tamanho; i++) {
		int idx = (int)(((double)(vetor[i] - min) / intervalo) * numBaldes);
		if (idx >= numBaldes) idx = numBaldes - 1;
		inserirNoBalde(&baldes[idx], vetor[i]);
	}
	// 3. Ordena cada balde (usando Insertion Sort) e concatena de volta no vetor
	int pos = 0;
	for (int i = 0; i < numBaldes; i++) {
		if (baldes[i].quantidade > 0) {
			insertionSort(baldes[i].dados, baldes[i].quantidade);
			for (int j = 0; j < baldes[i].quantidade; j++) {
				vetor[pos++] = baldes[i].dados[j];
			}
		}
		free(baldes[i].dados); // Libera a memória interna de cada balde
	}
	free(baldes); // Libera o array de baldes
}
int main() {
	int tamanho;
    double inicio, fim;
    printf("\t |||Projeto de Estrutura de Dados||| \n");
    printf("\nDigite a quantidade de numeros (ex: 1000, 10000, 50000, 100000, 500000): ");
    if (scanf("%d", &tamanho) != 1 || tamanho <= 0) {
        printf("\nTamanho invalido! Tente de novo!\n");
        return 1;
    }
    // Geração do vetor base e respetivas cópias idênticas
    int *meuVetor = gerarVetor(tamanho);
    int *copiaBubble = copiarVetor(meuVetor, tamanho);
    int *copiaInsertion = copiarVetor(meuVetor, tamanho);
    int *copiaSelection = copiarVetor(meuVetor, tamanho);
    int *copiaMerge = copiarVetor(meuVetor, tamanho);
    int *copiaQuick = copiarVetor(meuVetor, tamanho);
    int *copiaHeap = copiarVetor(meuVetor, tamanho);
    int *copiaCounting = copiarVetor(meuVetor, tamanho);
    int *copiaRadix = copiarVetor(meuVetor, tamanho);
    int *copiaBucket = copiarVetor(meuVetor, tamanho);
    printf("\t----------------------------------------------");
    printf("\n\tTestes de Algoritmos de Ordenacao (N = %d)\n\tObs: aguarde enquanto os testes sao executados\n", tamanho);
    printf("\t----------------------------------------------\n");
   	inicio = obterTempoMs();
    bubble(copiaBubble, tamanho);
    fim = obterTempoMs();
    printf("\nBubble Sort: %.2f ms", fim - inicio);
    // --- Insertion Sort ---
    inicio = obterTempoMs();
    insertionSort(copiaInsertion, tamanho);
    fim = obterTempoMs();
    printf("\nInsertion Sort: %.2f ms", fim - inicio);
    // --- Selection Sort ---
    inicio = obterTempoMs();
    selectionSort(copiaSelection, tamanho);
    fim = obterTempoMs();
    printf("\nSelection Sort: %.2f ms", fim - inicio);
    // --- Merge Sort ---
    inicio = obterTempoMs();
    mergeSort(copiaMerge, 0, tamanho - 1);
    fim = obterTempoMs();
    printf("\nMerge Sort: %.2f ms", fim - inicio);
    // --- Quick Sort ---
    inicio = obterTempoMs();
    quickSort(copiaQuick, 0, tamanho - 1);
    fim = obterTempoMs();
    printf("\nQuick Sort: %.2f ms", fim - inicio);
    // --- Heap Sort ---
    inicio = obterTempoMs();
    heapSort(copiaHeap, tamanho);
    fim = obterTempoMs();
    printf("\nHeap Sort: %.2f ms", fim - inicio);
    // --- Counting Sort ---
    inicio = obterTempoMs();
    countingSort(copiaCounting, tamanho);
    fim = obterTempoMs();
    printf("\nCounting Sort: %.2f ms", fim - inicio);
    // --- Radix Sort ---
    inicio = obterTempoMs();
    radixSort(copiaRadix, tamanho);
    fim = obterTempoMs();
    printf("\nRadix Sort: %.2f ms", fim - inicio);
    // --- Bucket Sort ---
    inicio = obterTempoMs();
    bucketSort(copiaBucket, tamanho);
    fim = obterTempoMs();
    printf("\nBucket Sort: %.2f ms\n", fim - inicio);
	// Libertação de toda a memória alocada (evita memory leaks)
	free(meuVetor);
	free(copiaBubble);
	free(copiaInsertion);
	free(copiaSelection);
	free(copiaMerge);
	free(copiaQuick);
	free(copiaHeap);
	free(copiaCounting);
	free(copiaRadix);
	free(copiaBucket);
	return 0;
}