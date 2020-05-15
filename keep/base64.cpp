#include "base64.h"

using Citadel::Keep::u32;
using namespace Citadel::Keep::Encoding;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool isBase64(u8 c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}


std::string
Base64::Encode(std::vector<u8> data) {
	std::string ret;
	int i = 0, j = 0;
	u8 charArray3[3];
	u8 charArray4[4];

	u8* buf = data.data();
	u32 count = (u32)data.size();

	while (count--) {
		charArray3[i++] = *(buf++);
		if (i == 4) {
			charArray4[0] = (charArray3[0] & 0xFC) >> 2;
			charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xF0) >> 4);
			charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xC0) >> 6);
			charArray4[3] = charArray3[2] & 0x3F;

			for (i = 0; i < 4; ++i) {
				ret += base64_chars[charArray4[i]];
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 3; ++j) {
			charArray3[j] = '\0';
		}

		charArray4[0] = (charArray3[0] & 0xFC) >> 2;
		charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xF0) >> 4);
		charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xC0) >> 6);
		charArray4[3] = charArray3[2] & 0x3F;

		for (j = 0; j < i + 1; ++j) {
			ret += base64_chars [charArray4[j]];
		}

		while (i++ < 3) {
			ret += '=';
		}
	}
	return ret;
}


std::vector<u8>
Base64::Decode(std::string  data) {
	u32 inLen = (u32)data.size();
	int i = 0;
	int j = 0;
	int in = 0;
	u8 charArray4[4], charArray3[3];
	std::vector<u8> ret;

	while(inLen-- && (data[in] != '=') && isBase64(data[in])) {
		charArray4[i++] = data[in];
		++in;

		if (i == 4) {
			for (i = 0; i < 4; ++i) {
				charArray4[i] = base64_chars.find(charArray4[i]);
			}

			charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
			charArray3[1] = ((charArray4[1] & 0xF) << 4) + ((charArray4[2] & 0x3C) >> 2);
			charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

			for (i = 0; i < 3; ++i) {
				ret.push_back(charArray3[i]);
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; ++j) {
			charArray4[j] = 0;
		}

		for (j = 0; j < 4; ++j) {
			charArray4[j] = base64_chars.find(charArray4[j]);
		}

		charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
		charArray3[1] = ((charArray4[1] & 0xF) << 4) + ((charArray4[2] & 0x3C) >> 2);
		charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];
		for (j = 0; j < i - 1; ++j) {
			ret.push_back(charArray3[j]);
		}
	}
	return ret;
}

