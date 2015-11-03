#include <iostream>
#include "task1.hpp"
using namespace std;

int main()
{
	Date d1(10, 10, 10), d2(50, 50, 50);

	Date d3;
	cin >> d3;
	cout << d3 << endl;
	Date d4(d3);
	Date d5 = move(d2);

	cout << "sum = " 		<< (d1+d2) <<endl;	
	cout << "difference = " << (d1-d2).get_year() << " in year" <<endl;
	cout << d1.seconds() << endl;

	cout << d1 << endl;
	cout << d2 << endl;
	cout << d3 << endl;
	cout << d4 << endl;
	cout << d5 << endl;
	return 0;
}
