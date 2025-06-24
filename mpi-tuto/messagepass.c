#include <mpi.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
	int pid = 0, n_proc = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &n_proc);	
	int master_pid = 0, any_tag = 10;

	if (pid == master_pid) {
		int mssge = 0;
		for (int i=1; i<n_proc; ++i)
		{
			mssge += i;
			int result = MPI_Send(&mssge, 1, MPI_INT, i, any_tag, MPI_COMM_WORLD);
			if (result == MPI_SUCCESS)
				printf("Master send mssge = %d to process %d\n", mssge, i);
				//std::cout << "Rank 0 OK!" << std::endl;
		}
	} 
	else
	{
		int mssge = 0;
		MPI_Status status;
		int result = MPI_Recv(&mssge, 1, MPI_INT, master_pid, any_tag, MPI_COMM_WORLD, &status);
		if (result == MPI_SUCCESS){
			printf("Process %d received mssge = %d\n", pid, mssge);
			//std::cout << "Rank 1 OK!" << std::endl;
		}
	}

	MPI_Finalize();
	return 0;
}