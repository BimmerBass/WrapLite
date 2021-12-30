#include "conversion.h"
#include "exceptions/api_exception.h"
#include "exceptions/sqlite3_exception.h"

#include <filesystem>


namespace wraplite::conversion_layer {

	/// <summary>
	/// Open a database and throw an exception if either (1) the file doesn't exist, or (2) sqlite3_open_v2 fails.
	/// </summary>
	/// <param name="file_path"></param>
	/// <returns></returns>
	types::session_t open_db(const std::string& file_path, const options::database_options& opts) {
		// Attempt to open the file.
		sqlite3* tmp = nullptr;
		int result = sqlite3_open_v2(
			file_path.c_str(),
			&tmp,
			static_cast<int>(opts.flags),
			NULL							// <---- TODO: Add proper VFS options.
		);
		std::shared_ptr<sqlite3> ret_val(tmp, [=](sqlite3* ptr) { sqlite3_close_v2(ptr); });

		if (result != SQLITE_OK) {
			throw tmp == nullptr ? exceptions::sqlite_exception() : exceptions::sqlite_exception(tmp, "sqlite3_open_v2");
		}

		// Return the pointer to the opened database.
		// Also specify a custom deleter.
		return ret_val;
	}


	/// <summary>
	/// Close an open database handle and throw if it fails.
	/// </summary>
	/// <param name="session"></param>
	void close_db(types::session_t session) {
		if (int result = sqlite3_close_v2(session.get()); result != SQLITE_OK) {
			throw exceptions::sqlite_exception(session.get(), "sqlite3_close_v2");
		}
	}


	/// <summary>
	/// Prepare an SQLite query.
	/// </summary>
	/// <param name="query_str"></param>
	/// <param name="session_ptr"></param>
	types::statement_t prepare_statement(const std::string& query_str, std::shared_ptr<sqlite3> session_ptr) {
		// Firstly prepare a raw statement pointer.
		sqlite3_stmt* tmp;
		if (int result = sqlite3_prepare_v2(
			session_ptr.get(), 
			query_str.c_str(), 
			-1, 
			&tmp, 
			NULL); result != SQLITE_OK) {

			throw exceptions::sqlite_exception(session_ptr.get(), "sqlite3_prepare_v2");
		}
		return types::statement_t(tmp, [=](sqlite3_stmt* st) { sqlite3_finalize(st); });
	}

}