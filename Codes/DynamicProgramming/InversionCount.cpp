#include <vector>
#include <iostream>
using namespace std;
struct InversionCount
{
	typedef long long Long;
	static Long inversionCount(vector<int> V)
	{
		int N = V.size();
		int arr[N];
		for(int i = 0; i < N; ++i)
			arr[i] = V[i];
		Long ret = 0;
		for(int i = 0; (1<<i) <= N; ++i)
		{
			for(int j = 0; j < N; j += (1<<i))
			{
				ret += countInv(arr+j,arr+j+(1<<i));
			}
		}
		return ret;
	}
	template <class ITERATOR>
	static Long countInv(ITERATOR begin , ITERATOR end)
	{
	    if(end - begin <= 1)return 0;
	    int N = end - begin;
	    vector<int> tmp(N);
	    
	    int ia = 0 , ib = N/2 , idx = 0;
	    Long ans = 0;
	    while(ib < N)
	    {
	        while(ia < N/2 && *(begin+ia) > *(begin+ib)){
	            tmp[idx++] = *(begin + ia);
	            ia++;
	        }
	        ans += ia;
	        tmp[idx++] = *(begin + ib);
	        ib++;
	    }
	    while(ia < N/2){
	        tmp[idx++] = *(begin + ia);
	        ia++;
	    }
	    for(int i = 0; i < N; ++i)
	    	*(begin + i) = tmp[i];
	    return ans;
	}
};
