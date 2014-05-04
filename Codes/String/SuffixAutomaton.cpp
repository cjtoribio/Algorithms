struct SuffixAutomaton {
	struct Node {
		int link;
		map<char, int> next;
		int len;
		int dp_num_of_substr, dp_len_of_substr;
		int cnt;
		Node() {
			dp_num_of_substr = dp_len_of_substr = -1;
			link = -1;
			next = map<char, int>();
			len = cnt = 0;
		}
	};
	vector<Node> V;
	SuffixAutomaton(const string &S) {
		// cout <<"----> "<< S << endl;
		V.clear();
		V.push_back(Node());
		Node tmp;
		int last = 0;
		for (int i = 0; i < S.size(); ++i) {
			char c = S[i];
			int curr = V.size();
			V.push_back(Node());
			V.back().cnt = 1;
			V[curr].len = V[last].len + 1;
			int p;
			for (p = last; p != -1 && !V[p].next.count(c); p = V[p].link)
				V[p].next[c] = curr;
			if (p == -1) {
				V[curr].link = 0;
			} else {
				int q = V[p].next[c];
				if (V[p].len + 1 == V[q].len) {
					V[curr].link = q;
				} else {
					int clone = V.size();
					V.push_back(Node());
					V[clone].len = V[p].len + 1;
					V[clone].next = V[q].next;
					V[clone].link = V[q].link;
					for (; p != -1 && V[p].next[c] == q; p = V[p].link)
						V[p].next[c] = clone;
					V[q].link = V[curr].link = clone;
				}
			}
			last = curr;
		}
	}
	void computeOcurrences() {
		int ma = 0;
		for (int i = 0; i < V.size(); ++i)
			ma = max(ma, V[i].len);
		vector<vector<int> > TEMP(ma + 1, vector<int>());
		for (int i = 0; i < V.size(); ++i)
			TEMP[V[i].len].push_back(i);
		for (int i = ma; i >= 1; --i)
			for (int j = 0; j < TEMP[i].size(); ++j)
				V[V[TEMP[i][j]].link].cnt += V[TEMP[i][j]].cnt;
	}
	bool check(const string &P) {
		int v = 0;
		for (int i = 0; i < P.size(); ++i) {
			if (V[v].next.count(P[i]) == 0)
				return false;
			else
				v = V[v].next[P[i]];
		}
		return true;
	}
	int numberOfSubstrings(int v = 0) {
		if (V[v].next.size() == 0)
			return 1;
		if (V[v].dp_num_of_substr != -1)
			return V[v].dp_num_of_substr;
		int w = v != 0;
		for (map<char, int>::iterator it = V[v].next.begin();
				it != V[v].next.end(); ++it)
			w += numberOfSubstrings(it->second);
		return V[v].dp_num_of_substr = w;
	}
	int totalLengthOfSubstrings(int v = 0, int l = 0) {
		if (V[v].next.size() == 0)
			return 0;
		if (V[v].dp_len_of_substr != -1)
			return V[v].dp_len_of_substr;
		int w = 0;
		for(map<char,int>::iterator it = V[v].next.begin();
				it != V[v].next.end(); ++it)
		w += totalLengthOfSubstrings(it->second) + numberOfSubstrings(it->second);
		return V[v].dp_len_of_substr = w;
	}
	int numberOfOcurrences(const string &P) {
		int v = 0;
		for (int i = 0; i < P.size(); ++i)
			if (V[v].next.count(P[i]) == 0)
				return 0;
			else
				v = V[v].next[P[i]];
		return V[v].cnt;
	}
	int numberRotationOcurrences(const string &P) {
		string P2 = P + P;
		int v = 0;
		for (int i = 0; i < P.size(); ++i)
			if (V[v].next.count(P[i]) == 0)
				return 0;
			else
				v = V[v].next[P[i]];
		return V[v].cnt;
	}
};