#ifndef BIMAP_H
#define BIMAP_H

#include <map>
#include <set>
#include <utility>
#include <iterator>

using namespace std;

template<class Key, class T, class Cmp_key=less<Key>, class Cmp_value=less<T>, class A=allocator<pair<const Key, T> > >
class BiMap
{
public:
	typedef Key key_type;
	typedef T mapped_type;

	typedef pair<const Key, T> value_type;
	typedef pair<const T, Key> value_type_right;
	typedef Cmp_key key_compare;
	typedef Cmp_value value_compare;
	typedef A allocator_type;

	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;

	struct iterator
	{
		typedef typename std::map<key_type, mapped_type, key_compare, allocator_type>::iterator Iter;
		iterator() : _iter(){}
		iterator(value_type* p) : _iter(p){}
		iterator(const iterator& other) : _iter(other._iter) {}
		iterator(const Iter& other) : _iter(other) {};
		iterator& operator =(const iterator& other)
   		{
      		if (this != &other){
         		_iter = other._iter;
         	}
      		return *this;
   		}
   		iterator& operator ++()
   		{
   			++_iter;
   			return *this;
   		}
   		iterator operator ++(int)
   		{
   			iterator tmp(*this);
   			++(*this);
   			return tmp;
   		}
   		bool operator == (iterator const& i)
    	{
        	return *(_iter) == *(i._iter);
    	}
    	bool operator != (iterator const& i)
    	{
        	return !(*this == i);
    	}
    	reference operator *() {return *(_iter);}
    	value_type* operator ->() {return &(*_iter);}

    	Iter get_iter()
    	{
    		return _iter;
    	}
    	Iter get_iter() const
    	{
    		return _iter;
    	}
		void change_iter(Iter tmp)
		{
			_iter = tmp;
		}

	private:
	    Iter _iter;
	};

	//типы

	typedef iterator iterator; //tmp
	typedef const iterator const_iterator; //tmp

	typedef typename A::size_type size_type;
	typedef typename A::difference_type difference_type;

	typedef std::reverse_iterator<iterator> reverse_iterator;//tmp
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;//tmp


	//итераторы
	iterator begin()
	{
		iterator tmp;
		tmp.change_iter(_left.begin());
		return tmp;
	}
	const_iterator begin() const
	{
		const_iterator tmp;
		tmp.change_iter(_left.begin());
		return tmp;
	}

	iterator end()
	{
		iterator tmp;
		tmp.change_iter(_left.end());
		return tmp;
	}
	const_iterator end() const
	{
		const_iterator tmp;
		tmp.change_iter( _left.end());
		return tmp;
	}

	reverse_iterator rbegin()
	{
		reverse_iterator tmp;
		tmp.change_iter (_left.rbegin());
		return tmp;
	}

	const_reverse_iterator rbegin() const
	{
		const_reverse_iterator tmp;
		tmp.change_iter = (_left.rbegin());
		return tmp;
	}

	reverse_iterator rend()
	{
		reverse_iterator tmp;
		tmp.change_iter( _left.rend());
		return tmp;
	}

	const_reverse_iterator rend() const
	{
		const_reverse_iterator tmp;
		tmp.change_iter( _left.rend());
		return tmp;
	}



	mapped_type& operator[] (const key_type& k) // доступ к элементу с ключом k
	{
		return _left.find(k)->second;
	}

	//конструкторы
	explicit BiMap(const Cmp_key& ck = Cmp_key(),const Cmp_value& cv = Cmp_value(), const A& a = A()) : _left(ck,a), _right(cv,a) {} //empty
	
	template <class InputIterator>
 	BiMap (InputIterator first, InputIterator last,
    	const key_compare& ck = key_compare(),
    	const value_compare& cv = value_compare(),
    	const allocator_type& a = allocator_type()) : _left(ck,a), _right(cv,a) //range
 	{
 		insert(first, last);
 	}

 	BiMap (const BiMap& x) : _left(x._left), _right(x._right) {} //copy

 	~BiMap() //delete
 	{
 		_left.~map();
 		_right.~map();
 	}

 	BiMap& operator= (const BiMap& x)
 	{
 		_left = x._left;
 		_right = x._right;
 		return *this;
 	}



 	iterator find(const key_type& k) // находит элемент с ключом k
 	{
 		iterator tmp(_left.find(k));
 		return tmp;
 	}
 	const_iterator find(const key_type& k) const
 	{
 		const_iterator tmp(_left.find(k));
 		return tmp;
 	}

 	iterator find_value(const mapped_type& m) // находит элемент по значению m
 	{
 		iterator tmp( _left.find(_right.find(m)->second));
 		return tmp;
 	}
 	const_iterator find_value(const mapped_type& m) const
 	{
 		const_iterator tmp( _left.find(_right.find(m)->second));
 		return tmp;
 	}

 	//size_type count(count key_type& k) const; //находит число элементов с ключом k
 	//iterator lower_bound(const key_type& k); //находит первый элемент с ключом k
 	//const_iterator lower_bound(const key_type& k) const;
 	//iterator upper_bound(const key_type& k); // находит первый элемент с ключом, большим k
 	//const_iterator upper_bound(const key_type& k) const;
 	//pair<iterator, iterator> equal_range(const key_type& k);
 	//pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
	
	pair<iterator,bool> insert (const value_type& val) //вставить пару (key, value)
	{
		if((find(val.first).get_iter() == _left.end())  && (_right.find(val.second) == _right.end())){
			_left.insert(val);
			const value_type_right tmp = make_pair(val.second, val.first);
			_right.insert(tmp);
			return make_pair(find(val.first), true);
		}
		else{
			return make_pair(end(), false);
		}
	}

	iterator insert (iterator position, const value_type& val) //position - подсказка начала поиска
	{
		if(find(val.first) == _left.end() && find_value(val.second) == _right.end()){
			_left.insert(position._iter, val);
			_right.insert(val.swap());
			return find(val.first);
		}
		return end();

	}
	
	template <class InputIterator> //вставить элементы из последовательности
	void 					insert (InputIterator first, InputIterator last)
	{
		while (first != last){
			insert(*first);
			++first;
		}
	}

	void 					erase (iterator pos) //удалить указуемый элемент
	{
		if (find(*pos) != end())
		{
			_left.erase(pos->first);
			_right.erase(pos->second);
		}
	}

	void 					erase (const key_type& k) //удалить элемент с ключом k
	{
		erase(find(k));
	}

	void 					erase (iterator first, iterator last) //удалить диапазон
	{
		while (first != last){
			iterator tmp = ++first;
			erase(first);
			first = tmp;
		}
	}

	void 					erase_value(const mapped_type& m) //удалить элемент со значением m
	{
		erase(find_value(m));
	}

	void 					clear() //удалить все элементы
	{
		_left.clear();
		_right.clear();
	}

	//емкость
	size_type 				size() const //число элементов
	{
		return _left.size();
	}
	size_type 				max_size() const // размер максимально возможного BiMap
	{
		return _left.max_size();
	}

	bool 					empty() const 
	{
		return size() == 0;
	}

	void 					swap (BiMap& x)
	{
		BiMap c(x);
		x = *this;
		*this = c;
	}

private:
	map<key_type, mapped_type, Cmp_key, A> 		_left;
	map<mapped_type, key_type, Cmp_value, A> 	_right;

};

	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator== ( const BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, const BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		return lhs._left == rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator!= ( const BiMap<Key,T,Cmp_key,A>& lhs, const BiMap<Key,T,Cmp_key,A>& rhs )
  	{
  		return lhs._left != rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator< ( const BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, const BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		return lhs._left < rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator<= ( const BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, const BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		return lhs._left <= rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator> ( const BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, const BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		return lhs._left > rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	bool operator>= ( const BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, const BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		return lhs._left >= rhs._left;
  	}

  	template <class Key, class T, class Cmp_key, class Cmp_value, class A>
  	void swap ( BiMap<Key,T,Cmp_key,Cmp_value,A>& lhs, BiMap<Key,T,Cmp_key,Cmp_value,A>& rhs )
  	{
  		BiMap<Key,T,Cmp_key,Cmp_value,A> c(lhs);
  		lhs = rhs;
  		rhs = c;	
  	}



#endif