#ifndef EXTRACTION_H
#define EXTRACTION_H
#include "types.h"


namespace wraplite::conversion_layer {

	/// <summary>
	/// Get an integer value from a column at the specified index.
	/// </summary>
	template<typename T> requires types::sql_integral<T>
	T get_column(types::statement_t stmt, int column_idx) {

	}

}


#endif