#pragma once
#include "core/MinInclude.h"
#include <numeric>

namespace Core::Math::Consts
{
	template<typename T>
	constexpr i32 Digits          = u32(std::numeric_limits<T>::digits);
	template<typename T>
	constexpr i32 SignificandBits = Digits<T> - 1;
	template<typename T>
	constexpr i32 Digits10        = u32(std::numeric_limits<T>::digits10);
	template<typename T>
	constexpr i32 Radix           = u32(std::numeric_limits<T>::radix);
	template<typename T>
	constexpr T MinVal            = std::numeric_limits<T>::min();
	template<typename T>
	constexpr T MaxVal            = std::numeric_limits<T>::max();
	template<typename T>
	constexpr T LowestVal         = std::numeric_limits<T>::lowest();
	template<typename T>
	constexpr T Infinity          = std::numeric_limits<T>::infinity();
	template<typename T>
	constexpr T QNaN              = std::numeric_limits<T>::quiet_NaN();
	template<typename T>
	constexpr T SNaN              = std::numeric_limits<T>::signaling_NaN();
	template<typename T>
	constexpr i32 MaxExp          = std::numeric_limits<T>::max_exponent - 1;
	template<typename T>
	constexpr i32 MaxExp10        = std::numeric_limits<T>::max_exponent10 - 1;
	template<typename T>
	constexpr i32 MinExp          = std::numeric_limits<T>::min_exponent - 1;
	template<typename T>
	constexpr i32 MinExp10        = std::numeric_limits<T>::min_exponent10 - 1;
	template<typename T>
	constexpr i32 ExpBias         = -MaxExp<T>;

	template<typename T>
	constexpr u32 ExpDigits       = 0;
	template<>
	constexpr u32 ExpDigits<f32>  = 8;
	template<>
	constexpr u32 ExpDigits<f64>  = 11;

	template<typename T>
	constexpr T MachineEpsilon    = std::numeric_limits<T>::epsilon();
	template<typename T>
	constexpr T MathEpsilon       = static_cast<T>(0.00001);

	template<typename T>
	constexpr T Pi                = static_cast<T>(3.14159265358979323846264338327950288);
	template<typename T>
	constexpr T TwoPi             = static_cast<T>(6.28318530717958647692528676655900576);
	template<typename T>
	constexpr T HalfPi            = static_cast<T>(1.57079632679489661923132169163975144);
	template<typename T>
	constexpr T ThreeOverTwoPi    = static_cast<T>(4.71238898038468985769396507491925432);
	template<typename T>
	constexpr T QuarterPi         = static_cast<T>(0.785398163397448309615660845819875721);
	template<typename T>
	constexpr T OneOverPi         = static_cast<T>(0.318309886183790671537767526745028724);
	template<typename T>
	constexpr T OneOverTwoPi      = static_cast<T>(0.159154943091895335768883763372514362);
	template<typename T>
	constexpr T TwoOverPi         = static_cast<T>(0.636619772367581343075535053490057448);
	template<typename T>
	constexpr T FourOverPi        = static_cast<T>(1.273239544735162686151070106980114898);

	template<typename T>
	constexpr T RootPi            = static_cast<T>(1.772453850905516027);
	template<typename T>
	constexpr T RootHalfPi        = static_cast<T>(1.253314137315500251);
	template<typename T>
	constexpr T RootTwoPi         = static_cast<T>(2.506628274631000502);
	template<typename T>
	constexpr T TwoOverRootPi     = static_cast<T>(1.12837916709551257389615890312154517);

	template<typename T>
	constexpr T OneOverRootTwo    = static_cast<T>(0.707106781186547524400844362104849039);

	template<typename T>
	constexpr T RootTwo           = static_cast<T>(1.41421356237309504880168872420969808);
	template<typename T>
	constexpr T RootThree         = static_cast<T>(1.73205080756887729352744634150587236);
	template<typename T>
	constexpr T RootFive          = static_cast<T>(2.23606797749978969640917366873127623);

	template<typename T>
	constexpr T LnTwo             = static_cast<T>(0.693147180559945309417232121458176568);
	template<typename T>
	constexpr T LnLnTwo           = static_cast<T>(-0.3665129205816643);
	template<typename T>
	constexpr T LnTen             = static_cast<T>(2.30258509299404568401799145468436421);
	template<typename T>
	constexpr T RootLnFour        = static_cast<T>(1.17741002251547469);

	template<typename T>
	constexpr T Third             = static_cast<T>(0.3333333333333333333333333333333333333333);
	template<typename T>
	constexpr T TwoThirds         = static_cast<T>(0.666666666666666666666666666666666666667);

	template<typename T>
	constexpr T E                 = static_cast<T>(2.71828182845904523536);
	template<typename T>
	constexpr T Euler             = static_cast<T>(0.577215664901532860606);
	template<typename T>
	constexpr T GoldenRatio       = static_cast<T>(1.61803398874989484820458683436563811);

	template<typename T>
	constexpr T Deg2Rad           = Pi<T> / static_cast<T>(180);
	template<typename T>
	constexpr T Rad2Deg           = static_cast<T>(180) / Pi<T>;
}
