#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

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
			int size_mssg = 0;
			MPI_Status status1;
			MPI_Status status2;
			int result1  = MPI_Recv(&size_mssg, 1, MPI_INT, i, any_tag, MPI_COMM_WORLD, &status1);
			char* mssge = (char*) malloc(sizeof(char) * (size_mssg+1));
			int result2  = MPI_Recv(mssge, size_mssg, MPI_CHAR, i, any_tag, MPI_COMM_WORLD, &status2);
			printf("Master receive: %s\n", mssge);
			free(mssge);
		}
	} 
	else
	{
		char pid_str[3];
		sprintf(pid_str, "%d", pid);
		char mssge[25] = "I am the process ";
		strcat(mssge, pid_str);
		int size_mssg = strlen(mssge) + 1;

		int result1 = MPI_Send(&size_mssg, 1, MPI_INT, master_pid, any_tag, MPI_COMM_WORLD);
		int result2 = MPI_Send(mssge, size_mssg, MPI_CHAR, master_pid, any_tag, MPI_COMM_WORLD);
		//if (result1 == result2 == MPI_SUCCESS)
			printf("Process %d send mssge = %s to master\n", pid, mssge);
	}

	MPI_Finalize();
	return 0;
}