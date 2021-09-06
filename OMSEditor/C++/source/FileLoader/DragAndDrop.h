
/// �O���C���N���[�h
//#include <Windows.h>

#pragma once

namespace DRAG_AND_DROP
{
	// window�Ƀh���b�O���h���b�v������
	// hwnd : window �� �n���h��
	void Initialize(HWND hwnd);

	/*******************************************************************
	  window �� �R�[���o�b�N�֐��̒��� case WM_DROPFILES: �̂Ƃ��ɌĂ�  
	*******************************************************************/

	// �h���b�v���ꂽ�t�@�C���̐�΃p�X���擾
	// out_buf  : <out> �t�@�C���p�X <�Ȃ�������NULL>
	// buf_size : <in>  out_buf�̃T�C�Y
	// param    : <in>  window���󂯎�������
	void Get_file_path(LPSTR out_buf, UINT buf_size, WPARAM param);

	// �h���b�v���ꂽ�����̃t�@�C���̐�΃p�X���擾
	// out_buf  : <out> �����̃t�@�C���p�X <�Q�����z��̃|�C���^�[>
	// num_buf  : <in>  out_buf�̐�     ���z��
	// buf_size : <in>  out_buf�̃T�C�Y ���z��
	// param    : <in>  window���󂯎�������
	void Get_some_file_path(LPSTR out_buf, UINT num_buf, UINT buf_size, WPARAM param);

	// �h���b�v���ꂽ�S�t�@�C���̐�΃p�X���擾
	// �߂�l   : �t�@�C���̐�
	// out_buf  : <out> �����̃t�@�C���p�X <�P�����z���new����>
	// buf_size : <in>  out_buf����؂�T�C�Y <�Q�����z��̑�Q�z�񒷂ɂ�����>
	// param    : <in>  window���󂯎�������
	UINT Get_all_file_path(LPSTR &out_buf, UINT buf_size, WPARAM param);
}
