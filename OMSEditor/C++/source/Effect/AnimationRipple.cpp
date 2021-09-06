
#include "AnimationRipple.h"

AnimationRipple::AnimationRipple(tdn2DObj *pImage, int EndFrame, int alphaNearFrame, int alphaFarFrame, float StartScale, float streach, bool IsRoop) :m_pImage(pImage), m_bAction(false), m_bRoop(IsRoop), m_alpha(0), m_AlphaFar(alphaFarFrame), m_AlphaNear(alphaNearFrame), m_EndFrame(EndFrame), m_scale(1), m_StartScale(StartScale), m_streach(streach), m_frame(0){}

void AnimationRipple::Action()
{
	m_bAction = true;
	m_frame = 0;
	m_scale = m_StartScale;// スケールを初期位置へ
}

void AnimationRipple::Stop()
{
	m_bAction = false;
	m_frame = 0;
}

void AnimationRipple::Update()
{
	if (!m_bAction)return;//実行されてないなら出てけ！！

	// フレーム更新
	m_frame++;
	if (m_bRoop == true)// ループだったらalphaFarの前のとこでループし続ける処理
	{
		if (m_frame >= (m_AlphaFar - 1))
		{
			m_frame = (m_AlphaFar - 1);
			return;
		}
	}
	// エンドフレームまで来たら終わる
	if (m_frame >= m_EndFrame) m_bAction = false;

	// 拡大率更新
	m_scale += m_streach;


	// 透明度更新
	// α二アーの前か後ろで判定を変える
	if (m_frame >= m_AlphaNear)
	{
		// 100-100=0  100-50=50   0/50
		m_alpha = ((float)m_EndFrame - m_frame) / ((float)m_EndFrame - m_AlphaFar);
	}
	else
	{
		// 最初の
		m_alpha = (float)m_frame / m_AlphaNear;//   0/30=0   60/30=2   1-(0~1)  
	}

	m_alpha = Math::Clamp(m_alpha);//指定された値を 0 ～ 1 の範囲にクランプします
}

void AnimationRipple::Render(float x, float y, DWORD dwFlag)
{
	if (!m_bAction)return;//実行されてないなら出てけ！！

	m_pImage->SetScale(m_scale);
	m_pImage->SetARGB((BYTE)(m_alpha * 255), (BYTE)255, (BYTE)255, (BYTE)255);
	m_pImage->Render((int)x, (int)y, dwFlag);
}