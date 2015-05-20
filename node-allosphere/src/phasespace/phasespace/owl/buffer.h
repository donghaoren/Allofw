// buffer.h
// delayed erase buffer

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdexcept>

#include <assert.h>

// this is a subset of std::vector class
// !!! only safe on primitives, do NOT use on classes !!!

namespace std {

template <class T>
class buffer {
public:

  typedef T* iterator;
  typedef const T* const_iterator;
  typedef T& reference;
  typedef const T& const_reference;

protected:

  iterator _start;
  iterator _finish;
  iterator _storage;
  iterator _end_of_storage;
  size_t erase_delay;

public:

  buffer(size_t n, size_t e=-1) : 
    _start(0), _finish(0), _storage(0), _end_of_storage(0), erase_delay(e)
  {
    reserve(n);
    if(e == (size_t)-1) erase_delay = n/2;
  }

  buffer(const buffer &b) : 
    _start(0), _finish(0), _storage(0), _end_of_storage(0), erase_delay(b.erase_delay)
  {
    reserve(b.capacity());
    append(b.begin(), b.end());
  }

  ~buffer() { if(_storage) delete[] _storage; _storage = 0; }

  iterator begin() { return _start; }
  const iterator begin() const { return _start; }
  iterator end() { return _finish; }
  const iterator end() const { return _finish; }

  bool empty() { return begin() == end(); }
  size_t size() const { return end() - begin(); }
  size_t capacity() const { return _end_of_storage - _storage; }
  size_t available() const { return _end_of_storage - _finish; }

  reference operator[](size_t n) { return *(begin() + n); }
  const reference operator[](size_t n) const { return *(begin() + n); }

  buffer &operator=(const buffer &b)
  {
    if(this != &b)
      {
        if(_storage) { delete _storage; _storage = 0; }
        clear();
        reserve(b.capacity());
        append(b.begin(), b.end());
      }
    return *this;
  }

  void append(const_iterator first, const_iterator last)
  {
    if(first > last) return; //throw std::range_error("buffer");
    size_t n = last - first;
    if(n > available()) return; //throw std::out_of_range("buffer");
    memmove(_finish, first, n);
    _finish += n;
  }

  void erase(iterator first, iterator last)
  {
    if(first < _start || last > _end_of_storage) return; //throw std::out_of_range("buffer");

    if(first == _start)
      {
	_start = last;

	// start > half storage
	//if(_start > _storage + (_end_of_storage - _storage) / 2)
	if(_start > _storage + erase_delay)
	  {
	    memmove(_storage, _start, _finish - _start);
	    _finish = _storage + (_finish - _start);
	    _start = _storage;
	  }
      }
    else
      {
	memmove(first, last, _finish - last);
	_finish -= last - first;
      }
  }

  void clear() { _start = _finish = _storage; }

  // increment size 
  size_t operator+=(size_t n)
  {
    if(n > available()) return size(); //throw std::out_of_range("buffer");
    _finish += n;
    return size();
  }

  // remove from front
  size_t operator-=(size_t n)
  {
    // old
    //if(n == size()) clear();
    //else erase(begin(), begin()+n);

    if(n > size()) n = size();

    if(n == size())
      {
	clear();
	return 0;
      }

    _start += n;

    // delayed erase
    if(_start - _storage >= (_end_of_storage - _storage) / 2)
      {
	size_t s = size();
	assert(s);
	memcpy(_storage, _start, s);
	_start = _storage;
	_finish = _storage + s;
      }
    
    return size();
  }

  void reserve(size_t n)
  {
    iterator storage = _storage;
    iterator start = _start, finish = _finish;
    _storage = new T[n];
    _start = _storage;
    _finish = _storage;
    _end_of_storage = _storage + n;
    if(_storage == 0)
      {
	cerr << "error: buffer::reserver(): out of memory" << endl;
	exit(-1);
      }
    if(start && finish) append(start, finish);
    if(storage) delete[] storage;
  }
};

#ifdef OSTREAM
template <class T>
ostream &operator<<(ostream &out, buffer<T> &b)
{
  out << " size=" << b.size() 
      << " avail=" << b.available() 
      << " cap=" << b.capacity();
  return out;
}

#endif // OSTREAM

} // namespace std

#endif // __BUFFER_H__
