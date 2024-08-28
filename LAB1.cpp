#include <iostream>
#include "mpi.h"
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int const L = 10;

	int n1 = L / size;
	int r = L % size;

	float a[L], b[L], c[L];
	
	int local_n = rank < r ? n1 + 1 : n1;

	float* a1 = new float[local_n];
	float* b1 = new float[local_n];
	float* c1 = new float[local_n];

	int* sendcounts = new int[size];
	int* displs = new int[size];

	if (rank == 0) {
		for (int i = 0; i < L; i++) {
			a[i] = rand() % 3 - 1;;
			b[i] = rand() % 3 - 1;
		}
	}

	int sum = 0;
	for (int i = 0; i < size; ++i) {
		sendcounts[i] = i < r ? n1 + 1 : n1;
		displs[i] = sum;
		sum += sendcounts[i];
	}

	MPI_Scatterv(&a[0], sendcounts, displs, MPI_FLOAT,
		a1, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(&b[0], sendcounts, displs, MPI_FLOAT,
		b1, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < local_n; i++) {
		c1[i] = a1[i] * b1[i];
	}

	MPI_Gatherv(c1, local_n, MPI_FLOAT,
		c, sendcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "       a[i]  b[i]  c[i]" << endl;
		float sum = 0;
		for (int i = 0; i < L; i++) {
			cout << "i = " << i << "     " << a[i] << "  " << b[i]<< "  " << c[i] << endl;
			sum += c[i];
		}
		cout << sum << endl;
	}

	MPI_Finalize();

	return 0;
}