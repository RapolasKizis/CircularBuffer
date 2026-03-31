#include "all.h"
#include "util.h"

//#define TEST_STREAMS_MODE

namespace Stream {
    Manip::Manip(std::string type) {
        this->type = type;
    }

    std::string Manip::get() const {
        return type;
    }

    const Manip endl("endl");

    Streams::Streams() {
    }

    int Streams::getSize() const {
        return v.size();
    }

    std::ostream& Streams::get(int i) const {
        Validator::validateRange(i, 0, v.size()-1);
        return *(v[i]);
    }

    void Streams::add(std::ostream& o) {
        v.push_back(&o);
    }

    Streams& operator<<(Streams& s, const Manip& o) {
        for (int i = 0; i < s.getSize(); ++i) {
            if(o.get() == "endl")
                s.get(i) << std::endl;
        }
        return s;
    }
}

using namespace std;

#ifdef TEST_STREAMS_MODE
int main() {
    try {
        Stream::Streams s;
        s.add(cout);
        s.add(cout);
        s.add(cout);
        s << 1 << "2" << Stream::endl;
    } catch(...) {
        cout << "Unexpected error occurred" << endl;
    }

    return 0;
}
#endif
