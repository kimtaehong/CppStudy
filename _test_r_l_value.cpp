#include "stdafx.h"
#include "gtest/gtest.h"

typedef class Point {
public:
	Point() {
		std::cout << "constructor" << std::endl;
	}
	~Point() {
		std::cout << "terminator" << std::endl;
	}
	/// ���� ������
	Point(const Point&) {
		std::cout << "copy constructor" << std::endl;
	}
private:
	int x, y;
}*PPoint;

///
///@brief test for l-value & r-value(http://yesarang.tistory.com/348)
///

Point foo()
{
	std::cout << "foo function start" << std::endl;

	/* ������ ���� ��ü�� �����ؼ� ��ȯ �ϰ� �� ��� ��ȯ�� �ӽð�ü�� �����ϰ�
	   ���� �����ڸ� ���� ��ȯ�� �ӽ� ��ü�� �����. ����, ��ü�� �Ҹ��Ѵ�.
	   �̶�, "Point p"�� ����� �ٽ� �ӽ� ��ü�� �����ϴ� �ߺ� �۾��� ���� ���ϸ�
	   �������� ������ �ٷ� �ӽð�ü�� ���� �����ϴ� ���� ȿ�����̴�.
	   �� ����� RVO(Return Value Optimization)�̶�� �θ���.
	   ������ RVO ����� ������� �ʰ� ���� �ڵ带 ����ϴ��� �����Ϸ�(MSVC)�� 
	   release ���� ������ �� ���, �����Ϸ��� �˾Ƽ� RVO�� �����Ѵ�.
	Point p;
	return p;
	*/
	return Point();
}

TEST(rValueRef, ValueTest)
{
	// r-value�� ���Ӽ��� ������ �ʴ� �ӽ����� ��
	// l-value�� ���Ӽ��� ������ ��
	
	//
	//local variable. �ڽ��� ������  block�� ���� �� �� �Ҹ�ȴ�.
	//
	// Point p1; 

	// 
	// �ӽ� ��ü - �ڽ��� ���Ե� ����(full expression)�� ������ �Ҹ�ȴ�.
	// Ŭ����() �̸����� �����Ѵ�.
	// Point();

	//
	// ����, p1 ��ü�� Ư�� �Լ��� �����ϰ� ���Ŀ� ����� �ʿ� ���� ��ü���
	// p�� �Լ��� ���� ������ �Ҹ���� �ʴ´�. �׷��� ������ �ܼ��� ���ڷθ�
	// ����� ��ü�� �ӽð�ü�� ���� ��� �ϱ⵵ �Ѵ�.
	//
	foo();

	
}