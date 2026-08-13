#include<bits/stdc++.h>
using namespace std;
int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int x;
        cin >> x;
        if(x%3==0)
        {
            cout<<0<<endl;
        }
        else{
            int nextMultipleOfFive = ((x/5)+1)*5;
            if(nextMultipleOfFive%3==0)
            {
                cout<<1 <<endl;
            }
            else{
                if(x%3==2)
                {
                    cout<<1<<endl;
                }
                else{
                    cout<<2<<endl;
                }
            }
        }
    }
}
