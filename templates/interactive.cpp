#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Interactive problems: NEVER buffer. endl flushes; do not untie cin/cout.
int query(int x) {
    cout << "? " << x << endl;
    int r; cin >> r;
    if (r == -1) exit(0);
    return r;
}

void solve() {
    int n; cin >> n;

    cout << "! " << 0 << endl;
}

int main() {
    int T = 1;
    cin >> T;
    while (T--) solve();
    return 0;
}
