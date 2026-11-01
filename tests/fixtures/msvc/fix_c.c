/* Fixture (T5, phase 1.2): tiny C translation unit for Rich/RSDS checks. Ours; no game content. */
int fix_cpp_twice(int x);

int fix_c_add(int a, int b)
{
    return a + b;
}

int main(int argc, char **argv)
{
    (void)argv;
    return fix_cpp_twice(fix_c_add(argc, 1)) & 0x7f;
}
