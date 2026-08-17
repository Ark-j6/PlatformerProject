#pragma once
#include <string>

struct MenuItem
{
	// 메뉴 선택 시 실행할 로직 저장을 위한 함수 포인터
	using OnSelected = void(*)();

	// 생성자
	MenuItem(const std::string& text, OnSelected onSelected)
		: text(text), onSelected(onSelected)
	{}

	// 메뉴 텍스트
	std::string text;

	// 메뉴를 선택했을 때 실행 로직
	OnSelected onSelected = nullptr;
};