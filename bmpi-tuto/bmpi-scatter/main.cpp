#include<boost/mpi.hpp>
#include<iostream>
#include<cstdlib>

namespace bmpi = boost::mpi;

int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world {};
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t masterpid  {0};
    uint32_t mine_value {0};

    std::srand(time(nullptr) + pid);
    std::vector<uint32_t> all_random{};

    // The scatter collective scatters the values from a vector in the "root" process in a communicator into values in all the processes of the communicator.
    // The /i/th element in the vector will correspond to the value received by the /i/th process.
    if (pid == masterpid){  
        all_random.resize(n_proc); // Resize is must called, the std::generate algorithm needs 'begin' and 'end' iterators.
        std::generate(begin(all_random), end(all_random), std::rand);
    }
    bmpi::scatter(world, all_random, mine_value, masterpid);
    std::cout<<"Here process #"<<pid<<", their value: "<<mine_value<<'\n';
    
    // world.barrier(); // !

    for (uint32_t i=0; i<n_proc; ++i)
    {
        /*  Todos los procesos ejecutan el for: eso es fundamental en MPI. No hay “estructuras globales” como en programación secuencial: cada proceso ejecuta su propio flujo.
            Una única barrera antes del for solo sincroniza una vez; luego el sistema operativo decide libremente cómo y cuándo ejecuta cada proceso → el std::cout se interlaza (output mezclado).
            Una barrera en cada iteración sincroniza paso a paso, y garantiza que solo el proceso correspondiente a i imprima en esa vuelta.
            El if (i == pid) actúa como un “turno”: permite que solamente un proceso imprima en cada vuelta, mientras los demás esperan.
            Esto no impone orden de ejecución total en MPI (los procesos aún pueden hacer otras tareas en paralelo), pero sí coordina la parte crítica que es la impresión, que en MPI es intrínsecamente no determinista si no se controla.
        */
        world.barrier(); // Wait for all processes within a communicator to reach (alcancen) the barrier.
                        // This routine is a collective operation that blocks each process until all processes have entered it, then releases all of the processes "simultaneously". It is equivalent to MPI_Barrier.
        if (i == pid)
            std::cout<<"Process #"<<pid<<" got "<<mine_value<<'\n';
    }
    
    return 0;
}