#include "TDNLIB.h"
#include<mbstring.h>

/************************/
//		tdnFont
/************************/

/****** �O���錾 *******/

//	�Ǘ��p�p�����[�^									
bool tdnFont::m_IsInitialized = false;
tdnFont::CacheDesc		tdnFont::m_CacheDesc = { 0 }; //�L���b�V���̐F�X

// �G����
LPCSTR tdnFont::m_lpcsPictGraphStrs[(int)PICTOGRAPH::MAX]
{
	"��"		// ���S�҃}�[�N
};

tdn2DObj* tdnFont::m_pPictograph = nullptr;

/***********************/


/***********************************************/
//		�������E���
/***********************************************/
void	tdnFont::Initialize()
{
	//	�ʏ핶���L���b�V���̏�����
	{
		//	���łɊm�ۂ���Ă���̈悪����ꍇ�͔j��
		if (m_CacheDesc.textureCacheList){ delete[] m_CacheDesc.textureCacheList; m_CacheDesc.textureCacheList = nullptr; }		//	�����e�N�X�`���̃��X�g
		if (m_CacheDesc.referenceDataList){ delete[] m_CacheDesc.referenceDataList; m_CacheDesc.referenceDataList = nullptr; }	//	�L���b�V���̏ƍ��p�f�[�^���X�g
															//	�L���b�V����ۑ�����̈���m��
		m_CacheDesc.textureCacheList = new tdn2DObj[CacheQty];  // �����e�N�X�`���̃��X�g
		m_CacheDesc.referenceDataList = new	RefData[CacheQty];	// �L���b�V���̏ƍ��p�f�[�^�̃��X�g
																//	�L���b�V����������
		for (UINT i = 0; i < CacheQty; i++)
		{
			m_CacheDesc.referenceDataList[i].isEnable = false;// �L���t���O��false
		}

		m_CacheDesc.nextUseCacheNum = 0;//	���Ɏg�p����L���b�V���̔ԍ�
	}

	// �G�����p
	m_pPictograph = new tdn2DObj("Data/System/Pictograph.png");

	//	�������t���O�𗧂Ă�
	m_IsInitialized = true;
}

void	tdnFont::Release()
{
	//	�ʏ�L���b�V��
	{
		if (m_CacheDesc.textureCacheList){ delete[] m_CacheDesc.textureCacheList; m_CacheDesc.textureCacheList = nullptr; }		//	�����e�N�X�`���̃��X�g
		if (m_CacheDesc.referenceDataList){ delete[] m_CacheDesc.referenceDataList; m_CacheDesc.referenceDataList = nullptr; }	//	�L���b�V���̏ƍ��p�f�[�^���X�g
		m_CacheDesc.nextUseCacheNum = 0;
	}

	// �G����
	SAFE_DELETE(m_pPictograph);
	
	//	�������t���O�����Z�b�g
	m_IsInitialized = false;
}

/***********************************************/
//		�����`��
/***********************************************/

//������`��
void	tdnFont::RenderString(LPCSTR string, int fontSize, int drawX, int  drawY, COLOR dwColor, LPCSTR fontName, u32 dwFlags)
{
	//	�ϐ��̗p��
	int	x = drawX;
	int	y = drawY;
	UINT	myByte = 0;
	//	�I�[�����܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);

		// ���s�R�[�h������Ή��s���Ď��̕�����
		if (string[i] == '\n')
		{
			x = drawX;
			y += (int)fontSize + 8;// �����̕����T�C�Y��+���ŉ����Ă���
			continue;
		}

		//	������`��
		int	drawCoordX = x;
		int	drawCoordY = y;
		Vector2	drawedSize = RenderCharacter(&string[i], fontName, fontSize, drawCoordX, drawCoordY, dwColor, dwFlags);

		//	�`����W�����炷
		x += (int)drawedSize.x;
	}
}

void tdnFont::RenderString(int fontSize, int drawX, int  drawY, COLOR dwColor, const char * _Format, ...)
{
	char str[1024]{};
	va_list list;
	va_start(list, _Format);
	vsprintf(str, _Format, list);
	va_end(list);

	RenderString(str, fontSize, drawX, drawY, dwColor, "���C���I");
}


// �ꕶ���`��
Vector2	tdnFont::RenderCharacter(LPCSTR character, LPCSTR fontName, int fontSize, int drawX, int  drawY, COLOR dwColor, u32 dwFlags)
{
	//	�����e�N�X�`���Ǘ��p�����[�^������������Ă��邩�`�F�b�N
	if (!m_IsInitialized)	Initialize();

	//	�ϐ��̗p��
	tdn2DObj*	pImage;
	UINT	createSize;
	UINT	byteSize = _mbclen((BYTE*)character);

	//	���ƍ������r���đ傫���ق�����Ƀe�N�X�`�����쐬
	int		createSizeW = (int)((float)fontSize);
	int		createSizeH = (int)((float)fontSize);
	if (createSizeW > createSizeH)	createSize = createSizeW;
	else							createSize = createSizeH;

	//	�g�p����e�N�X�`���̃A�h���X[�z��ԍ�]���Z�b�g
	pImage = &m_CacheDesc.textureCacheList[SearchCache(character, createSize, fontName)];

	//	�󔒃`�F�b�N
	bool	bDraw = true;
	
	switch (byteSize) 
	{
	case	1:
		if (character[0] == ' ') { bDraw = false;	}
		break;
	case	2:// �}���`�o�C�g
	{
		char	buffer[3] = { character[0], character[1], '\0' };
		if (strcmp(buffer, "�@") == 0) 
		{
			bDraw = false;
		}

		// ���G��������
		bDraw = SearchPictograph(buffer, drawX, drawY, pImage->GetWidth(), dwColor, dwFlags);
	}
	break;
	}

	//	�`��
	if (bDraw)
	{
		pImage->SetARGB(dwColor);
		pImage->Render(drawX, drawY, pImage->GetWidth(), pImage->GetHeight(),
			0, 0, pImage->GetWidth(), pImage->GetHeight(), dwFlags);
	}

	//	�`�悵�������̃T�C�Y��Ԃ�
	return	Vector2((float)pImage->GetWidth(), (float)pImage->GetHeight());
}


//  �Ȉ�
void tdnFont::RenderFont2D(LPCSTR _String, int _FontSize, int _DrawX, int _DrawY, COLOR dwColor)
{
	LPSTR	c_FontName = "HGP�n�p�p�߯�ߑ�";

	RenderString(_String, _FontSize,
		_DrawX, _DrawY, dwColor, c_FontName, RS::COPY);
}

// ��������
void tdnFont::RenderStringCentering(LPCSTR string, int fontSize, int drawX, int  drawY, COLOR dwColor, LPCSTR fontName, u32 dwFlags)
{
	//	�ϐ��̗p��
	int	x = drawX;
	int	y = drawY;
	UINT	myByte = 0;
	UINT	addByte = 0;
	UINT	prevByte = 0;
	std::string str;
	str = string;

	//	�I�[�������͉��s�܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; )
	{
		// �X�V
		i += myByte;

		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += _mbclen((BYTE*)&string[i]);


		// ���s�܂ł������x�`�悵�ă��Z�b�g
		if (string[i] == '\n'|| string[i] == '\0')
		{	
			
			// �������ւ̏���
			x = drawX - (int)(((addByte - prevByte)*(fontSize * 0.5f)) * 0.5f);

			// �����ŕ`��	
			std::string subString = str.substr(prevByte, (addByte - prevByte));
			prevByte = addByte;// �O���Byte����ύX

			// ������`��
			RenderString(subString.c_str(), fontSize, x, y, dwColor, fontName, dwFlags);

			// ���s�@�����̕����T�C�Y��+���ŉ����Ă���
			y += (int)fontSize + 8;

		}
		
	}
	

	

}


/****************************************/
//	 2DObj���當�������T�|�[�g�֐�
/****************************************/
UINT	tdnFont::SearchCache(LPCSTR chara, UINT size, LPCSTR fontName)
{
	//	�g�p�\�ȃL���b�V����T��
	UINT	sizeAllow = 0;							//	���e�T�C�Y��
	UINT	byteSize = _mbclen((BYTE*)chara);	//	�o�C�g��
												//	�L���b�V���������[�v
	for (UINT cNum = 0; cNum < CacheQty; cNum++) {
		RefData&	refData = m_CacheDesc.referenceDataList[cNum];
		if (!refData.isEnable)									continue;
		//	�o�C�g������
		if (refData.byte != byteSize)							continue;
		//	��������i�o�C�g���ɉ����ĕ���j
		if (byteSize != 2) {
			if (refData.chara[0] != chara[0])					continue;
		}
		else {
			if (refData.chara[0] != chara[0])					continue;
			if (refData.chara[1] != chara[1])					continue;
		}
		//	�T�C�Y����
		if (refData.size - sizeAllow > size)					continue;
		if (refData.size + sizeAllow < size)					continue;
		//	�t�H���g����
		if (strlen(refData.fontName) == 0) {
			if (strlen(fontName) != 0)						continue;
		}
		else {
			if (strlen(fontName) == 0)						continue;
			if (strcmp(refData.fontName, fontName) != 0)	continue;
		}
		//	������N���A
		return	cNum;
	}

	//	�V���������e�N�X�`�����쐬
	UINT	useNum = m_CacheDesc.nextUseCacheNum;
	m_CacheDesc.textureCacheList[useNum].LoadFontTexture(chara, size, fontName);
	//	�ƍ��p�p�����[�^���X�V
	RefData&	refData = m_CacheDesc.referenceDataList[useNum];
	refData.chara[0] = chara[0];
	refData.chara[1] = chara[1];
	refData.size = size;
	refData.byte = byteSize;
	refData.isEnable = true;
	refData.fontName = fontName;

	//	���ɏ���������L���b�V���̔ԍ����X�V
	if (++m_CacheDesc.nextUseCacheNum == CacheQty)	m_CacheDesc.nextUseCacheNum = 0;

	//	�V�����e�N�X�`�����쐬�����ԍ���Ԃ�
	return	useNum;
}

bool tdnFont::SearchPictograph(char buffer[3], int x, int y, UINT size, DWORD color, DWORD RenderFlag)
{
	// ���G��������


	// �����̐F�@(TODO) ���̓A���t�@�����ς��Ă��܂�
	DWORD col = color & 0xff000000;
	col = col | 0x00ffffff;

	m_pPictograph->SetARGB(col);

	FOR((int)PICTOGRAPH::MAX)
	{
		if (strcmp(buffer, m_lpcsPictGraphStrs[i]) == 0)
		{
			m_pPictograph->Render(x, y, size, size, i * 32, 0, 32, 32, RenderFlag);
			return false;
		}
	}


	// �G������������Ȃ������̂ŕ`�悵�Ă悵
	return true;
}
