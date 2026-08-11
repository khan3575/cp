// Z-function: z[i] = length of the longest common prefix of s and s[i..].
// O(|s|). Pattern search: run on pat + '\1' + text.
// Verified: CSES 1753 (String Matching).
vector<int> zfunc(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
