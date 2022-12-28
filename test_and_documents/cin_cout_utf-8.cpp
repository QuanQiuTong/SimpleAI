//#pragma GCC optimize(2)
#include<cstdio>
#include<iostream>
#include<string>
using namespace std;
inline int read(){
	register int x,ch;while((ch=getchar())<48||57<ch);
	x=ch^48;while(47<(ch=getchar())&&ch<58)x=x*10+(ch^48);
	return x;
}
int main(){
	string a;
	cin>>a;
	cout<<a;
	return 0;
}
