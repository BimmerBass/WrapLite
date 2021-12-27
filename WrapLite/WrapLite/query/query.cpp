#include "query.h"


namespace wraplite::sql {

	/// <summary>
	/// Initialize a new sqlite3_stmt object.
	/// </summary>
	/// <param name="query_str"></param>
	query::query(const std::string& query_str, std::shared_ptr<database_session> session_ptr) 
		: m_session(session_ptr), has_run(false), current_idx(0) {

		// Create the statement object.
		std::shared_ptr<sqlite3> connection((sqlite3*)(*session_ptr.get()));
		m_statement = conversion_layer::prepare_statement(query_str, connection);
	}

	/// <summary>
	/// Perform the query execution if it hasn't been done before and there isn't an uncaught exception being unrolled.
	/// </summary>
	/// <returns></returns>
	query::~query() noexcept(false) {
		if (std::uncaught_exceptions() <= 0 && !has_run) {
			execute();
		}
	}
}