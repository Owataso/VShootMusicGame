#pragma once

//*****************************************************************************
//
//		�S�Ă̋ȏ����Ǘ�����f�[�^�x�[�X<Singleton>
//
//*****************************************************************************

class FolderInfo;
class EditFolderInfo;

/********************************************/
//	�S�Ă̋ȏ����Ǘ�����f�[�^�x�[�X<Singleton>
/********************************************/
class MusicDataBase
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static MusicDataBase *GetInstance(){ static MusicDataBase instance; return &instance; }

	//===============================================
	//	�R���X�g���N�^�E�f�X�g���N�^
	//===============================================
	MusicDataBase();
	~MusicDataBase();

	//===============================================
	//	�Ȃ̏��̎擾
	//===============================================
	FolderInfo *GetEPContentsData(int no);
	EditFolderInfo *GetEditContentsData(int no);


	//===============================================
	//	�f�B���N�g���̎擾
	//===============================================
	DirectoryInfo *GetEPDirectory(){ return m_EPDirectory; }
	DirectoryInfo *GetEditDirectory(){ return m_EditDirectry; }


	//===============================================
	//	�Ȑ��̎擾
	//===============================================
	int GetNumEPContents();
	int GetNumEditContents();

private:
	//===============================================
	//	ElectronicPianism���^�ȃf�[�^
	//===============================================
	FolderInfo **m_EPContentsData;
	DirectoryInfo *m_EPDirectory;
	void LoadEPContents();

	//===============================================
	//	Edit���[�h�ō��ꂽ�ȃf�[�^
	//===============================================
	EditFolderInfo **m_EditContentsData;
	DirectoryInfo *m_EditDirectry;
	void LoadEditContents();

};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define MUSIC_DATA_BASE (MusicDataBase::GetInstance())

/*
�\�[�g����֐�
int cmp(const void *x, const void *y){
return(strcmp(*(char**)x,*(char**)y));
}
void main(void){
char *data[]={ "AKIRA","AKIKO","AIKOSANN" };
char *wk;
int i;

printf("before:\n");
for(i=0;i<3;i++)
printf("%s\n",data[i]);
qsort(data,sizeof(data)/sizeof(char*),sizeof(char*),cmp);
printf("after:\n");
for(i=0;i<3;i++)
printf("%s\n",data[i]);
}
*/