#ifndef __STRINGCONTAINER_H__
#define __STRINGCONTAINER_H__

#include "Types.h"
#include <vector>
#include <string>

namespace Citadel::Keep {
	template <typename T>
	inline std::size_t StrLength(const T* s) {
		std::size_t len = 0;
		while (*s++ != 0)
			++len;
		return len;
	}

	// Specialization of StrLength template for ANSI strings.
	template <>
	inline std::size_t StrLength<char>(const char* s) {
		return std::strlen(s);
	}

	// Specialization of StrLength template for Unicode strings.
	template <>
	inline std::size_t StrLength<wchar_t>(const wchar_t* s) {
		return std::wcslen(s);
	}

	template<class T>
	class StringContainerBase {
	public:
		void Clear() {
			data.clear();
			reserved = 0;
			offset = 0;
		}

		void Reserve(Keep::u64 len) {
			reserved += (len + 1);
		}

		const T* CopyString(const T* str) {
			return CopyStringPrimary(str, StrLength(str));
		}

		const T* CopyString(const std::string& str) {
			return CopyStringPrimary(str.data(), str.size());
		}

		const T* GetNextString(const T* ptr = nullptr) const {
			if (ptr != nullptr) {
				for (; IsInsideRange(ptr); ++ptr) {
					if (*ptr == 0) {
						ptr++;
						return IsInsideRange(ptr) ? ptr : nullptr;
					}
				}
			}
			else {
				return data.data();
			}
			return nullptr;
		}

	private:
		char* CopyStringPrimary(const T* str, Keep::u64 len) {
			const auto grow = len + 1;
			if (offset + grow > data.size()) {
				if (reserved > grow)
					data.resize(data.size() + reserved);
				else
					data.resize(data.size() + grow);
				reserved = 0;
			}

			auto dst = &data[offset];
			::memcpy(dst, str, sizeof(T) * grow);

			offset += grow;
			return dst;
		}

		bool IsInsideRange(const T* ptr) {
			return (ptr >= data.data() && ptr < data.data() + data.size());
		}
	private:
		std::vector<T> data;
		Keep::u64 reserved = 0;
		Keep::u64 offset = 0;
	};

	using StringContainer = StringContainerBase<char>;
	using WStringContainer = StringContainerBase<wchar_t>;
}

#endif