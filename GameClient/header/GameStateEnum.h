#pragma once
enum class GameState
{
	NORMAL,				// ���� ���۽� ���� ����
	START_ANIMATION,	// ���� ���� �� ī�޶� ���� ����
	PRE_PLACEMENT,		// ī�޶� ���� �� UI ó��
	PLACEMENT,			// ��ġ
	PRE_PLAY,			// ���� ���� �� ó��
	PLAY,				// ���� ����
	NONE_UI,			// UI ������ ó��
	POST_PLAY_ANIMATION,// ���� �Ϸ� �� ī�޶� ���� ����
	POST_PLAY,			// ī�޶� ���� �� UI ó��
	RESULT,				// ��� â ó��
	RESTART,			// ���� ����� ����

	TUTORIALEND,		// Ʃ�丮�� ���� ����. �ƹ��͵� ó������ �ʴ´�.
	NEXTLEVEL, 

	LAST,
};

enum class SettingState
{
	OPEN,         // ����â�� ���� ��ư�� ���� ����
	ADJUST,       // ����â�� ���� ���� ����
	CLOSED,       // ����â�� �ݴ� ��ư�� ���� ����
	NORMAL,		  // ����â�� ������ �ִ� ��� ����
	NONE,		  // �������� �������� ���� �� �ٲ�� ����(���Ŀ� �� ��)	

	LAST,
};

enum class PopUpState
{
	OPEN_MAINMENU,	// �˾�â�� ���� ��ư�� ���� ����
	OPEN_UNITDEPLOY,// �˾�â�� ���� ��ư�� ���� ����
	OPEN_EXITGAME,	// �˾�â�� ���� ��ư�� ���� ����
	OPEN_SKIP,		// ��ŵ �˾�â�� ���� ��ư�� ���� ����
	MAINMENU,		// ���� �޴� �˾� ����
	UNITDEPLOY,		// ���� ��ġ �˾� ����
	EXITGAME,		// ���� �˾� ����
	SKIP,
	CLOSED,			// �˾�â�� �ݴ� ��ư�� ���� ����
	NORMAL,			// �˾��� �����ִ� ����

	LAST,
};

enum class UIAnimationState
{
	POSTGAMESTART,	// ���� ��ŸƮ ���� �� �ִϸ��̼� �۵�(���� ���)
	NORMAL,			// �Ϲ����� �۵� ��(���� �޴�, ��Ʋ, �ε�) -> �� �κ��� ���� ������ �ڵ��� ���� �Ǿ�� ��
	PRELOADING,		// �ε� �� �ִϸ��̼� �۵�(���� ���)
	POSTLOATING,	// �ε� �� �ִϸ��̼� �۵�(�Ʒ��� �ϰ�)
	PREBATTLE,		// ���� �� �ִϸ��̼� �۵�(���� ����)
	POSTBATTLE,		// ���� �� �ִϸ��̼� �۵�(���� ����)
	PRETUTORIAL,	// Ʃ�丮�� �� �ִϸ��̼� �۵� (���� ����) -> ���� ���°� �� �ٸ��Ƿ� �׳� �и��Ѵ�.
	POSTTUTORIAL,	// Ʃ�丮�� �� �ִϸ��̼� �۵� (���� ����)

	VISIBLE,		// UI�� ���̰� �ϴ� ��� -> �� �κ��� ���� ������ �ڵ��� ���� �Ǿ�� ��
	UNVISIBLE,	// UI�� �� ���̰� �ϴ� ��� -> �� �κ��� ���� ������ �ڵ��� ���� �Ǿ�� ��
	NEXTSCENE,		// ���� ������ �Ѿ��� ������ ���� -> �� �κ��� ���� ������ �ڵ��� ���� �Ǿ�� ��

	// [���� ȭ��] : NORMAL(����) -> PREGAMESTART -> NEXTSCENE ->
	// [�ε� ȭ��] : PRELOADING -> NORMAL(�ε�) -> POSTLOADING -> NEXTSCENE ->
	// [���� ȭ��] : PREBATTLE -> NORMAL(��Ʋ) -> ??? -> NEXTSCENE ->
	// [���� ȭ��] : NORMAL(��Ʋ) -> ??? -> NEXTSCENE(����) -> ??? -> ũ����

	/// Ʃ�丮�� ���� �κе� �� �ٲ��� ��
	/// ��Ʋ �� ���� �κ� FadeINout ��ü �����ǰ� UI�� ��ü �� ����(����)

	LAST,
};