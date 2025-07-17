#include <boost/mpi.hpp>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "../util/timelog.hpp"

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env {argc, argv};
    bmpi::communicator world{};

    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t master_pid{0}, any_tag{10};

    // Create a single log file and time logger per process.
    std::ofstream log_file("bmpi-receive-from-all-block/log_" + std::to_string(pid) + ".txt");
    TimeLogger tlog{};

    if (pid == master_pid){
        std::string mssge_recv{};
        std::string mssge_send{};
        
        for (uint32_t i=1; i<n_proc; ++i) {
            auto status = world.recv(i /*bmpi::any_source*/, any_tag, mssge_recv);
            log_file << "[" << tlog.get_timestamp() << "] " << "Master received from process " << status.source() << ": " << mssge_recv << '\n';

            mssge_send = "Thank! process " + std::to_string(status.source());
            world.send(status.source(), any_tag, mssge_send);

            log_file << "[" << tlog.get_timestamp() << "] " << "Master sent to process " << status.source() << ": " << mssge_send << '\n';
        }
    } else {
        std::string mssge_send { "I am the process " + std::to_string(pid) };
        world.send(master_pid, any_tag, mssge_send);
        log_file << "[" << tlog.get_timestamp() << "] " << "Process " << pid << " sent to master: " << mssge_send << '\n';

        std::string mssge_recv{};
        world.recv(master_pid, any_tag, mssge_recv);
        log_file << "[" << tlog.get_timestamp() << "] " << "Process " << pid << " received from master: " << mssge_recv << '\n';
    }

    log_file.close();
    return 0;
}


//#include<boost/mpi.hpp>
//#include<iostream>
//#include<string>
//
//namespace bmpi = boost::mpi;
//
//int 
//main(int argc, char** argv)
//{
//    bmpi::environment env {argc, argv};
//    bmpi::communicator world{};
//
//    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
//    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
//    uint32_t master_pid{0}, any_tag{10};
//
//    if (pid == master_pid){
//        std::string mssge_recv{};
//        std::string mssge_send{};
//        
//        // Receive each message (pid) from other process (blocking).
//        for (uint32_t i=1; i<n_proc; ++i){
//            auto status = world.recv(/*bmpi::any_source*/ i, any_tag, mssge_recv);
//            std::cout<<"Master receive: "<< mssge_recv <<'\n';
//            mssge_send = "Thank! process " + std::to_string(status.source());
//            world.send(status.source(), any_tag, mssge_send);
//        }
//    }
//    else {
//        std::string mssge_send { "I am the process " +  std::to_string(pid) };
//        world.send(master_pid, any_tag, mssge_send); // Send message to master.
//        std::string mssge_recv{};
//        world.recv(master_pid, any_tag, mssge_recv);
//        std::cout<<"and master said: "<<mssge_recv<<'\n';
//    }
//
//    return 0;
//}
