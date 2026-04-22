#include "util/all.h"
#include "circularBuffer.h"

#define DEMO_MODE

using namespace std;

#ifdef DEMO_MODE
int main() {
    try {
        circularBufferSpace::CircularBuffer s1;
        cout << s1.toString() << endl;

        cout << s1.getCapacity() << endl;

        s1.setCapacity(10);
        cout << s1.toString() << endl;

        s1.addValue(10);
        cout << s1.toString() << endl;

        s1.removeValue(10);
        cout << s1.toString() << endl;

        for(int i = 0; i < 5; ++i)
            s1.addValue(Random::getInt(-100, 200));
        cout << s1.toString() << endl;

        s1.addValue(0);
        s1.addValue(0);
        s1.addValue(0);
        cout << s1.toString() << endl;

        s1.addValue(1);
        s1.addValue(0);
        s1.addValue(0);
        s1.addValue(0);
        cout << s1.toString() << endl;

        s1.editValue(101, 5);
        cout << s1.toString() << endl;

        cout << s1.positionOf(101) << endl;

        s1.clearBuffer();
        cout << s1.toString() << endl;

        s1+=3;
        s1+=15;
        s1+=0;
        cout << s1.toString() << endl;

        s1-=3;
        cout << s1.toString() << endl;

        std::pair<int,int> edit = {100, 1};
        s1*=edit;
        cout << s1.toString() << endl;

        cout << s1[15] << endl;

        !s1;
        cout << s1.toString() << endl;

        for(int i = 0; i < 5; ++i)
            s1.addValue(Random::getInt(-100, 200));
        cout << s1.toString() << endl;

        circularBufferSpace::CircularBuffer s2;
        s2 = s1;
        cout << s2.toString() << endl;
    } catch(...) {
        cout << "Unexpected error occurred" << endl;
    }

    return 0;
}
#endif
