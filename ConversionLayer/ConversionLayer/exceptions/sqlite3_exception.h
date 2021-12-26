#ifndef SQLITE3_EXCEPTION_H
#define SQLITE3_EXCEPTION_H
#include <exception>
#include <string>

#include <Legacy/sqlite3.h>

namespace wraplite::conversion_layer::exceptions {

	class sqlite_exception : public std::exception {
	public:
		sqlite_exception(sqlite3* connection, std::string function_name) noexcept {
			m_msg = "sqlite3 legacy-API function, '" + function_name + "', returned error code " + std::to_string(sqlite3_errcode(connection)) 
				+ " (extended: " + std::to_string(sqlite3_extended_errcode(connection)) + "), with message: '" + sqlite3_errmsg(connection) + "'.";
		}
		sqlite_exception(sqlite3* connection) noexcept {
			m_msg = "sqlite3 legacy-API returned error code " + std::to_string(sqlite3_errcode(connection)) + " (extended: " + std::to_string(sqlite3_extended_errcode(connection)) + "), with message: '"
				+ sqlite3_errmsg(connection) + "'.";
		}
		sqlite_exception() noexcept {
			m_msg = "sqlite3 legacy-API returned an error.";
		}

		const char* what() const noexcept override {
			return m_msg.c_str();
		}

	private:
		std::string m_msg;
	};

}

#endif