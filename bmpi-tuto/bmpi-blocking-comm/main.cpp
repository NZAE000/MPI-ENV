#include<boost/mpi.hpp>
#include<string>
#include<iostream>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    uint32_t n_process { static_cast<uint32_t>(world.size()) };
    uint32_t pid       { static_cast<uint32_t>(world.rank()) };
    uint32_t masterpid {0};

    // The first processor (rank 0) passes the message "Hello" to the second processor (rank 1) using tag 0. 
    // The second processor prints the string it receives, along with a comma, then passes the message "world" back to processor 0 with a different tag. 
    // The first processor then writes this message with the "!" and exits. 
    // All sends are accomplished with the communicator::send method and all receives use a corresponding communicator::recv call
    // (Non-blocking): In this expample, the first process passes a message to the second process, then prepares to receive a message. The second process does the send and receive in the opposite order. However, this sequence of events is just that--a sequence--meaning that there is essentially no parallelism. We can use non-blocking communication to ensure that the two messages are transmitted simultaneously
    if (pid == masterpid){
        world.send(1, 0, std::string("Hello"));
        std::string msg{};
        world.recv(1, 1, msg);
        std::cout << msg << "!" << std::endl;
    }
    else {
        std::string msg{};
        world.recv(0, 0, msg);
        std::cout << msg << ", ";
        std::cout.flush();
        world.send(0, 1, std::string("world"));
    }

    return 0;
}