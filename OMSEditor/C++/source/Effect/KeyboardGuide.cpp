#include "KeyboardGuide.h"

void KeyboardGuide::Render()
{
	// フェードインする感じで入ってくる感じ
	const float l_cfRate(pow((m_ciDelay) ? (float)m_iDelayFrame / m_ciDelay : 1.0f, 4));
	const float l_cfAlpha(Math::Blend(l_cfRate, 0, 1.0f));

#ifdef MIDI_INPUT_ON

	// 下地の描画
	m_pImage->SetAlpha(l_cfAlpha);
	m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 0, 256, 128);

	// 選択点滅中
	if (m_bSwitch)
	{
		// 選択
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, Math::Blend((sinf(m_fAlphaSinCurve) + 1) * 0.5f, m_cfMinAlpha, m_cfMaxAlpha)));	// -1〜1→0〜1
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 128, 256, 128);
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, 0.5f));

		// 決定
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 256, 256, 128);
	}

	// 決定点滅中
	else
	{
		// 決定
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, Math::Blend((sinf(m_fAlphaSinCurve) + 1) * 0.5f, m_cfMinAlpha, m_cfMaxAlpha)));	// -1〜1→0〜1
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 256, 256, 128);
		m_pImage->SetAlpha(Math::Blend(l_cfRate, 0, 0.5f));

		// 選択
		m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 256, 128, 0, 128, 256, 128);
	}
#else

	static int x(0), y(0); if (KeyBoardTRG(KB_A)) { x -= 10; } if (KeyBoardTRG(KB_D)) { x += 10; } if (KeyBoardTRG(KB_W)) { y -= 10; }if (KeyBoardTRG(KB_S)) { y += 10; }

	// 下地の描画
	m_pImage->SetAlpha(l_cfAlpha);
	m_pImage->Render((int)m_vPos.x, (int)m_vPos.y, 220, 128, 0, 0, 660, 256);

	tdnText::Draw((int)m_vPos.x + 28, (int)m_vPos.y + 24, ((BYTE)(255 * l_cfRate) << 24) | 0x00ffffff, "↑↓←→　: 選択\nＥＮＴＥＲ: 決定\nＳＰＡＣＥ: 戻る");
#endif

}
