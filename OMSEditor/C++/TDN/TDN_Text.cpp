#include	"TDNLIB.h"

// �S�p���p�Ƃ��Ŏg��
#include<mbstring.h>

LPD3DXFONT tdnText::font;

void tdnText::Init(){
	HFONT hf;
	D3DXFONT_DESC fd;

	hf = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hf, sizeof(LOGFONT), (LPSTR)&fd);
	fd.Height = 32;
	fd.Width = 0;
	fd.Italic = 0;
	fd.CharSet = SHIFTJIS_CHARSET;
	strcpy(fd.FaceName, "���C���I");

	D3DXCreateFontIndirect(tdnSystem::GetDevice(), &fd, &font);
}

void tdnText::CleanUpModule(){
	font->Release();
}

void tdnText::Draw(int x, int y, DWORD color, const char * _Format, ...){
	const int strsize = 1024;
	char str[strsize];
	ZeroMemory(str, sizeof(char)* strsize);
	va_list list;
	va_start(list, _Format);
	vsprintf(str, _Format, list);
	va_end(list);

	RECT rec;
	rec.left = x;
	rec.top = y;
	rec.right = tdnSystem::GetScreenSize().right;
	rec.bottom = tdnSystem::GetScreenSize().bottom;

	font->DrawText(NULL, str, -1, &rec, DT_LEFT | DT_WORDBREAK, color);
	return;
}

int tdnText::StrCount(char *str)
{
	int ret(0);
	for (int i1 = 0; str[i1] != '\0';)
	{
		// 2�o�C�g������������
		if (_ismbblead(str[i1]))
		{
			// �����̎����I�[���ǂ���(2�o�C�g�Ȃ̂ŁA1����΂�)
			if (str[i1 + 1] == '\0')
			{
				ret++;
				break;
			}
			ret++;
			i1 += 2;
		}

		// 1�o�C�g������������
		else
		{
			ret++;
			i1++;
		}
	}
	return ret;
}


void tdnText2::Init(int size, char *font_){
	HFONT hf;
	D3DXFONT_DESC fd;

	hf = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hf, sizeof(LOGFONT), (LPSTR)&fd);
	fd.Height = m_size = size;
	fd.Width = 0;
	fd.Italic = 0;
	fd.CharSet = SHIFTJIS_CHARSET;
	strcpy(fd.FaceName, font_);

	D3DXCreateFontIndirect(tdnSystem::GetDevice(), &fd, &m_font);
}

void tdnText2::CleanUpModule(){
	m_font->Release();
}

void tdnText2::Draw(int x, int y, DWORD color, const char * _Format, ...){
	const int strsize = 1024;
	char str[strsize];
	ZeroMemory(str, sizeof(char)* strsize);
	va_list list;
	va_start(list, _Format);
	vsprintf(str, _Format, list);
	va_end(list);

	RECT rec;
	rec.left = x;
	rec.top = y;
	rec.right = tdnSystem::GetScreenSize().right;
	rec.bottom = tdnSystem::GetScreenSize().bottom;

	m_font->DrawText(NULL, str, -1, &rec, DT_LEFT | DT_WORDBREAK, color);
	return;
}

void tdnText2::DrawCenter(int cx, int y, DWORD color, const char * _Format, ...){
	const int strsize = 1024;
	char str[strsize];
	ZeroMemory(str, sizeof(char)* strsize);
	va_list list;
	va_start(list, _Format);
	vsprintf(str, _Format, list);
	va_end(list);

	int len = tdnText::StrCount(str);	// ������

	const int shift = (int)(len * .5f * m_size);	// �������獶�ɂ��炷�l

	// ���炷
	cx -= shift;

	RECT rec;
	rec.left = cx;
	rec.top = y;
	rec.right = tdnSystem::GetScreenSize().right;
	rec.bottom = tdnSystem::GetScreenSize().bottom;

	m_font->DrawText(NULL, str, -1, &rec, DT_LEFT | DT_WORDBREAK, color);
	return;
}
