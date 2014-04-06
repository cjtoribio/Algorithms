//============================================================================
// Name        : Codes.cpp
// Author      : Carlos Toribio
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "../DynamicProgramming/InversionCount.cpp"

using namespace std;

int main() 
{

	vector<int> V;
	V.push_back(4);
	V.push_back(2);
	V.push_back(1);
	V.push_back(3);
	cout << InversionCount::inversionCount(V) << endl;
	
	cout << "Compiled !!!" << endl; // prints !!!Hello World!!!
	return 0;
	
	
	
}
