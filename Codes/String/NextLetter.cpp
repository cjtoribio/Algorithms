
struct NextLetter {
	vector<vector<int>> NEXT;
	static int serialize(char n){
		if('0' <= n && n <= '9')return n - '0';
		if('a' <= n && n <= 'z')return n - 'a' + 10;
		if('A' <= n && n <= 'Z')return n - 'A' + 36;
		return 0;
	}
	NextLetter(const string &s) {
		string ts;
		for (int i = 0; i < s.size(); ++i) {
			if('0' <= s[i] && s[i] <= '9')
				ts += s[i];
		}
		NEXT = vector<vector<int>>(ts.size()+1, vector<int>(10, -1));
		vector<int> L(10, -1);
		for (int i = ts.size()-1; i >= 0; --i) {
			L[serialize(ts[i])] = i;
			for(int j = 0; j < 10; ++j) {
				NEXT[i][j] = L[j];
			}
		}
	}
	bool search(const string &s) {
		int id = 0;
		for(char c : s){
			id = NEXT[id][serialize(c)];
			if(id == -1)return false;
			id++;
		}
		return true;
	}
};