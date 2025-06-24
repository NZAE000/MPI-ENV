#include<boost/mpi.hpp>
#include<iostream>
#include<string>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment  env{argc, argv};
    bmpi::communicator world{};

    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t master_pid{0}, any_tag {10};
    
    if (pid == master_pid){
        for (int i=1; i<n_proc; ++i)
        {
            std::string mssge { "I am master, I want to contact with pid " + std::to_string(i) };
            world.send(i, any_tag, mssge);
        }

    }
    else {
        std::string mssge {};
        world.recv(master_pid, any_tag, mssge);
        std::cout <<"Process "<<pid<<" received message: "<<mssge<<'\n';
    }

    return 0;
}