#include<boost/mpi.hpp>
#include<boost/serialization/vector.hpp>
#include<iostream>
//#include<vector>

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

    uint32_t getId() const noexcept { return id_; }
    std::string const& getMssge() const noexcept { return mssge_; }

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
}


namespace bmpi = boost::mpi;

int
main(int argc, char** argv)
{
    bmpi::environment  env {argc, argv};
    bmpi::communicator world {};

    uint32_t pid    { static_cast<uint32_t>(world.rank()) };
    uint32_t n_proc { static_cast<uint32_t>(world.size()) };
    uint32_t master_pid{0}, any_tag{20};

    if (pid == master_pid){
        
        for (uint32_t i=1; i<n_proc; ++i)
        {
            std::vector<Message_t> bag_mssg {};
            bag_mssg.reserve(2);
            bag_mssg.emplace_back(i, "Hi");
            bag_mssg.emplace_back(i, "Hi again");
            world.send(i, any_tag, bag_mssg);
        }
        //bag_mssg.clear();

    }
    else {
        std::vector<Message_t> bag_mssg {};
        world.recv(master_pid, any_tag, bag_mssg);

        for (auto const& mssg : bag_mssg){
            std::cout<<"Process "<<pid<< " receive message from master: "<<mssg.getMssge()<<" process "<<mssg.getId()<<'\n';
        }
        std::cout<<std::endl;
    }


    return 0;
}