#include "KeyboardGuide.h"

void KeyboardGuide::Render()
{
	// �t�F�[�h�C�����銴���œ����Ă��銴��
	const float l_cfRate(pow((m_ciDelay) ? (float)m_iDelayFrame / m_ciDelay : 1.0f, 4));
	const float l_cfAlpha(Math::Blend(l_cfRate, 0, 1.0f));

#ifdef MIDI_INPUT_ON

	// ���n�̕`��
	m_pImage->SetAlpha(l_cfAlpha);
	m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 0, 256, 128);

	// �I��_�Œ�
	if (m_bSwitch)
	{
		// �I��
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, Math::Blend((sinf(m_fAlphaSinCurve) + 1) * 0.5f, m_cfMinAlpha, m_cfMaxAlpha)));	// -1�`1��0�`1
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 128, 256, 128);
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, 0.5f));

		// ����
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 256, 256, 128);
	}

	// ����_�Œ�
	else
	{
		// ����
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, Math::Blend((sinf(m_fAlphaSinCurve) + 1) * 0.5f, m_cfMinAlpha, m_cfMaxAlpha)));	// -1�`1��0�`1
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 256, 256, 128);
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, 0.5f));

		// �I��
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 128, 256, 128);
	}
#else

	static int x(0), y(0); if (KeyBoardTRG(KB_A)) { x -= 10; } if (KeyBoardTRG(KB_D)) { x += 10; } if (KeyBoardTRG(KB_W)) { y -= 10; }if (KeyBoardTRG(KB_S)) { y += 10; }

	// ���n�̕`��
	m_pImage->SetAlpha(l_cfAlpha);
	m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 220, 128, 0, 0, 660, 256);

	tdnText::Draw((int)m_vPos.x + 28, (int)m_vPos.y + 24, ((BYTE)(255 * l_cfRate) << 24) | 0x00ffffff, "���������@: �I��\n�d�m�s�d�q: ����\n�r�o�`�b�d: �߂�");
#endif

}
