#include "../Music/MusicInfo.h"
#include "Gauge.h"

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
GaugeManager::GaugeManager(GAUGE_OPTION eOption, int *ipGauge) :
	m_ipGauge(ipGauge), 
	m_eOption(eOption), 
	m_fGauge(22.0f)
{

}

//*****************************************************************************************************************************
//	�Q�[�W���Z�b�g
//*****************************************************************************************************************************
void GaugeManager::GaugeSet(int iGauge)
{
	*m_ipGauge = iGauge;
	m_fGauge = (float)iGauge;
}

//*****************************************************************************************************************************
//		�m�[�c���ɉ����āA�m�}�Q�ƈՂ̃Q�[�W�������v�Z
//*****************************************************************************************************************************
void GaugeManager::SetNumNorts(int iNumNorts)
{
	// �g�[�^���l�Z�o
	const float l_cfTotalValue(max(7.605f * iNumNorts / (0.01f * iNumNorts + 6.5f), 260));

	// �g�[�^���l���m�[�c���Ŋ����1��������̑����ʂ����߂���
	m_fNormalAdd = l_cfTotalValue / iNumNorts;
}


//*****************************************************************************************************************************
//		����ɉ����ăQ�[�W�𑝌�������
//*****************************************************************************************************************************
void GaugeManager::Compute(JUDGE_TYPE type)
{
	switch (type)
	{
	case JUDGE_TYPE::SUPER:
	case JUDGE_TYPE::GREAT:
	{
		const float l_cfAddGauge[(int)GAUGE_OPTION::MAX]{ m_fNormalAdd, m_fNormalAdd, .16f, .16f, .16f, .16f };
		m_fGauge = min(m_fGauge + l_cfAddGauge[(int)m_eOption], 100);
	}
		break;
	case JUDGE_TYPE::GOOD:
	{
		const float l_cfAddGauge[(int)GAUGE_OPTION::MAX]{ m_fNormalAdd * .5f, m_fNormalAdd * .5f, .0f, .0f, .0f, .04f };
		m_fGauge = min(m_fGauge + l_cfAddGauge[(int)m_eOption], 100);
	}
		break;
	case JUDGE_TYPE::BAD:
	{
		static const float l_cfSubGauge[(int)GAUGE_OPTION::MAX]{ -2, -1.6f, -5, -10, -100, -1.5f };
		m_fGauge = max(m_fGauge + l_cfSubGauge[(int)m_eOption], (m_eOption == GAUGE_OPTION::EASY || m_eOption == GAUGE_OPTION::NORMAL) ? 2 : 0);	// �I�v�V�����ɉ����ăQ�[�W�����炷
	}
		break;
	}

	// �����_�؂�̂Ă̕��̌v�Z+2�̔{���Ɋۂ߂�
	*m_ipGauge = ((int)m_fGauge / 2) * 2;
}