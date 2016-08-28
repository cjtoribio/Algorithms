//============================================================================
// Name        : HLD.cpp
// Author      : cjtoribio
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;


struct MinVal
{
	struct Init {
		int val, id;
		Init(int val=0, int id=0){
			this->val = val;
			this->id  = id;
		}
	};
	int mId, mV, carry , sz;
	bool HasCarry;
	MinVal(){
		this->mId = 0;
		this->mV = 0;
		this->carry = 0;
		this->sz = 1;
		HasCarry = 0;
	}
	MinVal(Init n) {
		mId = n.id;
		mV  = n.val;
		this->carry = 0;
		this->sz = 1;
		HasCarry = 0;
	}
	MinVal operator+(const MinVal &N)const {
		MinVal ret;
		if(mV <= N.mV){
			ret.mV = mV;
			ret.mId = mId;
		}else{
			ret.mV = N.mV;
			ret.mId = N.mId;
		}
		ret.carry = 0;
		ret.HasCarry = 0;
		ret.sz = this->sz + N.sz;
		return ret;
	}
	void update(int val)
	{
		this->HasCarry = 1;
		this->carry += val;
		this->mV += val;
	}
	void clear(){
		this->HasCarry = 0;
		this->carry = 0;
	}
};

template<class T>
struct SegmentTree
{
	vector<T> V;
	int N;
	SegmentTree(int N)
	{
		this->V = vector<T>(4*N);
		this->N = N;
	}
	void create(vector<typename T::Init> &VEC,int n = 1,int b = 0,int e = -1)
	{
		if(e == -1)e = N - 1;
		if(b == e){
			V[n] = T(VEC[b]);
		}
		else
		{
			create(VEC,2*n,b,(e+b)/2);
			create(VEC,2*n+1,(e+b)/2+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}
	}
	T query(int i,int j, int n = 1,int b = 0,int e = -1)
	{
		if(e == -1)e = N - 1;
		if(i <= b && e <= j)return V[n];
		else
		{
			if(V[n].HasCarry)
			{
				V[2*n  ].update(V[n].carry);
				V[2*n+1].update(V[n].carry);
				V[n].clear();
			}
			int mid = (b+e)/2;
			if(i > mid)return query(i,j,2*n+1,mid+1,e);
			if(j <=mid)return query(i,j,2*n,b,mid);
			return query(i,j,2*n,b,mid) + query(i,j,2*n+1,mid+1,e);
		}
	}
	void update(int i,int j,int v,int n = 1,int b=0,int e=-1)
	{
		if(e == -1)e = N - 1;
		if(i <= b && e <= j)
		{
			V[n].update(v);
		}
		else if(i > e || j < b)
			return;
		else
		{
			if(V[n].HasCarry)
			{
				V[2*n  ].update(V[n].carry);
				V[2*n+1].update(V[n].carry);
				V[n].clear();
			}
			int mid = (b+e)/2;
			update(i,j,v,2*n,b,mid);
			update(i,j,v,2*n+1,mid+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}

	}
};