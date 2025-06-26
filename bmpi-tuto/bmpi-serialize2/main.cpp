#include<boost/mpi.hpp>
#include<boost/archive/text_oarchive.hpp>
#include<iostream>

namespace bmpi = boost::mpi;

struct Message_t {

    explicit Message_t() = default;

    explicit Message_t(uint32_t id, std::string mssge)
    : id_{id}, mssge_{mssge} {}

    //friend class boost::serialization::access; // Serialization needs acces to private data.
//
    //template<typename Archive>
    //void serialize(Archive& ar, const unsigned int version) // Use boost serialization templates.
    //{
    //    ar& id_; ar& mssge_;
    //}

//private:
    uint32_t id_{};
    std::string mssge_{};
};


// Non intrusive.
// NOTE: The main application of non-intrusive serialization is to permit serialization to be implemented for classes without changing the class definition. 
// In order for this to be possible, the class must expose enough information to reconstruct the class state. In this example, we presumed that the class had public members - not a common occurrence.
// Only classes which expose enough information to save and restore the class state will be serializable without changing the class definition.
namespace boost {
    namespace serialization {

    template<typename Archive>
    void serialize(Archive& ar, Message_t& mssg, const unsigned int version){
        ar& mssg.id_; ar& mssg.mssge_;
    }

    }
} // namespace boost::serialization


int 
main(int argc, char** argv)
{
    bmpi::environment env{argc, argv};
    bmpi::communicator world{};

    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t master_pid{0}, any_tag{20};

    if (pid == master_pid){
        for (uint32_t i=1; i<n_proc; ++i){
            world.send(i, any_tag, Message_t{i, "Message from master"});
        }
    }
    else{
        Message_t mssge{};
        world.recv(master_pid, any_tag, mssge);
        //std::ostream os;
        boost::archive::text_oarchive oa{std::cout};  // Use the term "archive" to refer to a specific rendering of this stream of bytes. This could be a file of binary data, text data, XML, or some other created by the user of this library.
        oa<<"Process "<<pid<<" received message: "<<mssge<<'\n';

        /*
        ouput f.eg: 22 serialization::archive 20 9 80 114 111 99 101 115 115 32 0 3 20 32 114 101 99 101 105 118 101 100 32 109 101 115 115 97 103 101 58 32 0 0 0 3 19 Message from master 10
                    22: lenght of serialization::archive
                    "serialization::archive": identificador de formato usado por Boost
                    20: versión del archivo o del archivo de texto Boost (es parte del protocolo de Boost)
                    9: lenght of "Process "
                    80  → P
                    114 → r
                    111 → o
                    99  → c
                    101 → e
                    115 → s
                    115 → s
                    32  → espacio ' '
                    0 3: pid del proceso que recibio el mensaje del master
                    20: longitud de la cadena
                    114 → r
                    ...
                    58  → :
                    32  → espacio
                    0 0 0 3: contenido de id_ del objeto mensaje (pid del proceso)
                    19: longitud de la cadena.
                    "Message from master": contenido de mssge_ del objeto mensaje
                    10 → salto de línea (\n en ASCII).
                    
        */
    }
    

    return 0;
}