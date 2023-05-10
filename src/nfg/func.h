/********************************************************************
** Filename: func.h
**
** Description: Function templates.
*/

#pragma once

#ifndef _FUNC_H_
#define _FUNC_H_

#include "utils.h"
#include "global.h"

#include <memory>


// In order to use one class to stand for all other templates.
class BaseFunc
{
public:
	virtual ~BaseFunc() {}
	virtual flp_t Evaluate(flp_t x) = 0;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Mono Function
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
template<typename _Func>
class MonoFunc : public BaseFunc
{
public:
	MonoFunc(const _Func& func) : _func(func) {}
	virtual ~MonoFunc() {}

	flp_t Evaluate(flp_t x) { return _func(x); }

private:
	_Func _func;
};

template<>
class MonoFunc<func_t> : public BaseFunc
{
public:
	MonoFunc(func_t func) { _func = func; }
	virtual ~MonoFunc() {}

	flp_t Evaluate(flp_t x) { return _func(x); }

private:
	flp_t(*_func)(flp_t);
};

template<>
class MonoFunc<disc_poly_t> : public BaseFunc
{
public:
	MonoFunc(const disc_poly_t& func) : _func(func) {}
	virtual ~MonoFunc() {}

	flp_t Evaluate(flp_t x)
	{
		return EvaluateDiscAsCont(_func, float_to_fixed(x, fxp_f));
	}

private:
	disc_poly_t _func;
};

template<>
class MonoFunc<cont_poly_t> : public BaseFunc
{
public:
	MonoFunc(const cont_poly_t& func) : _func(func) {}
	virtual ~MonoFunc() {}

	flp_t Evaluate(flp_t x) { return EvaluateCont(_func, x); }

private:
	cont_poly_t _func;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Segment Function
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

// Segment function DOES NOT validate if funcs and divs segment number matches.

template<typename _Func>
class SegmentFunc : public BaseFunc
{
public:
	SegmentFunc(const std::vector<_Func>& funcs,
				const std::vector<flp_t>& divs) :
		_funcs(funcs), _divs(divs)
	{
		// Hope there's no errors... :(
	}

	flp_t Evaluate(flp_t x)
	{
		int i = 0;
		while (i < _divs.size())
		{
			if (x < _divs[i])
				break;
		}
		i = std::min(i, (int)(_funcs.size() - 1));
		return _funcs[i](x);
	}

private:
	std::vector<_Func> _funcs;
	std::vector<flp_t> _divs;
};

#if 0	// Cannot pass compilation... :(
template<>
class SegmentFunc<func_t> : public BaseFunc
{
public:
	SegmentFunc(const std::vector<func_t>& funcs,
				const std::vector<flp_t>& divs) :
		_divs(divs)
	{
		// Hope there's no errors... :(
		for (int i = 0; i < funcs.size(); i++)
			_funcs.push_back(funcs[i]);
	}

	flp_t Evaluate(flp_t x)
	{
		int i = 0;
		while (i < _divs.size())
		{
			if (x < _divs[i])
				break;
		}
		i = std::min(i, (int)(_funcs.size() - 1));
		return _funcs[i](x);
	}

private:
	std::vector<func_t> _funcs;
	std::vector<flp_t> _divs;
};

#endif

template<>
class SegmentFunc<disc_poly_t> : public BaseFunc
{
	using _Func = disc_poly_ptr;
public:
	SegmentFunc(const std::vector<_Func>& funcs,
				const std::vector<flp_t>& divs) :
		_funcs(funcs), _divs(divs)
	{
		// Hope there's no errors... :(
	}

	flp_t Evaluate(flp_t x)
	{
		int i = 0;
		while (i < _divs.size())
		{
			if (x < _divs[i])
				break;
		}
		i = std::min(i, (int)(_funcs.size() - 1));
		return EvaluateDiscAsCont(_funcs[i], float_to_fixed(x, fxp_f));
	}

private:
	std::vector<_Func> _funcs;
	std::vector<flp_t> _divs;
};

template<>
class SegmentFunc<cont_poly_t> : public BaseFunc
{
	using _Func = cont_poly_ptr;
public:
	SegmentFunc(const std::vector<_Func>& funcs,
				const std::vector<flp_t>& divs) :
		_funcs(funcs), _divs(divs)
	{
		// Hope there's no errors... :(
	}

	flp_t Evaluate(flp_t x)
	{
		int i = 0;
		while (i < _divs.size())
		{
			if (x < _divs[i])
				break;
		}
		i = std::min(i, (int)(_funcs.size() - 1));
		return EvaluateCont(_funcs[i], x);
	}

private:
	std::vector<_Func> _funcs;
	std::vector<flp_t> _divs;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Binary Operator
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class BinaryOperator
{
public:
	virtual flp_t Calc(const flp_t& lhs, const flp_t& rhs) = 0;
};

class BinaryAdd : public BinaryOperator
{
public:
	virtual flp_t Calc(const flp_t& lhs, const flp_t& rhs)
	{
		return lhs + rhs;
	}
};

class BinarySub : public BinaryOperator
{
public:
	virtual flp_t Calc(const flp_t& lhs, const flp_t& rhs)
	{
		return lhs - rhs;
	}
};

class BinaryMul : public BinaryOperator
{
public:
	virtual flp_t Calc(const flp_t& lhs, const flp_t& rhs)
	{
		return lhs * rhs;
	}
};

class BinaryDiv : public BinaryOperator
{
public:
	virtual flp_t Calc(const flp_t& lhs, const flp_t& rhs)
	{
		return lhs / rhs;
	}
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Compound Function
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
template<typename _LhsFunc, typename _RhsFunc, typename _Operator>
class CompoundFunc : public BaseFunc
{
public:
	CompoundFunc(const _LhsFunc& _lhs, const _RhsFunc& _rhs, const _Operator& _op) :
		_lhs(_lhs), _rhs(_rhs), _op(_op)
	{
	}

	flp_t Evaluate(flp_t x)
	{
		return _op.Calc(_lhs.Evaluate(x), _rhs.Evaluate(x));
	}

private:
	_LhsFunc _lhs;
	_RhsFunc _rhs;
	_Operator _op;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Func Wrapper
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

typedef std::shared_ptr<BaseFunc> FuncPtr;


#endif
