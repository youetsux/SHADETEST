#pragma once
#include <string>


/// <summary>
/// ������̃R���o�[�^�[
/// string(����SJIS�Ƃ�ASCII�Ƃ��̃}���`�o�C�g�j����
/// wstring(�������j�R�[�h�Ƃ��̊m���ɂQ�o�C�g�ȏ�̕����j�ɕϊ�����
/// ����1�����Ȃ��B�B�B
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