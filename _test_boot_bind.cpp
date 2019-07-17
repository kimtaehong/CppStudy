/**
 * ref: https://github.com/somma/_MyLib/blob/master/_test_boost_bind.cpp
 */

#include "stdafx.h"
#include "gtest/gtest.h"

/**
 * @brief boost::bind() - part1
 */

int f(int a, int b)
{
	return a + b;
}

int g(int a, int b, int c)
{
	return a + b + c;
}

TEST(BoostBindTest, BindTest)
{
	// f(1, 2)와 동일한 함수 객체
	std::cout << boost::bind(f, 1, 2)() << std::endl;
	// g(1, 2, 3)와 동일한 함수 객체
	std::cout << boost::bind(g, 1, 2, 3)() << std::endl; 

	// boost::function을 사용하면 bind()된 함수객체를 저장 할 수 있다.
	//
	boost::function<int(int, int)> ff = boost::bind(f, 1, 2);
	std::cout << "ff(1, 2) = " << ff(1, 2) << "boost::bind(f, 1, 2)() = " << boost::bind(f, 1, 2)() << std::endl;

	// *중요 : f함수는 int(int, int) 타입이지만, boost::bind(f, 1, _1)로 파라미터를 고정해서
	// f(val) 형태로 호출이 가능하고, 항상 (1 + val)를 리턴하게 된다. 이런 특징을 이용해서
	// callback 함수에 부가적인 파라미터를 전달하거나, 파라미터 순서를 조작하는 등의 일을 할 수 있다.
	boost::function<int(int, int)> xf = boost::bind(f, 1, _1);
	std::cout << "xf(1, 2) = " << xf(1, 2) << "boost::bind(f, 1, _1)(2) = " << boost::bind(f, 1, _1)(2) << std::endl;

	int x = 3;
	int y = 4;
	int z = 5;

	//
	// _N(N=1, 2, 3, 4, ..., N)는 전달 되는 인수 목록의 
	// N 번째 자리의 인수를 _N이 써진 곳에 대입 하겠다는 것을 의미
	//
	ASSERT_TRUE(f(x, y) == bind(f, _2, _1)(x, y));
	ASSERT_TRUE(g(x, 9, x) == bind(g, _1, 9, _1)(x));
	ASSERT_TRUE(g(z, z, z) == bind(g, _3, _3, _3)(x, y, z));
	ASSERT_TRUE(g(x, x, x) == bind(g, _1, _1, _1)(x, y, z));
}

/**
 * @brief boost::bind() - part2 함수객체 바인드
 */

struct F
{
	int operator()(int a, int b) { return a - b; }
	bool operator()(long a, long b) { return a = b ? true : false; }
};

TEST(BoostBindTest, BindTest2)
{
	F f;

	int x = 104;
	// bind<Return-type>(...)을 지정해 주어야 bind 함수는 객체를 생성 할 수 있게 된다.
	// 여기서, 유심히 봐야 할 부분은 F::operator()에 2개의 오버로드된 함수가 있는데,
	// 이때, 바인드시 전달되는 x의 타입으로 결정 되어 진다. 
	//
	assert(0 == boost::bind<int>(f, _1, _1)(x));
}

/**
 * @brief boost::bind() - part3 함수 객체 바인드, 함수 객체 상태 처리
 *        - 내부 변수가 있는 경우 "result_type"을 재정의 안하면 에러가 난다.
 *          그 이유는..? -_-..
 *        - 
 */
struct FF
{
	int s;
	typedef void result_type;
	void operator() (int x) { s += x; }
};

TEST(BoostBindTest, BindTest3)
{
	FF f = { 0 };
	int a[] = { 1, 2, 3 };
	// 디폴트로 함수 객체를 "복사"해서 내부적으로 가지고 있다.
	// 이때, 복사 하는 비용이나, 특별히 상태를 갖어야 하는 함수 객체라면,
	// boost::ref(x)이나 boost::cref(x)로 전달 하는것이 좋다.
	//
	std::for_each(a, a + 3, boost::bind(boost::ref(f), _1));
	assert(6 == f.s);
}

/**
 * @brief boost::bind() - part4 클래스 메소드 바인딩
 */

typedef class A
{
private:
	int m;
public:
	A(int val) : m(val) {}

	int add_value(int val)
	{
		m += val;
		std::cout << __FUNCTION__ << ", m=" << m << std::endl;
		return m;
	}

	void start(int val)
	{
		std::cout << __FUNCTION__ << ", m=" << m << ", v=" << val <<std::endl;
	}
}*PA;

TEST(BoostBindTest, BindTest4)
{
	std::vector<int> vint;
	for(int idx = 0; idx < 12; ++idx)
	{
		vint.push_back(idx);
	}

	/*
	 - class method 바인딩
	 - 아래 코드와 동일
	   std::vector<int>::iterator it = vA.begin();
	   for(; vint.end(); ++it)
	   {
		   a.add_value(*it);
	   }
	 */

	A a(100);
	boost::function<int(int)> functor_add_value = boost::bind(&A::add_value, a, _1);
	std::for_each(vint.begin(), vint.end(), functor_add_value);


	/*
	 - class method 바인딩할때는 첫번째 파라미터가 class instance 이어야 함
	 - std::vector<PA> 일때나 std::vector<A> 일때나 functor 가 잘 동작함
	 - 아래와 코드와 동일함
	   std::vector<PA>::iterator it = vA.begin();
	   for(; it != vA.end(); ++it)
	   {
	     it->start();
	   }
	 - std::for_each() 는 [ vA.begin(), vA.end(), ... ]를 boost::bind(&A::start, _1)(it)이므로
	   it::start()와 동일함
	 */
	std::vector<PA> vA;
	for(int idx=0; idx < 10; ++idx)
	{
		PA pa = new A(idx);
		vA.push_back(pa);
	}

	int value = 0;
	std::for_each(vA.begin(), vA.end(), boost::bind(&A::start, _1, ++value));

	// free resources
	//
	for (auto aa : vA)
	{
		delete aa;
	}
	vA.clear();
}
