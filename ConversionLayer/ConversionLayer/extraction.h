#ifndef EXTRACTION_H
#define EXTRACTION_H
#include "types.h"


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
}


#endif