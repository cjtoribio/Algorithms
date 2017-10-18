#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <iostream>
#include <vector>
#include <cassert>
#include <complex>
#include <cstring>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;
typedef long long Long;

typedef vector<int> VI;
typedef vector<Long> VL;
typedef vector<VI> VVI;
struct FragmentedTree {
	VVI adj;
	VVI adj2;
	int N, SQ;
	VI L, P, G, SZ;
	VL V;
	struct Fragment {
		int u, p,maxL, N;
		VI nodes, &L;
		VI lvlFrec;
		Long tot;
		VL &V;
		Fragment(int u, int N, VL &V, VI &L):u(u),N(N),V(V),L(L){
			p = -1;
			tot  = 0;
			maxL = -1;
		}
		void add(int v){
			int nl = L[v] - L[u];
			nodes.push_back(v);
			if(v >= N)return;
			if(nl > maxL){
				maxL = nl;
				lvlFrec.resize(maxL+1);
			}
			lvlFrec[nl]++;
		}
		void update(int lvl, Long v){
			int nl = lvl - L[u];
			if(nl < 0 || nl > maxL)return;
			tot += v * lvlFrec[nl];
		}
		Long query(){
			return tot;
		}
	};
	vector<Fragment> frags;
	FragmentedTree(int N):N(N),adj(N),P(N,-1),L(N),G(N,-1),V(N),SZ(N){
		SQ = max((int)sqrt(N),2);
	}
	void addEdge(int u,int v){
		adj[u].push_back(v);
	}
	Long getSum(int u){
		if(leader(u) == u){
			Long w = frags[ G[u] ].query();
			for(int g : adj2[ G[u] ]){
				w += getSum( frags[g].u );
			}
			return w;
		}else{
			Long w = V[ L[u] ];
			for(int v : adj[u]){
				w += getSum(v);
			}
			return w;
		}
	}
	void update(int LVL, Long v){
		V[LVL] += v;
		for(Fragment &f : frags){
			f.update(LVL, v);
		}
	}
	int createNode(int u, VI nex){
		G.push_back(-1);
		P.push_back(u);
		L.push_back(u <= 0 ? 0 : L[u]+1);
		int nn = adj.size();
		adj.push_back(nex);
		return nn;
	}
	void dfsL(int u, int LVL = 0){
		L[u] = LVL;
		SZ[u] = 1;
		for(int v : adj[u]){
			dfsL(v, LVL+1);
			SZ[u] += SZ[v];
		}
	}
	void buildFragment(int u, Fragment &F){
		if(G[u] != -1)return;
		G[u] = frags.size() - 1;
		F.add(u);
		for(int v : adj[u]){
			buildFragment(v, F);
		}
	}
	void fragment(){
		dfsL(0);
		VVI nodesByLevel(N);
		for(int i = 0; i < N; ++i)
			nodesByLevel[ N-1-L[i] ].push_back(i);
		for(VI p : nodesByLevel){
			for(int u : p){
				vector<int> toF;
				VI temp, child = adj[u], spChild;
				int sz = 0;
				SZ[u] = 1;
				for(int v : child){
					SZ[u] += SZ[v];
				}
				for(int v : child){
					sz += SZ[v];
					temp.push_back(v);
					if(sz > SQ){
						int nn = createNode(u, temp);
						temp.clear();
						spChild.push_back(nn);
						frags.push_back(Fragment(nn, N, V, L));
						buildFragment(nn, frags.back());
						SZ[u] -= sz;
						sz = 0;
					}
				}
				adj[u] = temp;
				for(int v : spChild)
					adj[u].push_back(v);
			}
		}
		int nn = createNode(-1, VI(1,0));
		frags.push_back(Fragment(nn, N, V, L));
		buildFragment(nn, frags.back());
		adj2 = VVI(frags.size());
		for(int i = 0; i < frags.size(); ++i){
			Fragment &f = frags[i];
			if(P[f.u] == -1)continue;
			f.p = G[ P[f.u] ];
			adj2[ f.p ].push_back(i);
		}
	}
	int leader(int u){
		return frags[G[u]].u;
	}
	int lca(int u,int v){
		while(leader(u) != leader(v)){
			if(L[ leader(u) ] > L[ leader(v)]){
				u = P[ leader(u) ];
			}else{
				v = P[ leader(v) ];
			}
		}
		while(u != v){
			
			if(L[v] > L[u]){
				v = P[v];
			}else{
				u = P[u];
			}
		}
		return u;
	}
};

int main(){
	
	int N,M;
	scanf("%d%d", &N, &M);
	int fu = -1, fv = -1;
	FragmentedTree FT(N);
	for(int i = 0; i < N-1; ++i){
		int u,v;
		scanf("%d%d", &u, &v);
		u--;v--;
		if(fu == -1){
			fu = u;
			fv = v;
		}
		FT.addEdge(u,v);
	}
	FT.fragment();
	for(int i = 0; i < M; ++i){


		int c;
		scanf("%d", &c);
		if(c == 1){
			int L,Y;
			scanf("%d%d", &L, &Y);
			FT.update(L, Y);
		}else{
			int X;
			scanf("%d", &X);
			cout << FT.getSum(X) << endl;
		}
	}
	
}


/*
5 4
1 2
1 3
3 4
3 5
1 1 2
1 2 3
2 3
2 1
 0 
1  2
  3 4
 */