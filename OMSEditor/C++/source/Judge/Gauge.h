#pragma once

enum class JUDGE_TYPE;
enum class GAUGE_OPTION;

/********************************************/
//	�Q�[�W�̑������Ǘ�����N���X
/********************************************/
class GaugeManager
{
public:
	//===============================================
	//	�R���X�g���N�^�E�f�X�g���N�^
	//===============================================
	GaugeManager(GAUGE_OPTION eOption, int *ipGauge);
	~GaugeManager(){}


	//===============================================
	//	���肵���Ƃ��ɌĂяo��
	//===============================================
	void Compute(JUDGE_TYPE eJudge);


	//===============================================
	//	�A�N�Z�T�[
	//===============================================
	void GaugeSet(int iGauge);
	void SetNumNorts(int iNumNorts);
	bool isFailed(){ return (*m_ipGauge == 0); }	// �X�t���O
	GAUGE_OPTION GetGaugeOption(){ return m_eOption; }

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	float m_fGauge;			// �����I�Ɍv�Z����Q�[�W�ϐ�(�����_��)
	int *m_ipGauge;			// �O���Ƃ��ĕ\������p�̕ϐ��ւ̃|�C���^ �����؂�̂ĂƂ��ăQ�[���ɕ\������int�^�ւ̃A�h���X(�Q�Ƃŏ��������Ă���)
	GAUGE_OPTION m_eOption;	// �Q�[�W�I�v�V����
	float m_fNormalAdd;		// �m�}�Q�̂Ƃ��̃Q�[�W������(���m�[�c���Ōv�Z)


	//===============================================
	//	�f�t�H���g�R���X�g���N�^����
	//===============================================
	GaugeManager();
};