#include "CustomFilter.h"

physx::PxFilterFlags CustomFilter::CustomFilterShader(physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize)
{
	// ����� �浹���� ���
	if ((_filterData0.word0 & TYPE_GROUND) || (_filterData1.word0 & TYPE_GROUND))
	{
		// ���鳢���� �浹���� ����
		if ((_filterData0.word0 & TYPE_GROUND) && (_filterData1.word0 & TYPE_GROUND))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_UNIT) || (_filterData1.word0 & TYPE_UNIT))
		{
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;
		}
		// ��������� �浹�������� ����
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// ����ü�� ��� ��ü ������Ű�� ���� ����
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;
		}
		// ���� on ground �÷��� �߰�
		else if (_filterData0.word0 & TYPE_GOLD)
		{
			_filterData0.word1 |= ATTR_ON_GROUND;
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;

		}
		else if (_filterData1.word0 & TYPE_GOLD)
		{
			_filterData1.word1 |= ATTR_ON_GROUND;
			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return physx::PxFilterFlag::eDEFAULT;

		}

		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// ��ֹ��� �浹���� ���
	if ((_filterData0.word0 & TYPE_OBSTACLE) || (_filterData1.word0 & TYPE_OBSTACLE))
	{
		// �ٸ� ��ֹ����� �浹���� ����
		if ((_filterData0.word0 & TYPE_OBSTACLE) && (_filterData1.word0 & TYPE_OBSTACLE))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// ��������� �浹�������� ����
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}
		// ����ü�� ��� sleep���·� �ٲ�
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
// 			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 			return physx::PxFilterFlag::eDEFAULT;
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// ����, ����, �ٸ� ��ֹ����� ���������� �浹��
		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// ���ְ� �浹���� ���
	if ((_filterData0.word0 & TYPE_UNIT) || (_filterData1.word0 & TYPE_UNIT))
	{
		if ((_filterData0.word1 & ATTR_IS_DEAD) || (_filterData1.word1 & ATTR_IS_DEAD))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// ���ֳ����� ���� �浹 ������
		if ((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_UNIT))
		{
// 			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
// 			else
// 			{
// 				return physx::PxFilterFlag::eSUPPRESS;
// 			}
		}
		// �ٸ� ���� ��������� �浹 �� Ʈ���� �ߵ�
		else if ((_filterData0.word0 & TYPE_WEAPON) || (_filterData1.word0 & TYPE_WEAPON))
		{
			// ���� �ٸ� ���� ���
			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			// �̿��� �ٸ� ��ü����� �浹���� ����
			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
		{
			if ((_filterData0.word1 & ATTR_ON_GROUND) || (_filterData1.word1 & ATTR_ON_GROUND))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			return physx::PxFilterFlag::eSUPPRESS;
		}
		else if ((_filterData0.word0 & TYPE_PROJECTILE) || (_filterData1.word0 & TYPE_PROJECTILE))
		{
			// ���� �ٸ� ���� ���
			if ((_filterData0.word1 & ATTR_ALLY) != (_filterData1.word1 & ATTR_ALLY))
			{
				_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				return physx::PxFilterFlag::eDEFAULT;
			}

			return physx::PxFilterFlag::eSUPPRESS;
		}

	}

	// ���� �浹���� ���
	if ((_filterData0.word0 & TYPE_GOLD) || (_filterData1.word0 & TYPE_GOLD))
	{
		if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GOLD))
		{
			// ���� �� �� �ֵ���
		}

		return physx::PxFilterFlag::eSUPPRESS;
	}

	return physx::PxFilterFlag::eSUPPRESS;

}

// physx::PxFilterFlags CustomFilter::CustomFilterShader2(
// 	physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
// 	, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
// 	, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize)
// {
// 	// ����ü�鳢�� �浹���� �ʵ��� ����
// 	if ((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_PROJECTILE))
// 	{
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GOLD))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	// ���߿��� ������ ���� ���� �ٸ� ��ü�� �浹���� ����
// 	if (((_filterData0.word0 & TYPE_GOLD) && !(_filterData1.word0 & TYPE_GROUND)) ||
// 		((_filterData1.word0 & TYPE_GOLD) && !(_filterData0.word0 & TYPE_GROUND)))
// 	{
// 		if (_filterData0.word1 & ~ATTR_ON_GROUND || _filterData1.word1 & ~ATTR_ON_GROUND)
// 		{
// 			return physx::PxFilterFlag::eSUPPRESS;
// 		}
// 	}
// 
// 	// ���� ���� ��쿡�� ���ֳ����� �浹�ϵ��� ����
// 	if (((_filterData0.word1 & ATTR_ALLY) && (_filterData1.word1 & ATTR_ALLY)) ||
// 		((_filterData0.word1 & ATTR_ENEMY) && (_filterData1.word1 & ATTR_ENEMY)))
// 	{
// 		// ���ֳ��� �浹 �߻�
// 		if ((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_UNIT))
// 		{
// 			_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
// 			return physx::PxFilterFlag::eDEFAULT;
// 		}
// 
// 		// ������ �ƴ� �ٸ� ���� �浹 ����
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
// 		return physx::PxFilterFlag::eDEFAULT;
// 	}
// 
// 	// ����� ������ �浹 ó�� (Ʈ���� �ߵ�)
// 	if (((_filterData0.word0 & TYPE_WEAPON) && (_filterData1.word0 & TYPE_UNIT)) ||
// 		((_filterData0.word0 & TYPE_UNIT) && (_filterData1.word0 & TYPE_WEAPON)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eDEFAULT;
// 	}
// 
// 	// ��ȭ�� ����� �浹���� �� ó��
// 	if ((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_GROUND))
// 	{
// 		_filterData0.word1 |= ATTR_ON_GROUND;
// 	}
// 	else if ((_filterData1.word0 & TYPE_GOLD) && (_filterData0.word0 & TYPE_GROUND))
// 	{
// 		_filterData1.word1 |= ATTR_ON_GROUND;
// 	}
// 
// 	// ��ȭ�� ������ ���� ó��
// 	if (((_filterData0.word0 & TYPE_GOLD) && (_filterData1.word0 & TYPE_UNIT)) ||
// 		((_filterData1.word0 & TYPE_GOLD) && (_filterData0.word0 & TYPE_UNIT)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
// 		return physx::PxFilterFlag::eSUPPRESS;
// 	}
// 
// 	// ����ü�� ��ֹ� �� ����� �浹�� �� ó��
// 	if (((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_OBSTACLE)) ||
// 		((_filterData0.word0 & TYPE_OBSTACLE) && (_filterData1.word0 & TYPE_PROJECTILE)) ||
// 		((_filterData0.word0 & TYPE_PROJECTILE) && (_filterData1.word0 & TYPE_GROUND)) ||
// 		((_filterData0.word0 & TYPE_GROUND) && (_filterData1.word0 & TYPE_PROJECTILE)))
// 	{
// 		_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
// 		return physx::PxFilterFlag::eKILL;
// 	}
// 
// 	// �⺻ �浹 ó��
// 	_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
// 	return physx::PxFilterFlag::eDEFAULT;
// }
