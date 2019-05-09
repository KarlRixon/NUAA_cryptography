
#include<iostream>
using namespace std;
void ext_gcd(int a,int b,int &d,int &x,int &y) {
    if(b == 0) {
	d = a; x = 1; y = 0;
	return;
    }
    ext_gcd(b,a % b,d,y,x);
    y -= a / b * x;
    return;
}
int main() {
    int x,y,d;
    int a,b; 
    while(1) {
	cin >> a >> b; // eg. 26, 11 -> 3.  107, 37 -> 9
	ext_gcd(a,b,d,x,y);
	if(d != 1) cout << "There is no ans for this input." << endl;
	else cout << "The ans for this input : " << ((x + b) % b*a-1)/b << endl;
    }
    return 0;
}