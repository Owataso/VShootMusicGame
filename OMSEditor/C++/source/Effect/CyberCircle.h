#pragma once
#include "TDNLIB.h"

/********************************************/
//	�T�C�o�[�`�b�N�ȉ~�̃A�j���[�V����
/********************************************/
class CyberCircle
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	CyberCircle(const Vector2 &vPos, float fScale = 1, float fAlpha = 1) :
		m_iDelayFrame(-1)
	{
		m_ptagCircle[0] = std::make_unique<Circle>("DATA/UI/Other/cyber1.png", vPos, -.01f, fScale, fAlpha);
		m_ptagCircle[1] = std::make_unique<Circle>("DATA/UI/Other/cyber2.png", vPos, .008f, fScale, fAlpha);
	}

	//===============================================
	//	�X�V
	//===============================================
	void Update()
	{
		// �܂��A�N�V�������Ă��Ȃ��̂ŁA�X�V���Ȃ�
		if (m_iDelayFrame == -1) return;

		// �x�������t���[���X�V
		else if (m_iDelayFrame > 0)
		{
			m_iDelayFrame--;
			return;
		}

		// �~�̍X�V
		FOR(NUM_CIRCLE)m_ptagCircle[i]->Update();
	}


	//===============================================
	//	�`��
	//===============================================
	void Render()
	{
		// ��������̒x�����|���Ă���A�������̓A�N�V�������Ă��Ȃ��Ȃ�X���[
		if (m_iDelayFrame)return;

		// �~�̕`��
		FOR(NUM_CIRCLE)m_ptagCircle[i]->Render();
	}


	//===============================================
	//	�����g���K�[
	//===============================================
	void Action(int delay = 0) { m_iDelayFrame = delay; }

private:

	//===============================================
	//	�~�̒P�̂̃N���X
	//===============================================
	struct Circle
	{
	public:
		Circle(LPCSTR path, const Vector2 &vPos, float fAddAngle, float fScale, float fAlpha) :pImage(new tdn2DObj(path)), vPos(vPos), fAngle(0), fAddAngle(fAddAngle)
		{
			pImage->SetScale(fScale);
			pImage->SetAlpha(fAlpha);
		}
		void Update()
		{
			pImage->SetAngle((fAngle += fAddAngle));
		}
		void Render()
		{
			pImage->Render((int)vPos.x, (int)vPos.y);
		}

	private:
		std::unique_ptr<tdn2DObj> pImage;
		Vector2 vPos;
		float fAngle;
		const float fAddAngle;
	};

	//===============================================
	//	�����o�ϐ�
	//===============================================
	int m_iDelayFrame;									// -1�Ȃ�NO_ACTION
	static const int NUM_CIRCLE = 2;					// �d�˂�~�̐�
	std::unique_ptr<Circle> m_ptagCircle[NUM_CIRCLE];	// �~�̃N���X�̎���
};
