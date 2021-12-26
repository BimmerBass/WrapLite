#ifndef TYPES_H
#define TYPES_H
#include <memory>
#include <string>

#include <Legacy/sqlite3.h>

namespace wraplite::conversion_layer::types {

	// Type definitions.
	using session_t = std::shared_ptr<sqlite3>;
	using statement_t = std::shared_ptr<sqlite3_stmt>;

	// Input and output types.
	// TODO: Implement this as C++20 with concepts instead of empty structs.
	template<typename T>
	concept sql_integral =
		std::is_integral_v<T>;


	template<typename T>
	concept sql_decimal =
		std::is_floating_point_v<T> &&	// Should be a floating point type.
		sizeof(T) <= 8;					// Floating point numbers is stored as 8 bytes in SQLite.

	// Note: This should later be expanded to also support UTF-16 strings.
	template<typename T>
	concept sql_text =
		std::is_same_v<std::string, T> &&	// String...
		requires (T t) {
			{ t.c_str() } -> std::same_as<const char*>;
			{ t.length() } -> std::same_as<size_t>;
	};

	// General data types.
	template<typename T>
	concept sql_general_type =
		sql_integral<T> ||
		sql_decimal<T> ||
		sql_text<T>;
}

#endif