struct PersistentFenwick {
	struct ValVer{ 
		int val, ver; 
		ValVer(int v = 0, int ver = 0):val(v),ver(ver){}
		ValVer operator+(const ValVer &o)const{
			return ValVer(val + o.val , max(ver, o.ver));
		}
		struct IsBefore{
			bool operator()(const ValVer &v, const int &ver){
				return v.ver < ver;
			}
		};
	};
	int cv;
	vector<vector<ValVer>> V;
	PersistentFenwick(int N) : cv(0), V(N+2, vector<ValVer>(1, {0, 0})){}
	void add(int x, int v){
		cv++;
		for(int i = x+1; i < V.size(); i += i & -i){
			V[i].push_back(V[i].back() + ValVer(v, cv));
		}
	}
	int get(int x, int ver){
		int r = 0;
		for(int i = x+1; i > 0; i -= i & -i){
			r += getX(i, ver);
		}
		return r;
	}
	int getX(int x, int ver){
		int p = lower_bound(V[x].begin(), V[x].end(), ver+1, ValVer::IsBefore()) - V[x].begin();
		p--;
		return V[x][p].val;
	}
	
	void print(){
		for(vector<ValVer> V : V){
			for(ValVer v : V){
				cout << v.val << ":" << v.ver << " ";
			}
			cout << endl;
		}
	}
};