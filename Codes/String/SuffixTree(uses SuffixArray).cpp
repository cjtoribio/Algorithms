struct SuffixTree {
	struct Node {
		Node* child[26];
		Node* parent;
		int st, en;
		Node(int st, int en):st(st),en(en){
			memset(child, 0, 26 * sizeof(Node*));
			parent = NULL;
		}
		int size(){
			return en - st + 1;
		}
	};
	Node* root;
	string S;
	int N;
#define decode(ch) (ch=='$' ? 0 : (ch-'a' + 1))
	SuffixTree(const string &str):S(str + "$"){
		N = S.size();
		root = new Node(0, -1);
		SuffixArray SA(S);
		SA.BuildLCP();
		Node* act = root;
		for(int i = 0; i < SA.N; ++i){
			int st = SA.SA[i];
			int en = SA.N-1;
			Node* newNode = new Node(st, en);
			if(i == 0){
				act->child[ decode(S[st]) ] = newNode;
				newNode->parent = act;
				act = newNode;
			}else{
#define len(i) (N - i)
				int skip = len(SA.SA[i-1]) - SA.LCP[i];
				newNode->st += SA.LCP[i];
				while(skip > 0 && skip >= act->size()){
					skip -= act->size();
					act = act->parent;
				}
				if(skip == 0){
					act->child[ decode(S[st]) ] = newNode;
					newNode->parent = act;
					act = newNode;
				}else{
					int sp = act->size() - skip;
					act = split(act, sp);

					act->child[ decode(S[newNode->st]) ] = newNode;
					newNode->parent = act;
					act = newNode;
				}
			}
		}
	}
	Node* split(Node *v , int sz){
		int newEnd = v->st + sz - 1;
		int newStart = newEnd + 1;
		Node* newNode = new Node(v->st, newEnd);
		newNode->parent = v->parent;
		newNode->parent->child[ decode(S[newNode->st]) ] = newNode;
		newNode->child[ decode(S[newStart]) ] = v;
		v->st = newStart;
		v->parent = newNode;
		return newNode;
	}
	void print(ostream &out){
		print(out, root);
	}
	void print(ostream &out, Node *v, int LVL = 0){
		out << string(LVL, '\t') << S.substr(v->st, v->size()) << endl;
		for (int i = 0; i < 26; ++i) {
			if(v->child[i])
				print(out, v->child[i], LVL+1);
		}
	}
#undef decode
};