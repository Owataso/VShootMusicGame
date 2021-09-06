#pragma once

/*
	�p���p
	�������z�֐���
*/

enum class MESSAGE_TYPE
{
	MSG_ERROR,
	PLAY_MUSIC,	// ���ʂƋȂ̍Đ�
	STOP_MUSIC,	// ���ʂƋȂ̒�~
	START_EDIT,	// �G�f�B�b�g�J�n
	END_EDIT,	// �G�f�B�b�g�I��
	EDIT_SELECT_NOTE,// �m�[�c�I��
	MOUSE_LEFT_DOWN,	// �}�E�X�N���b�N�����u��
	MOUSE_LEFT_UP,		// �}�E�X�������u��
	MOUSE_LEFT_DRAG,	// �}�E�X�h���b�O
	MOUSE_RIGHT_DOWN,	// �}�E�X�N���b�N�����u��
	MOUSE_RIGHT_UP,		// �}�E�X�������u��
	MOUSE_RIGHT_DRAG,	// �}�E�X�h���b�O
	COPY_NOTE,			// �m�[�c�ʒu�R�s�[
	PASTE_NOTE,			// �m�[�c�ʒu�y�[�X�g
	REVERSE_X,			// X���]
	REVERSE_Y			// Y���]
};

/*�d��\����*/
struct Message
{
	//// ����Telegram�𑗂����G���e�B�e�B
	//ENTITY_ID          Sender;

	//// ����Telegram���󂯎��G���e�B�e�B
	//ENTITY_ID          Receiver;

	// ���b�Z�[�W�̓��e�B�@�i�񋓌^�ɐݒ肳��Ă�����́j
	MESSAGE_TYPE          Msg;

	// ���b�Z�[�W�̒ǉ���� (�L�����N�^�[�������� [�ǉ����] X�ɂQ�ړ� )
	// �ǉ��������O�ɒm�邱�Ƃ��s�\�Ȃ̂�Void*�Œ�
	void*        ExtraInfo;

	// ���b�Z�[�W�̒x������
	int       DispatchTime;

	Message() :DispatchTime(-1),
		//Sender(ENTITY_ID::ID_ERROR),
		//Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::MSG_ERROR)
	{}

	Message(int time,
		MESSAGE_TYPE    msg,
		void*  info = nullptr) : DispatchTime(time),
		Msg(msg),
		ExtraInfo(info)
	{}

};

// State���N���X���L�����N�^���ɍė��p�\��
// �N���X�e���v���[�g�ɂ���Ɛl�����y�ɂȂ�B
template <class entity_type>
class State
{
public:
	virtual ~State() {};

	// ����
	virtual void Enter(entity_type*) = 0;

	// ���s���܂�
	virtual void Execute(entity_type*) = 0;

	// �A��
	virtual void Exit(entity_type*) = 0;

	
	// [�ǉ�]�`��
	virtual void Render(entity_type*){}

	// �摜�Ƃ��̕`�悪�K�v�ɂȂ����̂ŁA2D�̕`������܂���
	virtual void Render2D(entity_type*){}

	/*
		BaseGameEntity�̃X�e�[�g�����b�Z�[�W���󂯎��
		������I���ł���悤�ɏC������K�v������
	*/
	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(entity_type*, const Message&) = 0;

};
