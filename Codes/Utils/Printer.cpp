#include <vector>
#include <cstdio>
#include <iostream>
using namespace std;

struct Printer
{
	template<class T>
	static void print(pair<T,T> P)
	{
		printf("(%d,%d)",P.first,P.second);
	}
	static void print(int n)
	{
		printf("%d",n);
	}
	template<class T>
	static void print(vector<T> V)
	{
		printf("{");
		for(int i = 0; i < V.size(); ++i){
			if(i > 0)printf(", ");
			print(V[i]);
		}
		printf("}\n");

	}
	template<class T>
	static void print(vector<vector<T> > V)
	{
		printf("{\n");
		for(int i = 0; i < V.size(); ++i){
			printf("  [%d]\t",i);
			print(V[i]);
		}
		printf("}\n");
	}
	template<class ITERATOR>
	static void print(ITERATOR begin, ITERATOR end)
	{
		printf("{\n");
		while(begin != end)
		{
			print(*begin);
			printf(", ");
			begin++;
		}
		printf("}\n");
	}
};
