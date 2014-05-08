struct Hungarian
{
	VI assign(VVI mat) 
	{
		int N = mat.size();
		VI R(N , -1) , C(N , -1) , DEL_R(N) , DEL_C(N);
		for(int i = 0; i < N; ++i) // STEP 1
		{
			int mi = 1000000000;
			for(int j = 0; j < N; ++j) mi = min(mi , mat[i][j]);
			for(int j = 0; j < N; ++j) mat[i][j] -= mi;
		}
		for(int j = 0; j < N; ++j) // STEP 2
		{
			int mi = 1000000000;
			for(int i = 0; i < N; ++i) mi = min(mi , mat[j][i]);
			for(int i = 0; i < N; ++i) mat[j][i] -= mi;
		}
		while(true)
		{
			queue<int> Q;
			for(int i = 0; i < N; ++i)
			{
				for(int j = 0; j < N; ++j)
				{
					if(mat[i][j] == 0 && C[j] == -1)
					{
						C[j] = i;
						R[i] = j;
						break;
					}
				}
				if(R[i] == -1)Q.push(i);
			}
			VI MARK_C(N);
			if(Q.size() == 0)return C;
			while(Q.empty())
			{
				int i = Q.front(); Q.pop();
				for(int j = 0; j < N; ++j)
				{
					if(C[j] != -1 && mat[i][j] == 0)
					{
						Q.push(R[i]);
						R[C[j]] = -1; 
						C[j] = -1;
						MARK_C[j] = 1;
					}
				}
			}
			int mi = 1000000000;
			for(int i = 0; i < N; ++i)
				for(int j = 0; j < N; ++j)
					if(R[i] == -1 && !MARK_C[j])
						mi = min(mi , mat[i][j]);
			for(int i = 0; i < N; ++i)
				for(int j = 0; j < N; ++j)
					if(R[i] != -1 && MARK_C[j])
						mat[i][j] += mi;
					else if(R[i] == -1 || !MARK_C[j])
						mat[i][j] -= mi;
		}
		return VI(N);
	}
};