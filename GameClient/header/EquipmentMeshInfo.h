#pragma once
#include <string>
#include <vector>
#include <iostream>

struct EquipmentMeshInfo
{
	std::string className;
	int buffLv;
	std::string equipmentFBXname;
	std::string equipmentMeshname;
	int targetBoneNum; // �ε��� ���ڷ� �ٲ� ������
	std::string textureName;

	// �⺻ ������ -> ��� 
	EquipmentMeshInfo()
		:className(""), buffLv(-1), equipmentFBXname(""), equipmentMeshname(""), targetBoneNum(-1), textureName("") {}

	EquipmentMeshInfo(const std::vector<std::string>& _row)
	{
		try
		{
			className = _row[0];
			buffLv = std::stof(_row[1]);
			equipmentFBXname = _row[2];
			equipmentMeshname = _row[3];
			targetBoneNum = std::stof(_row[4]);
			textureName = _row[5];
		}
		catch(const std::exception& e)
		{
			std::cerr << "Exception parsing data for unit " << className << ": " << e.what() << std::endl;
		}
	}
};