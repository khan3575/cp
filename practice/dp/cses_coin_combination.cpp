#include <bits/stdc++.h>
using namespace std;

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
        for (auto& e : x) { if (!f) cerr << ", "; f = false; _pr(e); }
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
const ll MOD = 1e9 + 7;
const int N = 1e6+5;
vector<ll> dp(N, -1);
vector<ll> v;

ll n, target;
// ai value pawar koyta way ache..
ll calculate(ll value)
{
    ll sum = 0;
    if(value == 0)
    {
        return 1;
    }
    if(value <0)
    {
        return 0;
    }
    if(dp[value] != -1)
    {
        return dp[value];
    }

    for(int i = 0; i < n; i++)
    {
        ll nextValue = value - v[i];

        if(nextValue>=0)
        {
           sum = (sum + calculate(nextValue) ) % MOD;
        }
    }
    return dp[value] = sum;
}

void solve() {

 cin >> n >> target;
 v.resize(n);


 for(int i = 0; i< n; i++)
 {
    cin >> v[i];
 }
 dp[0] = 1;
 cout<< calculate(target)<<endl;
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
