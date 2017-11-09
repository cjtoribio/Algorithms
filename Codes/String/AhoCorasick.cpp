/// Offline structure to find multiple strings inside a 
/// long text in O(N+M) where N = |text|, M = sum(|strings|)
struct Node
{
	int parent , fail , output , letter, str , id;
	map<short,int> childs;
	Node(char letter, int id): letter(letter), id(id) {
		str = parent = fail = output = -1;
	}
};
struct AhoCorasik {
	static const int ROOT = 0;
	static const int NONE = -1;
	vector<Node> nodes;
	vector<string> words;
	void create(vector<string> &V) { 
		nodes = vector<Node>(1,Node('*', 0));
		for(string S : V) { 
			int v = ROOT;
			for(int p : S){
				if(!nodes[v].childs.count(p)){
					nodes[v].childs[p] = nodes.size();
					nodes.push_back(Node(p, nodes.size()));
					nodes.back().parent = v;
				}
				v = nodes[v].childs[p];
			}
			nodes[v].str = words.size();
			words.push_back(S);
		}
		queue<int> Q;
		for(auto ch : nodes[ROOT].childs) {
			Node &n = nodes[ch.second];
			n.fail = ROOT;
			for(auto ch2 : n.childs)
					Q.push(ch2.second);
		}
		while(!Q.empty()) {
			int u = Q.front(); Q.pop();
			int p = nodes[nodes[u].parent].fail;
			int letter = nodes[u].letter;
			while(p != ROOT && !nodes[p].childs.count(letter))
				p = nodes[p].fail;
			nodes[u].fail = !nodes[p].childs.count(letter) ? p : nodes[p].childs[letter];
            		nodes[u].output = nodes[nodes[u].fail].str == NONE ? nodes[nodes[u].fail].output : nodes[u].fail;
			for(auto ch2 : nodes[u].childs)
					Q.push(ch2.second);
		}
	}
	/// @param STR: text to find all the strings
	/// @return map of found ids and index where it occurred
	/// @note if word is repeated you will only get the last index
	map< int , vector<int> > find(const string &STR) {
		map< int , vector<int> > RET; 
		int u = ROOT;
		for(int i = 0; i < STR.size(); ++i) {
			int p = STR[i];
			while(u != ROOT && !nodes[u].childs.count(p))
				u = nodes[u].fail;
			if(nodes[u].childs.count(p))
				u = nodes[u].childs[p];
			for(int outP = u ; outP != NONE ; outP = nodes[outP].output) {
				if(nodes[outP].str != NONE) {
					int &S = nodes[outP].str;
					RET[S].push_back(i-words[S].size()+1);
				}
			}
		}
		return RET;
	}
	void print(int v = ROOT, int LVL = 0)
	{
		Node &n = nodes[v];
		Node &f = nodes[v].fail == NONE ? nodes[v] : nodes[nodes[v].fail];
		printf("%s%c|%d,%d\n" , string(LVL,'\t').c_str() , n.letter, n.id, f.id);
		for(auto ch : nodes[v].childs)
			print(ch.second , LVL+1);
	}
};
