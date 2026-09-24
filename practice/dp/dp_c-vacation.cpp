// https://atcoder.jp/contests/dp/tasks/dp_c
// idea: TODO
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



void solve() {
    int n;
    cin >> n;
    vector<vector<int>> v(n+1, vector<int>(3,0));
    vector<vector<int>> dp(n+1, vector<int>(3,0));
    for(int i = 1; i<=n; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            cin>> v[i][j];
        }
    }
    dp[0][0] = 0;
    dp[0][1] = 0;
    dp[0][2] = 0;

    for(int i = 1; i <= n;  i++)
    {
        dp[i][0] = v[i][0] + max(dp[i-1][1],dp[i-1][2]);
        dp[i][1] = v[i][1] + max(dp[i-1][0],dp[i-1][2]);
        dp[i][2] = v[i][2] + max(dp[i-1][0],dp[i-1][1]);
    }
    cout<< max({dp[n][0],dp[n][1], dp[n][2]})<<endl;
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
