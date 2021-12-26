#ifndef CONVERSION_H
#define CONVERSION_H
#include <string>

#include "types.h"
#include "binds.h"

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
}


#endif