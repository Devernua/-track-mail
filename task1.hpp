#ifndef DATE_H
#define DATE_H

#include <iostream>
using namespace std;

class Date
{

public:
	
	Date ( int d = 0, int m = 0, int y = 0 );
	Date ( Date const & );

	Date& operator =  ( Date const & );
	Date  operator +  ( Date const & ) const;
	Date  operator -  ( Date const & ) const;
	bool  operator <  ( Date const & ) const;
	bool  operator >  ( Date const & ) const;
	bool  operator >= ( Date const & ) const;
	bool  operator <= ( Date const & ) const;
	bool  operator == ( Date const & ) const;
	bool  operator != ( Date const & ) const;

	friend istream& operator >> (istream &, Date &);
	friend ostream& operator << (ostream &, Date const &);

	int 	get_day()	const;
	int 	get_month()	const;
	int 	get_year() 	const;
	long 	days() 	const;
	int 	months() 	const;
	int 	years() 	const;
	long 	minutes() 	const;
	long 	seconds() 	const;

	void 	optimiz();
private:
	
	int day;		
	int month;
	int year;

};

#endif