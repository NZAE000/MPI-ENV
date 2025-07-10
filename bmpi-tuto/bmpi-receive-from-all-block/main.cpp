#include<boost/mpi.hpp>
#include<iostream>
#include<string>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env {argc, argv};
    bmpi::communicator world{};

    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t master_pid{0}, any_tag{10};

    if (pid == master_pid){
        std::string mssge{};
        
        // Receive each message (pid) from other process (blocking).
        for (uint32_t i=1; i<n_proc; ++i){
            world.recv(bmpi::any_source /*i*/, any_tag, mssge);
            std::cout<<"Master receive: "<< mssge <<'\n';
        }

    }else {
        std::string mssge { "I am the process " +  std::to_string(pid) };
        world.send(master_pid, any_tag, mssge); // Send message to master.
    }

    return 0;
}
