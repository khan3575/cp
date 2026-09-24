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
const int N = 2e5+5;
int n;
vector<vector<int>> v;
vector<vector<int>> dp(N,vector<int>(3,-1));

int pickNext(int row, int currentCol)
{
    if(row>=n)
    {
        return 0;
    }
    if(dp[row][currentCol]!=-1)
    {
        return dp[row][currentCol];
    }


    int ans = 0;
    for(int i = 0; i < 3; i++)
    {
        if(i == currentCol)
        {
            continue;
        }
        ans = max(ans, v[row][currentCol] + pickNext(row+1, i) );
    }
    return dp[row][currentCol] = ans;
}

void solve() {
    cin >> n;
    v.resize(n,vector<int>(3,0));
    for(int i = 0; i < n; i++)
    {
        cin>> v[i][0] >> v[i][1] >> v[i][2];
    }
    cout<< max({pickNext(0,0), pickNext(0,1), pickNext(0,2)})<<endl;
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
