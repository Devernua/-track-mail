#include <iostream>
#include "task1.hpp"
using namespace std;

int main()
{
	Date d1(10, 10, 10), d2(50, 50, 50);

	Date d3;
	cin >> d3;
	cout << d3 << endl;
	cout << "sum = " 		<< (d1+d2) <<endl;	
	cout << "difference = " << (d1-d2).get_year()      << " in year"      <<endl;
	cout << d1.seconds() << endl;
	return 0;
}
