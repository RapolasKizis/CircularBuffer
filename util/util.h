#ifndef UTIL_H_03302026
#define UTIL_H_03302026

#include "all.h"

class Validator {
public:
    static bool isInRange(int value, int low, int high);
    static void validateRange(int value, int low, int high);
};

class Random {
public:
    static int getInt(int low, int high);
    static double getDouble(double low, int n, int precision);
    static std::string getString(int length);
};

namespace Stream {
    class Manip {
    private:
        std::string type;
    public:
        Manip(std::string type);
        std::string get() const;
    };

    extern const Manip endl;

    class Streams {
    private:
        std::vector<std::ostream *> v;
    public:
        Streams();
    private:
        Streams(const Streams& other);
        Streams& operator=(const Streams& other);
    public:
        int getSize() const;
        std::ostream& get(int i) const;
        void add(std::ostream& o);

        template <class T>
        Streams& operator<<(const T& o) {
            for (int i = 0; i < getSize(); ++i) {
                get(i) << o;
            }
        return *this;
        }
    };

    Streams& operator<<(Streams& s, const Manip& o);
}

#endif // UTIL_H_03302026
