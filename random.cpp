#include "all.h"
#include "util.h"

//#define TEST_RANDOM_MODE

using namespace std;

int Random::getInt(int low, int n) {
    Validator::validateRange(n, 1, RAND_MAX);
    return (low + (rand() % n));
}

double Random::getDouble(double low, int n, int precision) {
    Validator::validateRange(n, 1, RAND_MAX);
    Validator::validateRange(precision, 0, 6);
    static int values[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
    return (low + ((rand() % n) / (double)values[precision]));
}

string Random::getString(int length) {
    string temp;
    for (int i = 0; i < length; ++i) {
        temp += ('A' + ((rand() % ('Z' - 'A' + 1) + (rand() % 2)*('a' - 'A'))));
    }
    return temp;
}

#ifdef TEST_RANDOM_MODE
int main() {
    try {
        assert(Random::getInt(1, 1) == 1);
        for (int i = 0; i < 10; ++i) {
            int temp = Random::getInt(1, 3);
            assert((1 <= temp) && (temp <= 3));
        }

        assert(Random::getDouble(1.0, 1, 3) == 1.0);
        for (int i = 0; i < 10; ++i) {
            double temp = Random::getDouble(1.0, 11, 1);
            assert((1.0 <= temp) && (temp <= 3.0));
        }

        assert(Random::getString(0).empty());
        for (int i = 0; i < 10; ++i) {
            assert(!(Random::getString(10).empty()));
        }

        cout << "All tests passed successfully" << endl;
    } catch(...) {
        cout << "Unexpected error occurred" << endl;
    }

    return 0;
}
#endif
