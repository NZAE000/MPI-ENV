#include<boost/mpi.hpp>
#include<iostream>

namespace bmpi = boost::mpi;

int
main(int argc, char** argv)
{

    uint32_t pid{0}, n_proc{0};
    bmpi::environment  env   {argc, argv};
    bmpi::communicator world {};

    pid    = world.rank();
    n_proc = world.size();
    
    std::cout<<"Process "<< pid << '/' <<n_proc << '\n';

    return 0;
}