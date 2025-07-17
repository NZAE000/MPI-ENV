#include<iostream>
#include<boost/mpi.hpp>


namespace bmpi = boost::mpi;


int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t masterpid { 0 };

    if (pid == masterpid){

    }
    else {
        
    }

    return 0;
}