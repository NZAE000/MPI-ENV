#include<boost/mpi.hpp>
#include<iostream>
#include<cstdlib>

namespace bmpi = boost::mpi;


int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world {};

    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t master_pid {0};

    std::srand(time(0) + pid);

    uint32_t my_rand {static_cast<uint32_t>(std::rand()) };
    //uint32_t min_rand {};

    //std::cout<<"I am process #"<<pid<< " and my rand is "<<my_rand<<"\n";
    //world.barrier(); // Yes, it synchronizes, but it does not guarantee the order of printing to the console (the out-of-order output is due to the asynchronous behavior of std::cout between processes).
    
    // For an orderly exit: use a coordinated printing loop with barriers.
    for (uint32_t i=0; i<n_proc; ++i) {
        world.barrier(); // Wait all process.
        if (pid == i) 
            std::cout<<"I am process #"<<pid<<" and my rand is "<<my_rand<<'\n';
            
    } world.barrier();

    // boost::mpi::all_reduce — Combine the values stored by each process into a single value available to all processes.
    // If there is no need to preserve the input value, the output value can be omitted. 
    // In that case the input value will be overridden with the output value and Boost.MPI is able, in some situation, 
    // to implement the operation with a more space efficient solution (using the MPI_IN_PLACE flag of the MPI C mapping).
    bmpi::all_reduce(world, my_rand, bmpi::minimum<uint32_t>());

    //if (pid == master_pid) std::cout<<std::endl;
    std::cout<<"I am process #"<<pid<< " and min rand is "<<my_rand<<'\n';

    return 0;

}