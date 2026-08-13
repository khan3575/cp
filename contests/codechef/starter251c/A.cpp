#include<bits/stdc++.h>
using namespace std;
int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int n, m, x;
        cin >> n >> m >> x;
        int rowNumber = ((x-1)/m) +1;
        cout<< min(rowNumber, n-rowNumber+1)<<endl;
    }
}
