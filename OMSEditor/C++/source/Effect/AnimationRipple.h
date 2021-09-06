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
	
	int	m_EndFrame;		// 描画終りのフレーム
	int	m_frame;		// 今のフレーム

	int	m_AlphaNear;		// 透明じゃなくなる　始めるフレーム
	int	m_AlphaFar;		// 透明じゃなくなる　終わるフレーム　
	float m_alpha;			// 透明度

	float m_scale;				// スケール
	float m_StartScale;			// 最初のスケール
	float m_streach;			// スケールをどう動かすか

	bool m_bAction;
	bool m_bRoop;				// ループするか
};
