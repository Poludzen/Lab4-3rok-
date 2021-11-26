#include<iostream>
#include"mpi.h"
#include <cmath>
int main(int argc, char** argv) {
	 int count_of_procs, rank_of_proc;
	 // MPI prog start
	 MPI_Init(&argc, &argv);
     // count or processes
     MPI_Comm_size(MPI_COMM_WORLD, &count_of_procs); 
   	 // current rank
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_proc);
	 MPI_Status status;
	 double curr_pi_value ;
	 int N = count_of_procs;
	 // start sending data, first val
	 if (rank_of_proc == 0) {
		 curr_pi_value = 4;
		 // to proc 1 sending 1 value
		 MPI_Send(&curr_pi_value, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	 }
	 for (int i = 1; i < N - 1; i++) {
		 if (rank_of_proc == i) {
			 // reciving value from previous
			 MPI_Recv(&curr_pi_value, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			 // calculate pi(formula with /(2i+1) cause i starts from 0
			 curr_pi_value += std::pow(-1, i ) / (2 * i + 1)*4;
			 //send to i+1 proc 1 value
			 MPI_Send(&curr_pi_value, 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
		 }
	 }
	 if (rank_of_proc == N - 1) {
		 // reciving value from previous
		 MPI_Recv(&curr_pi_value, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		 curr_pi_value += std::pow(-1, N-1) / (2 * (N-1) + 1) * 4;
		 std::cout << "Pi with N by Leibniz equals to : " << curr_pi_value << "\n";
	 }
     // end mpi programm
	 MPI_Finalize();
}
