#ifndef CTRE_V2__CTRE__RANGE__HPP
#define CTRE_V2__CTRE__RANGE__HPP

#include "iterators.hpp"

namespace ctre {

template <typename BeginIterator, typename EndIterator, typename RE> struct regex_range {
	BeginIterator _begin;
	const EndIterator _end;
	constexpr regex_range(BeginIterator begin, EndIterator end) noexcept: _begin{begin}, _end{end} { }
	
	constexpr auto begin() const noexcept {
		return regex_iterator<BeginIterator, EndIterator, RE>(_begin, _end);
	}
	constexpr auto end() const noexcept {
		return regex_end_iterator{};
	}
};


template <typename BeginIterator, typename EndIterator, typename RE> constexpr auto range(BeginIterator begin, EndIterator end, RE) noexcept {
	return regex_range<BeginIterator, EndIterator, RE>(begin, end);
}

#if __cpp_nontype_template_parameter_class
template <basic_fixed_string input, typename BeginIterator, typename EndIterator> __attribute__((flatten)) constexpr CTRE_FORCE_INLINE auto range(BeginIterator begin, EndIterator end) noexcept {

	using tmp = typename ctll::parser<ctre::pcre, input, ctre::pcre_actions>::template output<pcre_context<>>;
	static_assert(tmp(), "Regular Expression contains syntax error.");
	using re = decltype(front(typename tmp::output_type::stack_type()));
	return range(begin, end, re);
}
#endif

template <typename Subject, typename RE> constexpr auto range(const Subject & subject, RE re) noexcept {
	return range(subject.begin(), subject.end(), re);
}

#if __cpp_nontype_template_parameter_class
template <basic_fixed_string input, typename Subject> __attribute__((flatten)) constexpr CTRE_FORCE_INLINE auto range(const Subject & subject) noexcept {

	using tmp = typename ctll::parser<ctre::pcre, input, ctre::pcre_actions>::template output<pcre_context<>>;
	static_assert(tmp(), "Regular Expression contains syntax error.");
	using re = decltype(front(typename tmp::output_type::stack_type()));
	return range(subject.begin(), subject.end(), re);
}
#endif

}

#endif
