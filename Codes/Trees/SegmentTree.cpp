//============================================================================
// Name        : HLD.cpp
// Author      : cjtoribio
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;


struct SegmentNode{
	int sz;
	bool HasCarry;
	SegmentNode(){
		this->sz = 1;
		HasCarry = 0;
	}
	void join(const SegmentNode &l, const SegmentNode &r){
		sz = l.sz + r.sz;
	}
	void update(){
		HasCarry = 1;
	}
	void clear(){
		HasCarry = 0;
	}
};
template<class T>
struct SegmentTree
{
	T *V;
	int N;
	SegmentTree(int N)
	{
		this->V = new T[4*N];
		this->N = N;
	}
	void create(const vector<typename T::Init> &VEC,int n = 1,int b = 0,int e = -1)
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
	int findLastIndex( bool (*isOk)(T) ){
		int n = 1;
		T acum;
		acum.sz = 0;
		while(V[n].sz > 1){
			int l = 2*n , r = 2*n+1;
			T newAcum = (acum.sz == 0 ? V[r] : (V[r] + acum));
			if(isOk(newAcum)){
				n = r;
			}else{
				acum = newAcum;
				n = l;
			}
		}
		return N - acum.sz - 1;
	}
};

#define NONE 0
#define OPEN 1
#define CLOSE 2
struct MinVal : SegmentNode 
{
	struct Init {
		int val;
		Init(int val=NONE):val(val){ }
	};
	int open, close;
	int carry;
	MinVal() : SegmentNode(){
		open = 0;
		close = 0;
		carry = 0;
	}
	MinVal(Init n) : SegmentNode()  {
		open = (n.val == OPEN);
		close = (n.val == CLOSE);
		carry = 0;
	}
	MinVal operator+(const MinVal &N)const {
		MinVal ret; ret.join( *this , N );
		
		int cancel = min(open, N.close);
		ret.open = open + N.open - cancel;
		ret.close= close + N.close - cancel;
		
		return ret;
	}
	void update(int val)
	{	
		SegmentNode::update();
		if(val == OPEN){
			close = 0;
			open = sz;
		}else if(val == CLOSE){
			close = sz;
			open = 0;
		}else{
			open = 0;
			close = 0;
		}
	}
};