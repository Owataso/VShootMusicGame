#pragma once
#include "TDNLIB.h"

/********************************************/
//	�L�[�{�[�h�̃K�C�h(�I���E����)�A�j���[�V����
/********************************************/
class KeyboardGuide
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	KeyboardGuide(const Vector2 &vPos, int iFlashCount, float fAlphaSpeed, float fMaxAlpha, float fMinAlpha, int iDelay=0) :
		m_ciDelay(iDelay),
		m_iDelayFrame(0),
		m_vPos(vPos),
		m_cfAlphaSpeed(fAlphaSpeed),
		m_cfSwicthAngle(2 * iFlashCount * PI),
		m_cfMinAlpha(fMinAlpha),
		m_cfMaxAlpha(fMaxAlpha),
		m_fAlphaSinCurve(0),
		m_bSwitch(true),
		m_pImage(new tdn2DObj(
#ifdef MIDI_INPUT_ON
			"DATA/UI/Other/guide.png"
#else
			"DATA/UI/Other/info_window.png"
#endif
			))
	{}

	//===============================================
	//	�X�V
	//===============================================
	void Update()
	{
		// �����܂Ŏ኱�̗P�\����������
		if (m_iDelayFrame < m_ciDelay) m_iDelayFrame++;

		// �Е��̓_�ł�����񐔂܂ŗ�����
		if ((m_fAlphaSinCurve += m_cfAlphaSpeed) > m_cfSwicthAngle)
		{
			// �_�Ń`�F���W
			m_fAlphaSinCurve = 0;
			m_bSwitch = !m_bSwitch;
		}
	}


	//===============================================
	//	�`��
	//===============================================
	void Render();

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	int m_iDelayFrame;					// �J�n���̓t�F�[�h�C�����銿���ŏo��̂ł���p
	const int m_ciDelay;				// �J�n���̓t�F�[�h�C�����銴���ŏo��̂ł���p
	float m_fAlphaSinCurve;				// �_�ł��T�C���J�[�u�ōs���Ă���̂ł���p�̕ϐ�
	bool m_bSwitch;						// �؂�ւ��t���O(���݂�2��ނȂ̂�bool�^�ɂ��Ă���)
	const Vector2 m_vPos;				// ���W
	const float m_cfAlphaSpeed;			// �_�ő��x
	const float m_cfSwicthAngle;		// ����_�ł�����؂�ւ�邩
	const float m_cfMinAlpha;			// �_�ł̍ŏ��A���t�@�l
	const float m_cfMaxAlpha;			// �_�ł̍ő�A���t�@�l
	std::unique_ptr<tdn2DObj> m_pImage;	// �摜
};
