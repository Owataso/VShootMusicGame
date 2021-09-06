#include "TDNLIB.h"
#include "../Music/MusicInfo.h"
#include "MIDI_Loader.h"
#include "../Data/EditData.h"

OMS_RESULT MIDI_Loader::LoadMIDI( MIDILoadData *out, LPCSTR lpcFileName)
{
	std::ifstream infs(lpcFileName, std::ios::binary); // input file stream

	// �t�@�C���I�[�v���Ɏ��s�I
	if (!infs) return OMS_RESULT::FAILED_OPEN;

	//MyAssert(infs, "MIDI�t�@�C���I�[�v���Ɏ��s�B�t�@�C�������Ԉ���Ă��邩�A�t�@�C�������݂��Ȃ���\nFileName:%s", lpcFileName);

	// �G���f�B�A������
	ENDIAN endian = tdnSystem::GetEndian();

	/* �w�b�_�`�����N�N�� */
	char l_szType[4];
	infs.read(l_szType, 4);			// �w�b�_�`�����N�ł��邱�Ƃ�����"MThd"�Ƃ���4�����̃A�X�L�[�R�[�h �u4D 54 68 64�v(4�o�C�g)
	if (l_szType[0] != 'M'||
		l_szType[1] != 'T'||
		l_szType[2] != 'h'||
		l_szType[3] != 'd') return OMS_RESULT::FAILED_READ_MIDI;

	infs.read((char*)&out->iHeadChunkSize, 4);		// �w�b�_�`�����N�̃f�[�^�� �u00 00 00 06�v(4�o�C�g)
	infs.read((char*)&out->sFormat, 2);				// �t�H�[�}�b�g�u00 00�v�i�t�H�[�}�b�g�^�C�v��0�̏ꍇ�j (2�o�C�g)
	infs.read((char*)&out->sNumTrack, 2);			// �g���b�N�� �u00 01�v�i�g���b�N����1�̏ꍇ�j(2�o�C�g)
	infs.read((char*)&out->sDivision, 2);			// ����\(�f���^�^�C��)	�l�������̒���(�f�t�H���g����480)

	// �K�v�Ȃ�G���f�B�A���ϊ�
	if (endian == LITTLE_ENDIAN){   // ���g���G���f�B�A���Ȃ�v�ϊ�
		// �G���f�B�A���ϊ�����
		tdnSystem::ConvertEndian(&out->iHeadChunkSize, 4);
		tdnSystem::ConvertEndian(&out->sFormat, 2);
		(out->sFormat == 0) ? out->sNumTrack = 1 : tdnSystem::ConvertEndian(&out->sNumTrack, 2);
		tdnSystem::ConvertEndian(&out->sDivision, 2);
	}

	out->tagTrackDatas = new Track[out->sNumTrack];

	for (int i = 0; i < out->sNumTrack; i++)   // �g���b�N�������J�Ԃ�
	{
		/* �g���b�N�`�����N�N�� */
		infs.read(l_szType, 4);				// �g���b�N�`�����N�ł��邱�Ƃ��������ʎq"MTrk" �u4D 54 72 6B�v(4�o�C�g)

		if (
			l_szType[0] != 'M' ||
			l_szType[1] != 'T' ||
			l_szType[2] != 'r' ||
			l_szType[3] != 'k') return OMS_RESULT::FAILED_READ_MIDI;

		infs.read((char*)&out->tagTrackDatas[i].iSize, 4);		// �f�[�^���i���̃g���b�N�ɓ����Ă���f�[�^�Z�N�V�����̃f�[�^�̒����i16�i�@�f�[�^�̌��j�B�j (4�o�C�g)

		if (endian == LITTLE_ENDIAN)   // ���g���G���f�B�A���Ȃ�v�ϊ�
			tdnSystem::ConvertEndian(&out->tagTrackDatas[i].iSize, 4);

		// �f�[�^�̈�m��
		out->tagTrackDatas[i].cData = new char[out->tagTrackDatas[i].iSize];

		// �f�[�^�Z�N�V�����i�ϒ��j�f�[�^�Z�N�V�����̒����̓f�[�^���Ŏ������B�f�[�^���𐔂�����ē��͂���Ȃǃf�[�^�\���̐������������ƕW��MIDI�t�@�C���͉��t����Ȃ��B
		infs.read(out->tagTrackDatas[i].cData, out->tagTrackDatas[i].iSize);
	}

	return OMS_RESULT::OK;

	//// �ǂݎ�����g���b�N�`�����N�����o��
	//for (int i = 0; i < tracks; i++)   // �g���b�N�������J�Ԃ�
	//	LoadTrack(*tagTrackDatas[i], division);
	//
	//// �f�[�^���
	//for (int i = 0; i < tracks; i++)
	//{
	//	delete tagTrackDatas[i]->data;
	//	delete tagTrackDatas[i];
	//}
	//delete[] tagTrackDatas;
}

void MIDI_Loader::LoadTrack(MIDILoadData &track, int no)
{
	//BYTE temp(0);	 // �C�x���g��͂̍ۂɎg�p����ꎞ�ۑ��p�ϐ�
	//BYTE status(0);   // �X�e�[�^�X�o�C�g�p�̈ꎞ�ϐ�
	//int cnt(0);
	//unsigned int delta(0);
	//unsigned int current(0);	// �J�[�\���o�[�݂����Ȋ���

	//printf("# Track =====================\n");
	//printf("track_datas.size : %d\n", track.tagTrackDatas[no].iSize);
	//printf("track_datas.data\n");
	//for (int j = 0; j<track.tagTrackDatas[no].iSize; j++){   // �f�[�^�������J�Ԃ�

	//	delta = 0;   // ������

	//	// �f���^�^�C���v��(0�Ȃ瑽�����������A�������̓m�[�g�I�t�ƃI�����d�Ȃ����Ƃ�)
	//	while (true)
	//	{
	//		//�f���^�^�C���̒���
	//		delta = ((delta) << 7) | (track.tagTrackDatas[no].cData[j] & 0x7F);

	//		// 7�r�b�g�ڂ�0�Ȃ�f���^�^�C���I��(�f���^�^�C����4�o�C�g)
	//		if ((track.tagTrackDatas[no].cData[j++] & 0x80) == 0x00) break;
	//	}

	//	// �擪����̈ʒu
	//	current += delta;

	//	printf("%d:: \n", delta);   // �f���^�^�C���o��

	//	// �����j���O�X�e�[�^�X���[���ɑ΂��鏈��
	//	if ((track.tagTrackDatas[no].cData[j] & 0x80) == 0x80){
	//		// �����j���O�X�e�[�^�X���[�����K�p����Ȃ��ꍇ�́A�X�e�[�^�X�o�C�g�p�ϐ����X�V�B
	//		status = (BYTE)track.tagTrackDatas[no].cData[j];   // �X�e�[�^�X�o�C�g��ێ����Ă���
	//	}
	//	else {
	//		printf("@\n");   // �����j���O�X�e�[�^�X���[���K�p�̂��邵
	//		j--;   // �f�[�^�o�C�g�̒��O�̃o�C�g�i�f���^�^�C�����ȁH�j���w���悤�ɂ��Ă����B
	//		// ���̏�����j++����͂��Ȃ̂ŁA��������΃f�[�^�o�C�g�ɃA�N�Z�X�ł���B
	//	}

	//	// �f�[�^����
	//	if ((status & 0xf0) == 0x80){
	//		// �m�[�g�E�I�t�y�{�C�X���b�Z�[�W�z
	//		printf("Note Off [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		printf("Note%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//		printf("[0x%02x] \n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//		printf("Velocity=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//	}
	//	else if ((status & 0xf0) == 0x90){
	//		// �m�[�g�E�I���y�{�C�X���b�Z�[�W�z
	//		printf("Note On  [%02dch] : \n", (status & 0x0f));
	//		j++;

	//		int lane = (BYTE)track.tagTrackDatas[no].cData[j];
	//		printf("Note%d\n", lane);
	//		j++;

	//		printf("Velocity=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);

	//		event_mng->PushNoteOn(current, lane);
	//	}
	//	else if ((status & 0xf0) == 0xa0){
	//		// �|���t�H�j�b�N�E�L�[�E�v���b�V���[�y�{�C�X���b�Z�[�W�z
	//		printf("Polyphonic Key Pressure [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		printf("Note%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//		printf("[0x%02x] \n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//		printf("Pressure=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//	}
	//	else if ((status & 0xf0) == 0xb0){
	//		// �R���g���[���`�F���W�y�{�C�X���b�Z�[�W�z�A�y���[�h���b�Z�[�W�z
	//		printf("Control Change [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		temp = (BYTE)track.tagTrackDatas[no].cData[j++];
	//		if (temp <= 63){   // �A���σ^�C�v�̃G�t�F�N�g�Ɋւ���R���g���[�����iMSB��LSB���j
	//			// �i�z���g�́u0<=c && c<=63�v�Ə����������ǁAwarning���o��̂Łuc<=63�v�ɂ���j
	//			printf("VariableEffect(\n");
	//			switch (temp){
	//			case 0:    // ��ʃo�C�g[MSB]
	//			case 32:   // ���ʃo�C�g[LSB]
	//				printf("BankSelect[%s]\n", (temp == 0) ? "MSB\n" : "LSB\n");   // �o���N�E�Z���N�g
	//				break;
	//			case 1:
	//			case 33:
	//				printf("ModulationDepth[%s]\n", (temp == 1) ? "MSB\n" : "LSB\n");   // ���W�����[�V�����E�f�v�X
	//				break;
	//			case 2:
	//			case 34:
	//				printf("BreathType[%s]\n", (temp == 2) ? "MSB\n" : "LSB\n");   // �u���X�E�^�C�v
	//				break;
	//			case 4:
	//			case 36:
	//				printf("FootType[%s]\n", (temp == 4) ? "MSB\n" : "LSB\n");   // �t�b�g�E�^�C�v
	//				break;
	//			case 5:
	//			case 37:
	//				printf("PortamentoTime[%s]\n", (temp == 5) ? "MSB\n" : "LSB\n");   // �|���^�����g�E�^�C��
	//				break;
	//			case 6:
	//			case 38:
	//				printf("DataEntry[%s]\n", (temp == 6) ? "MSB\n" : "LSB\n");   // �f�[�^�E�G���g���[
	//				break;
	//			case 7:
	//			case 39:
	//				printf("MainVolume[%s]\n", (temp == 7) ? "MSB\n" : "LSB\n");   // ���C���E�{�����[��
	//				break;
	//			case 8:
	//			case 40:
	//				printf("BalanceControl[%s]\n", (temp == 8) ? "MSB\n" : "LSB\n");   // �o�����X�E�R���g���[��
	//				break;
	//			case 10:
	//			case 42:
	//				printf("Panpot[%s]\n", (temp == 10) ? "MSB\n" : "LSB\n");   // �p���|�b�g
	//				break;
	//			case 11:
	//			case 43:
	//				printf("Expression[%s]\n", (temp == 11) ? "MSB\n" : "LSB\n");   // �G�N�X�v���b�V����
	//				break;
	//			case 16:
	//			case 17:
	//			case 18:
	//			case 19:
	//			case 48:
	//			case 49:
	//			case 50:
	//			case 51:
	//				printf("SomethingElse_No_%d[%s]\n", temp, (temp == 16) ? "MSB\n" : "LSB\n");   // �ėp����q
	//				break;
	//			default:
	//				printf("##UndefinedType_No_%d[%s]", (temp<32) ? temp : temp - 32, (temp<32) ? "MSB\n" : "LSB\n");   // �悭�킩��Ȃ��R���g���[��
	//			}

	//			printf(")=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//		}
	//		else if (120 <= temp && temp <= 127){   // ���[�h�E���b�Z�[�W
	//			printf("ModeMessage(\n");
	//			switch (temp){
	//			case 120:
	//				printf("AllSoundOff\n");   // �I�[���E�T�E���h�E�I�t
	//				break;
	//			case 121:
	//				printf("ResetAllController\n");   // ���Z�b�g�E�I�[���E�R���g���[���[
	//				break;
	//			case 122:
	//				printf("LocalControl\n");   // ���[�J���E�R���g���[��
	//				break;
	//			case 123:
	//				printf("AllNoteOff\n");   // �I�[���E�m�[�g�E�I�t
	//				break;
	//			case 124:
	//				printf("OmniOn\n");   // �I���j�E�I��
	//				break;
	//			case 125:
	//				printf("OmniOff\n");   // �I���j�E�I�t
	//				break;
	//			case 126:
	//				printf("MonoModeOn\n");   // ���m���[�h�E�I���i�|�����[�h�E�I�t�j
	//				break;
	//			case 127:
	//				printf("PolyModeOn\n");   // �|�����[�h�E�I���i���m���[�h�E�I�t�j
	//				break;
	//			default:
	//				printf("##UndefinedType_No_%d\n", temp);   // �悭�킩��Ȃ��R���g���[��
	//			}
	//			printf(")=%d\n", temp, (BYTE)track.tagTrackDatas[no].cData[j]);
	//		}
	//		printf("\n");
	//	}
	//	else if ((status & 0xf0) == 0xc0){
	//		// �v���O�����E�`�F���W�y�{�C�X���b�Z�[�W�z
	//		printf("Program Change [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		printf("ToneNo=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//	}
	//	else if ((status & 0xf0) == 0xd0){
	//		// �`�����l���E�v���b�V���[�y�{�C�X���b�Z�[�W�z
	//		printf("Channel Pressure [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		printf("Pressure=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//	}
	//	else if ((status & 0xf0) == 0xe0){
	//		// �s�b�`�E�x���h�E�`�F���W�y�{�C�X���b�Z�[�W�z
	//		printf("Pitch Bend Change [%02dch] : \n", (status & 0x0f));
	//		j++;
	//		printf(" (LSB:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//		printf(", MSB:%d)\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//	}
	//	else if ((status & 0xf0) == 0xf0){
	//		// �y�V�X�e���E���b�Z�[�W�z
	//		switch (status & 0x0f){
	//		case 0x00:   // �G�N�X�N���[�V�u�E���b�Z�[�W
	//			printf("F0 Exclusive Message : \n");
	//			j++;

	//			// SysEx�̃f�[�^�����擾
	//			cnt = 0;   // ������
	//			while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
	//				cnt = cnt | temp;   // ����
	//				cnt <<= 7;   // 7bit���V�t�g
	//			}
	//			cnt = cnt | temp;   // ����
	//			printf(" Length=%u\n", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

	//			for (int k = 1; k <= cnt; k++){   // �����̕������f�[�^�擾
	//				printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//			}
	//			j--;   // �s���߂��������f�N�������g

	//			break;
	//		case 0x01:   // MIDI�^�C���R�[�h
	//			j++;
	//			break;
	//		case 0x02:   // �\���O�E�|�W�V�����E�|�C���^
	//			j++;
	//			break;
	//		case 0x03:   // �\���O�E�Z���N�g
	//			j++;
	//			break;
	//		case 0x07:   // �G���h�E�I�u�E�G�N�X�N���[�V�u�ł����邯�ǂ�...
	//			// F7�X�e�[�^�X�̏ꍇ�̃G�N�X�N���[�V�u�E���b�Z�[�W
	//			//printf("@End of Exclusive");
	//			printf("F7 Exclusive Message : \n");
	//			j++;

	//			// SysEx�̃f�[�^�����擾
	//			cnt = 0;   // ������
	//			while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
	//				cnt = cnt | temp;   // ����
	//				cnt <<= 7;   // 7bit���V�t�g
	//			}
	//			cnt = cnt | temp;   // ����
	//			printf(" Length=%u\n", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

	//			for (int k = 1; k <= cnt; k++){   // �����̕������f�[�^�擾
	//				printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//			}
	//			j--;   // �s���߂��������f�N�������g

	//			break;
	//		case 0x0F:   // ���^�C�x���g
	//			printf("Meta Ivent : \n");
	//			j++;
	//			switch ((BYTE)track.tagTrackDatas[no].cData[j]){
	//			case 0x00:   // �V�[�P���X�ԍ�
	//				printf("Sequence Number=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
	//				cnt <<= 8;   // 8bit���V�t�g
	//				cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j];
	//				printf("%d\n", cnt);
	//				break;
	//			case 0x01:   // �e�L�X�g[�ϒ�]
	//				printf("Text=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x02:   // ���쌠�\��[�ϒ�]
	//				printf("Copyright=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x03:   // �V�[�P���X���i�ȃ^�C�g���j�E�g���b�N��[�ϒ�]
	//				printf("Title=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x04:   // �y�햼[�ϒ�]
	//				printf("InstName=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x05:   // �̎�[�ϒ�]
	//				printf("Words=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x06:   // �}�[�J�[[�ϒ�]
	//				printf("Marker=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x07:   // �L���[�|�C���g[�ϒ�]
	//				printf("CuePoint=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x08:   // �v���O�������i���F���j[�ϒ�]
	//				printf("ProgramName=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x09:   // �f�o�C�X���i�������j[�ϒ�]
	//				printf("DeviceName=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("%c", track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			case 0x20:   // MIDI�`�����l���v���t�B�b�N�X[1byte]
	//				printf("MidiChannelPrefix=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (BYTE)track.tagTrackDatas[no].cData[j];
	//				printf("%d\n", cnt);
	//				break;
	//			case 0x21:   // �|�[�g�w��[1byte]
	//				printf("Port=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (BYTE)track.tagTrackDatas[no].cData[j];
	//				printf("%d\n", cnt);
	//				break;
	//			case 0x2F:   // �g���b�N�I�[[0byte]
	//				printf("End of Track\n");
	//				j += 1;   // �f�[�^���̕���ʂ�z��
	//				break;
	//			case 0x51:   // �e���|�ݒ�[3byte]
	//				printf("Temp=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
	//				cnt <<= 8;   // 8bit���V�t�g
	//				cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j++];
	//				cnt <<= 8;   // 8bit���V�t�g
	//				cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j];
	//				printf("%d\n", cnt);

	//				{
	//					// BPM����o��
	//					float bpm = 60000000 / (float)cnt;

	//					// �����_��1�Ő؂�̂ď���(10�ɂ���Ƒ��ʁA100�ɂ���Ƒ�2�ʂ܂Ő؂�̂Ă���)
	//					bpm = floorf(bpm * 10) / 10;

	//					// ���X�g�ɂԂ�����
	//					event_mng->PushBPM(current, bpm);
	//				}

	//				break;
	//			case 0x54:   // SMPTE�I�t�Z�b�g[5byte]
	//				printf("SMPTE_Offset=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
	//				switch (cnt & 0xC0){   // �t���[�����x
	//				case 0x00:
	//					printf("24fps\n");
	//					break;
	//				case 0x01:
	//					printf("25fps\n");
	//					break;
	//				case 0x10:
	//					printf("30fps(DropFrame)\n");
	//					break;
	//				case 0x11:
	//					printf("30fps\n");
	//					break;
	//				}
	//				printf(",Hour:%d\n", (cnt & 0x3F));   // ����
	//				printf(",Min:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // ��
	//				printf(",Sec:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // �b
	//				printf(",Frame:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // �t���[��
	//				printf(",SubFrame:%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);   // �T�u�t���[��
	//				break;
	//			case 0x58:   // ���q�ݒ�[4byte]
	//				printf("Rhythm=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				printf("%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//				cnt = 1;
	//				for (int k = 0; k <= (int)track.tagTrackDatas[no].cData[j]; k++){   // ���q�̕�����Z�o����
	//					cnt *= 2;   // 2�̗ݏ�
	//				}
	//				printf("/%d\n", cnt);
	//				j++;
	//				printf(" ClockPerBeat=%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//				printf(" 32NotePer4Note=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
	//				break;
	//			case 0x59:   // ���ݒ�[2byte]
	//				printf("Key=\n");
	//				j += 2;   // �f�[�^���̕���ʂ�z��
	//				cnt = (int)track.tagTrackDatas[no].cData[j++];
	//				if (temp > 0){
	//					printf("Sharp[%d]\n", temp);
	//				}
	//				else if (temp == 0){
	//					printf("C\n");
	//				}
	//				else {
	//					printf("Flat[%d]\n", temp);
	//				}
	//				cnt = (int)track.tagTrackDatas[no].cData[j];
	//				if (temp == 0){
	//					printf("_Major\n");
	//				}
	//				else {
	//					printf("_Minor\n");
	//				}

	//				break;
	//			case 0x7F:   // �V�[�P���T���胁�^�C�x���g
	//				printf("SpecialIvent=\n");
	//				cnt = 0;
	//				j += 1;
	//				while ((temp = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
	//					cnt = cnt | (temp & 0x7F);   // ����
	//					cnt <<= 7;   // 7bit���V�t�g
	//				}
	//				cnt = cnt | (temp & 0x7F);   // ����
	//				for (int k = 1; k <= cnt; k++){
	//					printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
	//				}
	//				j--;   // �s���߂��������f�N�������g
	//				break;
	//			}
	//			break;
	//		}
	//	}
	//}

}

void MIDI_Loader::LoadTrackALL(MIDILoadData &track, int no)
{
	BYTE c(0);			 // �C�x���g��͂̍ۂɎg�p����ꎞ�ۑ��p�ϐ�
	BYTE status(0);   // �X�e�[�^�X�o�C�g�p�̈ꎞ�ϐ�
	int cnt(0);
	unsigned int delta(0);   // ������

	printf("# Track[%02d] =====================\n", no + 1);
	printf("track_datas[%d].size : %d\n", no, track.tagTrackDatas[no].iSize);
	printf("track_datas[%d].data\n", no);
	for (int j = 0; j<track.tagTrackDatas[no].iSize; j++){   // �f�[�^�������J�Ԃ�

		delta = 0;   // ������
		while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
			delta = delta | (c & 0x7F);   // ����
			delta <<= 7;   // 7bit���V�t�g
		}
		delta = delta | c;   // ����
		printf("%d:: \n", delta);   // �f���^�^�C���o��

		// �����j���O�X�e�[�^�X���[���ɑ΂��鏈��
		if ((track.tagTrackDatas[no].cData[j] & 0x80) == 0x80){
			// �����j���O�X�e�[�^�X���[�����K�p����Ȃ��ꍇ�́A�X�e�[�^�X�o�C�g�p�ϐ����X�V�B
			status = (BYTE)track.tagTrackDatas[no].cData[j];   // �X�e�[�^�X�o�C�g��ێ����Ă���
		}
		else {
			printf("@\n");   // �����j���O�X�e�[�^�X���[���K�p�̂��邵
			j--;   // �f�[�^�o�C�g�̒��O�̃o�C�g�i�f���^�^�C�����ȁH�j���w���悤�ɂ��Ă����B
			// ���̏�����j++����͂��Ȃ̂ŁA��������΃f�[�^�o�C�g�ɃA�N�Z�X�ł���B
		}

		// �f�[�^����
		if ((status & 0xf0) == 0x80){
			// �m�[�g�E�I�t�y�{�C�X���b�Z�[�W�z
			printf("Note Off [%02dch] : \n", (status & 0x0f));
			j++;
			printf("Note%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			printf("[0x%02x] \n", (BYTE)track.tagTrackDatas[no].cData[j++]);
			printf("Velocity=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0x90){
			// �m�[�g�E�I���y�{�C�X���b�Z�[�W�z
			printf("Note On  [%02dch] : \n", (status & 0x0f));
			j++;
			printf("Note%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			printf("[0x%02x] \n", (BYTE)track.tagTrackDatas[no].cData[j++]);
			printf("Velocity=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0xa0){
			// �|���t�H�j�b�N�E�L�[�E�v���b�V���[�y�{�C�X���b�Z�[�W�z
			printf("Polyphonic Key Pressure [%02dch] : \n", (status & 0x0f));
			j++;
			printf("Note%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			printf("[0x%02x] \n", (BYTE)track.tagTrackDatas[no].cData[j++]);
			printf("Pressure=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0xb0){
			// �R���g���[���`�F���W�y�{�C�X���b�Z�[�W�z�A�y���[�h���b�Z�[�W�z
			printf("Control Change [%02dch] : \n", (status & 0x0f));
			j++;
			c = (BYTE)track.tagTrackDatas[no].cData[j++];
			if (c <= 63){   // �A���σ^�C�v�̃G�t�F�N�g�Ɋւ���R���g���[�����iMSB��LSB���j
				// �i�z���g�́u0<=c && c<=63�v�Ə����������ǁAwarning���o��̂Łuc<=63�v�ɂ���j
				printf("VariableEffect(\n");
				switch (c){
				case 0:    // ��ʃo�C�g[MSB]
				case 32:   // ���ʃo�C�g[LSB]
					printf("BankSelect[%s]\n", (c == 0) ? "MSB\n" : "LSB\n");   // �o���N�E�Z���N�g
					break;
				case 1:
				case 33:
					printf("ModulationDepth[%s]\n", (c == 1) ? "MSB\n" : "LSB\n");   // ���W�����[�V�����E�f�v�X
					break;
				case 2:
				case 34:
					printf("BreathType[%s]\n", (c == 2) ? "MSB\n" : "LSB\n");   // �u���X�E�^�C�v
					break;
				case 4:
				case 36:
					printf("FootType[%s]\n", (c == 4) ? "MSB\n" : "LSB\n");   // �t�b�g�E�^�C�v
					break;
				case 5:
				case 37:
					printf("PortamentoTime[%s]\n", (c == 5) ? "MSB\n" : "LSB\n");   // �|���^�����g�E�^�C��
					break;
				case 6:
				case 38:
					printf("DataEntry[%s]\n", (c == 6) ? "MSB\n" : "LSB\n");   // �f�[�^�E�G���g���[
					break;
				case 7:
				case 39:
					printf("MainVolume[%s]\n", (c == 7) ? "MSB\n" : "LSB\n");   // ���C���E�{�����[��
					break;
				case 8:
				case 40:
					printf("BalanceControl[%s]\n", (c == 8) ? "MSB\n" : "LSB\n");   // �o�����X�E�R���g���[��
					break;
				case 10:
				case 42:
					printf("Panpot[%s]\n", (c == 10) ? "MSB\n" : "LSB\n");   // �p���|�b�g
					break;
				case 11:
				case 43:
					printf("Expression[%s]\n", (c == 11) ? "MSB\n" : "LSB\n");   // �G�N�X�v���b�V����
					break;
				case 16:
				case 17:
				case 18:
				case 19:
				case 48:
				case 49:
				case 50:
				case 51:
					printf("SomethingElse_No_%d[%s]\n", c, (c == 16) ? "MSB\n" : "LSB\n");   // �ėp����q
					break;
				default:
					printf("##UndefinedType_No_%d[%s]", (c<32) ? c : c - 32, (c<32) ? "MSB\n" : "LSB\n");   // �悭�킩��Ȃ��R���g���[��
				}

				printf(")=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			}
			else if (64 <= c && c <= 95){   // �A���ςłȂ��^�C�v�̃G�t�F�N�g�Ɋւ���R���g���[�����
				printf("InvariableEffect(\n");
				switch (c){
				case 64:
					printf("Hold1(Damper)\n");   // �z�[���h�P�i�_���p�[�j
					break;
				case 65:
					printf("Portamento\n");   // �|���^�����g
					break;
				case 66:
					printf("Sostenuto\n");   // �\�X�e�k�[�g
					break;
				case 67:
					printf("SoftPedal\n");   // �\�t�g�E�y�_��
					break;
				case 69:
					printf("Hold2(Freeze)\n");   // �z�[���h�Q�i�t���[�Y�j
					break;
				case 71:
					printf("HarmonicIntensity\n");   // �n�[���j�b�N�E�C���e���V�e�B
					break;
				case 72:
					printf("ReleaseTime\n");   // �����[�X�E�^�C��
					break;
				case 73:
					printf("AttackTime\n");   // �A�^�b�N�E�^�C��
					break;
				case 74:
					printf("Brightness\n");   // �u���C�g�l�X
					break;
				case 80:
				case 81:
				case 82:
				case 83:
					printf("SomethingElse_No_%d\n", c);   // �ėp����q
					break;
				case 91:
					printf("ExternalEffect\n");   // �O���G�t�F�N�g
					break;
				case 92:
					printf("Tremol\no");   // �g������
					break;
				case 93:
					printf("Chorus\n");   // �R�[���X
					break;
				case 94:
					printf("Celeste\n");   // �Z���X�e
					break;
				case 95:
					printf("Phaser\n");   // �t�F�C�U�[
					break;
				default:
					printf("##UndefinedType_No_%d\n", c);   // �悭�킩��Ȃ��R���g���[��
				}
				printf(")=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			}
			else if (96 <= c && c <= 119){   // ����ȏ��
				printf("SpecialPurpose(\n");
				switch (c){
				case 96:
					printf("DataIncrement\n");   // �f�[�^�E�C���N�������g
					break;
				case 97:
					printf("DataDecrement\n");   // �f�N�������g
					break;
				case 98:
					printf("NRPN[LSB]\n");   // NRPN��LSB
					break;
				case 99:
					printf("NRPN[MSB]\n");   // NRPN��MSB
					break;
				case 100:
					printf("RPN[LSB]\n");   // RPN��LSB
					break;
				case 101:
					printf("RPN[MSB]\n");   // RPN��MSB
					break;
				default:
					printf("##UndefinedType_No_%d\n", c);   // �悭�킩��Ȃ��R���g���[��
				}
				printf(")=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
			}
			else if (120 <= c && c <= 127){   // ���[�h�E���b�Z�[�W
				printf("ModeMessage(\n");
				switch (c){
				case 120:
					printf("AllSoundOff\n");   // �I�[���E�T�E���h�E�I�t
					break;
				case 121:
					printf("ResetAllController\n");   // ���Z�b�g�E�I�[���E�R���g���[���[
					break;
				case 122:
					printf("LocalControl\n");   // ���[�J���E�R���g���[��
					break;
				case 123:
					printf("AllNoteOff\n");   // �I�[���E�m�[�g�E�I�t
					break;
				case 124:
					printf("OmniOn\n");   // �I���j�E�I��
					break;
				case 125:
					printf("OmniOff\n");   // �I���j�E�I�t
					break;
				case 126:
					printf("MonoModeOn\n");   // ���m���[�h�E�I���i�|�����[�h�E�I�t�j
					break;
				case 127:
					printf("PolyModeOn\n");   // �|�����[�h�E�I���i���m���[�h�E�I�t�j
					break;
				default:
					printf("##UndefinedType_No_%d\n", c);   // �悭�킩��Ȃ��R���g���[��
				}
				//printf(")=%d\n", c, (BYTE)track.tagTrackDatas[no].cData[j]);
			}
			printf("\n");
		}
		else if ((status & 0xf0) == 0xc0){
			// �v���O�����E�`�F���W�y�{�C�X���b�Z�[�W�z
			printf("Program Change [%02dch] : \n", (status & 0x0f));
			j++;
			printf("ToneNo=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0xd0){
			// �`�����l���E�v���b�V���[�y�{�C�X���b�Z�[�W�z
			printf("Channel Pressure [%02dch] : \n", (status & 0x0f));
			j++;
			printf("Pressure=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0xe0){
			// �s�b�`�E�x���h�E�`�F���W�y�{�C�X���b�Z�[�W�z
			printf("Pitch Bend Change [%02dch] : \n", (status & 0x0f));
			j++;
			printf(" (LSB:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
			printf(", MSB:%d)\n", (BYTE)track.tagTrackDatas[no].cData[j]);
		}
		else if ((status & 0xf0) == 0xf0){
			// �y�V�X�e���E���b�Z�[�W�z
			switch (status & 0x0f){
			case 0x00:   // �G�N�X�N���[�V�u�E���b�Z�[�W
				printf("F0 Exclusive Message : \n");
				j++;

				// SysEx�̃f�[�^�����擾
				cnt = 0;   // ������
				while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
					cnt = cnt | c;   // ����
					cnt <<= 7;   // 7bit���V�t�g
				}
				cnt = cnt | c;   // ����
				printf(" Length=%u\n", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

				for (int k = 1; k <= cnt; k++){   // �����̕������f�[�^�擾
					printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
				}
				j--;   // �s���߂��������f�N�������g

				break;
			case 0x01:   // MIDI�^�C���R�[�h
				printf("MIDI Time Code : \n");
				j++;
				printf("(frame/sec/min/hour)=%d", (BYTE)track.tagTrackDatas[no].cData[j]);
				break;
			case 0x02:   // �\���O�E�|�W�V�����E�|�C���^
				printf("Song Position Pointer : \n");
				j++;
				printf(" (LSB:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
				printf(", MSB:%d)\n", (BYTE)track.tagTrackDatas[no].cData[j]);
				break;
			case 0x03:   // �\���O�E�Z���N�g
				printf("Song Select : \n");
				j++;
				printf("SelectNo=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
				break;
			case 0x06:   // �`���[���E���N�G�X�g
				printf("Tune Request\n");
				break;
			case 0x07:   // �G���h�E�I�u�E�G�N�X�N���[�V�u�ł����邯�ǂ�...
				// F7�X�e�[�^�X�̏ꍇ�̃G�N�X�N���[�V�u�E���b�Z�[�W
				//printf("@End of Exclusive");
				printf("F7 Exclusive Message : \n");
				j++;

				// SysEx�̃f�[�^�����擾
				cnt = 0;   // ������
				while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){   // �t���O�r�b�g��1�̊Ԃ̓��[�v
					cnt = cnt | c;   // ����
					cnt <<= 7;   // 7bit���V�t�g
				}
				cnt = cnt | c;   // ����
				printf(" Length=%u\n", (unsigned int)cnt);   // SysEx�̃f�[�^�����擾

				for (int k = 1; k <= cnt; k++){   // �����̕������f�[�^�擾
					printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
				}
				j--;   // �s���߂��������f�N�������g

				break;
			case 0x08:   // �^�C�~���O�E�N���b�N
				printf("Timing Clock\n");
				break;
			case 0x0A:   // �X�^�[�g
				printf("Start\n");
				break;
			case 0x0B:   // �R���e�B�j���[
				printf("Continue\n");
				break;
			case 0x0C:   // �X�g�b�v
				printf("Stop\n");
				break;
			case 0x0E:   // �A�N�e�B�u�E�Z���V���O
				printf("Active Sensing\n");
				break;
				//case 0x0F:   // �V�X�e�����Z�b�g�i�����Ԉ���Ă���C������B�B�B�j
				//    printf("System Reset");
			case 0x0F:   // ���^�C�x���g
				printf("Meta Ivent : \n");
				j++;
				switch ((BYTE)track.tagTrackDatas[no].cData[j]){
				case 0x00:   // �V�[�P���X�ԍ�
					printf("Sequence Number=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
					cnt <<= 8;   // 8bit���V�t�g
					cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j];
					printf("%d\n", cnt);
					break;
				case 0x01:   // �e�L�X�g[�ϒ�]
					printf("Text=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x02:   // ���쌠�\��[�ϒ�]
					printf("Copyright=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x03:   // �V�[�P���X���i�ȃ^�C�g���j�E�g���b�N��[�ϒ�]
					printf("Title=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x04:   // �y�햼[�ϒ�]
					printf("InstName=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x05:   // �̎�[�ϒ�]
					printf("Words=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x06:   // �}�[�J�[[�ϒ�]
					printf("Marker=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x07:   // �L���[�|�C���g[�ϒ�]
					printf("CuePoint=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x08:   // �v���O�������i���F���j[�ϒ�]
					printf("ProgramName=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x09:   // �f�o�C�X���i�������j[�ϒ�]
					printf("DeviceName=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("%c", track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				case 0x20:   // MIDI�`�����l���v���t�B�b�N�X[1byte]
					printf("MidiChannelPrefix=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (BYTE)track.tagTrackDatas[no].cData[j];
					printf("%d\n", cnt);
					break;
				case 0x21:   // �|�[�g�w��[1byte]
					printf("Port=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (BYTE)track.tagTrackDatas[no].cData[j];
					printf("%d\n", cnt);
					break;
				case 0x2F:   // �g���b�N�I�[[0byte]
					printf("End of Track\n");
					j += 1;   // �f�[�^���̕���ʂ�z��
					break;
				case 0x51:   // �e���|�ݒ�[3byte]
					printf("Temp=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
					cnt <<= 8;   // 8bit���V�t�g
					cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j++];
					cnt <<= 8;   // 8bit���V�t�g
					cnt = cnt | (BYTE)track.tagTrackDatas[no].cData[j];
					printf("%d\n", cnt);
					break;
				case 0x54:   // SMPTE�I�t�Z�b�g[5byte]
					printf("SMPTE_Offset=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (BYTE)track.tagTrackDatas[no].cData[j++];
					switch (cnt & 0xC0){   // �t���[�����x
					case 0x00:
						printf("24fps\n");
						break;
					case 0x01:
						printf("25fps\n");
						break;
					case 0x10:
						printf("30fps(DropFrame)\n");
						break;
					case 0x11:
						printf("30fps\n");
						break;
					}
					printf(",Hour:%d\n", (cnt & 0x3F));   // ����
					printf(",Min:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // ��
					printf(",Sec:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // �b
					printf(",Frame:%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);   // �t���[��
					printf(",SubFrame:%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);   // �T�u�t���[��
					break;
				case 0x58:   // ���q�ݒ�[4byte]
					printf("Rhythm=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					printf("%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
					cnt = 1;
					for (int k = 0; k <= (int)track.tagTrackDatas[no].cData[j]; k++){   // ���q�̕�����Z�o����
						cnt *= 2;   // 2�̗ݏ�
					}
					printf("/%d\n", cnt);
					j++;
					printf(" ClockPerBeat=%d\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
					printf(" 32NotePer4Note=%d\n", (BYTE)track.tagTrackDatas[no].cData[j]);
					break;
				case 0x59:   // ���ݒ�[2byte]
					printf("Key=\n");
					j += 2;   // �f�[�^���̕���ʂ�z��
					cnt = (int)track.tagTrackDatas[no].cData[j++];
					if (c > 0){
						printf("Sharp[%d]\n", c);
					}
					else if (c == 0){
						printf("C\n");
					}
					else {
						printf("Flat[%d]\n", c);
					}
					cnt = (int)track.tagTrackDatas[no].cData[j];
					if (c == 0){
						printf("_Major\n");
					}
					else {
						printf("_Minor\n");
					}

					break;
				case 0x7F:   // �V�[�P���T���胁�^�C�x���g
					printf("SpecialIvent=\n");
					cnt = 0;
					j += 1;
					while ((c = (BYTE)track.tagTrackDatas[no].cData[j++]) & 0x80){
						cnt = cnt | (c & 0x7F);   // ����
						cnt <<= 7;   // 7bit���V�t�g
					}
					cnt = cnt | (c & 0x7F);   // ����
					for (int k = 1; k <= cnt; k++){
						printf("[%02x]\n", (BYTE)track.tagTrackDatas[no].cData[j++]);
					}
					j--;   // �s���߂��������f�N�������g
					break;
				default:
					;
				}
				break;
			default:
				printf("# SysEx (Something else...[Status:%02x])", status);   // ���m��ʃX�e�[�^�X�Ȃ�
			}
			printf("");
		}
		else {
			printf("## Something else...[Status:%02x]", status);   // ���m��ʃX�e�[�^�X�Ȃ�
		}
	}
	printf("");
}