int __ffssi2(long i)
{
    int b;
    if (i == 0)
        return 0;
    for (b = 1; !(i & 1); ++b)
        i = (unsigned int)i >> 1;
    return b;
}

