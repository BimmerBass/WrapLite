#ifndef CONVERSION_H
#define CONVERSION_H
#include <memory>
#include <string>

#include <Legacy/sqlite3.h>


namespace conversion_layer {

	// Type definitions.
	using session_t = std::shared_ptr<sqlite3>;

	/// <summary>
	/// Open a database and throw an exception if either (1) the file doesn't exist and should_exist is true, or (2) sqlite3_open_v2 fails.
	/// </summary>
	/// <param name="file_path"></param>
	/// <returns></returns>
	session_t open_db(const std::string& file_path, bool should_exist = false);


	/// <summary>
	/// Close an open database handle and throw if it fails.
	/// </summary>
	/// <param name="session"></param>
	void close_db(session_t session);
}


#endif