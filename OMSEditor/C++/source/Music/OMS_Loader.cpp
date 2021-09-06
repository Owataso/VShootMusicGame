#include "TDNLIB.h"
#include "OMS_Loader.h"
#include "../Music/MusicInfo.h"
#include "PlayCursor.h"

//*****************************************************************************************************************************
//	OMS�t�@�C���ǂݍ���
//*****************************************************************************************************************************


//OMS_RESULT OMSLoader::LoadOMS(MusicInfo *pOut, LPCSTR lpcFileName)
//{
//	// �ǂݏo������
//	std::ifstream ifs(lpcFileName, std::ios::binary);
//	//MyAssert(ifs,"OMS�t�@�C���I�[�v���Ɏ��s");
//	if (!ifs) return OMS_RESULT::FAILED_OPEN;
//
//	/* �w�b�_�`�����N�N�� */
//	char l_cChunkID[5];
//	ifs.read(l_cChunkID, 4);										// �w�b�_�`�����N�ł��邱�Ƃ�����"OMhd"�Ƃ���4�����̃A�X�L�[�R�[�h
//	if (
//		l_cChunkID[0] != 'O' ||
//		l_cChunkID[1] != 'M' ||
//		l_cChunkID[2] != 'h' ||
//		l_cChunkID[3] != 'd'
//		)
//		return OMS_RESULT::FAILED_READ_OMS;
//
//	// ���f�[�^�̃o�[�W����
//	BYTE l_byOMSType;
//	ifs.read((char*)&l_byOMSType, sizeof(BYTE));
//
//	//switch ((OMS_TYPE)l_byOMSType)
//	//{
//	//case OMS_TYPE::BASE:
//	//	ifs.read((char*)&out->tagOMSInfo, sizeof(BaseOMSInfo));
//	//	break;
//	//default:
//	//	break;
//	//}
//
//	/* �f�[�^�`�����N�N�� */
//	//ifs.read(work, 4);										// �f�[�^�`�����N�ł��邱�Ƃ�����"OMdt"�Ƃ���4�����̃A�X�L�[�R�[�h
//	//assert(
//	//	work[0] == 'O'&&
//	//	work[1] == 'M'&&
//	//	work[2] == 'd'&&
//	//	work[3] == 't'
//	//	);
//
//	// �܂��m�[�g
//	//out->tagNotes = new NoteInfo[out->tagOMSInfo.wNumNotes];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumNotes; i++)
//	//{
//	//	ifs.read((char*)&out->tagNotes, sizeof(NoteInfo));
//	//}
//
//	//// BPM�ύX���
//	//out->tagSoflans = new SoflanInfo[out->tagOMSInfo.wNumSoflans];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumSoflans; i++)
//	//{
//	//	ifs.read((char*)&out->tagSoflans, sizeof(SoflanInfo));
//	//}
//
//	//// ���q���
//	//out->tagBeats = new BeatInfo[out->tagOMSInfo.wNumBeats];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumBeats; i++)
//	//{
//	//	ifs.read((char*)&out->tagBeats, sizeof(BeatInfo));
//	//}
//
//	return OMS_RESULT::OK;
//}


OMS_RESULT EditOMSLoader::LoadBaseOMS(MusicInfoEP *pOut, std::ifstream *ifs)
{

	// OMS�t�H�[�}�b�g�ǂݍ���
	ifs->read((char*)&pOut->OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ifs->read((char*)&pOut->OMSInfo.wNumNotes, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumSoflans, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumBeats, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.sDivision, sizeof(short));
	ifs->read((char*)&pOut->OMSInfo.sShift, sizeof(short));

	char l_cChunkID[5];

	/* �f�[�^�`�����N�N�� */
	ifs->read(l_cChunkID, 4);										// �f�[�^�`�����N�ł��邱�Ƃ�����"OMdt"�Ƃ���4�����̃A�X�L�[�R�[�h
	if(
		l_cChunkID[0] != 'O'||
		l_cChunkID[1] != 'M'||
		l_cChunkID[2] != 'd'||
		l_cChunkID[3] != 't'
	)return OMS_RESULT::FAILED_READ_OMS;

	// �܂��m�[�g
	pOut->NoteDatas = new NoteDataEP[pOut->OMSInfo.wNumNotes];
	for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
	{
		ifs->read((char*)&pOut->NoteDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->NoteDatas[i].wSoflanNo, sizeof(WORD));
		ifs->read((char*)&pOut->NoteDatas[i].byNoteType, sizeof(BYTE));
		ifs->read((char*)&pOut->NoteDatas[i].byLaneNo, sizeof(BYTE));
	}

	// BPM�ύX���
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		ifs->read((char*)&pOut->SoflanDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->SoflanDatas[i].fBPM, sizeof(float));
		ifs->read((char*)&pOut->SoflanDatas[i].fTick, sizeof(float));
	}

	// ���q���
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		ifs->read((char*)&pOut->BeatDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->BeatDatas[i].byNumerator, sizeof(BYTE));
		ifs->read((char*)&pOut->BeatDatas[i].byDenominator, sizeof(BYTE));
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadVRShootOMS(MusicInfoVRShoot * pOut, std::ifstream * ifs)
{
	// OMS�t�H�[�}�b�g�ǂݍ���
	ifs->read((char*)&pOut->OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ifs->read((char*)&pOut->OMSInfo.wNumNotes, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumSoflans, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumBeats, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.sDivision, sizeof(short));
	ifs->read((char*)&pOut->OMSInfo.sShift, sizeof(short));

	char l_cChunkID[5];

	/* �f�[�^�`�����N�N�� */
	ifs->read(l_cChunkID, 4);										// �f�[�^�`�����N�ł��邱�Ƃ�����"OMdt"�Ƃ���4�����̃A�X�L�[�R�[�h
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// �܂��m�[�g
	pOut->NoteDatas = new NoteDataVRShoot[pOut->OMSInfo.wNumNotes];
	for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
	{
		ifs->read((char*)&pOut->NoteDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->NoteDatas[i].wSoflanNo, sizeof(WORD));
		ifs->read((char*)&pOut->NoteDatas[i].byNoteType, sizeof(BYTE));
		ifs->read((char*)&pOut->NoteDatas[i].fDegree, sizeof(float));
		ifs->read((char*)&pOut->NoteDatas[i].fRateY, sizeof(float));
		ifs->read((char*)&pOut->NoteDatas[i].wCNEndIndex, sizeof(WORD));
	}

	// BPM�ύX���
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		ifs->read((char*)&pOut->SoflanDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->SoflanDatas[i].fBPM, sizeof(float));
		ifs->read((char*)&pOut->SoflanDatas[i].fTick, sizeof(float));
	}

	// ���q���
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		ifs->read((char*)&pOut->BeatDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->BeatDatas[i].byNumerator, sizeof(BYTE));
		ifs->read((char*)&pOut->BeatDatas[i].byDenominator, sizeof(BYTE));
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadBaseOMT(MusicInfoEP * pOut, std::ifstream * ifs)
{
	// int�ȊO�̐����^(BYTE)�Ƃ��̓ǂݍ��݂Ɏg��(���ꂪ�Ȃ��ƒl�����������Ȃ�)
	int l_iValue;

	// OMS�t�H�[�}�b�g�ǂݍ���
	(*ifs) >> l_iValue;
	pOut->OMSInfo.byPlayMusicNo = (BYTE)l_iValue;

	// �m�[�c�͉ϒ��ɂ���
	//(*ifs) >> pOut->OMSInfo.wNumNotes;

	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumSoflans = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumBeats = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sDivision = (short)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sShift = (short)l_iValue;

	char l_cChunkID[16];

	/* �f�[�^�`�����N�N�� */

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'N' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'T' ||
		l_cChunkID[3] != 'E' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// �܂��m�[�g(���������₷���悤�ɉϒ��ɂ���)
	{
		std::vector<NoteDataEP> notes;

		// �I�[�R�[�h�܂Ŗ������[�v
		while(true)
		{
			NoteDataEP l_append;

			(*ifs) >> l_append.iEventTime;

			// ���m�[�c�I�[�R�[�h(-1)
			if (l_append.iEventTime == END_OF_NOTE)
			{
				// �̈�m��
				pOut->OMSInfo.wNumNotes = (WORD)notes.size();
				pOut->NoteDatas = new NoteDataEP[pOut->OMSInfo.wNumNotes];

				// �f�[�^�R�s�[
				for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
				{
					memcpy_s(&pOut->NoteDatas[i], sizeof(NoteDataEP), &notes[i], sizeof(NoteDataEP));
				}
				notes.clear();

				// ���[�v������
				break;
			}

			(*ifs) >> l_iValue;
			l_append.wSoflanNo = (WORD)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byNoteType = (BYTE)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byLaneNo = (BYTE)l_iValue;

			// ���X�g�ǉ�
			notes.push_back(l_append);
		}
	}

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'S' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'F' ||
		l_cChunkID[3] != 'L' ||
		l_cChunkID[4] != 'A' ||
		l_cChunkID[5] != 'N' ||
		l_cChunkID[6] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// BPM�ύX���
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		(*ifs) >> pOut->SoflanDatas[i].iEventTime;
		(*ifs) >> pOut->SoflanDatas[i].fBPM;
		(*ifs) >> pOut->SoflanDatas[i].fTick;
	}

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'B' ||
		l_cChunkID[1] != 'E' ||
		l_cChunkID[2] != 'A' ||
		l_cChunkID[3] != 'T' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// ���q���
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		(*ifs) >> pOut->BeatDatas[i].iEventTime;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byNumerator = (BYTE)l_iValue;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byDenominator = (BYTE)l_iValue;
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadVRShootOMT(MusicInfoVRShoot * pOut, std::ifstream * ifs)
{
	// int�ȊO�̐����^(BYTE)�Ƃ��̓ǂݍ��݂Ɏg��(���ꂪ�Ȃ��ƒl�����������Ȃ�)
	int l_iValue;

	// �ő�p�x�ǂݍ���
	//(*ifs) >> l_iValue;
	//pOut->iMaxDegree = l_iValue;

	// OMS�t�H�[�}�b�g�ǂݍ���
	(*ifs) >> l_iValue;
	pOut->OMSInfo.byPlayMusicNo = (BYTE)l_iValue;

	// �m�[�c�͉ϒ��ɂ���
	//(*ifs) >> pOut->OMSInfo.wNumNotes;

	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumSoflans = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumBeats = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sDivision = (short)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sShift = (short)l_iValue;

	char l_cChunkID[16];

	/* �f�[�^�`�����N�N�� */

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'N' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'T' ||
		l_cChunkID[3] != 'E' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// �܂��m�[�g(���������₷���悤�ɉϒ��ɂ���)
	{
		std::vector<NoteDataVRShoot> notes;

		// �I�[�R�[�h�܂Ŗ������[�v
		while (true)
		{
			NoteDataVRShoot l_append;

			(*ifs) >> l_append.iEventTime;

			// ���m�[�c�I�[�R�[�h(-1)
			if (l_append.iEventTime == END_OF_NOTE)
			{
				// �̈�m��
				pOut->OMSInfo.wNumNotes = (WORD)notes.size();
				pOut->NoteDatas = new NoteDataVRShoot[pOut->OMSInfo.wNumNotes];

				// �f�[�^�R�s�[
				for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
				{
					memcpy_s(&pOut->NoteDatas[i], sizeof(NoteDataVRShoot), &notes[i], sizeof(NoteDataVRShoot));
				}
				notes.clear();

				// ���[�v������
				break;
			}

			(*ifs) >> l_iValue;
			l_append.wSoflanNo = (WORD)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byNoteType = (BYTE)l_iValue;
			(*ifs) >> l_append.fDegree;
			(*ifs) >> l_append.fRateY;
			(*ifs) >> l_iValue;
			l_append.wCNEndIndex = (WORD)l_iValue;

			// ���X�g�ǉ�
			notes.push_back(l_append);
		}
	}

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'S' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'F' ||
		l_cChunkID[3] != 'L' ||
		l_cChunkID[4] != 'A' ||
		l_cChunkID[5] != 'N' ||
		l_cChunkID[6] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// BPM�ύX���
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		(*ifs) >> pOut->SoflanDatas[i].iEventTime;
		(*ifs) >> pOut->SoflanDatas[i].fBPM;
		(*ifs) >> pOut->SoflanDatas[i].fTick;
	}

	// �`�����N�`�F�b�N
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'B' ||
		l_cChunkID[1] != 'E' ||
		l_cChunkID[2] != 'A' ||
		l_cChunkID[3] != 'T' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// ���q���
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		(*ifs) >> pOut->BeatDatas[i].iEventTime;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byNumerator = (BYTE)l_iValue;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byDenominator = (BYTE)l_iValue;
	}

	return OMS_RESULT::OK;
}
