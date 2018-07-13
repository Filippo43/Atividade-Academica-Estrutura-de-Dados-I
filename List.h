/*
 * list.h
 *
 *  Created on: 20 de jun de 2017
 *      Author: gabri
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>

//******Estrutura*******
//Adaptação de Lista Encadeada para matrizes esparsas
struct list
{
	long long int value;
	long long unsigned row, column;
	struct list* next;
};

typedef struct list List;

//******Protótipos*******
List* retrieve_element(long long unsigned row, long long unsigned column, List* list);
List* create_list();
List* insert_ordened(long long int value, long long unsigned row, long long unsigned column, List* list);
List* remove_from_list(long long int value, List* list);
void print_list(List* list);
void free_list(List* list);
int is_empty_list(List* list);
int search_list(long long int value, List* list);


//******Funções*******
//Retorna o ponteiro para o elemento
List* retrieve_element(long long unsigned row, long long unsigned column, List* list)
{
	List* temp = list;

	while (temp != NULL)
	{
		if (temp->column == column && temp->row == row)
			return temp;

		/*Os elementos estão ordenados por linha e coluna
		se achado um elemento maior (que vem depois) na matriz,
		então ele é zero pois nã está listado*/
		else if (temp->row > row || (temp->row == row && temp->column > column))
			return NULL;

		temp = temp->next;
	}

	//Se não achou
	return NULL;
}
//Cria uma lista
List* create_list()
{
	return NULL;
}

//Insere ordenado na lista
List* insert_ordened(long long int val, long long unsigned row, long long unsigned column, List* list)
{

	List* element;

	element = (List*) malloc (sizeof (List));
	element->value = val;
	element->row = row;
	element->column = column;

	List* temp = list;
	List* prev = NULL;

	/*Procura pelo primeiro elemento que vem após o de inserção,
	 (linha maior, ou mesma linha e coluna maior) para inserí-lo antes*/
	while (temp != NULL && (temp->row < row || (temp->row == row && temp->column < column)))
	{
		prev = temp;
		temp = temp->next;
	}

	//O elemento a ser inserido é anterior a todos
	if (prev == NULL)
	{
		element->next = list;
		list = element;
	}

	//O elemento não deve ser inserido no início
	else
	{
		element->next = prev->next;
		prev->next = element;

	}

	return list;
}

//Imprime a Lista
void print_list (List* list)
{
	if (is_empty_list(list))
	{
		printf("Lista vazia\n");
		return;
	}

	List* temp;

	printf("Imprimindo Valores...\n\n");

	for (temp = list; temp != NULL; temp = temp->next)
	{
		printf("\t%lld[%lld][%lld]\n", temp->value, temp->row, temp->column);
	}

	return;

}

//Desaloca a memória utilizada para a lista
void free_list(List* list)
{
	List* temp = list;

	while (temp != NULL)
	{
		List* p = temp->next;
		free(temp);
		temp = p;
	}


	return;
}

//Booleana para lista vazia
int is_empty_list(List* list)
{

	return (list == NULL);
}

//Procura por valor na lista
int search_list(long long int val, List* list)
{
	List* temp;

	for (temp = list; temp != NULL; temp = temp->next)
	{
		if (val == temp->value)
		{
			return 1;
		}
	}

	return 0;
}

//Retira um elemento na lista pelo valor
List* remove_from_list(long long int val, List* list)
{
	if (is_empty_list(list))
		return list;

	List* temp = list;
	List* prev = NULL;

	//percorre a lista até chegar ao elemento
	while (temp != NULL && val != temp->value)
	{
		prev = temp;
		temp = temp->next;
	}

	//O elemento é o primeiro
	if (prev == NULL)
	{
		list = temp->next;
		free (temp);
	}

	else if (temp != NULL)
	{
		prev->next = temp->next;
		free(temp);
	}


	return list;
}





#endif /* LIST_H_ */
