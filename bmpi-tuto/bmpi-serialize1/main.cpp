#include <fstream>

// Include headers that implement a archive in simple text format.
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


/*
In general, built-in C++ types (ints, floats, characters, etc.) can be transmitted over MPI directly, while user-defined and library-defined types will need to first be serialized (packed) into a format that is amenable to transmission. 
Boost.MPI relies on the Boost.Serialization library to serialize and deserialize data types.

For types defined by the standard library (such as std::string or std::vector) and some types in Boost (such as boost::variant), the Boost.Serialization library already contains all of the required serialization code. 
In these cases, you need only include the appropriate header from the boost/serialization directory.

For types that do not already have a serialization header, you will first need to implement serialization code before the types can be transmitted using Boost.MPI. Consider a simple class gps_position that contains members degrees, minutes, and seconds. 
This class is made serializable by making it a friend of boost::serialization::access and introducing the templated serialize() function, as follows.
*/

/////////////////////////////////////////////////////////////
// gps coordinate
//
// Illustrates serialization for a simple type.
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }
    int degrees;
    int minutes;
    float seconds;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};

int main() {
    // Create and open a character archive for output
    std::ofstream ofs("filename");

    // Create class instance
    const gps_position g(35, 59, 24.567f);

    // Save data to archive
    {
        // Use the term "archive" to refer to a specific rendering of this stream of bytes. This could be a file of binary data, text data, XML, or some other created by the user of this library.
        boost::archive::text_oarchive oa(ofs);
        // Write class instance to archive
        oa << g;
    	// Archive and stream closed when destructors are called
    }

    // ... Some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // Create and open an archive for input
        std::ifstream ifs("filename");
        boost::archive::text_iarchive ia(ifs);
        // Read class state from archive
        ia >> newg;
        // Archive and stream closed when destructors are called
    }
    return 0;
}
