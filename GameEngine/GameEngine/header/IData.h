#pragma once
#include "nlohmann/json.hpp"
using Json = nlohmann::json;

/// <summary>
/// �ø�������� �� �����͵��� �������̽� Ŭ�����Դϴ�.
/// </summary>
class IData abstract
{
public:
	virtual ~IData() = default;

// 	virtual void Serialize() abstract;
// 	virtual void Deserialize() abstract;

};

