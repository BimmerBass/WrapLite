#include "api.h"
#include "ConversionLayer/exceptions/api_exception.h"

namespace wraplite {

	/// <summary>
	/// Open the database session.
	/// </summary>
	/// <param name="filepath"></param>
	sqlite_api::sqlite_api(const std::string& filepath) {
		m_session = std::make_shared<impl::database_session>(filepath);
	}

	/// <summary>
	/// Create a new SQL query from the string.
	/// </summary>
	/// <param name="sql"></param>
	/// <returns></returns>
	impl::sql::query sqlite_api::operator<<(const std::string& sql) {
		return impl::sql::query(sql, *m_session.get());
	}

	/// <summary>
	/// Create a new SQL query from the string.
	/// </summary>
	/// <param name="sql"></param>
	/// <returns></returns>
	impl::sql::query sqlite_api::operator<<(const char* sql) {
		return impl::sql::query(sql, *m_session.get());
	}
}