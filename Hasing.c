/* Arquivo com acesso direto via hashing */
#include <stdio.h>
#include <string.h>
#define N 57
typedef struct carro {
	char placa[8];
	char marca[15];
	char modelo[15];
	char cor[15];
	int status;  // 1 - ativo ou 0 - removido
} CARRO;

typedef struct noTabela {
	char placa[8];
	int posicao;
	struct noTabela* prox;
	struct noTabela* ant;
} No;

FILE* prepararArquivo(char nome[]);
void fecharArquivo(FILE* arq);
int buscar(No* tabelaHashing[], char placa[]);
void cadastrar(FILE* arq, No* tabelaHashing[]);
void consultar(FILE* arq, No* tabelaHashing[]);
void alterar(FILE* arq, No* tabelaHashing[]);
void remover(FILE* arq, No* tabelaHashing[]);
void exibirCadastro(FILE* arq);
void criarIndice(FILE* arq, No* tabelaHashing[]);
void inserirTabelaHash(No* tabelaHashing[], char placa[], int pos);
int hashing(char placa[]);
void exibirOpcoes();

int main() {
	char nome_Arquivo[] = "carros.dat";
	int op;
	FILE* arquivo;
	No* tabelaHashing[N];
	arquivo = prepararArquivo(nome_Arquivo);
	if (arquivo == NULL)
		printf("Erro na abertura do arquivo. Programa encerrado \n");
	else {
		criarIndice(arquivo, tabelaHashing);
		do {
			exibirOpcoes();
			scanf("%d",&op); fflush(stdin);
			switch (op) {
			case 1: cadastrar(arquivo, tabelaHashing);
		                              break;
			case 2: consultar(arquivo, tabelaHashing);
			             break;
			case 3: alterar(arquivo, tabelaHashing);
			             break;
			case 4: remover(arquivo, tabelaHashing);
			             break;
			case 5: exibirCadastro(arquivo);
			             break;
			case 0: liberarArquivo(arquivo);
			             desalocarIndice(tabelaHashing);
			             break;
			default: printf("Opcao invalida \n");
			}
		} while (op != 0);
	}
	return 0;
}

FILE* prepararArquivo(char nome[]) {
    FILE *arq;
    arq = fopen(nome,"r+b");
    if(arq == NULL){
        arq = fopen(nome,"w+b");
    }
    return arq; 
     /* Tenta abrir o arquivo para leitura e gravação.
      * Caso não consiga abrir o arquivo, tenta criá-lo para leitura e gravação.
      */ 
}
void inicializarTabaleHash(No * Tabelahash[]){
	for( int i = 0 ;i < N;i++){
		Tabelahash[i]= NULL;
	}
}


void liberarArquivo(FILE* arq) {
	
    FILE* arqTemp;
    CARRO car;
    int retorno;
    arqTemp = fopen("temp.dat", "w+b");
    if (arqTemp == NULL) {
        printf("Erro ao tentar criar o arquivo temporário \n");
        return;
    }
    fseek(arq, 0, SEEK_SET);
    while (fread(&car, sizeof(CARRO), 1, arq) == 1) {
        if (car.status == 1) {
            fwrite(&car, sizeof(CARRO), 1, arqTemp);
        }
    }
    fclose(arq);
    fclose(arqTemp);
    remove("carros.dat");
    rename("temp.dat", "carros.dat");
    arq = fopen("carros.dat", "r+b");
    if (arq == NULL) {
        printf("Erro ao tentar reabrir o arquivo atualizado \n");
    }
    else {
        printf("Arquivo limpo com sucesso \n");
        fclose(arq);
    }
}
	/* Apagar, fisicamente, os registros que foram deletados logicamente e fechar o arquivo.
	*      1.1 - Criar um arquivo novo (vazio).
	*      1.2 - Copiar todos os registros de STATUS igual a 1 do arquivo de carros para o arquivo novo.
	*      1.3 - Fechar os dois arquivos.
	*      1.4 - Remover o arquivo de carros ("carros.dat").
	*      1.5 - Renomear o arquivo novo com o nome "carros.dat".
	*/
}

void criarIndice(FILE* arq, No* tabelaHashing[]) {
	/* Preencher a tabela de hashing com as chaves dos registros que estão armazenados no arquivo
	       1.1 - Ler o arquivo registro a registro até o final.
	       1.2 - Para cada registro lido, aplicar a função de hashing à chave (ou seja, a placa).
	       1.3 - O resultado da função indica a posição na tabela onde a chave será inserida.
	       1.4 - Criar nó, preencher com a chave e a posição dela no arquivo e inserir na tabela, 
	                na lista encadeada correspondente, de forma que a lista permaneça ordenada 
                                 por ordem crescente de placa.
	*/
}

void desalocarIndice(No* tabelaHashing[]) {
	/* Desalocar os nós que compõem as listas da tabela de hashing.
	*/
}

void exibirOpcoes() {
	printf("Opções \n");
	printf("1 - Cadastrar um carro \n");
	printf("2 - Consultar carro \n");
	printf("3 - Alterar dados do carro \n");
	printf("4 - Remover carro \n");
	printf("5 - Exibir carros cadastrados \n");
	printf("0 - Encerrar programa \n");
	printf("Informe a opcao: ");
}

int buscar(No* tabelaHashing[], char placa[]) {
	No *aux;
	int retorno = hashing_Chave(placa);
	if(tabelaHashing[retorno]==NULL){
		return -1;
	}
	else{
		aux= tabelaHashing[retorno];
		if(tabelaHashing[retorno]==NULL){
			return -1;

		}
		else {
			aux = tabelaHashing[retorno];
			while(aux!=NULL){
				if(strcmp(aux->placa,placa)==0){
					return aux->posicao;
				}
				else if(strcmp(aux->placa,placa) > 0){
					return -1;
				}
				else {
					aux= aux->prox;
				}
			}
		}
	}
	
	/* Procurar na tabela de hashing a placa desejada e retornar a posição da placa no arquivo.
	   1 - Aplicar a função de hashing na chave (ou seja, na placa).
	   2 - Procurar a chave na lista indicada pelo resultado da função (usar busca sequencial melhorada).
	   3 - Caso encontre, retornar a posição da chave no arquivo.
	   4 - Caso não encontre, retornar -1 
                 */
}

No * CriarNo(char placa[], int pos){
	No* novo;
	novo=(No*)malloc(sizeof(No*));
	novo->ant= NULL;
	novo->prox= NULL;
	novo->posicao= pos;
	strcpy(novo->placa,placa);
	return novo;
}
void inserirTabelaHash(No* tabelaHashing[], char placa[], int pos) {
	// retorno recebe o retorno do void hash que no caso é a chave de onde ele vai ficar na tabela no caso de 0 ate o 57(tamanho todo da tabela )
	//strcmp Retorna um valor negativo se a primeira string for alfabeticamente menor que a segunda.
    //strcmp Retorna um valor positivo se a primeira string for alfabeticamente maior que a segunda.
    //strcpy(destino, origem)
	No* novo;
	No* aux;
	int retorno =hashing(placa);
	if(tabelaHashing[retorno]==NULL){
		novo = CriarNo(placa,pos);
		tabelaHashing[retorno] = novo;
	}
	else{
		aux = tabelaHashing[retorno];

		if(strcmp(tabelaHashing[retorno]->placa,placa) > 0){
			novo = CriarNo(placa,pos);
			novo->prox = aux;
			aux->ant = novo;
			tabelaHashing[retorno] = novo;
		}
		else{
			while(aux->prox != NULL && strcmp(aux->placa,placa) < 0){
				// ele vai passando enquanto o aux->info no caso a placa for menor que a do novo no indo ate o final da lista
				aux = aux->prox;
			}
			novo = CriarNo(placa,pos);

			if(aux->prox==NULL && strcmp(aux->placa,placa) < 0){
				aux->prox= novo;
				novo->ant= aux;
				novo->prox= NULL;
			}
			else{
			novo->prox=aux;
			novo->ant= aux->ant;
			aux->ant->prox= novo;
			aux->ant= novo;
			}
		}

	}
	
	/* Inserir na tabela hashing, na lista encadeada indicada pela função de hashing, 
	* uma chave e sua posição no arquivo.
	* 1 - Aplicar a função de hashing à chave (ou seja, a placa).
	* 2 - O resultado da função indica a posição na tabela onde a chave será inserida.
	* 3 - Criar nó, preencher com a chave e a posição dela no arquivo e inserir na tabela, 
	* na lista encadeada correspondente, de forma que a lista permaneça ordenada 
                 * por ordem crescente de placa.
	*/
}

void removerTabelaHash(No* tabelaHashing[], char placa[], int posTabela) {

	No* aux;
	if(strcmp(tabelaHashing[posTabela]->placa,placa)==0){
		aux = tabelaHashing[posTabela];
		tabelaHashing[posTabela] = tabelaHashing[posTabela]->prox;
		free(aux);
	}
	else {
		aux = tabelaHashing[posTabela];
		while(aux->prox != NULL && strcmp(aux->placa,placa) < 0){
			aux = aux->prox;
		}
		
		if(aux->prox == NULL && strcmp(aux->placa,placa) ==0){
			aux->ant->prox = NULL;
			free(aux);
		}
        else { 
        	aux->ant->prox = aux->prox; 
            aux->prox->ant = aux->ant;
			free(aux);
		
		}
	/* Remover da tabela de hashing o nó que contem a placa passada como parâmetro. 
	* Recebe como parâmetro também a posição na tabela onde a chave se encontra.
	*/
	}
}

	


int hashing_Chave(char placa[]){
    int i;
    int soma=0;
    int tam =strlen(placa);
    for(i= 0 ; i<tam ; i++){
        soma = soma +placa[i] <<  (i % 8);
        //soma+=(placa[i] << (i%8) );
    }
    return abs(soma) % N; 

    // regra  pra placas como BAc, ou bac, cab ficarei no mesmo lugar;
    // DUvida na permutção alfa numericas

	/* A função “hashing” recebe com parâmetro a chave (ou seja, a placa) e 
	* retorna o valor calculado segundo o método da permutação para chaves alfanuméricas 
	* (visto em sala).
	*/
}

void cadastrar(FILE* arq, No* tabelaHashing[]) {
	CARRO carro;
	int retorno
	
	/* Cadastrar o registro do carro no arquivo e inserir a chave (placa) na tabela de hashing.
	* 1 - Solicita a placa do carro a ser cadastrado.
	* 2 - Procura pela placa na tabela de hashing.
	* 3 - Caso encontre, informa que o carro já está no cadastro.
	* 4 - Caso não encontre, solicita os demais dados do carro, o insere no final do arquivo.
	* 5 - Insere a chave, juntamente com sua posição no arquivo, na tabela de hashing.
	*     Utilize para isso o procedimento "inserirTabelaHash".
	*/
}

void consultar(FILE* arq, No* tabelaHashing[]) {
	/* Consultar o registro do carro no arquivo
                  * 1 - Solicita a placa do carro a ser consultado.
                  * 2 - Procura pela placa na tabela de hashing.
                  * 3 - Caso não encontre, informa que o carro não está no cadastro.
                  * 4 - Caso encontre, vai ao arquivo, na posição indicada, 
	 *     lê o registro do carro e exibe seus dados.
                  */
}

void alterar(FILE* arq, No* tabelaHashing[]) {
	/* Alterar o registro do carro no arquivo.
	 * 1 - Solicita a placa do carro a ser alterado.
	 * 2 - Procura pela placa na tabela de hashing.
	 * 3 - Caso não encontre, informa que o carro não está no cadastro.
	 * 4 - Caso encontre, vai ao arquivo, na posição indicada, lê o registro do carro e 
                         exibe seus dados.
	 * 5 - Pergunta ao usuário quais dados deseja alterar. Efetiva a alteração dos dados no arquivo.
	*/
}

void remover(FILE* arq, No* tabelaHashing[]) {
	/* Remover o registro do carro do arquivo
	 * 1 - Solicita a placa do carro a ser removido.
	 * 2 - Procura pela placa na tabela de hashing.
	 * 3 - Caso não encontre, informa que o carro não está no cadastro.
	 * 4 - Caso encontre, vai ao arquivo, na posição indicada, lê o registro do carro e exibe seus dados.
	 * 5 - Pergunta ao usuário se deseja realmente removê-lo. Efetiva a remoção que consiste 
                         em alterando no arquivo o STATUS do registro do carro para 0 (removido).
	 * 5 - Remove o nó que contém a chave, juntamente com sua posição no arquivo, 
                          da tabela de hashing. Utilize para isso o procedimento "removerTabelaHash".
	*/
}

void exibirCadastro(FILE* arq) {
	
	CARRO car;
    int retorno;
    fseek(arq, 0, SEEK_SET);

    do {
        retorno = fread(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
            if (car.status == 1) {
                printf("Placa : %s\n", car.placa);
                printf("Marca: %s\n", car.marca);
                printf("Modelo: %s\n", car.modelo);
                printf("Modelo: %s\n", car.cor);
            }
        }
    } while (!feof(arq));
	
}