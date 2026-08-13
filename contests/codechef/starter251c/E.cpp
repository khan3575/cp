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
    //input
    int n;
    cin >> n;
    string s;
    cin >> s;
    string ans = "";
    for(int i = 0; i < n; i++)
    {
        ans += "0";
    }


    for(int i = 0; i < n; i++)
    {
         int j = i;
         while(j<n && s[i] == s[j])
         {
            j++;
         }
         if(j<n)
         {
            int k = j;
            while(k < n && s[k] == s[j])
            {
                k++;
            }
            string operatedSubstringOne = s.substr(i,j-i-1) + ((s[j] == 0) ? '1': '0') + s.substr(j, k-j);
            string operatedSubstringTwo = s.substr(i,j-i) + ((s[j+1] == 0) ? '1': '0') + s.substr(j+1,k-j-1);
            //dbg(operatedSubstringOne, operatedSubstringTwo);
            if(operatedSubstringOne > ans)
            {
                ans = operatedSubstringOne;
            }
            if(operatedSubstringTwo > ans)
            {
                ans= operatedSubstringTwo;
            }
         }
         i = j - 1;
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
