#pragma once

//------------------------------------------------------
//	�O���錾
//------------------------------------------------------
struct Note;
struct Soflan;
class BaseMusicInfo;
enum class JUDGE_TYPE;
enum class GAUGE_OPTION;
struct GameScore;
class SceneMain;

/********************************************/
//	���t��ʂ�UI�Ǘ��N���X
/********************************************/
class UI
{
public:
	//------------------------------------------------------
	//	�R���X�g���N�^
	//------------------------------------------------------
	UI();

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	void Initialize(SceneMain *pMain, GAUGE_OPTION GaugeOP);

	//------------------------------------------------------
	//	�`��
	//------------------------------------------------------
	void Render();
	void RenderBack();	// �w�i�`��(UI�œ��������Ȃ����߂ɕ�����)
	void RenderMusicName(char *name);


	//------------------------------------------------------
	//	�A�N�Z�T
	//------------------------------------------------------
	void SetBeam(char no, bool on);
	void SetBeam(bool list[]);

private:
	//------------------------------------------------------
	//	�摜��ID
	//------------------------------------------------------
	enum IMAGE
	{
		BACK,
		GUIDE,
		NOTE_GUIDE,
		SKIN,
		STAGE,
		NOTE,
		BEAM,
		SUDDEN,
		NUMBER,
		DIFFICULTY,
		GAUGE,
		JUDGE_BAR,
		ON_LIGHT,	// ���Ղ������Đ�����摜
		MAX
	};

	//------------------------------------------------------
	//	�����o�ϐ�
	//------------------------------------------------------
	int m_iGaugeSrcY;				// �Q�[�W�̂Ƃ��Ă�����W(�摜�ߖ�)
	bool m_bBeam[CHAR_MAX];			// �r�[��
	SceneMain *m_pMain;				// �V�[�����C��
	GameScore *m_pGameScore;		// �Q�Ƃ��邾��Score
	std::unique_ptr<tdn2DObj> m_pImages[IMAGE::MAX];	// �摜����

	//------------------------------------------------------
	//	�����o�֐�
	//------------------------------------------------------
	void InfoRender();		// ��{�I�ȏ��̕`��
	void SuddenRender();	// �T�h�v���̕`��
	void OptionRender();	// �X�y�[�X�L�[�����ꂽ��ĂԁB�n�C�X�s�A�T�h�v���̐����Ƃ��̕\��
	void NoteRender();		// �m�[�c�`��
	void BeamRender();		// �Ō��r�[���`��
	void PushRender();		// ���݂���
	void GuideNoteRender(int x, int y, int lane);	// �m�[�c�̃K�C�h�`��
	BOOL isBlackKey(int lane);	// �������ǂ���
};
