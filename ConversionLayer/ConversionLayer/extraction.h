#ifndef EXTRACTION_H
#define EXTRACTION_H
#include "types.h"
#include <vector>


namespace wraplite::conversion_layer {

	/// <summary>
	/// Get an integer value from a column at the specified index.
	/// </summary>
	template<typename T> requires types::sql_int<T>
	T get_column(types::statement_t stmt, int column_idx) {
		return sqlite3_column_type(stmt.get(), column_idx) == SQLITE_NULL ?
			0 : static_cast<T>(sqlite3_column_int(stmt.get(), column_idx));
	}

	template<typename T> requires types::sql_bigint<T>
	T get_column(types::statement_t stmt, int column_idx) {
		return sqlite3_column_type(stmt.get(), column_idx) == SQLITE_NULL ?
			0 : static_cast<T>(sqlite3_column_int64(stmt.get(), column_idx));
	}

	/// <summary>
	/// Get a floating point value from the specified index.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="column_idx"></param>
	/// <returns></returns>
	template<typename T> requires types::sql_decimal<T>
	T get_column(types::statement_t stmt, int column_idx) {
		return sqlite3_column_type(stmt.get(), column_idx) == SQLITE_NULL ?
			0.0 : static_cast<T>(sqlite3_column_double(stmt.get(), column_idx));
	}

	/// <summary>
	/// Get a string from the specified column index
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="column_idx"></param>
	/// <returns></returns>
	template<typename T> requires types::sql_text<T>
	T get_column(types::statement_t stmt, int column_idx) {
		if (sqlite3_column_type(stmt.get(), column_idx) == SQLITE_NULL) {
			return T("");
		}

		// From the documentation, sqlite3_column_text always return null-terminated strings, so we can immediately cast it to the desired string format and return.
		return T(reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), column_idx)));
	}

	/// <summary>
	/// Get a BLOB returned as a vector.
	/// </summary>
	/// <typeparam name="_Ty"></typeparam>
	/// <typeparam name="_Alloc"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="column_idx"></param>
	/// <returns></returns>
	template<typename _Ty> requires types::sql_general_type<_Ty>
	std::vector<_Ty> get_column(types::statement_t stmt, int column_idx) {
		if (sqlite3_column_type(stmt.get(), column_idx) == SQLITE_NULL) {
			return std::vector<_Ty>();
		}

		// Get the length in bytes and the void pointer.
		auto column_data = sqlite3_column_blob(stmt.get(), column_idx);
		auto data_length = sqlite3_column_bytes(stmt.get(), column_idx);

		// Now convert the data and its length to match the requested data type.
		_Ty* converted_data = static_cast<_Ty*>(column_data);
		size_t converted_length = data_length / sizeof(_Ty);

		return std::vector<_Ty>(converted_data, converted_data + converted_length);
	}
}


#endif