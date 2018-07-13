
#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include <time.h>
#include <stdlib.h>

//******Estruturas*******
struct matrix
{
	List* values;
	long long unsigned height;
	long long unsigned width;
};

typedef struct matrix Matrix;

//******Protótipos*******
Matrix* mult_sparseMatrix(Matrix* a, Matrix* b);
Matrix* sum_sparseMatrix(Matrix* a, Matrix* b);
Matrix* create_sparseMatrix(long long unsigned rows, long long unsigned columns);
int pickValue(Matrix* matrix, long long unsigned row, long long unsigned column);
void checkFor_nullElements(Matrix* matrix);
void removeFrom_Position(long long unsigned row, long long unsigned column, Matrix* matrix);
void insertBy_Position(long long unsigned value,long long unsigned row, long long unsigned column, Matrix* matrix);
void createInput_sparseMatrix(Matrix* matrix);
void createAleatory_sparsematrix(long long unsigned elements, Matrix* matrix);
void printCompact_sparseMatrix(Matrix* matrix);
void print_sparseMatrix(Matrix* matrix);
void destroy_sparseMatrix(Matrix* matrix);

//******Funções*******

//Multiplicação de de duas matrizes esparsas
Matrix* mult_sparseMatrix(Matrix* a, Matrix* b)
{
	//Verifica se pode ser efetuada a multiplicação
	if (a->width != b->height)
	{
		printf("\t***Abortando: *Função mult_sparseMatrix* Número de linhas da matriz A é diferente da B\n\n");
		exit(1);
	}

	//Cria a matriz resposta
	Matrix* destiny = create_sparseMatrix(a->height, a->width);

	List* temp_a = a->values;
	List* temp_b;

	/*A partir do elemento não nulo A, procura-se
	os elementos de B que correspondem ao elemento
	em A(número da coluna de A = número da linha de B).
	Soma-se o elemento na matriz resposta
	(posição[linha de A][Coluna de B]) com a multiplicação
	dos correspondentes*/
	while (temp_a != NULL)
	{
		temp_b = b->values;

		while(temp_b != NULL)
		{
			if (temp_a->column == temp_b->row)
			{
				//Se o elemento já é não-nulo, soma-se
				if (retrieve_element(temp_a->row, temp_b->column, destiny->values) != NULL)
				{
					retrieve_element(temp_a->row, temp_b->column, destiny->values)->value += temp_a->value * temp_b->value;
				}
				//Cria o elemento para guardar a multiplicação
				else
				{
					destiny->values = insert_ordened(temp_a->value * temp_b->value, temp_a->row, temp_b->column, destiny->values);
				}

			}

			temp_b = temp_b->next;
		}

		temp_a = temp_a->next;

	}

	//Checa se há na lista da matriz resultado algum elemento nulo
	checkFor_nullElements(destiny);


	return destiny;
}

//Soma duas matrizes sparsas
Matrix* sum_sparseMatrix(Matrix* a, Matrix* b)
{
	if (a->height != b->height || a->width != b->width)
	{
		printf("\t***Abortando: *Função sum_sparseMatrix* Soma inválida, matrizes com tamanhos diferentes\n\n");
		exit(1);
	}

	//Cria a matriz resposta
	Matrix* destiny = create_sparseMatrix(a->height, a->width);

	List* temp_a = a->values;
	List* temp_b = b->values;

	//Faz uma comparação elemento-não-nulo a elemento-não-nulo
	while (temp_a != NULL && temp_b != NULL)
	{
		//Se existe dois elementos nas mesmas posições
		if (temp_a->row == temp_b->row && temp_a->column == temp_b->column)
		{
			destiny->values = insert_ordened(temp_a->value + temp_b->value, temp_a->row, temp_a->column, destiny->values);
			temp_a = temp_a->next;
			temp_b = temp_b->next;
		}

		//Se A possui elemento antes(posição) de B, então soma-se elemento de A com 0
		//(pois assume-se que este elemento em B é 0)
		else if (temp_a->row < temp_b->row || (temp_a->row == temp_b->row && temp_a->column < temp_b->column))
		{
			destiny->values = insert_ordened(temp_a->value, temp_a->row, temp_a->column, destiny->values);
			temp_a = temp_a->next;
		}

		//Se B possui elemento antes(posição) de A, então soma-se elemento de B com 0
		//(pois assume-se que este elemento em A é 0)
		else
		{
			destiny->values = insert_ordened(temp_b->value, temp_b->row, temp_b->column, destiny->values);
			temp_b = temp_b->next;

		}

	}

	//Significa que acabaram os elmentos-não-nulos em B
	while(temp_a != NULL)
	{
		destiny->values = insert_ordened(temp_a->value, temp_a->row, temp_a->column, destiny->values);
		temp_a = temp_a->next;
	}

	//Significa que acabaram os elementos-não-nulos em A
	while(temp_b != NULL)
	{
		destiny->values = insert_ordened(temp_b->value, temp_b->row, temp_b->column, destiny->values);
		temp_b = temp_b->next;
	}

	//Checa se há na lista da matriz resultado algum elemento nulo
	checkFor_nullElements(destiny);

	return destiny;
}

//Inicializa uma matrix esparsa inicialmente assumindo todos os valores 0;
Matrix* create_sparseMatrix(long long unsigned rows, long long unsigned columns)
{
	Matrix* matrix = (Matrix*) malloc (sizeof(Matrix));

	matrix->values = create_list();
	matrix->height = rows;
	matrix->width = columns;

	return matrix;

}

//Função que retorna o valor em uma determinada  posição
int pickValue(Matrix* matrix, long long unsigned row, long long unsigned column)
{

	if (row > matrix->height || column >  matrix->width)
	{
		printf("\t***Abortando: *Função pickValue* Tentando acessar valor fora dos limites da matriz\n\n");
		exit(1);
	}


	List* temp = matrix->values;

	while (temp != NULL)
	{
		if (temp->column == column && temp->row == row)
			return temp->value;

		//Os elementos estão ordenados por linha e coluna
		//se achado um elemento maior (que vem depois) na matriz, então ele é zero
		else if (temp->row > row || (temp->row == row && temp->column > column))
			return 0;

		temp=temp->next;
	}

	//se não achou, assume-se que o elemento é zero
	return 0;
}

//Função que checa se algum elemento é nulo
void checkFor_nullElements(Matrix* matrix)
{

	List* temp = matrix->values;
	List* element;

	while (temp != NULL)
	{
		if (temp->value == 0)
		{
			element = temp;
			removeFrom_Position(element->row, element->column, matrix);
		}

		temp = temp->next;
	}

	return;

}

//Função que remove um elemento em uma posição da matriz (Transofrma em zero)
void removeFrom_Position(long long unsigned row, long long unsigned column, Matrix* matrix)
{
	if (row > matrix->height || column >  matrix->width)
	{
		printf("\t***Abortando: *Função removeFrom_Position* Tentando acessar valor fora dos limites da matriz\n\n");
		exit(1);
	}

	List* temp = matrix->values;
	List* prev = NULL;

	//Percorre a lista até chegar na posição
	while (temp != NULL && row != temp->row && column != temp->column)
	{
		prev = temp;
		temp = temp->next;
	}

	//Remoção no início da lista
	if (prev == NULL)
	{
		matrix->values = temp->next;
		free (temp);
	}

	//Remoção no fim da lista
	else if (temp != NULL)
	{
		prev->next = temp->next;
		free(temp);
	}


	return;
}

//Função que insere um elemento em determinada posição
void insertBy_Position(long long unsigned value,long long unsigned row, long long unsigned column, Matrix* matrix)
{
	char answer;

	if (row > matrix->height || column >  matrix->width)
		{
			printf("\t***Abortando: *Função insertBy_Position* Tentando acessar valor fora dos limites da matriz\n\n");
			exit(1);
		}

	/*Caso já exista um elemento nesta posição
	é verificado se o usuário deseja sobrescrever ou
	ignoara a inserção*/
	if (pickValue(matrix, row, column) != 0)
	{
		fflush(stdout);
		printf("Já existe um elemento não nulo na posição [%llu][%llu]\n", row, column);
		printf("Sobrescrever? (s = sim, n = pular inserção)\n\n");

		fflush(stdout);


		scanf("\n%c", &answer);

		//Se for desejado não sobrescrever, ignora-se a inserção
		if (answer == 'n' || answer == 'N')
			return;

		/*Se for desejado sobrescrever, remove-se o elemento existente
		para adicionar o desejado*/
		else
			removeFrom_Position(row, column, matrix);

	}

	//Só insere se o valor a ser inserido é diferente de zero
	if (value != 0)
		matrix->values = insert_ordened(value, row, column, matrix->values);

	return;

}

//Interage com o usuário para criar uma matriz onde este determina os números não nulos
void createInput_sparseMatrix(Matrix* matrix)
{
	long long unsigned elements;
	long long unsigned temp, row, column;
	char answer;

	//Verifica com o usuário a quantidade de elementos não nulos desejado
	do
	{
		printf("Entre com a quantidade de elmentos não entre 0 e %Lu\n\n", matrix->height*matrix->width);
		fflush(stdout);
		scanf("%llu", &elements);
	}
	while (elements < 0 || elements > matrix->height*matrix->width );


	//Verifica com o usuário o valor a ser inserido e sua posição para cada elemento
	for (long long unsigned i = 0; i < elements; i ++)
	{
		fflush(stdout);
		printf("Entre com o %lluº elemento e suas posições\n\n", i +1);
		fflush(stdout);
		scanf("%llu %llu %llu", &temp, &row, &column);

		/*Caso já exista um elemento nesta posição
		é verificado se o usuário deseja sobrescrever ou
		ignoara a inserção*/
		if (pickValue(matrix, row, column) != 0)
		{
			fflush(stdout);
			printf("Já existe um elemento não nulo na posição [%llu][%llu]\n", row, column);
			printf("Sobrescrever? (s = sim, n = pular inserção)\n\n");

			fflush(stdout);

			scanf("\n%c", &answer);

			//Se for desejado não sobrescrever, ignora-se a inserção
			if (answer == 'n' || answer == 'N')
				continue;
			/*Se for desejado sobrescrever, remove-se o elemento existente
			para adicionar o desejado*/
			else
				removeFrom_Position(row, column, matrix);

		}

		if (temp != 0)
			matrix->values = insert_ordened(temp, row, column, matrix->values);
	}

	return;

}

//Gera uma matriz esparsa aleatória de acordo com a quantidade de elementos não nulos que o usuário deseja
void createAleatory_sparsematrix(long long unsigned elements, Matrix* matrix)
{
	if (elements > matrix->height*matrix->width)
	{
		printf("\t***Abortando: Tentando criar uma matriz aleatória com mais elementos do que ela pode ter\n\n");
		exit(1);
	}
	srand(time(NULL));

	long long unsigned row, column, val;
	for (long long unsigned i = 0; i < elements; i ++)
	{
		//Gera aleatoriamente valores até 100, linhas e colunas
		row = rand()%matrix->height;
		column = rand()%matrix->width;
		val = rand()%100;

		while (val == 0)
			val = rand()%100;

		//Checa se não existe elemento não nulo
		if (pickValue(matrix, row, column) == 0)
			matrix->values = insert_ordened(val, row, column, matrix->values);
		//Reinicia o processo de inserção novamente
		else
			i--;
	}

	return;
}

//Mostra a matriz completa
void print_sparseMatrix(Matrix* matrix)
{
	printf("Dimemnsão da matriz %Lu x %Lu\n\n", matrix->height, matrix->width);

	List* temp = matrix->values;

	int i = 0;

	//Percorre todos os elementos da matriz
	while (i < matrix->width * matrix->height)
	{
		//Se o elemento da vez é não nulo, o imprime
		if (temp != NULL && i == temp->row * matrix->width + temp->column)
		{
			//Se o elemento a ser impresso está no início de uma linha
			if (temp->column == 0)
				printf("\n\t%lld ", temp->value);

			else
				printf("\t%lld ", temp->value);

			temp = temp->next;
		}
		else
		{
			//Se o elemento a ser impresso está no início de uma linha
			if (i % matrix->width == 0)
				printf("\n\t0 ");
			else
				printf("\t0 ");
		}

		i++;
	}

	printf("\n\n");

	return;

}

//Mostra no terminal apenas os elementos não nulos e suas posições
void printCompact_sparseMatrix(Matrix* matrix)
{
	printf("Dimemnsão da matriz %Lu x %Lu\n\n", matrix->height, matrix->width);

	if(is_empty_list(matrix->values))
	{
		printf("\tMatriz totalmente nula\n\n");
		return;
	}

	print_list(matrix->values);
	printf("\tO resto dos elementos são nulos\n\n");

	return;

}

//Desaloca a memória alocada inicialmente para a matrize seus elementos não nulos (em lista)
void destroy_sparseMatrix(Matrix* matrix)
{

	free_list(matrix->values);
	free(matrix);
	return;

}





