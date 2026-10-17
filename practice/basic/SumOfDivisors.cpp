// URL:
// idea:
#include <bits/stdc++.h>
using namespace std;

// Formatter is off down to the dashed line, so the aliases and debug block stay compact.
// clang-format off
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define all(x)  (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x)   (int)(x).size()

// ---------- local-only debug (stripped in judge builds) ----------
#ifdef LOCAL
template <class T> void _pr(const T& x) {
    if constexpr (requires { cerr << x; }) {
        cerr << x;
    } else if constexpr (requires { x.first; x.second; }) {
        cerr << '('; _pr(x.first); cerr << ", "; _pr(x.second); cerr << ')';
    } else {
        cerr << '{'; bool f = true;
        for (const auto& e : x) { if (!f) cerr << ", "; f = false; _pr(e); }
        cerr << '}';
    }
}
inline void _dbg() { cerr << '\n'; }
template <class T, class... A> void _dbg(const T& x, const A&... a) {
    _pr(x); if (sizeof...(a)) cerr << " | "; _dbg(a...);
}
#define dbg(...) (cerr << "[" << #__VA_ARGS__ << "] = ", _dbg(__VA_ARGS__))
#else
#define dbg(...) ((void)0)
#endif
// -----------------------------------------------------------------
// clang-format on
const ll MOD = 1e9 + 7;
const ll inv2 = 500000004;

void solve() {
    ll n;
    cin >> n;
    ll totalSum = 0;
    for(ll i = 1, r; i <= n; i= r+1)
    {
        ll v = n / i;
        r = n / v;

        ll count = (r - i + 1) %MOD;

        ll sumOfEndpoints = (i % MOD + r % MOD ) % MOD;

        ll sumOfRange = (count * sumOfEndpoints) % MOD;
        sumOfRange = (sumOfRange * inv2) % MOD;
        ll res = (v % MOD) * sumOfRange  % MOD;
        totalSum = (totalSum + res)% MOD;
    }
    cout<< totalSum <<endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        // cout << "Case " << tc << ": ";
        solve();
    }
    return 0;
}
