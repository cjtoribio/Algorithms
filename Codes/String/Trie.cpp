struct Trie
{
	struct Node
	{
		int childs[26] , cnt;
		Node(){
			cnt = 0;
			memset(childs , -1, sizeof(childs));
		}
	};
	vector<Node> V;
	Trie(int reserved = 0)
	{
		V = vector<Node>();
		V.reserve(reserved);
		V.push_back(Node());
	}
	void insert(const string &S)
	{
		int v = 0;
		V[v].cnt++;
		for(int i = 0; i < S.size(); ++i)
		{
			int p = S[i] - 'a';
			if(V[v].childs[p] == -1){
				V[v].childs[p] = V.size();
				V.push_back(Node());
			}
			v = V[v].childs[p];
			V[v].cnt++;
		}
	}
};