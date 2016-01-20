#pragma once

#include "intrinsic.h"

template <typename base_t, int intshift>
struct fixed_t;

template <int intshift>
struct fixed_t<int32_t, intshift>
{
	fixed_t(const int32_t& value)
	: _value(value)
	{
	}

	fixed_t()
	{
	}

	/*fixed_t(const double& value)
	: _value(round(value * (1ULL<<(31-intshift))))
	{
	}*/

	int32_t get() const
	{
		return _value;
	}
	
	/*double getdouble() const
	{
		return (double)_value * (1.0 / (1ULL << (31-intshift)));
	}*/

	int shift() const
	{
		return intshift;
	}

	template <int otherintshift>
	operator fixed_t<int32_t, otherintshift> () const
	{
		return fixed_t<int32_t, otherintshift> (_value << (intshift - otherintshift));
	}

	int32_t _value;
};

template <int intshift>
fixed_t<int32_t, intshift>
operator- (const fixed_t<int32_t, intshift>& value)
{
	return fixed_t<int32_t, intshift> (-value.get());
}

template <int leftintshift, int rightintshift>
fixed_t<int32_t, leftintshift>
operator+ (const fixed_t<int32_t, leftintshift>& left,
	   const fixed_t<int32_t, rightintshift>& right)
{
	if (leftintshift <= rightintshift) {
		return fixed_t<int32_t, leftintshift> (
			__SQADD(left.get(), (right.get() << (rightintshift - leftintshift))));
	}
	else {
		return fixed_t<int32_t, leftintshift> (
			__SQADD(left.get(), (right.get() >> (leftintshift - rightintshift))));
	}
}

template <int leftintshift, int rightintshift>
fixed_t<int32_t, leftintshift>
operator- (const fixed_t<int32_t, leftintshift>& left,
	   const fixed_t<int32_t, rightintshift>& right)
{
	if (leftintshift <= rightintshift) {
		return fixed_t<int32_t, leftintshift> (
			__SQSUB(left.get(), (right.get() << (rightintshift - leftintshift))));
	}
	else {
		return fixed_t<int32_t, leftintshift> (
			__SQSUB(left.get(), (right.get() >> (leftintshift - rightintshift))));
	}
}

template <int leftintshift, int rightintshift>
fixed_t<int32_t, leftintshift+rightintshift+1>
operator* (const fixed_t<int32_t, leftintshift>& left,
	   const fixed_t<int32_t, rightintshift>& right)
{
	return fixed_t<int32_t, leftintshift+rightintshift+1> (
			smmul(left.get(), right.get()) );
}

template <int intshift>
fixed_t<int32_t, intshift>
operator* (const fixed_t<int32_t, intshift>& left,
	       const int32_t& right)
{
	return fixed_t<int32_t, intshift> (left.get() * right);
}

template <int shiftleft, int intshift>
fixed_t<int32_t, intshift - shiftleft>
leftscale (const fixed_t<int32_t, intshift>& value)
{
	return fixed_t<int32_t, intshift - shiftleft> (value.get() << shiftleft);
}

template <int shiftright, int intshift>
fixed_t<int32_t, intshift + shiftright>
rightscale (const fixed_t<int32_t, intshift>& value)
{
	return fixed_t<int32_t, intshift + shiftright> (value.get() >> shiftright);
}

template <int shiftleft, int intshift>
fixed_t<int32_t, intshift - shiftleft>
scaleshift (const fixed_t<int32_t, intshift>& value,
			const int& valueshiftleft)
{
	if (valueshiftleft - shiftleft >= 0) {
		return fixed_t<int32_t, intshift - shiftleft> (value.get() << (valueshiftleft + shiftleft));
	}
	else {
		return fixed_t<int32_t, intshift - shiftleft> (value.get() << (shiftleft + valueshiftleft));
	}
}

template <int intshift>
fixed_t<int32_t, intshift>
operator<< (const fixed_t<int32_t, intshift>& left,
	        const int32_t& right)
{
	return fixed_t<int32_t, intshift> (left.get() << right);
}

template <int intshift>
fixed_t<int32_t, intshift>
operator>> (const fixed_t<int32_t, intshift>& left,
	        const int32_t& right)
{
	return fixed_t<int32_t, intshift> (left.get() >> right);
}

template <int saturateshift, int intshift>
fixed_t<int32_t, intshift>
saturate(const fixed_t<int32_t, intshift>& value)
{
	static_assert(32-intshift+saturateshift <= 32, "saturation out of bounds");
	static_assert(32-intshift+saturateshift > 0, "saturation out of bounds");
	return __SSAT_NOSHIFT(value.get(), 32-intshift+saturateshift);
}

template <int accintshift, int leftintshift, int rightintshift>
fixed_t<int32_t, accintshift>
mac (const fixed_t<int32_t, accintshift>& acc,
	 const fixed_t<int32_t, leftintshift>& left,
	 const fixed_t<int32_t, rightintshift>& right)
{
	return fixed_t<int32_t, accintshift> (
			smmla(acc.get(),
					left.get(),
					right.get() << ((leftintshift+rightintshift+1) - accintshift)) );
}
