#pragma once
#include "tdnlib.h"
#include "../system/System.h"

class AnimationRipple
{
public:
	AnimationRipple(tdn2DObj *image, int endFlame, int alphaNearFlame, int alphaFarFlame, float StartScale, float ScaleMove, bool isRoop = false);
	//~AnimationRipple();

	void Action();
	void Stop();

	void Update();
	void Render(float x, float y, DWORD dwFlag);

	bool isAction(){ return m_bAction; }
private:
	tdn2DObj *m_pImage;
	
	int	m_EndFrame;		// �`��I��̃t���[��
	int	m_frame;		// ���̃t���[��

	int	m_AlphaNear;		// ��������Ȃ��Ȃ�@�n�߂�t���[��
	int	m_AlphaFar;		// ��������Ȃ��Ȃ�@�I���t���[���@
	float m_alpha;			// �����x

	float m_scale;				// �X�P�[��
	float m_StartScale;			// �ŏ��̃X�P�[��
	float m_streach;			// �X�P�[�����ǂ���������

	bool m_bAction;
	bool m_bRoop;				// ���[�v���邩
};
