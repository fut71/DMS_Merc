#pragma once
#include "pch.h"
#include <random>
inline constexpr int MAX_FILE_LENGTH = 256;

using UID = uint32_t;

/// <summary>
/// ���ڿ�, ���� �ý��۵��� ��ƿ��Ƽ �Լ����� ��Ƶ� Ŭ�����Դϴ�.
/// </summary>
class Utility
{
public:
	// std::wstring�� std::string���� ��ȯ�մϴ�. 
	static std::string ToString(const std::wstring_view _value);

	static std::string ToString(const TCHAR* _value);

	static std::wstring ToWString(const std::string_view _value);

	/// <summary>
	/// ���� ���̾�α׸� ���� ���õ� ���� ��θ� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>���õ� ���� ���</returns>
	static std::wstring OpenFileDialog();
};

class UIDGenerator
{
public:
	template <typename T>
	static UID GenerateUID()
	{
		static std::atomic<UID> counter(0);
		return counter++;
	}

};

class RandomUtil
{
public:
	static int RandomInt(int _min, int _max);
	static float RandomFloat(float _min, float _max);
};