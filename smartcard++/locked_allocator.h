#include <memory>

template <class T> class locked_allocator;

//void specialization
template <>class locked_allocator<void> {
public:
  typedef void*       pointer;
  typedef const void* const_pointer;
  // reference-to-void members are impossible
  typedef void        value_type;

  template <class U>
  struct rebind
  {
	 typedef locked_allocator<U> other;
  };
};

template<class T>
class locked_allocator
{
public:
	typedef T			value_type;
	typedef T    *pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	pointer	adress(reference x) const
	{
		return &x;
	}
	const_pointer	adress(const_reference x) const
	{
		return &x;
	}
	void	construct(pointer p, const T &val)
	{
		new ((void*)p) T(val);
	}
	void	destroy(pointer p)
	{
		p->T::~T();
	}

	explicit locked_allocator() 
	{
	}
	locked_allocator(const locked_allocator<T> &other) 
	{
	}
	~locked_allocator()
	{
	}

	pointer	allocate(size_type n, void* hint= NULL)
	{
		return ((T *)::operator new(n * sizeof (T)));
	}
	void deallocate(void *p, size_type n)
	{
		return;
	}

	template <class U>
	struct rebind {
		typedef locked_allocator<U> other;
	};

	size_type max_size () const
	{
		std::allocator<T> al;
		return al.max_size();
	}

};

