#include<boost/mpi.hpp>
#include<iostream>


namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t masterpid {0};
    std::string mssg{};

    // The broadcast algorithm is by far the simplest collective operation. It broadcasts a value from a single process to all other processes within a communicator.
    if (pid == masterpid){
        mssg = "Hello, world!";
    }
    bmpi::broadcast(world, mssg, 0);

    std::cout<<"Process #"<<pid<<" says "<<mssg<<'\n';

    return 0;
}