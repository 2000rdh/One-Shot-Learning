#include<stdlib.h>
#include<stdio.h>
#include "learn.h"

int main (int argc, char ** argv) {
	FILE * fp = NULL;
	char * trainingdata = NULL; //training data
	char * testingdata = NULL; //testing data
	int trainingsets; //number of rows of the matrix represented by letter N
  int attributes; //number of columns of the matrix represented by letter K
	int numoftests;
	double ** matrixX; //Matrix x represents the file containing training data. It is a Nx(K+1) matrix.
	double ** matrixXTranspose; //the transpose of matrix x
	double temp;
	int insertcounter = 0;
	double ** matrixY; //Matrix Y will respresent the values of the house of the last column of the training data
  double ** matrixXTX; //result of a matrix matrixmultiplication of X transpose and X
	double ** inversematrix; //inversematrix using Guassian elimination
	double ** inverseXT; //the inverse of matrix XTX multiplied by XT
	double ** weightmatrix; //the weight matrix that represents the training data
	double value = 0;
	int i = 0, j = 0;

	if (argc < 3) {
		printf("error\n");
		exit(0);
	}

	trainingdata = argv[1];
	testingdata = argv[2];

	fp = fopen(trainingdata, "r");

	if (fp == NULL) {
		printf("0\n");
		exit(0);
	}

  fscanf(fp, "%d", &attributes); //scaning in the number of attributes
	fscanf(fp, "%d", &trainingsets); //scaning in the number of sets to train the data with

  matrixX = (double**)malloc(sizeof(double*) * (trainingsets)); //allocating space for matrixX
	matrixXTranspose = (double**)malloc(sizeof(double*) * (attributes+1)); //allocating space for the transpose of matrixX
	matrixXTX = (double**)malloc(sizeof(double*) * (attributes+1)); //allocating space for the multiplication of a X and its transpose
	inversematrix = (double**)malloc(sizeof(double*) * (attributes+1)); //allocating space for the inversematrix
	matrixY = (double**)malloc(sizeof(double*) * (trainingsets)); //allocating space for the vector Y
	inverseXT = (double**) malloc(sizeof(double*) * (attributes+1));
	weightmatrix = (double**) malloc(sizeof(double*) * (attributes+1));

	for (i = 0; i < trainingsets; i++) {
    matrixX[i] = malloc((attributes+1)*sizeof(double));
		matrixY[i] = malloc(sizeof(double));
  }

	for (i = 0; i < attributes+1; i++) {
		matrixXTranspose[i] = malloc((trainingsets) *sizeof(double));
		matrixXTX[i] = malloc((attributes+1) * sizeof(double));
		inversematrix[i] = malloc((attributes+1) * sizeof(double));
		inverseXT[i] = malloc((trainingsets) * sizeof(double));
		weightmatrix[i] = malloc(sizeof(double));
	}

	//this opens the file and inserts all the elements in their respective matricies
	while (!feof(fp)) {
		for (i = 0; i < trainingsets; i++) {
			insertcounter = 0;
			for (j = 0; j < attributes+2; j++) {
				if (j == 0) {
					matrixX[i][j] = 1;
				}
				else {
					value = fscanf(fp, "%lf,", &temp);
					if (value == -1) {
						break;
					}
					if (insertcounter == attributes) {
						matrixY[i][0] = temp;
						break;
					}
					matrixX[i][j] = temp;
					insertcounter++;
				}
			}
		}
	}

	for (int i = 0; i < attributes+1; i++) {
		for (int j = 0; j < attributes+1; j++) {
			if (i == j) {
				inversematrix[i][j] = 1;
			}
			if (i != j) {
				inversematrix[i][j] = 0;
			}
		}
	}

	fclose(fp);

	//print out matrixX
	//printf("\nmatrixX\n");
	//matrixprint(matrixX, trainingsets, attributes+1);

	//print out matrixY
	//printf("\nmatrixY\n");
	//matrixprint(matrixY, trainingsets, 1);

	//print out the transpose of matrixX
	//printf("\nmatrixXTranspose\n");
	matrixXTranspose = matrixTranspose(matrixX, matrixXTranspose, attributes, trainingsets);
	//matrixprint(matrixXTranspose, attributes+1, trainingsets);

	matrixXTX = matrixMultiplication(matrixXTranspose, matrixX, matrixXTX, attributes+1, trainingsets, trainingsets, attributes+1); //this is the result of multiplication of X and its transpose

	//printf("\nmatrixXTX\n");
	//matrixprint(matrixXTX, attributes+1, attributes+1);

	//print out a standard identity matrix
	//printf("\nIdentity Matrix\n");
	//matrixprint(inversematrix, attributes+1, attributes+1);

	//printf("\nInverse Matrix\n");
	inversematrix = matrixInverseRREF(matrixXTX, inversematrix, attributes+1);
	//matrixprint(inversematrix, attributes+1, attributes+1);

	//printf("\nInverse Matrix XT\n");
	inverseXT = matrixMultiplication(inversematrix, matrixXTranspose, inverseXT, attributes + 1, attributes + 1, attributes + 1, trainingsets);
	//matrixprint(inverseXT, attributes+1, trainingsets);

	//printf("\nWeight Matrix\n");
	weightmatrix = matrixMultiplication(inverseXT, matrixY, weightmatrix, attributes+1, trainingsets, trainingsets, 1);
	//matrixprint(weightmatrix, attributes+1, 1);

	fp = fopen(testingdata, "r");

	fscanf(fp, "%d", &numoftests);
	value = 0;
	double test = 0;
	printf("\nPredicted House Prices are\n");
	while (!feof(fp)) {
		for (i = 0; i < numoftests; i++) {
			value = weightmatrix[0][0];
			for (j = 1; j < attributes + 1; j++) {
				test = fscanf(fp, "%lf,", &temp);
				value = value + (weightmatrix[j][0]*temp);
			}
			if (test == 1) {
				printf("%0.0lf\n", value);
			}
		}
	}

	fclose(fp);

	for (i = 0; i < trainingsets; i++) {
    free(matrixX[i]);
		free(matrixY[i]);
  }

	for (i = 0; i < attributes+1; i++) {
		free(matrixXTranspose[i]);
		free(matrixXTX[i]);
		free(inversematrix[i]);
		free(inverseXT[i]);
		free(weightmatrix[i]);
	}

  free(matrixX);
  free(matrixY);
  free(matrixXTranspose);
	free(matrixXTX);
	free(inversematrix);
	free(inverseXT);
	free(weightmatrix);
  return 0;
}

double ** matrixMultiplication(double ** matrix1, double ** matrix2, double ** result, int matrix1r, int matrix1c, int matrix2r, int matrix2c) {
	int c1 = 0; //incrementer for columns of matrix 1
	int r2 = 0; //incrementer for rows of matrix 2
	int rr = 0; //incrementer for rows of the resulting matrix
	int rc = 0; //incrementer for columns of the resulting matrix
	double addition = 0; //adds the rsult of the previous multiplication to the current multiplication
	//int resultdimensions = matrix1r; //the dimensions of the the resulting matrix is the # of attributes+1 and it always becomes a square matrix
	for (rr = 0; rr < matrix1r; rr++) {
		for (rc = 0; rc < matrix2c; rc++) {
			addition = 0; c1 = 0;
			for (r2 = 0; r2 < matrix2r; r2++) {
				addition = addition + (matrix1[rr][c1] * matrix2[r2][rc]);
				c1++;
			}
			result[rr][rc] = addition;
		}
	}
	return result;
}

double ** matrixTranspose(double ** matrixX, double ** matrixXTranspose, int attributes, int trainingsets) {
	int i = 0, j = 0;
	for (i = 0; i < attributes+1; i++) {
		for (j = 0; j < trainingsets; j++) {
			matrixXTranspose[i][j] = matrixX[j][i];
		}
	}
	return matrixXTranspose;
}

double ** matrixInverseRREF(double ** XTX, double ** inverse, int attributes) {
	int counter = 0;
	int j = 0;
	int i = 0;
	int k = 0;
	int r = 0;
	int c = 0;
	int l = 0;
	double makepivot1 = 0;
	double temp = 0;
	for (i = 0; i < attributes; i++) {
		counter = 0; r = 0;
		while (counter < attributes) {
			//printf("Entering while loop with counter = %d\n", counter);
			c = j;
			if (XTX[i][j] == 1) {
				//printf("\nrow i = %d column j =  %d row r = %d col c = %d\n", i, j, r, c);
				if (XTX[r][c] != 0 && r != i) {
					temp = XTX[r][c];
					//printf("R%d = R%d - %f*R%d\n", r+1, r+1, temp, i+1);
					for (k = 0; k < attributes; k++) {
						//printf("Entering for loop to do row operation with temp = %lf with l = %d\n", temp, l);
						XTX[r][k] = XTX[r][k] - (temp*XTX[i][l]);
						inverse[r][k] = inverse[r][k] - (temp*inverse[i][l]);
						l++;
					}
					l = 0;
				}
				//printf("\nInverse\n");
				//matrixprint(inverse, attributes, attributes);
				//printf("\ntestmatrix\n");
				//matrixprint(XTX, attributes, attributes);
				r++;
				counter++;
			}
			else if (XTX[i][j] != 1) { // start off my making the pivot a 1
				//printf("Entering makepivot1 with i = %d and j = %d\n", i, j);
				makepivot1 = XTX[i][j];
				//printf("R%d = R%d/%f\n", i+1, i+1, makepivot1);
				for (k = 0; k < attributes; k++) {
					if (XTX[i][k] == 0) {
						inverse[i][k] = inverse[i][k]/makepivot1;
						continue;
					}
					XTX[i][k] = XTX[i][k]/makepivot1;
					inverse[i][k] = inverse[i][k]/makepivot1;
				}
				//printf("\nInverse\n");
				//matrixprint(inverse, attributes, attributes);
				//printf("\ntestmatrix\n");
				//matrixprint(XTX, attributes, attributes);
			}
		}
		j++;
	}
	return inverse;
}

void matrixprint(double** matrix, int rows, int cols) {
	int i = 0, j = 0;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			printf("%f\t", matrix[i][j]);
		}
		printf("\n");
	}
}
