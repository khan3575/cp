// https://codeforces.com/contest/2264/problem/B
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
    ll n, k;
    cin >> n >> k;
    vector<ll>v(n);
    for(ll &i: v) cin >> i;

    vector<ll> contributors;
    for(int i = 0; i < n; i++)
    {
        if(i==0)
        {
            contributors.push_back(0);
            continue;
        }

        ll dif = v[i]-v[i-1];
        contributors.push_back((dif));
    }
    dbg(contributors);
    // removing negetive contri from last
    vector<pair<ll,int>> afterNegetiveRemoved;
    int needToDiscard =  n - k;
    dbg("before", needToDiscard);
    for(int i = n-1; i>=0; i--)
    {
        if(contributors[i] < 0  && needToDiscard >0 )
        {
            needToDiscard--;
        }
        else{
            afterNegetiveRemoved.push_back({contributors[i],i});
        }
    }
    reverse(afterNegetiveRemoved.begin(),afterNegetiveRemoved.end());
    dbg(afterNegetiveRemoved);
    dbg(needToDiscard);

    if(needToDiscard != 0)
    {
        // coping the afterNegativeRemoved
        // and remove the most smallest values from this
        sort(afterNegetiveRemoved.begin(), afterNegetiveRemoved.end() , [](const pair<ll,int>& a, const pair<ll,int>& b){
            return a.first < b.first;
        });
        vector<pair<ll,int>> temp;
        for(int i = needToDiscard; i<(int)afterNegetiveRemoved.size(); i++)
        {
            temp.push_back(afterNegetiveRemoved[i]);
        }
        sort(temp.begin(),temp.end(), [](const pair<ll,int>& a, const pair<ll,int>& b){
            return a.second < b.second;
        });
        afterNegetiveRemoved=temp;
        dbg(temp);
    }
    ll ans = 0;

    for(int i = 0; i <(int)afterNegetiveRemoved.size(); i++)
    {
        dbg( (i+1) , afterNegetiveRemoved[i].first);
        ans += (i+1)*afterNegetiveRemoved[i].first;
    }

    cout<<ans<<endl;
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
