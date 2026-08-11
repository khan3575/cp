// Modular exponentiation and inverse (MOD must be prime for inv).
// O(log e).
const long long MOD = 1'000'000'007;
long long mpow(long long b, long long e, long long m = MOD) {
    long long r = 1;
    b %= m;
    if (b < 0) b += m;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}
long long minv(long long a, long long m = MOD) { return mpow(a, m - 2, m); }
