#include "tdnlib.h"
#include "../Music/MusicInfo.h"
#include "InputMIDI.h"
#include "../UI/ui.h"

#define MIDIMSG(a,b,c,d) ((DWORD)((a<<24) | (b<<16) | (c<<8) | (d)))

bool InputMIDIManager::GetEPSelectInput(EP_INPUT type, int speed)
{
	return (
		GetEPInputFrame(type) == 1 || 
		GetEPInputFrame(type) > 0 && InputMIDIMgr->GetEPInputFrame(type) % speed == 0);
}

void CALLBACK InputMIDIManager::MidiInProc(HMIDIIN midi_in_handle,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	switch (wMsg)
	{
	case MIM_OPEN:
		//printf("MIDI device was opened\n");
		break;
	case MIM_CLOSE:
		//printf("MIDI device was closed\n");
		break;
	case MIM_DATA:
	{
					 const BYTE l_byEvent(dwParam1 & 0x000000ff);

					 // �m�[�g�I�t�C�x���g
					 if (l_byEvent == 0x80)
					 {
						 const BYTE note((dwParam1 & 0x0000ff00) >> 8);

						 // �R�[���o�b�N����Ȃ����ɓn���Ă�����
						 InputMIDIMgr->SetON(false, note);
					 }
					 // �m�[�g�I���C�x���g
					 else if (l_byEvent == 0x90)
					 {
						 const BYTE velocity((BYTE)((dwParam1 & 0x00ff0000) >> 16));
						 const BYTE note((dwParam1 & 0x0000ff00) >> 8);
						 //printf("MIM_DATA: wMsg=%08X, p1=%08X, p2=%08X\n", wMsg, dwParam1, dwParam2);
						 //printf("note = %u, velocity = %u\n", note, velocity);

						 // �R�[���o�b�N����Ȃ����ɓn���Ă�����
						 InputMIDIMgr->SetON((velocity != 0), note);	// velocity��0�Ƃ������͂��Ȃ킿���Ղ������ꂽ�u��
					 }
					 break;
	}
		break;
	case MIM_LONGDATA:
	case MIM_ERROR:
	case MIM_LONGERROR:
	case MIM_MOREDATA:
	default:
		break;
	}
}


//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
InputMIDIManager::InputMIDIManager() :m_bInit(false), m_bSoundOn(false), m_cBaseOctave(0), m_cVelocity(127), m_bMIDIIn(false), m_uiNumOctave(MAX_OCTAVE){}
void InputMIDIManager::Initialize()
{
	HRESULT hr;

#ifdef MIDI_INPUT_ON
	//tdnSystem::OpenDebugWindow();	// �R���\�[��ON

	// MIDI�A�E�g�f�o�C�X����
	//MIDIOUTCAPS OutCaps;
	//UINT NumDevice = midiOutGetNumDevs();
	//printf("MIDIOutputDecices:\n");
	//for (UINT i = 0; i < NumDevice; i++)
	//{
	//	if (midiOutGetDevCaps(i, &OutCaps, sizeof(OutCaps)) == MMSYSERR_NOERROR)
	//	{
	//		printf("ID = %d, %s\n", i, OutCaps.szPname);
	//	}
	//}

	//// MIDI�C���v�b�g�f�o�C�X����
	//MIDIINCAPS InCaps;
	//printf("MIDIInputDecices:\n");
	//UINT NumDevice = midiInGetNumDevs();
	//for (UINT i = 0; i < NumDevice; i++)
	//{
	//	if (midiInGetDevCaps(i, &InCaps, sizeof(InCaps)) == MMSYSERR_NOERROR)
	//	{
	//		printf("ID = %d, %s\n", i, InCaps.szPname);
	//	}
	//}
	//
	//printf("�g�p����InputMIDI�̔ԍ���I�����Ă�������->");
	UINT DeviceID(0);
	//std::cin >> DeviceID;

	// ���ɓ����Ă�f�o�C�XID�Ɗ�{�I�N�^�[�u�̃e�L�X�g��ǂݍ���
	{
		// ifstream�ł���Ă������̂����AFILE�ł���Ă݂����C���������̂ł���Ă݂�
		FILE *fp;
		fopen_s(&fp, "Keyboard.txt", "rb");
		MyAssert(fp, "�L�[�{�[�h�f�o�C�X�̃e�L�X�g�����ĂȂ��I");

		auto ReadNumber = [](FILE *fp)
		{
			int ret(0);
			const int DIGIT = 4;	// �Ƃ肠����4��
			char receive[DIGIT];
			for (int i = 0; i < DIGIT; i++)
			{
				fread(&receive[i], 1, 1, fp);

				// ��������Ȃ�(�I��)
				if ((receive[i] < '0') || (receive[i] > '9'))
				{
					//int mul(1);	// �����Z�p
					//for (int j = i - 1; j >= 0; j--)
					//{
					//	if ((receive[j] < '0') || (receive[j] > '9')) MyAssert((receive[j] >= '0' && receive[j] <= '9'), "KeyboardInfo.txt����������Ȃ������������Ă�");
					//	ret += (int)(receive[j] - '0') * mul;
					//	mul *= 10;
					//}
					receive[i] = '\0';
					ret = atoi(receive);
					break;	// �Y��Ă�����Ђǂ����ƂɂȂ���
				}
			}
			return ret;
		};

		char skip;
		// 1�s�ڂ�:�܂�
		do
		{
			fread(&skip,1,1,fp);
		}while(skip != ':');

		// �����ǂݍ���
		DeviceID = ReadNumber(fp);

		// 2�s�ڂ�:�܂�
		do
		{
			fread(&skip, 1, 1, fp);
		} while (skip != ':');

		// �����ǂݍ���
		m_cInputBasisOctave = ReadNumber(fp);
		m_cInputBasisOctave *= 12;	// �I�N�^�[�u������ɂ��������̂ŁA���̎d�l�ɂ���

		// 3�s�ڂ�:�܂�
		do
		{
			fread(&skip,1,1,fp);
		}while(skip != ':');

		// �����ǂݍ���
		m_uiNumOctave = (UINT)ReadNumber(fp);

		fclose(fp);
	}

	hr = midiInOpen(&m_hMIDIIn, DeviceID, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
	if (hr == MMSYSERR_NOERROR)
	{
		// MIDI�C���v�b�g�J�n
		midiInStart(m_hMIDIIn);
		m_bMIDIIn = true;
	}
	else
	{
		midiInGetErrorText(hr, errmsg, sizeof(errmsg));
		std::string msg = "MIDI�L�[�{�[�h�ڑ����s�I\nKeyboardChecker�ŃL�[�{�[�h�̐ݒ�����Ă�����x�N�����Ă��������B\nErrMsg:";
		msg += errmsg;
		MessageBox(0, msg.c_str(), "ElectronicPianism", MB_OK);
		m_bMIDIIn = false;
	}

	//tdnSystem::CloseDebugWindow();
#endif

	// MIDI�̉��o���������
	hr = midiOutOpen(&m_hMIDIOut, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
	assert(hr == MMSYSERR_NOERROR);

	// �L�[�{�[�h��ONOFF���
	m_InputList = new bool[NUM_KEYBOARD];
	memset(m_InputList, false, sizeof(bool)*NUM_KEYBOARD);

	// �Q�[������p�L�[�{�[�h�C���v�b�g�t���[��
	memset(m_iInputFrames, 0, sizeof(m_iInputFrames));
	// �C���v�b�g�`�F�b�N�p�֐��Ɗ֐��|�C���^
	m_InputCheckFunctions[(int)EP_INPUT::LEFT] = &InputMIDIManager::CheckLeft;
	m_InputCheckFunctions[(int)EP_INPUT::RIGHT] = &InputMIDIManager::CheckRight;
	m_InputCheckFunctions[(int)EP_INPUT::START] = &InputMIDIManager::CheckStart;
	m_InputCheckFunctions[(int)EP_INPUT::SELECT] = &InputMIDIManager::CheckSelect;
	m_InputCheckFunctions[(int)EP_INPUT::ENTER] = &InputMIDIManager::CheckEnter;
	m_InputCheckFunctions[(int)EP_INPUT::BACK] = &InputMIDIManager::CheckBack;
	m_InputCheckFunctions[(int)EP_INPUT::C] = &InputMIDIManager::CheckC;
	m_InputCheckFunctions[(int)EP_INPUT::D] = &InputMIDIManager::CheckD;
	m_InputCheckFunctions[(int)EP_INPUT::E] = &InputMIDIManager::CheckE;
	m_InputCheckFunctions[(int)EP_INPUT::WHITE] = &InputMIDIManager::CheckWhite;
	m_InputCheckFunctions[(int)EP_INPUT::BLACK] = &InputMIDIManager::CheckBlack;

	// ������OK
	m_bInit = true;
}

//------------------------------------------------------
//	���
//------------------------------------------------------
void InputMIDIManager::Release()
{
	//AllOff();

	delete[] m_InputList;
	midiOutClose(m_hMIDIOut);

#ifdef MIDI_INPUT_ON
	if (m_bMIDIIn)
	{
		// MIDI�C���v�b�g���
		midiInStop(m_hMIDIIn);
		midiInReset(m_hMIDIIn);
		midiInClose(m_hMIDIIn);
	}
#endif
}


//*****************************************************************************************************************************
//	�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//	�X�V
//------------------------------------------------------
void InputMIDIManager::Update()
{
	FOR((int)EP_INPUT::MAX)
	{
		// �C���v�b�g�X�V
		m_iInputFrames[i] = (this->*m_InputCheckFunctions[i])() ?m_iInputFrames[i] + 1 : 0;	// �����Ă���t���[���𑫂��Ă���
	}
}

//------------------------------------------------------
//		���̃I���I�t
//------------------------------------------------------
bool InputMIDIManager::GetON(char no)
{
	assert(no >= 0 && no < NUM_KEYBOARD);
	return m_InputList[no];
}

//------------------------------------------------------
//		�m�[�g�I��
//------------------------------------------------------
void InputMIDIManager::SetON(bool ON, char no, bool bAuto)
{
#ifdef MIDI_INPUT_ON
	// ���������ĂȂ�������͂���(�R�[���o�b�N���炱�̊֐����Ă΂�邩��)
	if (!m_bInit) return;

	if(!bAuto)
	{
		if (no < m_cInputBasisOctave ||				// �ݒ肵���͈͂�菬����
			no > m_cInputBasisOctave + NUM_KEYBOARD - 1	// �ݒ肵���͈͂��傫��
			)return;

		no -= m_cInputBasisOctave;	// �ݒ肵���I�N�^�[�u�ɒ���
	}
#endif

	if (ON)
	{
		if (!m_InputList[no])
		{
			m_InputList[no] = true;
			if (m_bSoundOn) midiOutShortMsg(m_hMIDIOut, MIDIMSG(0x00, m_cVelocity, (m_cBaseOctave + no), 0x90)); // ����
		}
	}
	else
	{
#ifdef MIDI_INPUT_ON
		//if (m_InputList[no]) midiOutShortMsg(hmo, MIDIMSG(0x00, 0x70, (base_octave + no), 0x80)); // ����
#endif
		m_InputList[no] = false;
	}
}

//------------------------------------------------------
//		EP�p�̓���
//------------------------------------------------------
bool InputMIDIManager::CheckLeft()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12]) return true;		// ���̃t���[����C��������Ă���
	return false;	// ���̃t���[����C�͉�����Ȃ�����
#else
	return ((KeyBoard(KB_LEFT)) || 
		//(KeyBoard(KB_A)) || 
		(KeyBoard(KB_UP))
		//|| (KeyBoard(KB_W))
		);	// �L�[�{�[�h�ō�����֘A�̃L�[
#endif
}

bool InputMIDIManager::CheckRight()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 2]) return true;		// ���̃t���[����D��������Ă���
	return false;	// ���̃t���[����D�͉�����Ȃ�����
#else
	return ((KeyBoard(KB_RIGHT)) || 
		//(KeyBoard(KB_D)) || 
		(KeyBoard(KB_DOWN))
		//|| (KeyBoard(KB_S))
		);	// �L�[�{�[�h�ŉE�����֘A�̃L�[
#endif
}

bool InputMIDIManager::CheckStart()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 1]) return true;		// ���̃t���[����C#��������Ă���
	return false;	// ���̃t���[����C�͉�����Ȃ�����
#else
	return (KeyBoard(KB_W) > 0);	// �L�[�{�[�h��1�L�[
#endif
}

bool InputMIDIManager::CheckSelect()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 3]) return true;		// ���̃t���[����D#��������Ă���
	return false;	// ���̃t���[����C�͉�����Ȃ�����
#else
	return (KeyBoard(KB_E) > 0);	// �L�[�{�[�h��2�L�[
#endif
}

bool InputMIDIManager::CheckEnter()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 5] || m_InputList[i * 12 + 7] || m_InputList[i * 12 + 9] || m_InputList[i * 12 + 11]) return true;		// ���̃t���[����F or G or A or B��������Ă���
	return false;	// ���̃t���[����F or G or A or B�͉�����Ȃ�����
#else
	return (KeyBoard(KB_ENTER) > 0);	// �L�[�{�[�h�ŃG���^�[�L�[
#endif
}

bool InputMIDIManager::CheckBack()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 6] || m_InputList[i * 12 + 8] || m_InputList[i * 12 + 10]) return true;		// ���̃t���[����F# or G# or A#��������Ă���
	return false;	// ���̃t���[����F# or G# or A#�͉�����Ȃ�����
#else
	return (KeyBoard(KB_SPACE) > 0);	// �L�[�{�[�h�ŃX�y�[�X�L�[
#endif
}

bool InputMIDIManager::CheckC()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12]) return true;		// ���̃t���[����C��������Ă���
	return false;	// ���̃t���[����C�͉�����Ȃ�����
#else
	return (KeyBoard(KB_A) > 0);	// �L�[�{�[�h��2�L�[
#endif
}

bool InputMIDIManager::CheckD()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 2]) return true;		// ���̃t���[����D��������Ă���
	return false;	// ���̃t���[����F or G or A or B�͉�����Ȃ�����
#else
	return (KeyBoard(KB_S) > 0);	// �L�[�{�[�h��X�L�[
#endif
}

bool InputMIDIManager::CheckE()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 4]) return true;		// ���̃t���[����E��������Ă���
	return false;	// ���̃t���[����F# or G# or A#�͉�����Ȃ�����
#else
	return (KeyBoard(KB_D) > 0);	// �L�[�{�[�h��C�L�[
#endif
}

bool InputMIDIManager::CheckWhite()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12] || m_InputList[i * 12 + 2] || m_InputList[i * 12 + 4] || m_InputList[i * 12 + 5] || m_InputList[i * 12 + 7] || m_InputList[i * 12 + 9] || m_InputList[i * 12 + 11]) return true;		// ���̃t���[���ɔ�����������Ă���
	return false;	// ���̃t���[���ɔ����͉�����Ȃ�����
#else
	return (KeyBoard(KB_ENTER) > 0);	// �L�[�{�[�h�ŃG���^�[�L�[
#endif
}

bool InputMIDIManager::CheckBlack()
{
#ifdef MIDI_INPUT_ON
	FOR(MAX_OCTAVE)if (m_InputList[i * 12 + 1] || m_InputList[i * 12 + 3] || m_InputList[i * 12 + 6] || m_InputList[i * 12 + 8] || m_InputList[i * 12 + 10]) return true;		// ���̃t���[���ɍ�����������Ă���
	return false;	// ���̃t���[���ɍ����͉�����Ȃ�����
#else
	return (KeyBoard(KB_SPACE) > 0);	// �L�[�{�[�h�ŃX�y�[�X�L�[
#endif
}

