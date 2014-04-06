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
	static Long divide(Long a, Long b, Long mod)
	{
		return (((egcd(b,mod) % mod + mod) % mod) * a) % mod;
	}
	
	static int ncr(int n,int k,int mod)
	{
		static vector<int> F;
		if(F.size() <= n)
		{
			int oSZ = F.size();
			F.resize(n+1);
			if(oSZ == 0)F[0] = 1 , oSZ++;
			for(int i = oSZ; i <= n; ++i)
				F[i] = (1LL * F[i-1] * i) % mod;
		}
		if(k < 0 || n < 0 || n < k)return 0;
		return divide(F[n] , (1LL * F[k] * F[n-k]) % mod , mod);
	}
};
