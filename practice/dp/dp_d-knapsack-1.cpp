// https://atcoder.jp/contests/dp/tasks/dp_d
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
ll n, totalWeight;
vector<ll> values;
vector<ll> weights;
vector<vector<ll>> dp;
// return korbe value and condition hocche weight
// either pick or not pick

ll pick(ll idx, ll weight)
{
    if(idx>=n)
    {
        return 0;
    }

    if(dp[idx][weight] != -1)
    {
        return dp[idx][weight];
    }
    ll ans = 0;
    //pick the current weighted item
    if(weight + weights[idx] <= totalWeight)
    {
        ans = values[idx] + max(ans, pick(idx+1, weight+ weights[idx]));
    }

    //dont pick the item
    ans = max(ans, pick(idx+1, weight));
    return dp[idx][weight] = ans;
}
void solve() {

    cin >> n >> totalWeight;
    values.resize(n);
    weights.resize(n);
    dp.resize(n+1,vector<ll>(totalWeight + 1 , -1));
    for(int i = 0; i < n; i++)
    {
        cin >> weights[i] >> values[i];
    }

    cout<<pick(0,0)<<endl;


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
