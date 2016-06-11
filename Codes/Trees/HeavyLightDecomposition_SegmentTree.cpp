#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <cassert>
#include <cmath>
using namespace std;
typedef long long Long;

struct NodeInit {
	Long l;
	NodeInit(Long l=0){
		this->l = l;
	}
};
struct Node
{
	Long st, en, ma, tot, carry , sz;
	bool HasCarry;
	Node(){
		st = en = ma = tot = 0;
		this->carry = 0;
		this->sz = 0;
		HasCarry = 0;
	}
	Node(NodeInit n) {
		st = en = ma = max(0LL,n.l);
		tot = n.l;
		this->carry = 0;
		this->sz = 1;
		HasCarry = 0;
	}
	Node operator+(const Node &N)const {
		Node ret;
		ret.st = max(st, tot + N.st);
		ret.en = max(N.en, en + N.tot);
		ret.tot= tot + N.tot;
		ret.ma = max( max(ma,N.ma), en + N.st);
		
		ret.carry = 0;
		ret.HasCarry = 0;
		ret.sz = this->sz + N.sz;
		return ret;
	}
	Node flip(){
		Node n;
		n.en = st;
		n.st = en;
		n.ma = ma;
		n.tot = tot;
		n.sz = sz;
		return n;
	}
	void update(Long val)
	{
		
	}
	void clear(){
		this->HasCarry = 0;
		this->carry = 0;
	}
};
struct SegmentTree
{
	vector<Node> V;
	int N;
	SegmentTree(int N)
	{
//		this->V = new Node[4 * N];
		this->V = vector<Node>(4*N);
		this->N = N;
	}
//	~SegmentTree(){ delete [] this->V; }
	void create(vector<NodeInit> &VEC,int n = 1,int b = 0,int e = -1)
	{
		if(e == -1)e = N - 1;
		if(b == e){
			V[n] = Node(VEC[b]);
		}
		else
		{
			create(VEC,2*n,b,(e+b)/2);
			create(VEC,2*n+1,(e+b)/2+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}
	}
	Node query(int i,int j, int n = 1,int b = 0,int e = -1)
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
	void update(int i,int j,Long v,int n = 1,int b=0,int e=-1)
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


typedef vector<long long> VI;
typedef vector<VI> VVI;
struct HLD
{
	VI P , LVL , SZ , PC, DIS;
	VVI adj, dis;
	VVI groups;
	VI groupId;
	VI groupPos;
	vector<SegmentTree> ST;
	int N;
	HLD(VVI adj, VVI dis) {
		N = adj.size();
		P = PC = VI(N , -1);
		LVL = DIS = SZ = groupId = groupPos = VI(N , 0);
		this->adj = adj;
		this->dis = dis;
		for(int i = 0; i < N; ++i)
		{
			if(SZ[i] == 0)
			{
				DIS[i] = 0;
				initDfs(i);
				groups.push_back(VI());
				for(int j = i; j != -1; j = PC[j])
					groups.back().push_back(j) ,
					groupId[j] = groups.size() - 1 ,
					groupPos[j] = groups.back().size()-1;
				
				ST.push_back(SegmentTree(groups.back().size()));
				vector<NodeInit> N_INIT;
				for(int j = 0; j < groups.back().size(); ++j){
					int n = groups.back()[j];
//					cout << DIS[n] << " ";
					N_INIT.push_back(NodeInit( DIS[n] ));
				}
//				cout << endl;
				ST.back().create(N_INIT);
			}
		}
	}
	void initDfs(int u,int level = 0, Long d = 0)
	{
		LVL[u] = level;
		DIS[u] = d;
		SZ[u] = 1;
		for(int i = 0; i < adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(SZ[v])continue;
			P[v] = u;
			initDfs(v,level + 1, dis[u][i]);
			SZ[u] += SZ[v];
		}
		for(int i = 0; i < adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(v == P[u])continue;
			if(2*SZ[v] > SZ[u]-1){
				PC[u] = v;
			}
			else
			{
				groups.push_back(VI());
				for(int j = v; j != -1; j = PC[j])
					groups.back().push_back(j) ,
					groupId[j] = groups.size() - 1 ,
					groupPos[j] = groups.back().size()-1;
				ST.push_back(SegmentTree(groups.back().size()));
				vector<NodeInit> N_INIT;
				for(int j = 0; j < groups.back().size(); ++j){
					int n = groups.back()[j];
					N_INIT.push_back(NodeInit( DIS[n] ));
				}
				ST.back().create(N_INIT);
			}
		}
	}
	int LCA(int u,int v)
	{
		while(u != v && u != -1 && v != -1)
		{
			if(LVL[v] < LVL[u])swap(u,v);
			if(groupId[u] == groupId[v])return u;
			bool vGoesUp = LVL[groups[groupId[u]][0]] < LVL[groups[groupId[v]][0]];
			(vGoesUp ? v : u) = P[ groups[groupId[(vGoesUp ? v : u)]][0] ];
		}
		return u == v ? v : -1;
	}
	Node path(int u,int v,int debug = 0)
	{
		Node A, B;
		while(u != v && u != -1 && v != -1)
		{
			if(groupId[u] == groupId[v]){
				int pu = groupPos[u], pv = groupPos[v];
				if(debug)cout << "same: "<< pu << " " << pv << endl;
				int g = groupId[u];
				if(LVL[u] < LVL[v]){
					B = ST[g].query(pu+1,pv) + B;
				} else {
					A = ST[g].query(pv+1,pu) + A;
				}
				break;
			}
			if(LVL[groups[groupId[u]][0]] < LVL[groups[groupId[v]][0]]){
				// v goes up
				int g = groupId[v], p = groupPos[v];
				B = ST[g].query(0,p) + B;
				if(debug)cout << v << "-->" << P[groups[g][0]] << endl;
				v = P[groups[g][0]];
			} else {
				// u goes up
				int g = groupId[u], p = groupPos[u];
				A = ST[g].query(0,p) + A;
				if(debug)cout << u << "-->" << P[groups[g][0]] << endl;
				u = P[groups[g][0]];
			}
		}
		return A.flip() + B;
	}
};

vector<Long> A;
bool bf(int p, int u, int fv, VVI &adj, VVI &dis){
	if(u == fv)return true;
	for(int i = 0; i < adj[u].size(); ++i){
		int v = adj[u][i];
		if(v == p)continue;
		if(bf(u,v,fv,adj,dis)){
			A.push_back(dis[u][i]);
			return true;
		}
	}
	return false;
}

int main()
{
	int N, M;
	cin >> N >> M;
	VVI adj(N), dis(N);
	for(int i = 0; i < N-1; ++i){
		Long u,v,c;
		cin >> u >> v >> c;
		u--;v--;
		adj[u].push_back(v);
		dis[u].push_back(c);
		
		adj[v].push_back(u);
		dis[v].push_back(c);
	}
	HLD H(adj,dis);
	for(int i = 0; i < M; ++i){
		int u,v;
		cin >> u >> v;
		u--;v--;
		cout << H.path(u,v).ma << endl;
	}
	
}

/*
7 11
1 2 -4
5 4 -2
5 1 10
2 7 8
3 6 7
7 6 9
7 1
1 2
4 3
5 5
3 4
5 7
7 5
1 1
3 7
1 6
2 4


7 10
1 2 -4000000000
5 4 -2000000000
5 1 10000000000
2 7 8000000000
3 6 7000000000
7 6 9000000000
7 1
1 2
4 3
5 5
3 4
5 7
7 5
1 1
3 7
1 6

*/
