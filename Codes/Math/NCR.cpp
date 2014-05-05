#include <vector>
#include <iostream>
using namespace std;
struct Math
{
	typedef long long Long;
	static int egcd(int a,int b)
	{
		int r = a , o_r = b;
		int s = 0 , o_s = 1;
		int t = 1 , o_t = 0;
		while(r != 0)
		{
			int q = o_r / r;
			int tem;
			
			tem = r;
			r = o_r - r * q;
			o_r = tem;
			
			tem = o_s;
			o_s = o_s - s * q;
			o_s = tem;
			
			tem = t;
			t = o_t - t * q;
			o_t = tem;
		}
		// coeficients = o_t , o_s 
		// gcd         = o_r
		// quotients   = t , s
		return o_t;
	}
	static int MOD;
	static Long divide(Long a, Long b)
	{
		return (((egcd(b,MOD) % MOD + MOD) % MOD) * a) % MOD;
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
};
int Math::MOD = 1000000007;