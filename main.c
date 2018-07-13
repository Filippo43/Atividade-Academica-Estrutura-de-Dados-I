/*
 * main.c
 *
 *  Created on: 20 de jun de 2017
 *      Author: gabri
 */

#include <stdio.h>
#include <stdlib.h>
#include "Sparse.h"

int main()
{

	//Cria uma matriz
	Matrix* matrix = create_sparseMatrix(900,900);

	//Cria uma matriz
	Matrix* othermatrix = create_sparseMatrix(900,900);

	//Gera aleatóriamente os elementos não nulos
	createAleatory_sparsematrix(10, matrix);
	createAleatory_sparsematrix(10, othermatrix);

	//Interage com o usuário para inserir os elementos não nulos
	//createInput_sparseMatrix(othermatrix);

	//Imprime no console os elmentos-não-nulos e suas posições
	printf("Matriz A: ");
	printCompact_sparseMatrix(matrix);
	//print_sparseMatrix(matrix);

	//Imprime no console os elmentos-não-nulos e suas posições
	printf("Matriz B: ");
	printCompact_sparseMatrix(othermatrix);
	//print_sparseMatrix(othermatrix);


	//Realiza a soma de duas matrizes esparsas
	Matrix* sum = sum_sparseMatrix(matrix, othermatrix);

	//Realiza a multiplicação de duas matrizes esparsas
	Matrix* mult = mult_sparseMatrix(matrix, othermatrix);


	//Imprime no console os elmentos-não-nulos e suas posições
	printf("A + B: ");
	printCompact_sparseMatrix(sum);
	//print_sparseMatrix(sum);

	//Imprime no console os elmentos-não-nulos e suas posições
	printf("A * B: ");
	printCompact_sparseMatrix(mult);
	//print_sparseMatrix(mult);


	//Libera a memória alocada
	destroy_sparseMatrix(matrix);
	destroy_sparseMatrix(othermatrix);
	destroy_sparseMatrix(sum);
	destroy_sparseMatrix(mult);

}
