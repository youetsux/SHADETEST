#pragma once
#include <string>


/// <summary>
/// 文字列のコンバーター
/// string(多分SJISとかASCIIとかのマルチバイト）から
/// wstring(多分ユニコードとかの確実に２バイト以上の文字）に変換する
/// 今は1個しかない。。。
/// </summary>

class StringConverter
{
public:
	inline static std::wstring StringToWide(std::string str)
	{
		std::wstring wide_string(str.begin(), str.end());
		return wide_string;
	}
};