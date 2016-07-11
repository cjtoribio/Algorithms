typedef vector<int> VI;
typedef vector<VI> VVI;
struct Konig {
	vector<int> ML, MR;
	VVI adj;
	int L,R;
	Konig(int L,int R) : ML(L,-1), MR(R,-1), adj(L), L(L), R(R) {}
	void AddEdge(int u, int v){
		adj[u].push_back(v);
	}
	void setMatch(int u, int v){
		ML[u] = v;
		MR[v] = u;
	}
	pair< vector<int> , vector<int> > getCover(){
		queue<int> QL;
		vector<int> VIS_L(L), VIS_R(R);
		for(int i = 0; i < L; ++i){
			if(ML[i] == -1){ 
				QL.push(i);
				VIS_L[i] = 1;
			}
		}
		while(QL.size()){
			int u = QL.front(); QL.pop();
			for(int i = 0; i < adj[u].size(); ++i){
				int v = adj[u][i];
				VIS_R[v] = 1;
				if(MR[v] != -1 && !VIS_L[ MR[v] ]){
					QL.push( MR[v] );
					VIS_L[ MR[v] ] = 1;
				}
			}
		}
		vector<int> AL, AR;
		for(int i = 0; i < L; ++i)
			if(!VIS_L[i]) AL.push_back(i);
		for(int i = 0; i < R; ++i)
			if(VIS_R[i]) AR.push_back(i);
		return make_pair(AL,AR);
	}
};