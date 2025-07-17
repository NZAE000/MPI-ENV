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
        std::string mssge_recv {};
        for (int i=1; i<n_proc; ++i)
        {
            std::string mssge { "Master want to contact with pid " + std::to_string(i) };
            world.send(i, any_tag, mssge);
            world.recv(i, any_tag, mssge_recv);
            std::cout<< "and master receive: "<<mssge_recv<<'\n';
        }
    }
    else {
        std::string mssge {};
        world.recv(master_pid, any_tag, mssge);
        std::cout <<"Process "<<pid<<" received message: "<<mssge<<'\n';
        std::string mssge_send { "Process " + std::to_string(pid) + " said thank!" };
        world.send(master_pid, any_tag, mssge_send);
    }

    return 0;
}