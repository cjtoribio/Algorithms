#include <vector>
#include <string>
#include <queue>
#include <map>
#include <cstdio>

using namespace std;

struct Node
{
	int parent , fail , output , letter, str , id;
	map<short,int> childs;
	Node(char letter)
	{
		static int genID = 0;
		id = genID++;
		this->letter = letter;
		str = parent = fail = output = -1;
	}
};
struct AhoCorasik
{
#define foreach(it,M) for(typeof((M).begin()) it=(M).begin();it!=(M).end();++it)
	static const int ROOT = 0;
	static const int NONE = -1;
	vector<Node> nodes;
	vector<string> words;
	string& getWord(int n){ return words[n]; }
	template<typename ITERATOR>
	void create(ITERATOR first , ITERATOR second)
	{
		nodes = vector<Node>(1,Node('*'-'a'));
		for(ITERATOR it = first; it < second; ++it)
		{
			string &S = *it;
			int v = ROOT;
			for(int i = 0; i < S.size(); ++i)
			{
				int p = S[i] - 'a';
				if(!nodes[v].childs.count(p)){
					nodes[v].childs[p] = nodes.size();
					nodes.push_back(Node(p));
					nodes.back().parent = v;
				}
				v = nodes[v].childs[p];
			}
			nodes[v].str = words.size();
			words.push_back(*it);
		}
		queue<int> Q;
		foreach(it1,nodes[ROOT].childs)
		{
			Node &n = nodes[it1->second];
			n.fail = ROOT;
			foreach(it2,n.childs)
				Q.push((it2->second));
		}
		while(!Q.empty())
		{
			int u = Q.front(); Q.pop();
			int p = nodes[nodes[u].parent].fail;
			int letter = nodes[u].letter;
			while(p != ROOT && !nodes[p].childs.count(letter))
				p = nodes[p].fail;
			nodes[u].fail = !nodes[p].childs.count(letter) ? p : nodes[p].childs[letter];
			nodes[u].output = nodes[nodes[u].fail].str == NONE ? nodes[u].output : nodes[u].fail;
			foreach(it2,nodes[u].childs)
				Q.push((it2->second));
		}
	}
	map< int , vector<int> > find(const string STR)
	{
		map< int , vector<int> > RET;
		int u = ROOT;
		for(int i = 0; i < STR.size(); ++i)
		{
			int p = STR[i] - 'a';
			while(!nodes[u].childs.count(p))
				u = nodes[u].fail;
			if(!nodes[u].childs.count(p))
				u = nodes[u].childs[p];
			for(int outP = u ; outP != NONE ; outP = nodes[outP].output)
			{
				if(nodes[outP].str != NONE)
				{
					int &S = nodes[outP].str;
					if(RET.count(S))
						RET[S].push_back(i-words[S].size()+1);
					else
						RET[S] = vector<int>(1,i-words[S].size()+1);
				}
			}
		}
		return RET;
	}
	void print(int v = ROOT, int LVL = 0)
	{
		Node &n = nodes[v];
		Node &f = nodes[v].fail == NONE ? nodes[v] : nodes[nodes[v].fail];
		printf("%s%c|%d,%d\n" , string(LVL,'\t').c_str() , n.letter + 'a', n.id, f.id);
		for(int i = 0; i < 26; ++i)
			if(nodes[v].childs[i] != NONE)
				print(nodes[v].childs[i] , LVL+1);
	}
};
