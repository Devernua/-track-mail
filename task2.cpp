

#include <string>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <map>
#include "task2.hpp"
using namespace std;

void foo(pair<int, string> a)
{
	cout << a.first << " <--> " << a.second << endl;
}

int main()
{
	BiMap<int, string> bm;
	

	bm.insert(make_pair(1, "one"));
	bm.insert(make_pair(2, "one"));//no effects
	bm.insert(make_pair(2, "two"));
	bm[3]="three";// no effect
	bm.insert(make_pair(3, "three"));
	bm.insert(make_pair(4, "four"));
	bm.insert(make_pair(5, "five"));

	cout << (bm.begin())->first << "<-->" << (*bm.begin()).second << endl << endl;

	for_each(bm.begin(), bm.end(), foo);
	cout<<endl;
	//reverse(bm.begin(), bm.end());

	for_each(bm.begin(), bm.end(), foo);


	bm.clear();
    return 0;
}

