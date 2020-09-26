#ifndef __DATATYPE_H__
#define __DATATYPE_H__

namespace Citadel::Watchtower::Formats {
	enum struct DataType {
		Undefined,

		Int8,
		UInt8,

		Int16,
		UInt16,

		Int32,
		UInt32,

		Float16,
		Float32,
		Float64,
	};
}

#endif