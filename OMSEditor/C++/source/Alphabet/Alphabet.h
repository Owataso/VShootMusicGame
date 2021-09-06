#pragma once

#include "TDNLIB.h"

//===============================================
//	�A���t�@�x�b�g�e�[�u��
//===============================================
static const char EP_AlphabetTable[]
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '!', '?', '#', '$', ' ',
	'\0'	// �I�[�R�[�h(���O���͂̍ۂ͂�����u���O���͏I���v�Ƃ���)
};


/********************************************/
//	���O�A���t�@�x�b�g�`��p(��Ƀv���C���[�̖��O)
/********************************************/
class AlphabetRenderer
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	AlphabetRenderer() :m_pAlphabet(new tdn2DObj("DATA/UI/Other/alphabet.png"))
	{
		// �}�b�v�R���e�i������
		for (int i = 0; i < _countof(EP_AlphabetTable); i++)
		{
			m_id[EP_AlphabetTable[i]] = i;
		}
	}

	//===============================================
	//	�`��(����c�̃A���t�@�x�b�g��`��)
	//===============================================
	void Render(int x, int y, char c)
	{
		int ID = m_id[c];
		int srcX, srcY;

		// �I���t���O(END)
		if (c == '\0')
		{
			srcX = 7, srcY = 7;	// ��ԉE��
		}
		// ��
		else if (c == ' ')
		{
			srcX = 6, srcY = 7;	// ��ԉE����1��
		}

		// ����ȊO�̕��ʂ̕���
		else
		{
			srcX = ID % 8, srcY = ID / 8;
		}

		m_pAlphabet->Render(x, y, 64, 64, srcX * 64, srcY * 64, 64, 64);
	}
	int GetAlphabetID(char c){ return m_id[c]; }

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	std::unique_ptr<tdn2DObj> m_pAlphabet;	// �A���t�@�x�b�g�̉摜
	std::map<char, int> m_id;				// 
};