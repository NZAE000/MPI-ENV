#include<iostream>
#include<boost/mpi.hpp>
#include<cstdlib>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    // The gather collective gathers the values produced by every process in a communicator into a vector of values on the "root" process (specified by an argument to gather). 
    // The /i/th element in the vector will correspond to the value gathered from the /i/th process.
    // The gather operation collects values from every process into a vector at one process. 
    // If instead the values from every process need to be collected into identical vectors on every process, use the all_gather algorithm, which is semantically equivalent to calling gather followed by a broadcast of the resulting vector.
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t master_pid{0};
    
    std::srand(time(0) + pid);
    uint32_t my_random { static_cast<uint32_t>(std::rand()) };

    if (pid == master_pid)
    {
        std::vector<uint32_t> all_random{};
        all_random.reserve(n_proc);
        bmpi::gather(world, my_random, all_random, master_pid); // (Also receive the master random).
        
        std::cout<<"Process #"<<master_pid<< " recolected: \n"; 
        for (uint32_t i=0; i<n_proc; ++i){
            std::cout<<"\tProcess #"<<i<<" thought of "<<all_random[i]<<'\n';
        }
    }
    else {
        std::cout<<"I am process #"<<pid<<" with rand value = "<<my_random<<'\n';
        bmpi::gather(world, my_random, master_pid);
    }

    return 0;
}