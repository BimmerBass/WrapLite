#include "query.h"


namespace wraplite::impl::sql {

	/// <summary>
	/// Initialize a new sqlite3_stmt object.
	/// </summary>
	/// <param name="query_str"></param>
	query::query(const std::string& query_str, const database_session& session)
		: m_session(session.get_connection()), has_run(false), current_idx(0) {

		// Create the statement object.
		m_statement = conversion_layer::prepare_statement(query_str, m_session);
	}

	/// <summary>
	/// Perform the query execution if it hasn't been done before and there isn't an uncaught exception being unrolled.
	/// </summary>
	/// <returns></returns>
	query::~query() noexcept(false) {
		if (std::uncaught_exceptions() <= 0 && !has_run) {
			execute({});
		}
	}
}