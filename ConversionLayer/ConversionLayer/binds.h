#ifndef BINDS_H
#define BINDS_H
#include <vector>

#include "types.h"
#include "exceptions/sqlite3_exception.h"

namespace wraplite::conversion_layer {

	/// <summary>
	/// Bind an integer to the statement at the desired index.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="idx"></param>
	/// <param name="insert_val"></param>
	template<typename T> requires types::sql_int<T>
	void bind(types::statement_t stmt, int idx, const T& val) {
		if (int result = sqlite3_bind_int(stmt.get(), idx, val); result != SQLITE_OK) {
			throw exceptions::sqlite_exception();
		}
	}

	/// <summary>
	/// Overload for the integral bind function for big integers.
	/// </summary>
	/// <param name="stmt"></param>
	/// <param name="idx"></param>
	/// <param name="insert_val"></param>
	template<typename T> requires types::sql_bigint<T>
	void bind(types::statement_t stmt, int idx, const T& val) {
		if (int result = sqlite3_bind_int64(stmt.get(), idx, val); result != SQLITE_OK) {
			throw exceptions::sqlite_exception();
		}
	}

	/// <summary>
	/// Bind a floating point type to a statement at the required index.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="idx"></param>
	/// <param name="val"></param>
	template<typename T> requires types::sql_decimal<T>
	void bind(types::statement_t stmt, int idx, const T& val) {
		auto insert_val = static_cast<double>(val);
		if (int result = sqlite3_bind_double(stmt.get(), idx, insert_val); result != SQLITE_OK) {
			throw exceptions::sqlite_exception();
		}
	}

	/// <summary>
	/// Bind text to the statement at the desired index. TODO: Specialize this for UTF-16 strings.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="idx"></param>
	/// <param name="val"></param>
	template<typename T> requires types::sql_text<T>
	void bind(types::statement_t stmt, int idx, const T& val) {
		// Convert to char* and bind.
		const char* data = val.c_str();
		int length = static_cast<int>(val.length());
		if (int result = sqlite3_bind_text(stmt.get(), idx, data, length, SQLITE_TRANSIENT); result != SQLITE_OK) {
			throw exceptions::sqlite_exception();
		}
	}

	/// <summary>
	/// Bind a vector of data as a blob.
	/// </summary>
	template<typename _Ty, typename _Alloc> requires types::sql_general_type<_Ty>
	void bind(types::statement_t stmt, int idx, const std::vector<_Ty, _Alloc>& values) {
		// Step 1. Convert vector to a void pointer and calculate its length.
		auto value_ptr = static_cast<const void*>(values.data());
		int byte_count = values.size() * sizeof(_Ty); // Bytes in vector.

		if (int result = sqlite3_bind_blob(stmt.get(), idx, value_ptr, byte_count, SQLITE_TRANSIENT); result != SQLITE_OK) {
			throw exceptions::sqlite_exception();
		}
	}
}

#endif