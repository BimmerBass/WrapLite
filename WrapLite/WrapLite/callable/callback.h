#ifndef CALLBACK_H
#define CALLBACK_H
#pragma warning(disable: 4100)
#include <functional>

#include <ConversionLayer/conversion.h>


/// <summary>
/// The following file is either completely stolen or inspired from sqlite_modern_cpp
/// </summary>
namespace wraplite::impl::callable {


	namespace utility {


		template<typename> struct function_traits;

		template <typename Function>
		struct function_traits : public function_traits<
			decltype(&std::remove_reference<Function>::type::operator())
		> { };

		template <
			typename    ClassType,
			typename    ReturnType,
			typename... Arguments
		>
			struct function_traits<
			ReturnType(ClassType::*)(Arguments...) const
			> : function_traits<ReturnType(*)(Arguments...)> { };

		/* support the non-const operator ()
		 * this will work with user defined functors */
		template <
			typename    ClassType,
			typename    ReturnType,
			typename... Arguments
		>
			struct function_traits<
			ReturnType(ClassType::*)(Arguments...)
			> : function_traits<ReturnType(*)(Arguments...)> { };

		template <
			typename    ReturnType,
			typename... Arguments
		>
			struct function_traits<
			ReturnType(*)(Arguments...)
			> {
			typedef ReturnType result_type;

			template <std::size_t Index>
			using argument = typename std::tuple_element<
				Index,
				std::tuple<Arguments...>
			>::type;

			static const std::size_t arity = sizeof...(Arguments);
		};
	}
	
	
	template<size_t count>
	class callback_binder {
	public:

		template<typename functor, typename... args, size_t boundary = count>
		static typename std::enable_if<(sizeof...(args) < boundary), void>::type create(
			conversion_layer::types::statement_t& stmt,
			functor&& callback,
			args&&... values
		) {
			typename std::remove_cv<typename std::remove_reference<nth_argument_type<functor, sizeof...(args)>>::type>::type val{};
			val = conversion_layer::get_column<nth_argument_type<functor, sizeof...(args)>>(stmt, sizeof...(args));

			create<functor>(stmt, callback, std::forward<args>(values)..., std::move(val));
		}

		template<typename functor, typename... args, size_t boundary = count>
		static typename std::enable_if<(sizeof...(args) == boundary), void>::type create(
			conversion_layer::types::statement_t& stmt,
			functor&& callback,
			args&&... values
		) {
			callback(std::move(values)...);
		}

	private:
		template<typename function, size_t idx>
		using nth_argument_type = typename utility::function_traits<function>::template argument<idx>;
	};

	namespace tuples {
		namespace detail {

			template<size_t i = 0, typename functor, typename... args>
			typename std::enable_if<i == sizeof...(args)>::type for_each(std::tuple<args...>&, functor, conversion_layer::types::statement_t&) {}
		
			template<size_t i = 0, typename functor, typename... args>
			typename std::enable_if<(i < sizeof...(args))>::type for_each(std::tuple<args...>& tpl, functor func, conversion_layer::types::statement_t& stmt) {
				func(stmt, i, std::get<i>(tpl));
				for_each<i + 1, functor, args...>(tpl, func, stmt);
			}

			struct callback_functor {
				template<typename T>
				void operator() (conversion_layer::types::statement_t& stmt, int i, T&& t) {
					conversion_layer::reference_wrap(stmt, i, t);
				}
			};
		}

		template<typename... args>
		void for_each_in_tuple(conversion_layer::types::statement_t& stmt, std::tuple<args...>& t) {
			conversion_layer::execute_single_query(stmt, [&]() {
				detail::for_each(t, detail::callback_functor(), stmt);
			});
		}
		
	}
}


#endif