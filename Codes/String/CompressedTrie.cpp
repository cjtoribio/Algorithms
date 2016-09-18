struct CompressedTrie {
	vector<string> S;
	struct Node{
		int i,j,sz;
		int child[26], w;
		Node(int i=0,int j=0,int sz=0):i(i),j(j),sz(sz){
			w = 0;
			memset(child,-1,sizeof(child));
		}
	};
	vector<Node> V;
	int createNode(int i,int j, int sz){
		int id = V.size();
		V.push_back(Node(i,j,sz));
		V[id].w++;
		return id;
	}
	CompressedTrie():S(1,""),V(1){ }
	void insert(const string &s){
		S.push_back(s);
		insert(S.size()-1,0,0);
	}
	int insert(int i, int j, int v){
		if(v == -1){
			return createNode(i, j, S[i].size()-j);
		}else{
			Node &av = V[v];
			int k = 0;
			while(k < av.sz && j+k < S[i].size()  && S[av.i][av.j+k] == S[i][j+k])
				k++;
			if(k < av.sz){
				int old = v;
				v = createNode(av.i,av.j,k);
				int p = S[av.i][av.j+k] - 'a';
				V[v].child[p] = old;
				V[old].sz -= k;
				V[old].j  += k;
			}
			if(j+k < S[i].size()){
				int p = S[i][j+k]-'a';
				V[v].child[p] = insert(i, j+k, V[v].child[p]);
			}else{
				V[v].w++;
			}
			return v;
		}
	}
	int search(string s){
		Node *v = &V[0];
		for(int i = 0; i < s.size(); ){
			int p = s[i] - 'a';
			if(v->child[p] == -1)return 0;
			v = &V[v->child[p]];
			int j = 0;
			while(i < s.size() && j < v->sz && s[i]==S[v->i][v->j+j]){
				j++;i++;
			}
			if(i == s.size() && j == v->sz){
				cout << v->w << endl;
				return v->w;
			}else if(i == s.size()){
				return 0;
			}
		}
	}
	void print(int v = 0,int LVL = 0){
		Node &av = V[v];
		cout << string(LVL,'\t')<< "'" << S[av.i].substr(av.j,av.sz) << "'" << endl;
		for(int i = 0; i < 26; ++i){
			if(V[v].child[i] != -1)
				print(V[v].child[i], LVL+1);
		}
	}
};