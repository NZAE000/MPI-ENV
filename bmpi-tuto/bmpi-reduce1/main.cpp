#include<boost/mpi.hpp>
#include<iostream>
#include<cstdlib>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t master_pid {0};
    
    // boost::mpi::reduce — Combine the values stored by each process into a single value at the root (consolidate data into a single root process).
    // The reduce collective summarizes the values from each process into a single value at the user-specified "root" process. 
    // The Boost.MPI reduce operation is similar in spirit to the STL accumulate operation, because it takes a sequence of values (one per process) and combines them via a function object. 
    // For instance, we can randomly generate values in each process and the compute the minimum value over all processes via a call to reduce.
    std::srand(time(0) + pid );
    uint32_t my_rand { static_cast<uint32_t>(std::rand()) };
    
    if (pid == master_pid)
    {
        uint32_t min{};
        bmpi::reduce(world, my_rand, min, bmpi::minimum<uint32_t>(), master_pid); // Master process receives result.
        std::cout<<"The minmum rand value is "<<min<<'\n';
    }
    else {
        std::cout<<"Hi, i am process #"<<pid<< ", my rand = "<<my_rand<<'\n';                                                                                
        bmpi::reduce(world, my_rand, bmpi::minimum<uint32_t>(), master_pid);      // Other process only participate.
    }
    
    return 0;
}
