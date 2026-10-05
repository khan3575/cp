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
int n, m;
vector<ll> a, b;
vector<vector<int>> memo;

int cal(int n1, int m1)
{
    if(n1 < 0 || m1 < 0)
    {
        return 0;
    }
    //dbg(n1, m1);
    if(memo[n1][m1] != -1)
    {
        return memo[n1][m1];
    }
    if(a[n1] == b[m1])
    {
        return memo[n1][m1] = 1 + cal(n1-1, m1-1);
    }
    return memo[n1][m1] = max(cal(n1-1,m1), cal(n1, m1-1));
}

vector<ll> getPath (int n1, int m1)
{
    vector<ll>path;
    while(n1 >=0 && m1 >= 0)
    {
        if(a[n1] == b[m1])
        {
            path.push_back(a[n1]);
            n1--;
            m1--;
        }
        else{
            int up_value = cal(n1-1, m1);
            int left_value = cal(n1, m1-1);

            if(up_value >= left_value)
            {
                n1--;
            }
            else{
                m1--;
            }
        }
    }
    reverse(path.begin(), path.end());
    return path;
}
void solve() {
    cin >> n >> m;
    a.resize(n);
    b.resize(m);
    memo.resize(n,vector<int>(m,-1));

    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for(int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    cout<< cal(n-1, m-1)<<endl;
    vector<ll> ans = getPath(n-1, m-1);
    for(auto x: ans)
    {
        cout << x<<" ";
    }
    cout<<endl;

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
