#include <vector>
#include <iostream>
using namespace std;
struct Math
{
	typedef long long Long;
	static void assign(int &a, int &b, int c, int d){
		a = c;
		b = d;
	}
	static int egcd(int a, int b){
		int x,y;
		return egcd(a,b,x,y);
	}
	// ax + by = c
	static int egcd(int a,int b, int &x, int &y) {
		x = 1, y = 0;
		int nx = 0, ny = 1;
		int nr = b, r = a;
		while(nr != 0)
		{
			int q = r / nr;
			assign(r, nr, nr, r - q * nr);
			assign(x, nx, nx, x - q * nx);
			assign(y, ny, ny, y - q * ny);
		}
		// bezout = x , y 
		// gcd    = o_r
		return r;
	}
	static int MOD;
	static Long modInv(Long a, Long b)
	{
		int x,y; egcd(a,b,x,y);
		return x < 0 ? x + b : x;
	}
	static Long divide(Long a, Long b)
	{
		return a * modInv(b, MOD) % MOD;
	}
	static int fact(int n)
	{
		static vector<int> F;
		if(F.size() <= n)
		{
			int oSZ = F.size();
			F.resize(n+1);
			if(oSZ == 0)F[0] = 1 , oSZ++;
			for(int i = oSZ; i <= n; ++i)
				F[i] = (1LL * F[i-1] * i) % MOD;
		}
		return F[n];
	}
	static int catalan(int n)
	{
		return divide(ncr(2*n,n), n+1);
	}
	static int ncr(int n,int k)
	{
		if(k < 0 || n < 0 || n < k)return 0;
		return divide(fact(n) , (1LL * fact(k) * fact(n-k)) % MOD);
	}
	static int mul(int a,int b){
		return (1LL*(a%MOD)*(b%MOD))%MOD;
	}
	static int  mul(int a,int b,int c){
		return (1LL*mul(a,b)*(c%MOD))%MOD;
	}
	static int  mul(int a,int b,int c,int d){
		return (1LL*mul(a,b)*mul(c,d))%MOD;
	}
	static bool solveDiophantine(int a, int b, int c, int &x, int &y) {
		int gc = egcd(a,b,x,y);
		if(c % gc != 0)return false;
		int d = c / gc;
		x *= d;
		y *= d;
		return true;
	}
	static void moveBezout(int a,int b, int c, int &x, int &y, int pos = 1){
		int gc = egcd(a,b);
		b /= gc;
		a /= gc;
		x += b * pos;
		y -= a * pos;
	}
};
int Math::MOD = 1000000007;