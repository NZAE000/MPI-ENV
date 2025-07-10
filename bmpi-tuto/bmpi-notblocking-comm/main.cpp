#include<boost/mpi.hpp>
#include<iostream>


namespace bmpi = boost::mpi;


int 
main(int argc, char** argv)
{
    constexpr uint32_t N_REQS {2};    

    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    uint32_t n_proc     { static_cast<uint32_t>(world.size()) };
    uint32_t pid        { static_cast<uint32_t>(world.rank()) };
    uint32_t masterpid  {0};

    // We have replaced calls to the communicator::send and communicator::recv members with similar calls to their non-blocking counterparts, 
    // communicator::isend and communicator::irecv. The prefix i indicates that the operations return immediately with a mpi::request object, 
    // which allows one to query the status of a communication request (see the test method) or wait until it has completed (see the wait method). 
    // Multiple requests can be completed at the same time with the wait_all operation.
    // (See tags between both process of send-recv respectively).

    // IMPORTANT: Regarding communication completion/progress: The MPI standard requires users to keep the request handle for a non-blocking communication, 
    // and to call the "wait" operation (or successfully test for completion) to complete the send or receive. Unlike most C MPI implementations, which allow
    // the user to discard the request for a non-blocking send, Boost.MPI requires the user to call "wait" or "test", since the request object might contain 
    // temporary buffers that have to be kept until the send is completed. Moreover, the MPI standard does not guarantee that the receive makes any progress 
    // before a call to "wait" or "test", although most implementations of the C MPI do allow receives to progress before the call to "wait" or "test". Boost.MPI,
    // on the other hand, generally requires "test" or "wait" calls to make progress. More specifically, Boost.MPI guarantee that calling "test" multiple time will 
    // eventually complete the communication (this is due to the fact that serialized communication are potentially a multi step operation.).
    if (pid == masterpid)
    {
        bmpi::request reqs[N_REQS];
        std::string mssg, out_mssg = "Hello";
        reqs[0] = world.isend(1, 0, out_mssg);  // Send to process 1 with tag 0.
        reqs[1] = world.irecv(1, 1, mssg);      // Receive from process 1 with tag 1.
        bmpi::wait_all(reqs, reqs + N_REQS);
        std::cout<< mssg << "!\n";
    }
    else {
        bmpi::request reqs[N_REQS];
        std::string mssg, out_mssg = "world";
        reqs[0] = world.isend(0, 1, out_mssg);  // Send to process 0 with tag 1.
        reqs[1] = world.irecv(0, 0, mssg);      // Receive from process 0 with tag 0.
        bmpi::wait_all(reqs, reqs + N_REQS);
        std::cout<< mssg << ", ";
    }
    
    return 0;
}
