// Fixture (T5, phase 1.2): tiny C++ translation unit for Rich/RSDS checks. Ours; no game content.
struct Fix
{
    int v;
    explicit Fix(int x) : v(x) {}
    int twice() const { return v * 2; }
};

extern "C" int fix_cpp_twice(int x)
{
    return Fix(x).twice();
}
