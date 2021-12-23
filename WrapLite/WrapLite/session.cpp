#include <filesystem>

#include "session.h"
#include "exceptions/api_exception.h"
#include "exceptions/sqlite3_exception.h"

namespace wraplite {

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filepath"></param>
	database_session::database_session(const std::string& filepath) {
		// Check if the databse file exists. Throw if it doesn't.
		if (filepath != ":memory:" && !std::filesystem::exists(filepath)) {
			throw exceptions::api_exception("No database file at the specified path.");
		}

		// Open the connection to the database.
		sqlite3* tmp = nullptr;
		if (int result = sqlite3_open_v2(
			filepath.c_str(),
			&tmp,
			SQLITE_OPEN_NOMUTEX,
			NULL
		); result != SQLITE_OK) {
			// Even though and error occured, the docs say that the connection should still be closed properly.
			exceptions::sqlite_exception e(tmp, "sqlite3_open_v2");
			sqlite3_close_v2(tmp);
			throw e;
		}

		// Copy the pointer to the opened database.
		// Also specify a custom deleter.
		m_session = std::make_shared<sqlite3>(tmp, [=](sqlite3* ptr) {sqlite3_close_v2(ptr); });
	}

}