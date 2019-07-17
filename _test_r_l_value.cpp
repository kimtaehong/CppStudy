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
	/// 복사 생성자
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

	/* 다음과 같이 객체를 생성해서 반환 하게 될 경우 반환할 임시객체를 생성하고
	   복사 생성자를 통해 반환할 임시 객체를 만든다. 이후, 객체를 소멸한다.
	   이때, "Point p"를 만들고 다시 임시 객체를 생성하는 중복 작업이 성능 저하를
	   가져오기 때문에 바로 임시객체를 만들어서 전달하는 것이 효과적이다.
	   이 기법을 RVO(Return Value Optimization)이라고 부른다.
	   하지만 RVO 기법을 사용하지 않고 이전 코드를 사용하더라도 컴파일러(MSVC)가 
	   release 모드로 컴파일 할 경우, 컴파일러가 알아서 RVO를 적용한다.
	Point p;
	return p;
	*/
	return Point();
}

TEST(rValueRef, ValueTest)
{
	// r-value는 지속성을 가지지 않는 임시적인 값
	// l-value는 지속성을 가지는 값
	
	//
	//local variable. 자신을 선언한  block을 벗어 날 때 소멸된다.
	//
	// Point p1; 

	// 
	// 임시 객체 - 자신이 포함된 문자(full expression)이 끝날때 소멸된다.
	// 클래스() 이름으로 생성한다.
	// Point();

	//
	// 만약, p1 객체가 특정 함수에 전달하고 이후에 사용할 필요 없는 객체라면
	// p는 함수가 끝날 때까지 소멸되지 않는다. 그렇기 때문에 단순히 인자로만
	// 사용할 객체는 임시객체로 만들어서 사용 하기도 한다.
	//
	foo();

	
}