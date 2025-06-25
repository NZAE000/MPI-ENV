#include<iostream>
#include<boost/mpi.hpp>
#include<string>

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
    // Any associative binary function or function object will work provided it's stateless. For instance, to concatenate strings with reduce one could use the function object std::plus<std::string>.
    std::string names[6] = {"zero ", "one ", "two ", "three ", "four ", "five "};//, "six ", "seven ", "eight ", "nine "};
    std::string result_concat{};

    //bmpi::reduce(world, pid < 6? names[pid] : std::string("many "), result_concat, std::plus<std::string>(), 0);
    //if (pid == master_pid) std::cout<<"The result is "<<result_concat<<'\n';
    
    if (pid == master_pid){
        bmpi::reduce(world, pid < 6? names[pid] : std::string("many "), result_concat, std::plus<std::string>(), 0); // Master process receives result.
        std::cout<<"The result is "<<result_concat<<'\n';
    }
    else {
        bmpi::reduce(world, pid < 6? names[pid] : std::string("many "), std::plus<std::string>(), 0); // Other process only participate.
    }

    return 0;
}