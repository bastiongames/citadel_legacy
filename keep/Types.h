#ifndef __TYPES_H__
#define __TYPES_H__

#include <cstdint>
#include <memory>

namespace Citadel::Keep {
	typedef uint64_t u64;
	typedef uint32_t u32;
	typedef uint16_t u16;
	typedef uint8_t u8;

	typedef int64_t s64;
	typedef int32_t s32;
	typedef int16_t s16;
	typedef int8_t s8;


	typedef float real32;
	typedef double real64;
	typedef real32 real;

	typedef std::shared_ptr<void> handle;
	
	template<class T>
	using UPtr = std::unique_ptr<T>;
	
	template<class T>
	using SPtr = std::shared_ptr<T>;

	template<class T, typename ...Args>
	UPtr<T> MakeUPtr(Args&& ...args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<class T, typename ...Args>
	SPtr<T> MakeSPtr(Args&& ...args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T>
	SPtr<T> SPtrFromHandle(handle ptr) {
		return std::reinterpret_pointer_cast<T, void>(ptr);
	}

	template<class T>
	handle HandleFromSPtr(SPtr<T> ptr) {
		return std::reinterpret_pointer_cast<void, T>(ptr);
	}

	struct real3 {
		real3(real x, real y, real z) :x{ x }, y{ y }, z{ z } { }
		real x;
		real y;
		real z;
	};

	struct rgb {
		rgb(real r, real g, real b): r{r}, g{g}, b{b} { }

		real r;
		real g;
		real b;
	};

	struct rgba {
		rgba(real r, real g, real b, real a = 1.0f): r{ r }, g{ g }, b{ b }, a{ a } { }
		rgba(rgb color, real a = 1.0f): r{ color.r }, g{ color.g }, b{ color.b }, a{ a } { }
		real r;
		real g;
		real b;
		real a;
	};

	struct uv {
		uv(real u, real v) : u{ u }, v{ v } { }

		real u;
		real v;
	};
}
#endif