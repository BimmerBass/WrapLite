#ifndef SESSION_H
#define SESSION_H
#include <string>

#include <ConversionLayer/conversion.h>

namespace wraplite {
	using session_t = conversion_layer::types::session_t;

	/// <summary>
	/// Wrapper for the sqlite3* object used to establish and break the connection to the database.
	/// </summary>
	class database_session final {
	public:
		// No default construction
		database_session() = delete;
		database_session(const std::string& filepath);
		database_session(const database_session& _Src);

		auto get_connection() const { return m_session; }
	private:
		session_t m_session = nullptr;
	};
}

#endif