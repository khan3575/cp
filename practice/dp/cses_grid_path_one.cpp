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
vector<vector<ll>> dp;
vector<vector<ll>> arr;

ll calculate(int i, int j, int n)
{
    if(i >= n || j >= n)
    {
        return 0;
    }
    if(i == n-1 && j == n-1 && arr[i][j])
    {
        return dp[i][j] = 1;
    }
    if(dp[i][j] != -1)
    {
        return dp[i][j];
    }

    ll cnt = 0;

    cnt =  (cnt+ arr[i][j] * ((calculate(i+1, j, n) + calculate(i, j+1, n)) %MOD));
    //dbg(i,j,arr[i][j]);
    return dp[i][j] = cnt % MOD;
}

void solve() {
    int n;
    cin >> n;
    vector<string> v(n);
    dp.resize(n, vector<ll>(n,-1));
    arr.resize(n, vector<ll>(n,0));

    for(int i = 0; i < n; i++)
    {
        cin >> v[i];
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(v[i][j] == '.')
            {
                arr[i][j] = 1;
            }
            else{
                arr[i][j] = 0;
            }
        }
    }
    cout << calculate(0,0,n)<<endl;

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
