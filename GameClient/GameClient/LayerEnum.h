#pragma once

enum class Layer
{
	Fade,	// ȿ��				 : 0
	EFFECT,	// ȿ��				 : 0
	POP_B,	// �˾�â ��ư, img	 : 1
	POP_P,	// �˾�â �г�		 : 2
	POP,	// �˾�â			 : 3
	SET_B,	// ����â ��ư, img	 : 4
	SET_P,	// ����â �г�		 : 5
	SET,	// ����â			 : 6
	COM_M,	// �Ϲ� �޼���â		 : 7
	COM_B,	// �Ϲ�â ��ư, img   : 8
	COM_P,	// �Ϲ�â �г�		 : 9
	COM,	// �Ϲ�â			 : 10
	BG_B,	// ��׶��� ��ư, img: 11
	BG_P,	// ��׶��� �г�	 : 12
	BG,		// ��׶���		 : 13

	Last,
};