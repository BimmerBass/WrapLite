#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <tuple>
#include <functional>

#include <ConversionLayer/types.h>
#include "../session.h"
#include "../callable/callback.h"


namespace wraplite::sql {
	namespace types = conversion_layer::types;

	/// <summary>
	/// A wrapper for the sqlite3_stmt
	/// </summary>
	class query {
	public:
		query() = delete; // No default construction.
		query(const std::string& query_str, const database_session& session);

		// The destructor is allowed to throw exceptions.
		~query() noexcept(false);

		// Stream operators for inserting data.
		template<typename T> requires types::sql_integral<T>
		query& operator<<(const T& integer) {
			conversion_layer::bind<T>(m_statement, next_idx(), integer);
			return *this;
		}

		template<typename T> requires types::sql_decimal<T>
		query& operator<<(const T& decimal) {
			conversion_layer::bind<T>(m_statement, next_idx(), decimal);
			return *this;
		}

		template<typename T> requires types::sql_text<T>
		query& operator<<(const T& str) {
			conversion_layer::bind<T>(m_statement, next_idx(), str);
			return *this;
		}

		template<typename T, typename _Alloc> requires types::sql_general_type<T>
		query& operator<<(const std::vector<T, _Alloc>& data) {
			conversion_layer::bind<T, _Alloc>(m_statement, next_idx(), data);
			return *this;
		}


		// Operators for result extraction.
		template<typename T> requires types::sql_general_type<T>
		void operator>>(T& out_val) {
			out_val = single_callback_bind<T>();
		}

		template<typename ... args>
		void operator>>(std::tuple<args...>&& out_vals) {
			has_run = true;
			callable::tuples::for_each_in_tuple<args...>(m_statement, out_vals);
		}

		template<typename functor>
		void operator>>(functor&& func) {
			typedef callable::utility::function_traits<functor> traits;

			execute([&func, this]() {
				callable::callback_binder<traits::arity>::create(this->get_statement(), func);
				});
		}

		
		/// <summary>
		/// Execute a query and call the callback method.
		/// </summary>
		void execute(std::function<void(void)> callback) {
			has_run = true;
			conversion_layer::execute_query(m_statement, callback);
		}

		/// <summary>
		/// Execute a single row query.
		/// </summary>
		/// <param name="callback"></param>
		void execute_single(std::function<void(void)> callback) {
			has_run = true;
			conversion_layer::execute_single_query(m_statement, callback);
		}

		types::statement_t& get_statement() { return m_statement; }
	private:
		// The session that allows us to communicate with the database.
		std::shared_ptr<session_t::element_type> m_session;

		// The statement itself.
		types::statement_t m_statement;

		// Whether or not the query has run.
		bool has_run;

		// The current index we're at in the statement parameters.
		size_t current_idx;

		size_t next_idx() {
			return ++current_idx;
		}

		template<typename T>
		T single_callback_bind() {
			T out_var;

			auto callback = [&](T v) {
				out_var = v;
			};
			typedef callable::utility::function_traits<decltype(callback)> traits;

			execute_single([callback, this]() {
				callable::callback_binder<traits::arity>::create(this->get_statement(), callback);
				});

			return out_var;
		}

		//template<typename... args>
		//void tuple_callback(std::tuple<args...>& out_tuple) {
		//	// 1. Create a callback method that assigns the values to the tuple.
		//	auto callback = [&](args... values) {
		//		out_tuple = std::make_tuple(values...);
		//	};
		//	typedef callable::utility::function_traits<decltype(callback)> traits;
		//
		//	// 2. Execute the query.
		//	execute_single([callback, this]() {
		//		callable::callback_binder<traits::arity>::create(this->get_statement(), callback);
		//	});
		//}
	};
}



#endif