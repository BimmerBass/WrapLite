#ifndef CONVERSION_H
#define CONVERSION_H
#include <string>
#include <functional>

#include "types.h"
#include "binds.h"
#include "extraction.h"

namespace wraplite::conversion_layer {

	/// <summary>
	/// Open a database and throw an exception if either (1) the file doesn't exist and should_exist is true, or (2) sqlite3_open_v2 fails.
	/// </summary>
	/// <param name="file_path"></param>
	/// <returns></returns>
	types::session_t open_db(const std::string& file_path, bool should_exist = false);


	/// <summary>
	/// Close an open database handle and throw if it fails.
	/// </summary>
	/// <param name="session"></param>
	void close_db(types::session_t session);

	/// <summary>
	/// Execute a query
	/// </summary>
	/// <typeparam name="function"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="callback"></param>
	void execute_query(types::statement_t stmt, std::function<void(void)> callback) {

		// Keep executing until there are no more rows.
		int result;
		while ((result = sqlite3_step(stmt.get())) == SQLITE_ROW) {
			callback();
		}
		if (result != SQLITE_DONE) { // Error!
			throw exceptions::sqlite_exception();
		}

	}

	/// <summary>
	/// Execute a query, but only a single time.
	/// </summary>
	/// <typeparam name="function"></typeparam>
	/// <param name="stmt"></param>
	/// <param name="callback"></param>
	void execute_single_query(types::statement_t stmt, std::function<void(void)> callback) {
		// Execute the query once.
		int result = sqlite3_step(stmt.get());

		switch (result) {
		case SQLITE_ROW:
			callback(); break;
		case SQLITE_DONE:	// There were no data, so just return.
			return;
		default: // An error occured.
			throw exceptions::sqlite_exception();
		}

		// Now check if there is more data, and throw an exception if there is.
		if (result = sqlite3_step(stmt.get()); result == SQLITE_ROW) {
			throw std::runtime_error("the query contained multiple results. Only a single was expected.");
		}

		if (result != SQLITE_DONE) {
			throw exceptions::sqlite_exception();
		}
	}
}


#endif