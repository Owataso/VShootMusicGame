#include	"TDNLIB.h"
#include	"Fade.h"

//*********************************************************************************************
//		�p�����[�^�̐ݒ�
//*********************************************************************************************
Fade::MODE Fade::m_eMode;		/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
BYTE Fade::m_bySpeed;				/*	�t�F�[�h���鑬�x	*/
BYTE Fade::m_byLimit;				/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)	*/
BYTE Fade::m_byAlpha;				/*	���l(0�`255)	*/
DWORD Fade::m_dwColor;			/*	�J���[(0x����000000 �` 0x����ffffff)�����́����̕�����K��00�ɂ��Ă������ƁI	*/


//=============================================================================================
//		��	��	��
void Fade::Initialize()
{
	m_eMode = FADE_NONE;
	m_bySpeed = 0x01;
	m_byAlpha = 0xff;
	m_dwColor = 0x00000000;
}
//
//=============================================================================================


//=============================================================================================
//		�t�F�[�h�ݒ�
void Fade::Set(MODE eMode, BYTE bySpeed, BYTE byLimit, BYTE byStartAlpha, DWORD dwColor)
{
	m_eMode = eMode;		// �t�F�[�h���[�h(FADE_IN or FADE_OUT)
	m_bySpeed = bySpeed;		// �t�F�[�h�𑝌�������X�s�[�h(5�Ȃ�A���t���[���u5�v�����������)
	m_byLimit = byLimit;		// �ڕWm_byAlpha�l(FADE_IN�Ȃ�u0�v�AFADE_OUT�Ȃ�u255�v����{)
	m_byAlpha = byStartAlpha;	// �t�F�[�h�J�n����m_byAlpha�l(FADE_IN�Ȃ�u255�v�ADADE_OUT�Ȃ�u0�v����{)
	m_dwColor = dwColor;		// �t�F�[�h�F

	// 0x����ffffff�́��̕�����0�ɂ��鏈��(m_byAlpha�l�̒���)
	m_dwColor &= 0x00ffffff;
}


void Fade::Set(MODE eMode, BYTE bySpeed, DWORD dwColor)
{
	m_eMode = eMode;
	m_bySpeed = bySpeed;
	m_dwColor = dwColor;

	switch (m_eMode)
	{
	case FADE_IN:
		m_byAlpha = 255;
		m_byLimit = 0;
		break;

	case FADE_OUT:
		m_byAlpha = 0;
		m_byLimit = 255;
		break;

	case FADE_NONE:
		break;
	}
}
//
//=============================================================================================



//=============================================================================================
//		�X		�V
void Fade::Update()
{
	switch (m_eMode)
	{
	case FADE_IN:

		// �ړI���܂ł�����A�X�g�b�v
		if (m_byAlpha <= m_byLimit + m_bySpeed){		// BYTE�Ȃ̂ŃI�[�o�[�t���[�h�~
			m_byAlpha = m_byLimit;
			m_eMode = FADE_NONE;
		}
		else
			m_byAlpha -= m_bySpeed;

		break;

	case FADE_OUT:

		// �ړI���܂ł�����A�X�g�b�v
		if (m_byAlpha >= m_byLimit - m_bySpeed){		// BYTE�Ȃ̂ŃI�[�o�[�t���[�h�~
			m_byAlpha = m_byLimit;
			m_eMode = FADE_NONE;
		}
		else
			m_byAlpha += m_bySpeed;

		break;

	case FADE_NONE:
		break;	// NothingDo
	}
}


//=============================================================================================
//		�`			��
void Fade::Render(u32 dwFlags)
{
	// argb�̃t�H�[�}�b�g�ɂ���
	const DWORD l_cdwColor((m_byAlpha << 24) | m_dwColor);	

	// ��ʑS�̂Ɏl�p�`��`��
	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, dwFlags, l_cdwColor);
}
//
//=============================================================================================