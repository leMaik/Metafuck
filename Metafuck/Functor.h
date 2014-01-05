template <typename... T>
class TFunctor
{
public:

	// two possible functions to call member function. virtual cause derived
	// classes will use a pointer to an object and a pointer to a member function
	// to make the function call
	virtual void operator()(T...) = 0;  // call using operator
	virtual void Call(T...) = 0;        // call using function
};


// derived template class
template <class TClass, typename... T>
class TSpecificFunctor : public TFunctor<T...>
{
private:
	void (TClass::*fpt)(T...);   // pointer to member function
	TClass* pt2Object;                  // pointer to object

public:

	// constructor - takes pointer to an object and pointer to a member and stores
	// them in two private variables
	TSpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)(T...))
		: pt2Object(_pt2Object), fpt(_fpt) {};

	// override operator "()"
	virtual void operator()(T... parameters)
	{
		(*pt2Object.*fpt)(parameters...);
	}

	// override function "Call"
	virtual void Call(T... parameters)
	{
		(*pt2Object.*fpt)(parameters...);
	}
};

template <typename... T>
class TGeneralFunctor : public TFunctor<T...>
{
private:
	void(*fpt_)(T...);

public:
	TGeneralFunctor(void(*fpt)(T...))
		: fpt_(fpt) {};

	virtual void operator()(T...)
	{
		(*fpt_)(T...);
	}

	virtual void Call(T...)
	{
		(*fpt_)(T...);
	}
};