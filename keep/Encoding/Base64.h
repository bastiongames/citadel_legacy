#ifndef __BASE64_H__
#define __BASE64_H__

#include "keep/Types.h"
#include <vector>
#include <string>

using Citadel::Keep::u8;


namespace Citadel::Keep::Encoding {
	class Base64 {
	public:
		static std::string Encode(std::vector<u8> data);
		static std::vector<u8> Decode(std::string data);
	};

	/*
	std::string base64_encode(std::vector<u8> data);
	std::vector<u8> base64_decode(std::string data);
	*/
}
#endif