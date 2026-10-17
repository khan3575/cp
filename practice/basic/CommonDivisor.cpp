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
const int N = 1e6+5;
vector<int> presentNumber(N);

void solve() {
    int n;
    cin >> n;
    vector<int> v(n);
    for(int i = 0; i < n; i++)
    {
        cin >> v[i];
        presentNumber[v[i]]++;
    }
    sort(v.begin(),v.end());

    int answer = 1;
    for(int i = N; i >= 1; i--)
    {
        int count = 0;
        for(int j =  i; j <= N; j+=i)
        {
            if( presentNumber[j])
            {
                count+= presentNumber[j];
            }
        }
        if(count>=2)
        {
           // dbg(i, count);
            answer = i;
            break;
        }
    }
    cout<< answer <<endl;
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
