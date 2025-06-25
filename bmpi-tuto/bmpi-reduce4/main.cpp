#include<iostream>
#include<boost/mpi.hpp>


struct Sum {
    uint32_t operator()(uint32_t a, uint32_t b){ return a + b; }
};

namespace bmpi = boost::mpi;

/*
    Any kind of binary function objects can be used with reduce. For instance, and there are many such function objects in the C++ standard <functional> header and the Boost.MPI header <boost/mpi/operations.hpp>. 
    Or, you can create your own function object. Function objects used with reduce must be associative, i.e. f(x, f(y, z)) must be equivalent to f(f(x, y), z). 
    If they are also commutative (i..e, f(x, y) == f(y, x)), Boost.MPI can use a more efficient implementation of reduce. To state that a function object is commutative, you will need to specialize the class is_commutative. 
    For instance, we could modify the previous example by telling Boost.MPI that string concatenation is commutative:

*/
namespace boost { 
    namespace mpi {

        // Boost.MPI will assume that sum with two values is commutative and employ a different parallel algorithm for the reduce operation (NOTE: Never declare as commutative an operation that is not really commutative.). 
        template<>
        struct is_commutative<Sum, uint32_t> : mpl::true_ {}; // Here it is safe to mark as commutative and Boost could optimize without risk.

    }
} // namespace boost::mpi

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
    uint32_t names[6] = {1, 2, 3, 4, 5, 6};
    uint32_t result_sum{};

    
    if (pid == master_pid){
        bmpi::reduce(world, pid < 6? names[pid] : 0, result_sum, Sum(), 0); // Master process receives result.
        std::cout<<"The sum is "<<result_sum<<'\n';
    }
    else {
        bmpi::reduce(world, pid < 6? names[pid] : 0, Sum(), 0); // Other process only participate.
    }

    return 0;
}