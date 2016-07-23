struct CountMST{
	struct Edge {
		int u,v,w;
		bool operator<(const Edge &C)const{
			return w < C.w;
		}
	};
	int N;
	vector<Edge> E;
	CountMST(int N):N(N){ }
	void addEdge(int u,int v,int w){
		E.push_back((Edge){u,v,w});
	}
	mint countMST(){
		sort(E.begin(),E.end());
		// exists MST
		{
			DisjointSet DS(N);
			for(Edge &e : E){
				DS.join(e.u, e.v);
			}
			if(DS.size() > 1){
				return 0;
			}
		}
		
		mint r = 1;
		// count
		{
			DisjointSet DS(N);
			for(int i = 0; i < E.size(); ){
				vector<Edge> S;
				{// put equal edges
					int j = i;
					while(j < E.size() && E[i].w == E[j].w){
						S.push_back(E[j]);
						j++;
					}
					i = j;
				}
				for(Edge &e : S){
					e.u = DS.find(e.u);
					e.v = DS.find(e.v);
				}
				for(Edge &e : S){
					DS.join(e.u, e.v);
				}
				vector< pair<int,Edge> > G;
				for(Edge &e : S){
					if(e.u != e.v){
						G.push_back(make_pair(DS.find(e.u), e));
					}
				}
				sort(G.begin(),G.end());
				for(int j = 0; j < G.size(); ){
					vector<Edge> ee;
					{// put all same node
						int k = j;
						while(k < G.size() && G[k].first == G[j].first){
							ee.push_back(G[k].second);
							k++;
						}
						j = k;
					}
					vector<int> NODS;
					for(Edge &e : ee){
						NODS.push_back(e.u);
						NODS.push_back(e.v);
					}
					sort(NODS.begin(),NODS.end());
					NODS.resize(unique(NODS.begin(),NODS.end())- NODS.begin());
					CountSpanningTree CS(NODS.size());
					for(Edge &e : ee){
						int u = lower_bound(NODS.begin(),NODS.end(),e.u)-NODS.begin();
						int v = lower_bound(NODS.begin(),NODS.end(),e.v)-NODS.begin();
						CS.addEdge(u,v);
					}
					r = r * CS.run();
					
				}
				
			}
		}
		return r;
	}
};