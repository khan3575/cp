#include<bits/stdc++.h>
using namespace std;
int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int n; cin >> n;
        vector<int> v(n);
        int alice =0;
        int oddCount =0;
        int sumOdd = 0;
        int sumEven = 0;
        int sum = 0;
        for(int i = 0; i < n; i++)
        {
            cin >> v[i];
            if(v[i]&1)
            {
                oddCount++;
                sumOdd+= v[i];
            }
            else{
                sumEven+=v[i];
            }
            sum += v[i];
        }
        if(sum&1)
        {
            alice = oddCount/2;
        }
        else{
            alice = sumEven + oddCount/2 + (sumOdd - (oddCount));
        }
        cout<<alice<<endl;

    }
}
