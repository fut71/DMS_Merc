#pragma once

enum CollisionType	// word0
{
	TYPE_GROUND = 1 << 0, // ��
	TYPE_UNIT = 1 << 1, // ����
	TYPE_PROJECTILE = 1 << 2, // ����ü
	TYPE_WEAPON = 1 << 3, // ����
	TYPE_GOLD = 1 << 4, // ��ȭ
	TYPE_OBSTACLE = 1 << 5  // ��ֹ�
};

enum CollisionAttribute	// word1
{
	ATTR_ALLY = 1 << 0, // �Ʊ�
	ATTR_ENEMY = 1 << 1, // ����
	ATTR_ON_GROUND = 1 << 2, // ���鿡 �پ� �ִ��� ����
	ATTR_IS_DEAD = 1 << 3,
	ATTR_ON_OBSTACLE = 1 << 4,	// ��ֹ��� �浹�ߴ���
};