#include "session.h"

#include <filesystem>


namespace wraplite {

	/// <summary>
	/// Open the database connection
	/// </summary>
	/// <param name="filepath"></param>
	database_session::database_session(const std::string& filepath) {
		m_session = conversion_layer::open_db(filepath);
	}

}