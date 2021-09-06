#pragma once

/********************************************/
//	�h�D���h�D������Ă邠��
/********************************************/
class BeatBar
{
public:
	//------------------------------------------------------
	//	�R���X�g���N�^
	//------------------------------------------------------
	BeatBar() :
		m_pImage(new tdn2DObj("DATA/UI/Playing/judge_line.png")), 
		m_tagBeat(Beat()){}

	//------------------------------------------------------
	//	�X�V�E�`��
	//------------------------------------------------------
	void Update(int iPlayTime){ m_tagBeat.SetPlayTime(iPlayTime); }
	void Render(int x, int y)
	{
		const float percent(m_tagBeat.GetPercent());
		m_pImage->SetAlpha(Math::Blend(percent, .25f, 1));
		m_pImage->Render(x, y, 1120, 48, 0, 32, 1120, 48);
	}

	//------------------------------------------------------
	//	BPM�ύX���ɌĂяo��
	//------------------------------------------------------
	void SetBPM(float fBPM){ m_tagBeat.SetBPM(fBPM); }

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	void Reset(){ m_tagBeat.Reset(); }

private:

	/********************************************/
	//	���v���p�N���X
	/********************************************/
	class Beat
	{
	public:
		//------------------------------------------------------
		//	������
		//------------------------------------------------------
		Beat(){ Reset(); }
		void Reset()
		{
			// ���ԏ�����
			m_iCurrentBeatMSecond = m_iCurrentPlayTime = 0;

			// ����BPM60
			SetBPM(60);
		}

		//------------------------------------------------------
		//	���t���[���Ăяo���A���݂̍Đ�����(�~���b)��ݒ肷��
		//------------------------------------------------------
		void SetPlayTime(int iPlayTime)
		{
			const int iDelta(iPlayTime - m_iCurrentPlayTime);

			// ������������I�[��������Ȃ��̂Ŏ~�߂�
			if (iDelta < 0)
			{
				return;
			}
			m_iCurrentPlayTime = iPlayTime;
			m_iCurrentBeatMSecond += iDelta;
		}

		//------------------------------------------------------
		//	BPM�ύX���ɌĂяo��
		//------------------------------------------------------
		void SetBPM(float fBPM)
		{
			// ���݂̔��v���~���b��������
			m_iCurrentBeatMSecond = 0;

			// BPM�̒l����A���~���b��1�����v�Z����
			m_iBeatMSecond = (int)(1000 * (60 / fBPM));
		}

		//------------------------------------------------------
		//	���̊���(0�`1)�擾
		//------------------------------------------------------
		float GetPercent()
		{
			while (m_iCurrentBeatMSecond > m_iBeatMSecond) m_iCurrentBeatMSecond -= m_iBeatMSecond;
			return 1 - (float)m_iCurrentBeatMSecond / m_iBeatMSecond; 
		}

	private:
		int m_iCurrentPlayTime;		// ���݂̍Đ��J�[�\��(�f���^�^�C���̌v���Ŏg��)
		int m_iCurrentBeatMSecond;	// ���݂�()
		int m_iBeatMSecond;		// ���~���b���ɔ���ł�
	}m_tagBeat;


	//------------------------------------------------------
	//	�����o�ϐ�
	//------------------------------------------------------
	std::unique_ptr<tdn2DObj> m_pImage;	// �o�[�̉摜
};
