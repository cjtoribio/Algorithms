#include <iostream>
#include <vector>
using namespace std;

struct SegmentNode{
	int sz = 1;
	bool HasCarry = 0;
	void join(const SegmentNode &l, const SegmentNode &r){
		sz = l.sz + r.sz;
	}
	void update(){ HasCarry = 1; }
	void clear(){ HasCarry = 0; }
};
template<class T>
struct SegmentTree
{
	vector<T> V;
	int N;
	SegmentTree(int N) : V(4*N), N(N) {}
	void create(const vector<typename T::Init> &VEC,int n = 1,int b = 0,int e = -1)
	{
		if (e == -1) e = N - 1;
		if (b == e) V[n] = T(VEC[b]);
		else {
			create(VEC,2*n,b,(e+b)/2);
			create(VEC,2*n+1,(e+b)/2+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}
	}
	T query(int i,int j, int n = 1,int b = 0,int e = -1)
	{
		if (e == -1)e = N - 1;
		if (i <= b && e <= j) return V[n];
		else {
			if(V[n].HasCarry) {
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
		if (e == -1) e = N - 1;
		if (i <= b && e <= j) V[n].update(v);
		else if (i > e || j < b) return;
		else {
			if(V[n].HasCarry) {
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
	int findLastIndex( bool (*isOk)(T) ){ // almost never needed
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
struct OpenBrackets : SegmentNode 
{
	struct Init { // type to send to the create
		int val;
		Init(int val=NONE):val(val){ }
	};
	int open, close;
	int carry;
	OpenBrackets() : SegmentNode(){ // default initialization if create not called
		open = close = carry = 0;
	}
	OpenBrackets(Init n) : SegmentNode()  { // initialization with create called
		open = (n.val == OPEN);
		close = (n.val == CLOSE);
		carry = 0;
	}
	OpenBrackets operator+(const OpenBrackets &N)const { // join
		OpenBrackets ret; ret.join( *this , N );
		
		int cancel = min(open, N.close);
		ret.open = open + N.open - cancel;
		ret.close= close + N.close - cancel;
		
		return ret;
	}
	void update(int val) {	// update node
		SegmentNode::update();
		if (val == OPEN) {
			close = 0;
			open = sz;
		} else if(val == CLOSE) {
			close = sz;
			open = 0;
		} else {
			open = 0;
			close = 0;
		}
	}
};

int main() {
	int N = 6;
	SegmentTree<OpenBrackets> ST(N);
	ST.create(vector<OpenBrackets::Init>(N, OPEN));
	cout << ST.query(0, N-1).open << endl;
}
