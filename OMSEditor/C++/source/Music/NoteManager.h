#pragma once

//===============================================
//	�O���錾
//===============================================
struct NoteDataEP;
struct NoteDataVRShoot;
struct SoflanInfo;
class MusicInfoEP;
class MusicInfoVRShoot;
class SoflanManager;
class JudgeManager;


/********************************************/
//	����Ă���m�[�c�̏��
/********************************************/
struct PlayNoteInfo
{
	//UINT *EventTimeArray;	// ���\�t��������Ƃ����̂ŁA�\�t�������̃C�x���g�^�C��������
	//std::shared_ptr<NoteDataEP> pNote;		// �m�[�c����
	NoteDataEP *pNoteEP;
	NoteDataVRShoot *pNoteVRShoot;
	int iPosition;			// �ʒu
	bool bErase;			// ��ڂ��I������
	WORD wIndex;

	PlayNoteInfo():pNoteEP(nullptr),pNoteVRShoot(nullptr), wIndex(0){}
	~PlayNoteInfo(){}

	void SetNote(NoteDataEP *pNote)
	{
		pNoteEP = pNote;
		pNoteVRShoot = nullptr;
	}
	void SetNote(NoteDataVRShoot *pNote)
	{
		pNoteVRShoot = pNote;
		pNoteEP = nullptr;
	}
	void UpdatePos(DWORD dwCurrentPosition);
};


/********************************************/
//	�m�[�c�Ǘ��N���X
/********************************************/
class NoteManager
{
public:
	//===============================================
	//	�R���X�g���N�^�E�f�X�g���N�^
	//===============================================
	NoteManager();
	~NoteManager();

	//===============================================
	//	�������E���
	//===============================================
	void Initialize(SoflanManager *pSoflanMgr, JudgeManager *pJudgeMgr);

	//===============================================
	//	�X�V
	//===============================================
	void Update(DWORD dwCurrentPosition);
	void EditUpdate(DWORD dwCurrentPosition);

	//===============================================
	//	�A�N�Z�T
	//===============================================
	std::list<PlayNoteInfo*> *GetList();	// ���X�g�擾
	UINT GetNumNorts(){ return m_iNumNorts; }
	void GetStartNotesLane(std::vector<int> *pOutLanes);
	void GetNoteAbsolutePos(const PlayNoteInfo &note, int *pos_y);	// ��ΓI�ȃm�[�c���W(��ɔ���Ŏg�p)
	void GetNoteHiSpeedPos(const PlayNoteInfo &note, int *pos_y);	// BPM�Ƃ��A�n�C�X�s�Ɉˑ������m�[�c�̍��W(���UI�Ŏg�p)
	void Set(MusicInfoEP *info);
	void Set(MusicInfoVRShoot *info, DWORD dwOffset = 0);

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	SoflanManager *m_pSoflanMgr;		// �\�t�����Ǘ����w������
	JudgeManager *m_pJudgeMgr;		// ����Ǘ�����̎��̂��w������
	UINT m_iNumNorts;					// �m�[�c��
	//char m_cBaseOctave;					// ��{�I�N�^�[�u
	//std::list<PlayNoteInfo*> *m_Lists;	// �m�[�c���X�g
	std::list<PlayNoteInfo*> m_NoteList;	// �m�[�c���X�g(�P��)
	bool isVR;

	//===============================================
	//	�����o�֐�
	//===============================================
	void Clear();	// �m�[�c�N���A
	void SetTick(int second, float bpm);
	// �œK���O�ƍœK����ŕ������m�[�c�X�V
	void EPUpdate(DWORD dwCurrentPosition);
	void VRShootUpdate(DWORD dwCurrentPosition);
};