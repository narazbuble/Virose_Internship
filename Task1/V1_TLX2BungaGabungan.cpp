#include <iostream>
using namespace std;
int main(){
    int p,q,t=1;
    cin>>p;
    int jp = p*p;
    cin>>q;
    int jq = q*q;
    int jumlah = jp + jq + t;
    int perank = jumlah/4;
    if(jumlah%4==0){
        printf("%d", perank);
    }
    else{
        printf("-1");
    }

    return 0;
}