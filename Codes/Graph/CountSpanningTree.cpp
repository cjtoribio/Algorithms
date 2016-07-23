struct CountSpanningTree {
	typedef vector<mint> VM;
	typedef vector<VM> VVM;
	VVM mat;
	int N;
	CountSpanningTree(int N):N(N){
		mat = VVM(N, VM(N));
	}
	void addEdge(int u, int v){
		mat[u][u] += 1;
		mat[v][v] += 1;
		mat[u][v] -= 1;
		mat[v][u] -= 1;
	}
	mint run(){
		for(int i = 0; i < N; ++i){
			for(int j = i+1; j < N; ++j){
				if(mat[i][i].n == 0)return 0;
				mint F = mat[j][i] / mat[i][i];
				for(int k = 0; k < N; ++k){
					mat[j][k] -= mat[i][k] * F;
				}
			}
		}
		mint r = 1;
		for(int i = 0; i < N-1; ++i){
			r = r * mat[i][i];
		}
		if(mat[N-1][N-1].n != 0)assert(false);
		return r;
	}
};