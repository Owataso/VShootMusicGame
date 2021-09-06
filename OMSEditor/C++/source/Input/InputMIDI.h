#pragma once

#ifdef _DEBUG
#else
//#define MIDI_INPUT_ON
#endif

//===============================================
//	�s�A�m���͗p�̒萔
//===============================================
enum class EP_INPUT
{
	LEFT,		// ��(C)
	RIGHT,		// �E(D)
	START,		// �X�^�[�g�{�^��(�I�ȉ�ʂ̃I�v�V�����J��)
	SELECT,		// �Z���N�g�{�^��(�I�ȉ�ʂ̓�Փx�ύX)
	ENTER,		// ����(F or G or A or B)	�v�͌㔼4�̔���
	BACK,		// �߂�(F# or G# or A#)		�v�͌㔼3�̍���
	C,			// �I�v�V�����p(�h)
	D,			// �I�v�V�����p(��)
	E,			// �I�v�V�����p(�~)
	WHITE,		// ����
	BLACK,		// ����
	MAX
};

/********************************************/
//	MIDI���͊Ǘ�<Singleton>
/********************************************/
class InputMIDIManager
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static InputMIDIManager *GetInstance(){ static InputMIDIManager instance; return &instance; }

	//===============================================
	//	�������E���
	//===============================================
	void Initialize();
	void Release();

	//===============================================
	//	�X�V
	//===============================================
	void Update();

	//===============================================
	//	MIDI�M���擾�p�̃R�[���o�b�N�֐�
	//===============================================
	static void CALLBACK MidiInProc(HMIDIIN midi_in_handle, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

	//===============================================
	//	�A�N�Z�T�[
	//===============================================
	void SetBaseOctave(char val){ m_cBaseOctave = val; }
	void SetTone(char val){ if (val != val){ midiOutShortMsg(m_hMIDIOut, (0x000000C0 | (val << 8))); } }// ���F��ς���
	void SetVelocity(char val){ m_cVelocity = val; }
	void SetON(bool on, char no, bool bAuto = false);
	bool GetON(char no);
	void SetSoundOn(bool b){ m_bSoundOn = b; }
	void SetInputBasisOctave(char c){ m_cInputBasisOctave = c; }
	char GetInputBasisOctave(){ return m_cInputBasisOctave; }
	UINT GetNumOctave(){ return m_uiNumOctave; }
	void AllOff(){ memset(m_InputList, false, sizeof(m_InputList)); }

	//===============================================
	//	���͏��̎擾
	//===============================================
	int GetEPInputFrame(EP_INPUT type){return m_iInputFrames[(int)type]; }
	bool GetEPInputTRG(EP_INPUT type){ return (m_iInputFrames[(int)type] == 1); }
	bool GetEPSelectInput(EP_INPUT type, int speed = 11);
	bool GetAnyTrigger(){ FOR((int)EP_INPUT::MAX) { if (m_iInputFrames[i] == 1) return true; }return false; }

	//===============================================
	//	���͏����i�[����bool�^�̔z��
	//===============================================
	bool *m_InputList;	// �萔���w�b�_�[�ɏ����ƃA�E�g�Ȃ̂ŁA�ϒ��|�C���^�ɂ��āAcpp���Ŋm�ۂ���(48�̔z��Ǝv���Ă����������)

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	HMIDIOUT m_hMIDIOut;					// MIDI�A�E�g�n���h��
	HMIDIIN m_hMIDIIn;						// MIDI�C���n���h��
	char m_cTone;							// ���F
	char m_cBaseOctave;						// ��ԉ��̉�
	char m_cVelocity;						// ���̋���
	char m_cInputBasisOctave;				// MIDI�C���v�b�g�̔ԍ�-���̐����Ă��B
	UINT m_uiNumOctave;						// �ڑ����Ă���L�[�{�[�h�̃I�N�^�[�u�̐�
	bool m_bSoundOn;						// �L�[�������ĉ����o�邩�łȂ����̃t���O
	bool m_bInit;							// �������ς݂�(�������O�ɃR�[���o�b�N����Ă΂��Ɨ����邩��͂���)
	bool m_bMIDIIn;							// MIDI�������Ă邩
	char errmsg[MAXERRORLENGTH];			// �G���[���b�Z�[�W
	int m_iInputFrames[(int)EP_INPUT::MAX];	// EP����̓��͏��i�[(���͉������t���[�����ɂ���)

	//===============================================
	//	���̓`�F�b�N�p�֐�
	//===============================================
	bool(InputMIDIManager::*m_InputCheckFunctions[(int)EP_INPUT::MAX])();
	bool CheckLeft();
	bool CheckRight();
	bool CheckStart();
	bool CheckSelect();
	bool CheckEnter();
	bool CheckBack();
	bool CheckC();
	bool CheckD();
	bool CheckE();
	bool CheckWhite();
	bool CheckBlack();

	//===============================================
	//	�V���O���g���̍�@
	//===============================================
	InputMIDIManager();
	InputMIDIManager(const InputMIDIManager&){}
	InputMIDIManager &operator =(const InputMIDIManager&){}
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define InputMIDIMgr (InputMIDIManager::GetInstance())