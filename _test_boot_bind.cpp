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
	// f(1, 2)�� ������ �Լ� ��ü
	std::cout << boost::bind(f, 1, 2)() << std::endl;
	// g(1, 2, 3)�� ������ �Լ� ��ü
	std::cout << boost::bind(g, 1, 2, 3)() << std::endl; 

	// boost::function�� ����ϸ� bind()�� �Լ���ü�� ���� �� �� �ִ�.
	//
	boost::function<int(int, int)> ff = boost::bind(f, 1, 2);
	std::cout << "ff(1, 2) = " << ff(1, 2) << "boost::bind(f, 1, 2)() = " << boost::bind(f, 1, 2)() << std::endl;

	// *�߿� : f�Լ��� int(int, int) Ÿ��������, boost::bind(f, 1, _1)�� �Ķ���͸� �����ؼ�
	// f(val) ���·� ȣ���� �����ϰ�, �׻� (1 + val)�� �����ϰ� �ȴ�. �̷� Ư¡�� �̿��ؼ�
	// callback �Լ��� �ΰ����� �Ķ���͸� �����ϰų�, �Ķ���� ������ �����ϴ� ���� ���� �� �� �ִ�.
	boost::function<int(int, int)> xf = boost::bind(f, 1, _1);
	std::cout << "xf(1, 2) = " << xf(1, 2) << "boost::bind(f, 1, _1)(2) = " << boost::bind(f, 1, _1)(2) << std::endl;

	int x = 3;
	int y = 4;
	int z = 5;

	//
	// _N(N=1, 2, 3, 4, ..., N)�� ���� �Ǵ� �μ� ����� 
	// N ��° �ڸ��� �μ��� _N�� ���� ���� ���� �ϰڴٴ� ���� �ǹ�
	//
	ASSERT_TRUE(f(x, y) == bind(f, _2, _1)(x, y));
	ASSERT_TRUE(g(x, 9, x) == bind(g, _1, 9, _1)(x));
	ASSERT_TRUE(g(z, z, z) == bind(g, _3, _3, _3)(x, y, z));
	ASSERT_TRUE(g(x, x, x) == bind(g, _1, _1, _1)(x, y, z));
}

/**
 * @brief boost::bind() - part2 �Լ���ü ���ε�
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
	// bind<Return-type>(...)�� ������ �־�� bind �Լ��� ��ü�� ���� �� �� �ְ� �ȴ�.
	// ���⼭, ������ ���� �� �κ��� F::operator()�� 2���� �����ε�� �Լ��� �ִµ�,
	// �̶�, ���ε�� ���޵Ǵ� x�� Ÿ������ ���� �Ǿ� ����. 
	//
	assert(0 == boost::bind<int>(f, _1, _1)(x));
}

/**
 * @brief boost::bind() - part3 �Լ� ��ü ���ε�, �Լ� ��ü ���� ó��
 *        - ���� ������ �ִ� ��� "result_type"�� ������ ���ϸ� ������ ����.
 *          �� ������..? -_-..
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
	// ����Ʈ�� �Լ� ��ü�� "����"�ؼ� ���������� ������ �ִ�.
	// �̶�, ���� �ϴ� ����̳�, Ư���� ���¸� ����� �ϴ� �Լ� ��ü���,
	// boost::ref(x)�̳� boost::cref(x)�� ���� �ϴ°��� ����.
	//
	std::for_each(a, a + 3, boost::bind(boost::ref(f), _1));
	assert(6 == f.s);
}

/**
 * @brief boost::bind() - part4 Ŭ���� �޼ҵ� ���ε�
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
	 - class method ���ε�
	 - �Ʒ� �ڵ�� ����
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
	 - class method ���ε��Ҷ��� ù��° �Ķ���Ͱ� class instance �̾�� ��
	 - std::vector<PA> �϶��� std::vector<A> �϶��� functor �� �� ������
	 - �Ʒ��� �ڵ�� ������
	   std::vector<PA>::iterator it = vA.begin();
	   for(; it != vA.end(); ++it)
	   {
	     it->start();
	   }
	 - std::for_each() �� [ vA.begin(), vA.end(), ... ]�� boost::bind(&A::start, _1)(it)�̹Ƿ�
	   it::start()�� ������
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
