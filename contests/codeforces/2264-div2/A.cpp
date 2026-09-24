// https://codeforces.com/contest/2264/problem/A
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
    vector<int>v(n);
    vector<int> missMatched;
    vector<int>locationOfMissMatched;
    for(int i = 0; i< n; i++)
    {
        cin >> v[i];
    }
    for(int i = 0; i < n; i++)
    {
        int x = i+1;
        if(v[i] != x)
        {
            missMatched.push_back(v[i]);
            locationOfMissMatched.push_back(i);
        }
    }
    if(missMatched.size() == 0)
    {
        cout<<"YES\n";
        return;
    }

    reverse(missMatched.begin(),missMatched.end());
    int cnt = missMatched.size();
    for(int i = 0; i< (int)missMatched.size(); i++)
    {
        //checking after reversse
        if(locationOfMissMatched[i] + 1 == missMatched[i])
        {
            cnt--;
        }
    }

    if(cnt == 0)
    {
        cout<<"YES\n";
    }
    else{
        cout<<"NO\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        // cout << "Case " << tc << ": ";
        solve();
    }
    return 0;
}
