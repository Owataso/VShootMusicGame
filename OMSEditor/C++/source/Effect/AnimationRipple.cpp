
#include "AnimationRipple.h"

AnimationRipple::AnimationRipple(tdn2DObj *pImage, int EndFrame, int alphaNearFrame, int alphaFarFrame, float StartScale, float streach, bool IsRoop) :m_pImage(pImage), m_bAction(false), m_bRoop(IsRoop), m_alpha(0), m_AlphaFar(alphaFarFrame), m_AlphaNear(alphaNearFrame), m_EndFrame(EndFrame), m_scale(1), m_StartScale(StartScale), m_streach(streach), m_frame(0){}

void AnimationRipple::Action()
{
	m_bAction = true;
	m_frame = 0;
	m_scale = m_StartScale;// �X�P�[���������ʒu��
}

void AnimationRipple::Stop()
{
	m_bAction = false;
	m_frame = 0;
}

void AnimationRipple::Update()
{
	if (!m_bAction)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// �t���[���X�V
	m_frame++;
	if (m_bRoop == true)// ���[�v��������alphaFar�̑O�̂Ƃ��Ń��[�v�������鏈��
	{
		if (m_frame >= (m_AlphaFar - 1))
		{
			m_frame = (m_AlphaFar - 1);
			return;
		}
	}
	// �G���h�t���[���܂ŗ�����I���
	if (m_frame >= m_EndFrame) m_bAction = false;

	// �g�嗦�X�V
	m_scale += m_streach;


	// �����x�X�V
	// ����A�[�̑O�����Ŕ����ς���
	if (m_frame >= m_AlphaNear)
	{
		// 100-100=0  100-50=50   0/50
		m_alpha = ((float)m_EndFrame - m_frame) / ((float)m_EndFrame - m_AlphaFar);
	}
	else
	{
		// �ŏ���
		m_alpha = (float)m_frame / m_AlphaNear;//   0/30=0   60/30=2   1-(0~1)  
	}

	m_alpha = Math::Clamp(m_alpha);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�
}

void AnimationRipple::Render(float x, float y, DWORD dwFlag)
{
	if (!m_bAction)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	m_pImage->SetScale(m_scale);
	m_pImage->SetARGB((BYTE)(m_alpha * 255), (BYTE)255, (BYTE)255, (BYTE)255);
	m_pImage->Render((int)x, (int)y, dwFlag);
}