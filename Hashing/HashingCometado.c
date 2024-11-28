/* Arquivo com acesso direto via hashing */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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





FILE* prepararArquivo(char nome[]) {
    FILE *arq;
    arq = fopen(nome,"r+b");
    if(arq == NULL){
        arq = fopen(nome,"w+b");
    }
    return arq; 
}
void inicializarTabaleHash(No * Tabelahash[]){
	for( int i = 0 ;i < N;i++){
		Tabelahash[i]= NULL;
	}
}

//Basicamente essa funcionalidade criar uma arquivo binario temporario onde vão ficar apenas os carros com status ativo
//fseek(arq, 0, SEEK_SET) reposiciona o ponteiro que serve tanto para escrever como para ler o arquivo para o inicio
//fread(&car, sizeof(CARRO), 1, arq) lê um registro do tipo CARRO que é uma struct e se o status do carro for 1 ele escreve no arqtemp
//remove o arquivo antigo original descantando e depois modifica o arqTemp para o mesmo nome do arquivo que foi apagado para nesse arquivo so ter carros com status ativo

void liberarArquivo(FILE* arq) {
	
    FILE* arqTemp;
    CARRO car;
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

void criarIndice(FILE* arq, No* tabelaHashing[]) {
	
	int retorno;
	int cont=-1;
    CARRO car;
    fseek(arq, 0, SEEK_SET);
    do{
        retorno = fread(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
           cont++;
           inserirTabelaHash(tabelaHashing,car.placa,cont);  
        }
        
    } while (!feof(arq));
}
// Resetar toda tabela hash 
void desalocarIndice(No* tabelaHashing[]) {
	int i;
    No *aux1,*aux2;
    for(i=0;i<N;i++){
        if(tabelaHashing[i]!= NULL){
            aux1 = tabelaHashing[i];
            while(aux1 != NULL){
                aux2 = aux1;
                aux1 = aux1->prox;
                free(aux2);
            }
            tabelaHashing[i] = NULL;
        }
    }
	
	
}



int buscar(No* tabelaHashing[], char placa[]) {
	No *aux;
	int retorno = hashing(placa);
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
			while(aux->prox != NULL){
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
}

int hashing(char placa[]){
    int i;
    int soma=0;
    int tam =strlen(placa);
    for(i= 0 ; i<tam ; i++){
        soma = (soma + placa[i]) <<  (i % 8);
        //soma+=(placa[i] << (i%8) );
    }
    return abs(soma) % N; 
}

No * CriarNo(char placa[], int pos){
	No* novo;
	novo=(No*)malloc(sizeof(No));
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

	}
}

	




//fseek posiciona o ponteiro no final do arquivo, pronto para adicionar o novo registro.
//ftell calcula o índice do novo registro no arquivo ,Retorna a posição atual do ponteiro no arquivo (em bytes). Dividindo pelo tamanho de um registro sizeof(CARRO)
//fwrite grava o registro no final do arquivo.
//fflush força a escrita imediata dos dados que estão no buffer para o arquivo 
//Essa função é utilizada para adicionar um novo carro ao arquivo binário e registrar sua localização em uma tabela de hashing

void cadastrar(FILE* arq, No* tabelaHashing[]) {
	
	CARRO car;
    int retorno;
	int posicao,posicao_no_Arq;
    printf("Informe Placa: ");
    scanf("%s",car.placa);
    posicao = buscar(tabelaHashing, car.placa);
    if (posicao != -1) {
        printf("Carro ja cadastrado\n");
    }
    else {

        printf("Informe Marca: ");
        scanf("%s",car.marca);
        printf("Informe Modelo: ");
        scanf("%s",car.modelo);
        printf("Informe cor: ");
        scanf("%s",car.cor);
        car.status = 1;
        fseek(arq, 0, SEEK_END);
        posicao_no_Arq = ftell(arq) / sizeof(CARRO);
        retorno = fwrite(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
            fflush(arq);
            printf("Carro cadastrado com sucesso\n");
            inserirTabelaHash(tabelaHashing, car.placa,posicao_no_Arq);
        }
        else {
            printf("Erro na gravação\n");

        }
    }
}


void consultar(FILE* arq, No* tabelaHashing[]) {
	CARRO car;
    int retorno, posicao;
    printf("Informe Placa a ser consultada: ");
    scanf("%s",car.placa);
    posicao = buscar(tabelaHashing, car.placa);
    if (posicao == -1) {
        printf("Carro nao cadastrado\n");
    }
    else {
        fseek(arq, posicao * sizeof(CARRO), SEEK_SET);
        retorno = fread(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
            printf("Placa : %s\n", car.placa);
            printf("Marca: %s\n", car.marca);
            printf("Modelo: %s\n", car.modelo);
            printf("Modelo: %s\n", car.cor);
        }
        else {
            printf("Erro de leitura\n");
        }
    }
}

void alterar(FILE* arq, No* tabelaHashing[]) {
	CARRO car;
    int retorno, posicao, op;
    printf("Informe Placa a ser consultada: ");
    scanf("%s",car.placa);
    posicao = buscar(tabelaHashing, car.placa);

    if (posicao == -1) {
        printf("Carro nao cadastrado\n");
    }
    else {
        fseek(arq, posicao * sizeof(CARRO), SEEK_SET);
        retorno = fread(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
            printf("Voce deseja:\n ");
            printf("1. Alterar Marca\n ");
            printf("2. Alterar Modelo\n ");
            printf("3. Alterar cor\n ");
            scanf("%d",&op);
            switch (op){
                case 1: printf("Informe a nova marca: ");
                scanf("%s", &car.marca);
                fseek(arq, -sizeof(CARRO), SEEK_CUR);
                retorno = fwrite(&car, sizeof(CARRO), 1, arq);
                if (retorno == 1) {
                    printf("Alterecao realizada com sucesso\n");

                }
                else {
                    printf("Erro de gravacao\n");
                }
                break;
                case 2:printf("Informe a novo modelo: ");
                scanf(" %s", &car.modelo);
                fseek(arq, -sizeof(CARRO), SEEK_CUR);
                retorno = fwrite(&car, sizeof(CARRO), 1, arq);
                if (retorno == 1) {
                    printf("Alterecao realizada com sucesso\n");

                }
                else {
                    printf("Erro de gravacao\n");
                }
                break;
                case 3:printf("Informe a nova cor: ");
                scanf(" %s", &car.cor);
                fseek(arq, -sizeof(CARRO), SEEK_CUR);
                retorno = fwrite(&car, sizeof(CARRO), 1, arq);
                if (retorno == 1) {
                    printf("Alterecao realizada com sucesso\n");

                }
                else {
                    printf("Erro de gravacao\n");
                }
                break;
                default: printf("Opção inválida! \n");
            }
        }
        else {
            printf("Erro de leitura\n");
        }
    }
}

void remover(FILE* arq, No* tabelaHashing[]) {
	CARRO car;
    int retorno, posicao,op;
    printf("Informe Placa a ser consultada: ");
    scanf("%s",car.placa);
    posicao = buscar(tabelaHashing, car.placa);
    if (posicao == -1) {
        printf("Carro nao cadastrado\n");
    }
    else {
        fseek(arq, posicao * sizeof(CARRO), SEEK_SET);
        retorno = fread(&car, sizeof(CARRO), 1, arq);
        if (retorno == 1) {
            printf("Placa : %s\n", car.placa);
            printf("Marca: %s\n", car.marca);
            printf("Modelo: %s\n", car.modelo);
            printf("Modelo: %s\n", car.cor);
            printf("Voce tem certeza que deseja remover? \n");
            printf("1. SIM\n");
            printf("2. NAO\n");
            scanf("%d",&op);
            switch (op){
            case 1: car.status = 0;
            
            fseek(arq, -sizeof(CARRO), SEEK_CUR);
            retorno = fwrite(&car, sizeof(CARRO), 1, arq);
            if (retorno == 1) {
                printf("Remocao realizada com sucesso\n");
                removerTabelaHash(tabelaHashing,car.placa,hashing(car.placa));
            }
            else {
                printf("Erro de gravacao\n");

            }
                break;
            case 2: printf("Operacao cancelada\n");
                break;
                
            default: printf("Opção inválida! \n");
            
                break;
            }
        }
        else {
            printf("Erro de leitura\n");
        }
    }
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

int main() {
	char nome_Arquivo[] = "carros.dat";
	int op;
	FILE* arquivo;
	No* tabelaHashing[N];
    inicializarTabaleHash(tabelaHashing);
	arquivo = prepararArquivo(nome_Arquivo);
	if (arquivo == NULL)
		printf("Erro na abertura do arquivo. Programa encerrado \n");
	else {
		criarIndice(arquivo, tabelaHashing);
		do {
			exibirOpcoes();
			scanf("%d",&op); fflush(stdin);
			switch (op) {
			case 1: 
				cadastrar(arquivo, tabelaHashing);
		        break;
			case 2: 
				consultar(arquivo, tabelaHashing);
			    break;
			case 3: 
				alterar(arquivo, tabelaHashing);
			    break;
			case 4: 
				remover(arquivo, tabelaHashing);
			    break;
			case 5: 
				exibirCadastro(arquivo);
			    break;
			case 0: 	
				liberarArquivo(arquivo);
			    desalocarIndice(tabelaHashing);
			    break;
			default: printf("Opcao invalida \n");
			}
		} while (op != 0);
	}
	return 0;
}

