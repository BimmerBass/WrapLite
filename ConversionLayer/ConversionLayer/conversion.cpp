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
	types::session_t open_db(const std::string& file_path, bool should_exist) {
		// Step 1. Check if the file exists, and throw if it doesn't.
		if (file_path != ":memory:" && should_exist && !std::filesystem::exists(file_path)) {
			throw exceptions::api_exception("No database file at the specified path.");
		}

		// Step 2. Now attempt to open the file.
		sqlite3* tmp = nullptr;
		if (int result = sqlite3_open_v2(
			file_path.c_str(),
			&tmp,
			SQLITE_OPEN_NOMUTEX,
			NULL
		); result != SQLITE_OK) {
			// Even though and error occured, the docs say that the connection should still be closed properly.
			exceptions::sqlite_exception e(tmp, "sqlite3_open_v2");
			sqlite3_close_v2(tmp);
			throw e;
		}

		// Return the pointer to the opened database.
		// Also specify a custom deleter.
		return std::shared_ptr<sqlite3>(tmp, [=](sqlite3* ptr) { sqlite3_close_v2(ptr); });
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

}