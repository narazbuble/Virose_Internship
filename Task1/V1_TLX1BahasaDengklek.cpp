#include <bits/stdc++.h>

using namespace std; 

int main(){
   string arr;
   std::cin >> arr;
   for (int i = 0; i<arr.length(); i++) {
        char c = arr[i];
        if (c >= 'A' && c <= 'Z') {
            arr[i] = arr[i] - 'A' + 'a';
        } else if (c >= 'a' && c <= 'z') {
            arr[i] = arr[i] - 'a' + 'A';
        }

    }

    cout<<arr;

    return 0;
}
