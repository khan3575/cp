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
const int N = 1e6 + 5;
vector<ll> dp(N,-1);

ll calculate(ll n)
{
    if(n == 0)
    {
        return 0;
    }
    if(dp[n] != -1)
    {
        return dp[n];
    }

    vector<ll> extractedDigitList;
    ll temp = n;
    while(temp)
    {
        int digit = temp % 10;
        extractedDigitList.push_back(digit);
        temp /= 10;
    }
    sort(extractedDigitList.begin(),extractedDigitList.end());
    int sz = extractedDigitList.size();
    return dp[n] =(1LL + calculate(n - extractedDigitList[sz-1]));
}
void solve() {
    int n;
    cin >> n;
    for (int i = 1; i <=9; ++i)
    {
        dp[i] = 1;
    }
    cout<< calculate(n)<<endl;

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
