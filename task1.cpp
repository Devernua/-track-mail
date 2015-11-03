#include <iostream>
#include "task1.hpp"

using namespace std;

Date::Date( int d, int m , int y )
		:day(d), month(m),year(y)
{
	optimiz();
}

Date::Date ( Date const &d ) 
		: day 	(  	d.get_day()		),
          month ( 	d.get_month()	),
          year 	(	d.get_year()	) 
{
	optimiz();
}

Date::Date ( Date &&d ) 
		: day 	(  	move(d.get_day()) 	),
          month ( 	move(d.get_month())	),
          year 	(	move(d.get_year())	) 
{
	optimiz();
}


Date& Date::operator = ( Date const &d )
{
	day = d.get_day();
	month = d.get_month();
	year = d.get_year();
	optimiz();
	return *this;
}

Date& Date::operator = ( Date &&d )
{
    day  	= move(d.get_day());
    month 	= move(d.get_month());
    year 	= move(d.get_year());
	optimiz();
	return *this;
}




Date Date::operator + ( Date const &y ) const
{
	return Date( day + y.get_day(), month + y.get_month(), year + y.get_year() );
}

Date Date::operator - ( Date const &y ) const
{

	return Date( day - y.get_day(), month - y.get_month(), year - y.get_year() );
}

bool Date::operator < ( Date const &right ) const
{
	bool retVal = false;
	if(this->year <= right.get_year())
	{
		if(this->year < right.year)
		{
			retVal =  true;
		}
		else if(this->month <= right.get_month())
		{
			if(this->month < right.get_month())
			{
				retVal =  true;
			}
			else if(this->day < right.get_day())
			{
				retVal = true;
			}
		}
	}
	else
	{
		return retVal;
	}
	return retVal;
}

bool Date::operator > ( Date const &right) const
{
	bool retVal = false;
	if(this->year >= right.get_year())
	{
		if(this->year > right.get_year())
		{
			retVal =  true;
		}
		else if(this->month >= right.get_month())
		{
			if(this->month > right.get_month())
			{
				retVal =  true;
			}
			else if(this->day > right.get_day())
			{
				retVal = true;
			}
		}
	}
	else
	{
		return retVal;
	}
	return retVal;
}

bool Date::operator <= ( Date const &right) const
{
	bool retVal = true;
	if(this->year > right.get_year())
	{
		retVal = false;
	}
	else if(this->month > right.get_month())
	{
		retVal = false;
	}
	else if(this->day > right.get_day())
	{
		retVal = false;
	}
	return retVal;
}

bool Date::operator >= ( Date const &right) const
{
	bool retVal = true;
	if(this->year < right.get_year())
	{
		retVal = false;
	}
	else if(this->month < right.get_month())
	{
		retVal = false;
	}
	else if(this->day < right.get_day())
	{
		retVal = false;
	}
	return retVal;
}

bool Date::operator == ( Date const &right) const
{
	bool retVal = false;
	if(this->year == right.get_year() && this->month == right.get_month() && this->day == right.get_day())
	{
		retVal = true;
	}
	return retVal;
}

bool Date::operator != ( Date const &right) const
{
	bool retVal = true;
	if(this->year == right.get_year() && this->month == right.get_month() && this->day == right.get_day())
	{
		retVal = false;
	}
	return retVal;
}

int Date::get_day()	const
{
	return day;
}

int Date::get_month() const
{
	return month;
}

int Date::get_year() const
{
	return year;
}
long Date::days() const
{
	return day + 30 * month + 365 * year;
}
int Date::months() const
{
	return month + 12 * year;
}
int Date::years() const
{
	return year;
}
long Date::minutes() const
{
	return days() * 24 * 60;
}
long Date::seconds() const
{
	return minutes() * 60;
}

void Date::optimiz()
{
	month 	= month + day / 30;
	day 	= day % 30;
	year 	= year + month / 12;
	month 	= month % 12;
}


ostream& operator << ( ostream &out, Date const &d )
{
	out << d.day << '/' << d.month << '/' << d.year;
	return out;
}
 
istream& operator >> ( istream &in, Date &d ) 
{
  char c;
  in >> d.day >> c >> d.month >> c >> d.year;
  return in;
}



