template <typename RtrnT, typename... T>
class TFunctor
{
public:

	// two possible functions to call member function. virtual cause derived
	// classes will use a pointer to an object and a pointer to a member function
	// to make the function call
	virtual RtrnT operator()(T...) = 0;  // call using operator
	virtual RtrnT Call(T...) = 0;        // call using function
};


// derived template class
template <typename RtrnT, class TClass, typename... T>
class TSpecificFunctor : public TFunctor<RtrnT, T...>
{
private:
	RtrnT (TClass::*fpt)(T...);   // pointer to member function
	TClass* pt2Object;                  // pointer to object

public:

	// constructor - takes pointer to an object and pointer to a member and stores
	// them in two private variables
	TSpecificFunctor(TClass* _pt2Object, RtrnT(TClass::*_fpt)(T...))
		: pt2Object(_pt2Object), fpt(_fpt) {};

	// override operator "()"
	virtual RtrnT operator()(T... parameters)
	{
		return (*pt2Object.*fpt)(parameters...);
	}

	// override function "Call"
	virtual RtrnT Call(T... parameters)
	{
		return (*pt2Object.*fpt)(parameters...);
	}
};

template <typename RtrnT, typename... T>
class TGeneralFunctor : public TFunctor<RtrnT, T...>
{
private:
	void(*fpt_)(T...);

public:
	TGeneralFunctor(RtrnT(*fpt)(T...))
		: fpt_(fpt) {};

	virtual RtrnT operator()(T...)
	{
		(*fpt_)(T...);
	}

	virtual RtrnT Call(T...)
	{
		(*fpt_)(T...);
	}
};