#ifndef TYPES_H
#define TYPES_H
#include <type_traits>
#include <string>


namespace wraplite::types {

	// Concept declarations.
	template<typename T>
	concept sql_integral =
		std::is_integral_v<T> &&	// Must be an integer type.
		sizeof(T) <= 8;				// No more than 64-bits can be stored as an integer in SQLite.

	template<typename T>
	concept sql_decimal =
		std::is_floating_point_v<T> &&	// Should be a floating point type.
		sizeof(T) <= 8;					// Floating point numbers is stored as 8 bytes in SQLite.

	// Note: This should later be expanded to also support UTF-16 strings.
	template<typename T>
	concept sql_text =
		std::is_same_v<std::string, T>;	// String...

	// General data types.
	template<typename T>
	concept sql_general_type =
		sql_integral<T> ||
		sql_decimal<T> ||
		sql_text<T>;
}

#endif