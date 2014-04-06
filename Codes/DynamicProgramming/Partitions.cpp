#include <vector>
#include <iostream>
using namespace std;
struct Partitions
{
	const static int MOD = 1000000007;
	/**
	 * It is known that MaxParts < SQRT(2*MaxSum) + 5 otherwise DP[MaxParts][MaxSum] = 0
	 * parts from the theory of taking a PARTITION of size K parts with the
	 * smallest partition of size X and then SUBTRACTING X from each partition
	 * which is the same number of DP(PARTS-1 , SUM - X*PARTS)
	 * then DP(PARTS,SUM) += DP(PARTS-1, SUM - X*PARTS ) FOR X = [1..SUM]
	 */
	static vector< vector<int> > IncreasingDistinctParts(int MaxParts,int MaxSum)
	{
		vector<vector<int> > mem(MaxParts+1 , vector<int>(MaxSum+1));
		for(int i = 1; i <= MaxParts; ++i)
		{
			vector<int> prev(MaxSum+1);
			for(int k = 1; k <= MaxSum; ++k)
			{
				prev[k] = mem[i-1][k];
				if(i == 1)
					mem[i][k] = 1;
				else if(k >= i)
				{
					mem[i][k] = prev[k-i];
					prev[k] += prev[k-i];
					if(prev[k] >= MOD)prev[k] -= MOD;
				}
			}
		}
		return mem;
	}
	/**
	 * parts from the theory of taking a PARTITION of size K parts with the
	 * smallest partition of size X and then SUBTRACTING X from each partition
	 * which is the same number of DP(PARTS-1 , SUM - X*PARTS)
	 * then DP(PARTS,SUM) += DP(PARTS-1, SUM - X*PARTS ) FOR X = [0..SUM]
	 */
	static vector< vector<int> > IncreasingParts(int MaxParts,int MaxSum)
	{
		vector<vector<int> > mem(MaxParts+1 , vector<int>(MaxSum+1));
		for(int i = 1; i <= MaxParts; ++i)
		{
			vector<int> prev(MaxSum+1);
			for(int k = 0; k <= MaxSum; ++k)
			{
				prev[k] = mem[i-1][k];
				if(i == 1)
					mem[i][k] = 1;
				else if(k >= i)
				{
					prev[k] += prev[k-i];
					if(prev[k] >= MOD)prev[k] -= MOD;
					mem[i][k] = prev[k];
				}
			}
		}
		return mem;
	}
	static vector< vector<int> > PermutedDistinctParts(int MaxParts,int MaxSum)
	{
		vector<vector<int> > mem = IncreasingDistinctParts(MaxParts,MaxSum);
		int F = 1;
		for(int i = 1; i <= MaxParts; ++i)
		{
			F = (1LL * F * i) % MOD;
			for(int k = 0; k <= MaxSum; ++k)
			{
				mem[i][k] = (1LL * mem[i][k] * F) % MOD;
			}
		}
		return mem;
	}
	static vector< vector<int> > PermutedParts(int MaxParts,int MaxSum)
	{
		vector<vector<int> > mem(MaxParts+1 , vector<int>(MaxSum+1));
		for(int i = 1; i <= MaxParts; ++i)
		{
			vector<int> prev(MaxSum+1);
			for(int k = 0; k <= MaxSum; ++k)
			{
				prev[k] = mem[i-1][k];
				if(i == 1)
					mem[i][k] = 1;
				else
				{
					if(k > 0)prev[k] += prev[k-1];
					if(prev[k] >= MOD)prev[k] -= MOD;
					mem[i][k] = prev[k];
				}
			}
		}
		return mem;
	}
};
