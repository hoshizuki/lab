// C++ でカリー化する実験
//
// 実行結果: http://melpon.org/wandbox/permlink/cRaiDb5Xs992Ql4O

#include <iostream>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_pointer.hpp>

template <typename F, typename Enable = void>
struct function_ptr {
	typedef F type;
};
template <typename F>
struct function_ptr<F, typename std::enable_if<std::is_class<F>::value>::type> {
	typedef typename boost::function_types::parameter_types<decltype(&F::operator())>::type function_params;
	typedef typename boost::function_types::function_pointer<function_params>::type type;
};

template <typename F, typename A1>
auto curry_impl( F f ) -> auto { return f; }

template <typename F, typename A1, typename A2, typename... Args>
auto curry_impl( F f ) -> auto {
	return [f]( A1 a1 ) {
		auto g = [f,a1]( A2 a2, Args... args ) { return f( a1, a2, args... ); };
		return curry_impl<decltype(g), A2, Args...>( g );
	};
}

template <typename F, typename R, typename... Args>
auto curry_proxy( F f, R (*)(Args...) ) {
	return curry_impl<F, Args...>( f );
}

template <typename F>
auto curry( F f ) -> auto {
	typename function_ptr<F>::type p = nullptr;
	return curry_proxy( f, p );
}

int f( int x, int y, int z ) { return z * 100 + y * 10 + x; }

int main()
{
	std::cout << curry([](int x){return x*2;})(3) << std::endl;
	std::cout << curry([](int x, int y){return x*10+y;})(1)(2) << std::endl;
	std::cout << curry([](int x, int y, int z){return x*100+y*10+z;})(4)(1)(2) << std::endl;
	std::cout << curry(f)(1)(2)(3) << std::endl;
	return 0;
}
