#include "util/all.h"
#include "circularBuffer.h"

#define TEST_MODE

using namespace std;
using namespace circularBufferSpace;

#ifdef TEST_MODE

// ── helpers ──────────────────────────────────────────────────────────────────

static int passed = 0;
static int failed = 0;

void report(Stream::Streams& log, const string& name, bool ok) {
    log << (ok ? "[PASS] " : "[FAIL] ") << name << Stream::endl;
    if (ok) ++passed; else ++failed;
}

void section(Stream::Streams& log, const string& title) {
    log << "--- " << title << " ---" << Stream::endl;
}

// ── main ─────────────────────────────────────────────────────────────────────

int main() {
    ofstream logFile("log.txt");
    Stream::Streams log;
    log.add(cout);
    log.add(logFile);

    log << "========== CircularBuffer Tests ==========" << Stream::endl;

    // ── 1. Default constructor ────────────────────────────────────────────────
    section(log, "Constructors");

    CircularBuffer b1;
    report(log, "Default constructor: capacity is 8",     b1.getCapacity() == 8);
    report(log, "Default constructor: toString is empty", b1.toString() == "0/8 {}");

    CircularBuffer b2(5);
    report(log, "Capacity constructor: capacity is 5",    b2.getCapacity() == 5);

    bool threw = false;
    try { CircularBuffer bad(0); } catch (const out_of_range&) { threw = true; }
    report(log, "Capacity constructor: rejects 0",        threw);

    // ── 2. addValue / operator+= ──────────────────────────────────────────────
    section(log, "addValue / operator+=");

    CircularBuffer b3(4);
    b3.addValue(10);
    b3.addValue(20);
    b3.addValue(30);
    report(log, "addValue: toString after 3 adds",        b3.toString() == "3/4 {10, 20, 30}");

    b3 += 40;
    report(log, "operator+=: toString after 4th add",     b3.toString() == "4/4 {10, 20, 30, 40}");

    b3 += 50; // overwrite oldest (10)
    report(log, "addValue: overwrites oldest when full",  b3.toString() == "4/4 {20, 30, 40, 50}");

    // ── 3. Copy constructor ───────────────────────────────────────────────────
    section(log, "Copy constructor");

    CircularBuffer b4(b3);
    report(log, "Copy constructor: same toString",        b4.toString() == b3.toString());
    report(log, "Copy constructor: same capacity",        b4.getCapacity() == b3.getCapacity());

    b4 += 99;
    report(log, "Copy is deep: original unchanged",       b3.toString() == "4/4 {20, 30, 40, 50}");

    // ── 4. operator= ─────────────────────────────────────────────────────────
    section(log, "operator=");

    CircularBuffer b5(3);
    b5 += 1; b5 += 2; b5 += 3;
    CircularBuffer b6;
    b6 = b5;
    report(log, "operator=: same toString",               b6.toString() == b5.toString());

    b6 += 99;
    report(log, "operator=: deep copy (original unchanged)", b5.toString() == "3/3 {1, 2, 3}");

    b5 = b5; // self-assignment
    report(log, "operator=: self-assignment safe",        b5.toString() == "3/3 {1, 2, 3}");

    // ── 5. removeValue / operator-= ──────────────────────────────────────────
    section(log, "removeValue / operator-=");

    CircularBuffer b7(5);
    b7 += 1; b7 += 2; b7 += 3; b7 += 4; b7 += 5;

    b7.removeValue(3); // remove middle
    report(log, "removeValue: remove middle",             b7.toString() == "4/5 {1, 2, 4, 5}");

    b7.removeValue(1); // remove oldest
    report(log, "removeValue: remove oldest",             b7.toString() == "3/5 {2, 4, 5}");

    b7.removeValue(5); // remove newest
    report(log, "removeValue: remove newest",             b7.toString() == "2/5 {2, 4}");

    b7 -= 2;
    b7 -= 4;
    report(log, "operator-=: remove until empty",         b7.toString() == "0/5 {}");

    threw = false;
    try { b7.removeValue(99); } catch (const ValueNotFoundException&) { threw = true; }
    report(log, "removeValue: throws on empty buffer",    threw);

    CircularBuffer b8(3);
    b8 += 7;
    threw = false;
    try { b8.removeValue(99); } catch (const ValueNotFoundException&) { threw = true; }
    report(log, "removeValue: throws when value missing", threw);

    // ── 6. editValue / operator*= ─────────────────────────────────────────────
    section(log, "editValue / operator*=");

    CircularBuffer b9(4);
    b9 += 10; b9 += 20; b9 += 30; b9 += 40;

    b9.editValue(99, 0);
    report(log, "editValue: edit position 0",             b9.toString() == "4/4 {99, 20, 30, 40}");

    b9.editValue(88, 3);
    report(log, "editValue: edit last position",          b9.toString() == "4/4 {99, 20, 30, 88}");

    b9.editValue(77, 4); // position 4 % 4 = 0
    report(log, "editValue: position wraps (4%4=0)",      b9.toString() == "4/4 {77, 20, 30, 88}");

    std::pair<int,int> edit = {55, 1}; // editValue(55, position=1)
    b9 *= edit;
    report(log, "operator*=: edit via pair",              b9.toString() == "4/4 {77, 55, 30, 88}");

    // ── 7. positionOf / operator[] ────────────────────────────────────────────
    section(log, "positionOf / operator[]");

    CircularBuffer b10(4);
    b10 += 10; b10 += 20; b10 += 30;

    report(log, "positionOf: finds first element",        b10.positionOf(10) == 0);
    report(log, "positionOf: finds middle element",       b10.positionOf(20) == 1);
    report(log, "positionOf: finds last element",         b10.positionOf(30) == 2);
    report(log, "operator[]: same as positionOf",         b10[20] == 1);

    threw = false;
    try { b10.positionOf(99); } catch (const ValueNotFoundException&) { threw = true; }
    report(log, "positionOf: throws when missing",        threw);

    threw = false;
    try { b10[99]; } catch (const ValueNotFoundException&) { threw = true; }
    report(log, "operator[]: throws when missing",        threw);

    // ── 8. getCapacity / setCapacity ─────────────────────────────────────────
    section(log, "getCapacity / setCapacity");

    CircularBuffer b11(3);
    report(log, "getCapacity: returns correct value",     b11.getCapacity() == 3);

    b11.setCapacity(10);
    report(log, "setCapacity: updates capacity",          b11.getCapacity() == 10);

    b11 += 1; b11 += 2; b11 += 3;
    threw = false;
    try { b11.setCapacity(2); } catch (const out_of_range&) { threw = true; }
    report(log, "setCapacity: rejects capacity < nodeCount", threw);

    // ── 9. clearBuffer / operator! ────────────────────────────────────────────
    section(log, "clearBuffer / operator!");

    CircularBuffer b12(4);
    b12 += 1; b12 += 2; b12 += 3;
    b12.clearBuffer();
    report(log, "clearBuffer: empties buffer",            b12.toString() == "0/4 {}");

    b12 += 5; b12 += 6;
    !b12;
    report(log, "operator!: empties buffer",              b12.toString() == "0/4 {}");

    b12.clearBuffer(); // clear already empty — should not crash
    report(log, "clearBuffer: safe on empty buffer",      b12.toString() == "0/4 {}");

    // ── 10. Comparison operators ──────────────────────────────────────────────
    section(log, "Comparison operators");

    CircularBuffer c1(5), c2(5), c3(5);
    c1 += 1; c1 += 2; c1 += 3;   // sum=6
    c2 += 1; c2 += 2; c2 += 3;   // sum=6
    c3 += 10; c3 += 20; c3 += 30; // sum=60

    report(log, "operator==: equal buffers",              c1 == c2);
    report(log, "operator==: unequal buffers",            !(c1 == c3));
    report(log, "operator!=: unequal buffers",            c1 != c3);
    report(log, "operator!=: equal buffers",              !(c1 != c2));
    report(log, "operator<:  less than",                  c1 < c3);
    report(log, "operator<:  not less than equal",        !(c3 < c1));
    report(log, "operator<=: less or equal (less)",       c1 <= c3);
    report(log, "operator<=: less or equal (equal)",      c1 <= c2);
    report(log, "operator<=: not less or equal",          !(c3 <= c1));
    report(log, "operator>:  greater than",               c3 > c1);
    report(log, "operator>:  not greater than equal",     !(c1 > c3));
    report(log, "operator>=: greater or equal (greater)", c3 >= c1);
    report(log, "operator>=: greater or equal (equal)",   c1 >= c2);
    report(log, "operator>=: not greater or equal",       !(c1 >= c3));

    // ── summary ───────────────────────────────────────────────────────────────
    log << "==========================================" << Stream::endl;
    log << "Results: " << passed << " passed, " << failed << " failed." << Stream::endl;
    log << "==========================================" << Stream::endl;

    return 0;
}
#endif
