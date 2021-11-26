#include<cmath>
#include<iostream>
#include"mpi.h"
#include <cmath>
// function in integral
double f(double x) {
	return x*x;
}
// calculate integral by trapezoid method on 2 points
double calculate_trapezoid_on_point(double a, double b) {
	return (f(a) + f(b)) * (b - a) / 2;
}
// calculate integral by rectangle method on 2 points

double calculate_rectangles_on_point(double a, double b) {
	return f((a + b) / 2)*(b - a);
}

int main(int argc, char** argv) {
	int count_of_procs, rank_of_proc;
	// MPI programm start
	MPI_Init(&argc, &argv);
	// count of processes
	MPI_Comm_size(MPI_COMM_WORLD, &count_of_procs);
	// rank of current process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_proc);
	MPI_Status status;
	// array of 2 integral values
	double* curr_integral_value_both = new double[2];
	// count of points
	int N = count_of_procs;
	// interval
	int a = rand() % 16 - 8;
	int b = rand() % 8 + a + 1;
	// [a,b] on n points, so a and b also 2 points
	double* points = new double[N];
	double h = double((b - a)) / (N - 1);
	for (int i = 0; i < N; i++) {
		points[i] = a + i * h;
	}

	// we need every procces to know about points
	MPI_Bcast(points, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// start sending data, first val
	if (rank_of_proc == N-1) {
		
		curr_integral_value_both[0] = calculate_trapezoid_on_point(points[N-2],points[N-1]);
		curr_integral_value_both[1] = calculate_rectangles_on_point(points[N - 2], points[N - 1]);
		// to proc N-2 sending 2 values
		MPI_Send(curr_integral_value_both, 2, MPI_DOUBLE, N-2, 0, MPI_COMM_WORLD);
	}
	for (int i = N-2; i >=1; i--) {
		if (rank_of_proc == i) {
			// recive values
			MPI_Recv(curr_integral_value_both, 2, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			// calculate intergral on this points by both methods

			curr_integral_value_both[0] += calculate_trapezoid_on_point(points[i - 1], points[i ]);
			curr_integral_value_both[1] += calculate_rectangles_on_point(points[i - 1], points[i ]);
			//send to i-1 proc
			MPI_Send(curr_integral_value_both, 2, MPI_DOUBLE, i - 1, 0, MPI_COMM_WORLD);
		}
	}
	if (rank_of_proc == 0) {
		// recive values 
		MPI_Recv(curr_integral_value_both, 2, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		std::cout << "X^2 integral on a:"<< a<<" to b:"<<b <<"\nby trapezoid  equals to : " << curr_integral_value_both[0] << "\n";
		std::cout << "\nby rectanlges  equals to : " << curr_integral_value_both[1] << "\n";
	}
	// end mpi programm
	MPI_Finalize();
}
