#include "all.h"
#include "util.h"

//#define TEST_VALIDATOR_MODE

using namespace std;

bool Validator::isInRange(int value, int low, int high) {
    if (low <= high)
        return ((low <= value) && (value <= high));
    else {
        stringstream ss;
        ss << "Logic error: the first value of the range must be lower or equal than the second value" <<endl;
        throw logic_error(ss.str());
    }
}

void Validator::validateRange(int value, int low, int high) {
    if(!isInRange(value, low, high)) {
        stringstream ss;
        ss << "Out of range: " << value << " is not in between " << low << " and " << high <<endl;
        throw out_of_range(ss.str());
    }
}

#ifdef TEST_VALIDATOR_MODE
int main() {
    try {
        assert(Validator::isInRange(0, 1, 10) == false);
        assert(Validator::isInRange(1, 1, 10) == true);
        assert(Validator::isInRange(2, 1, 10) == true);

        bool thrown = false;
        try {
            Validator::validateRange(0, 1, 10);
        } catch(const exception& e) {
            thrown = true;
        }
        assert(thrown);

        thrown = false;
        try {
            Validator::validateRange(1, 1, 10);
        } catch(const exception& e) {
            thrown = true;
            cout << e.what();
        }
        assert(!thrown);

        thrown = false;
        try {
            Validator::validateRange(2, 1, 10);
        } catch(const exception& e) {
            thrown = true;
        }
        assert(!thrown);

        cout << "All tests passed successfully" << endl;
    } catch(...) {
        cout << "Unexpected error occurred" << endl;
    }

    return 0;
}
#endif
