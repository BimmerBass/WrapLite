#include "session.h"

#include <filesystem>


namespace wraplite::impl {

	/// <summary>
	/// Open the database connection
	/// </summary>
	/// <param name="filepath"></param>
	database_session::database_session(const std::string& filepath, const conversion_layer::options::database_options& opts) {
		m_session = conversion_layer::open_db(filepath, opts);
	}

	database_session::database_session(const database_session& _Src) {
		m_session = _Src.m_session;
	}
}