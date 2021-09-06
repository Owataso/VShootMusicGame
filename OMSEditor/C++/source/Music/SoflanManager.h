#pragma once

//===============================================
//	�O���錾
//===============================================
struct SoflanData;
class BaseMusicInfo;

/********************************************/
//	�\�t�����Ǘ��N���X
/********************************************/
class SoflanManager
{
public:
	//===============================================
	//	�������E���
	//===============================================
	SoflanManager();
	~SoflanManager();

	//===============================================
	//	�X�V
	//===============================================
	bool Update(DWORD dwCurrentPosition);

	//===============================================
	//	�A�N�Z�T
	//===============================================
	float GetBPM();
	void Set(BaseMusicInfo *info);
	SoflanData *GetSoflans() { return m_Soflans; }

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	UINT m_iNumSoflans;
	SoflanData *m_Soflans;
	UINT m_iSoflanStep;
};