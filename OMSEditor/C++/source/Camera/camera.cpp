#include "TDNLIB.h"
#include "camera.h"


//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
Camera::Camera() :
m_angle(Vector3(0, 0, 0)), m_pos(Vector3(0, 100, 0)), m_target(Vector3(0, 100, 1)), m_dist(47)
{
	//	ビュー設定
	tdnView::Init();

	m_NextTarget = m_target;
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
Camera::~Camera(){}

//*****************************************************************************************************************************
//		更新
//*****************************************************************************************************************************
//------------------------------------------------------
//		更新
//------------------------------------------------------
void Camera::Update()
{
	const float fAngleSpeed = 0.1f;
	if (GetKeyState(KB_NUMPAD6) & 0x80)
	{
		m_angle.y += fAngleSpeed;
		TargetAngleUpdate();
	}
	if (GetKeyState(KB_NUMPAD4) & 0x80)
	{
		m_angle.y -= fAngleSpeed;
		TargetAngleUpdate();
	}
	if (GetKeyState(KB_NUMPAD8) & 0x80)
	{
		m_angle.x += fAngleSpeed/2;
		m_angle.x = Math::Clamp(m_angle.x, -1.5f, 1.5f);	// 上向き防止
		TargetAngleUpdate();
	}
	if (GetKeyState(KB_NUMPAD2) & 0x80)
	{
		m_angle.x -= fAngleSpeed/2;
		m_angle.x = Math::Clamp(m_angle.x, -1.5f, 1.5f);	// 上向き防止
		TargetAngleUpdate();
	}
	//else if (GetKeyState(MOUSE_CENTAR) & 0x80)
	//{
	//	// 移動量作成
	//	Vector3 move =
	//		Vector3(matView._11, matView._21, matView._31) * -tdnMouse::GetMove().x*.1f +
	//		Vector3(matView._12, matView._22, matView._32) * tdnMouse::GetMove().y*.1f;
	//	m_target += move;
	//}



	tdnView::Set(m_pos, (m_target = m_target * 0.8f + m_NextTarget * 0.2f));
}

void Camera::TargetAngleUpdate()
{
	// 角度の値によるベクトルを作成
	float ay_x = sinf(m_angle.y);
	float ay_z = cosf(m_angle.y);
	float ax_y = sinf(m_angle.x);
	float ax_z = cosf(m_angle.x);

	Vector3 vec(
		ay_x * ax_z,
		ax_y,
		ay_z * ax_z);

	// ベクトルの長さ決定
	//vec *= m_dist;

	m_NextTarget = m_pos + vec;
}

//*****************************************************************************************************************************
//	描画準備
//*****************************************************************************************************************************
void Camera::Activate()
{
	tdnView::Activate();
	//tdnView::Clear(0x000000);
	//tdnView::Clear(0x00451e);	// 天鵞絨
	tdnView::Clear(0x2f5d50);	// 天鵞絨2
}