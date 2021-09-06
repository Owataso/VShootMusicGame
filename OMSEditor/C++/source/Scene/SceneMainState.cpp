#include "TDNLIB.h"
#include "SceneMainState.h"
#include "SceneMain.h"
#include "../Player/Player.h"
#include "../Judge/JudgeManager.h"
#include "../Music/PlayCursor.h"
#include "../Music/NoteManager.h"
#include "../Music/SoflanManager.h"
#include "../Music/BeatManager.h"
#include "../UI/ui.h"
#include "../Music/BeatBar.h"
#include "../Data/EditData.h"
#include "../Camera/camera.h"

//void NoteRender(NoteDataVRShoot *pNotes, int index, tdn2DObj *image, RS rs = RS::COPY)
//{
//	Vector2 vNotePos = pNotes[index].GetScreenPosition();
//
//	int srcY = 1792;
//	if ((pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_START) != 0)
//	{
//		srcY = 896;
//
//		// プロット
//		WORD wCNEndIndex = pNotes[index].wCNEndIndex;
//		for (int i = 0; i < 16; i++)
//		{
//			float fRate = i / 16.0f;
//			Vector2 vPlotPos;
//			Math::Blend(&vPlotPos, fRate, pNotes[index].GetScreenPosition(), pNotes[wCNEndIndex].GetScreenPosition());
//			image->Render((int)vPlotPos.x, (int)vPlotPos.y, 256, 256, 1792 - 256, srcY, 256, 256);
//		}
//	}
//	srcY = ((pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_START) != 0 || (pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_END) != 0) ? 896 : 1792;
//	// ノーツ
//	image->Render((int)vNotePos.x, (int)vNotePos.y, 256, 256, 1792, srcY, 256, 256, rs);
//	// 同時判定
//	if (pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CHORDS) image->Render((int)vNotePos.x - 64, (int)vNotePos.y - 64, 384, 384, 1664, 1152, 384, 384, rs);
//}

//*****************************************************************************************************************************
//
//		プロジェクト選択
//
//*****************************************************************************************************************************
//------------------------------------------------------
//	開始
//------------------------------------------------------
void SceneMainState::ProjectSelect::Enter(SceneMain *pMain)
{

}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void SceneMainState::ProjectSelect::Execute(SceneMain *pMain)
{

}

//------------------------------------------------------
//	終了
//------------------------------------------------------
void SceneMainState::ProjectSelect::Exit(SceneMain *pMain)
{

}

//------------------------------------------------------
//	描画
//------------------------------------------------------
void SceneMainState::ProjectSelect::Render(SceneMain *pMain)
{
	tdnFont::RenderStringCentering("プロジェクト読み込んで、どうぞ", 32, 640, 320);
}

bool SceneMainState::ProjectSelect::OnMessage(SceneMain * pMain, const Message & msg)
{
	return false;
}


//*****************************************************************************************************************************
//
//		プロジェクト読み込んだ後
//
//*****************************************************************************************************************************
//------------------------------------------------------
//	開始
//------------------------------------------------------
void SceneMainState::ProjectLoaded::Enter(SceneMain *pMain)
{
	// Shift値設定
	//if (pMain->m_pDataMgr->GetOMSOpenType() != OMS_TYPE::NO_LOAD)
	//{
	//	pMain->m_sShift = pMain->m_pDataMgr->GetMusicInfo()->OMSInfo.sShift;
	//}
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void SceneMainState::ProjectLoaded::Execute(SceneMain *pMain)
{

}

//------------------------------------------------------
//	終了
//------------------------------------------------------
void SceneMainState::ProjectLoaded::Exit(SceneMain *pMain)
{

}

//------------------------------------------------------
//	描画
//------------------------------------------------------
void SceneMainState::ProjectLoaded::Render(SceneMain *pMain)
{
	DWORD l_dwCol;
	LPCSTR l_lpcComment;

	l_dwCol = (pMain->m_pDataMgr->GetOMSOpenType() == OMS_TYPE::NO_LOAD) ? 0xffffffff : 0xff00ffff;
	switch (pMain->m_pDataMgr->GetOMSOpenType())
	{
	case OMS_TYPE::NO_LOAD:
		l_lpcComment = "譜面データなし";
		break;
	case OMS_TYPE::BASE:
		l_lpcComment = "汎用OMSデータ";
		break;
	case OMS_TYPE::VR_SHOOT:
		l_lpcComment = "VRシューティング用OMSデータ";
		break;
	}
	tdnText::Draw(32, 32, l_dwCol, l_lpcComment);

	l_dwCol = pMain->m_pDataMgr->isMusicOpend() ? 0xff00ffff : 0xffffffff;
	l_lpcComment = pMain->m_pDataMgr->isMusicOpend() ? "曲データあり" : "曲データなし";
	tdnText::Draw(32, 64, l_dwCol, l_lpcComment);
}

bool SceneMainState::ProjectLoaded::OnMessage(SceneMain * pMain, const Message & msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PLAY_MUSIC:
		// 譜面と曲を両方読み込んでいたら
		if (pMain->m_pDataMgr->GetOMSOpenType() != OMS_TYPE::NO_LOAD && pMain->m_pDataMgr->isMusicOpend())
		{
			// 再生ステートへ
			if(pMain->m_pDataMgr->GetOMSOpenType() == OMS_TYPE::BASE)pMain->m_pState->ChangeState(SceneMainState::PlayingEP::GetInstance());
			else if (pMain->m_pDataMgr->GetOMSOpenType() == OMS_TYPE::VR_SHOOT)pMain->m_pState->ChangeState(SceneMainState::PlayingVRShoot::GetInstance());
			return true;
		}
		// 読み込んで、どうぞ
		return false;
		break;

	case MESSAGE_TYPE::START_EDIT:
		// 譜面と曲を両方読み込んでいたら
		if (pMain->m_pDataMgr->GetOMSOpenType() != OMS_TYPE::NO_LOAD && pMain->m_pDataMgr->isMusicOpend())
		{
			// エディットステートへ
			switch (pMain->m_pDataMgr->GetOMSOpenType())
			{
			case OMS_TYPE::BASE:
				pMain->m_pState->ChangeState(SceneMainState::EditEP::GetInstance());
				break;

			case OMS_TYPE::VR_SHOOT:
				pMain->m_pState->ChangeState(SceneMainState::EditVRShoot::GetInstance());
				break;
			}

			return true;
		}
		// 読み込んで、どうぞ
		return false;
	}
	return false;
}


//*****************************************************************************************************************************
//
//		編集状態
//
//*****************************************************************************************************************************
//------------------------------------------------------
//	開始
//------------------------------------------------------
void SceneMainState::EditEP::Enter(SceneMain *pMain)
{
	// ソフランとノーツの情報をセッティング
	pMain->m_pSoflanMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());
	pMain->m_pNoteMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());
	pMain->m_pBeatMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());

	// スコアとコンボ数リセット
	pMain->m_pJudgeMgr->Reset();
	pMain->m_pBeatBar->Reset();
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void SceneMainState::EditEP::Execute(SceneMain *pMain)
{
	DWORD m_dwCurrentPosition = pMain->m_dwCurrentPosition;

	// ノーツ更新
	pMain->m_pNoteMgr->EditUpdate(m_dwCurrentPosition);

	//// ソフラン更新(★BPMが切り替わったらtrueが返るので、rhythmバーを切り替えてあげる)
	//pMain->m_pSoflanMgr->Update(m_dwCurrentPosition);
	//
	//// 拍子更新(拍子が切り替わったらtrueが返る)
	//pMain->m_pBeatMgr->Update(m_dwCurrentPosition);
}

//------------------------------------------------------
//	終了
//------------------------------------------------------
void SceneMainState::EditEP::Exit(SceneMain *pMain)
{
}

//------------------------------------------------------
//	描画
//------------------------------------------------------
void SceneMainState::EditEP::Render(SceneMain *pMain)
{
	// UI背景
	pMain->m_pUI->RenderBack();

	// UI描画
	pMain->m_pUI->Render();
}

//------------------------------------------------------
//	メッセージ受信
//------------------------------------------------------
bool SceneMainState::EditEP::OnMessage(SceneMain *pMain, const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::END_EDIT:
		// 元のステートに戻る
		pMain->m_pState->ChangeState(SceneMainState::ProjectLoaded::GetInstance());
		return true;
	}

	return false;
}



void SceneMainState::EditVRShoot::Enter(SceneMain * pMain)
{
	pMain->m_SelectNoteIndices.clear();

	//// ソフランとノーツの情報をセッティング
	//pMain->m_pSoflanMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot());
	//pMain->m_pNoteMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot());
	//pMain->m_pBeatMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot());

	//// スコアとコンボ数リセット
	//pMain->m_pJudgeMgr->Reset();
	//pMain->m_pBeatBar->Reset();

	// エディットカーソル初期化
	//pMain->m_pDataMgr->EditStart();
}

void SceneMainState::EditVRShoot::Execute(SceneMain * pMain)
{
	DWORD m_dwCurrentPosition = pMain->m_dwCurrentPosition;

	// ノーツ更新
	pMain->m_pNoteMgr->EditUpdate(m_dwCurrentPosition);

	// マウス更新
	//tdnMouse::Update();

	//WORD wSelectIndex = pMain->m_pDataMgr->GetEditNoteIndex();
	//// エディットノーツ選択していたら
	//if (wSelectIndex != EDIT_NO_SELECT_NOTE)
	//{
	//	tdnDebug::OutPutLog("%d\n", wSelectIndex);
	//	// カメラの位置を選択したノーツの位置に合わせてやる
	//	pMain->m_pCamera->SetTarget(pMain->m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[wSelectIndex].GetPosition());
	//}

	// エディットコントロール
	//pMain->m_pDataMgr->EditUpdateVRShoot();

	//// ソフラン更新(★BPMが切り替わったらtrueが返るので、rhythmバーを切り替えてあげる)
	//pMain->m_pSoflanMgr->Update(m_dwCurrentPosition);
	//
	//// 拍子更新(拍子が切り替わったらtrueが返る)
	//pMain->m_pBeatMgr->Update(m_dwCurrentPosition);
}

void SceneMainState::EditVRShoot::Exit(SceneMain * pMain)
{
}

void SceneMainState::EditVRShoot::Render(SceneMain * pMain)
{
	// お空
	//pMain->m_pSky->Render();

	// 背景色(度に応じてって感じで)
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xff400040);
	tdnPolygon::Rect(1280 / 4 * (pMain->m_iMaxDegree / 360), 0, (int)(1280 * (180.0f / pMain->m_iMaxDegree)), 720, RS::COPY, 0xff004040);
	int x = 1280 / 2;
	for (int i = 0; i < pMain->m_iMaxDegree / 180; i++) x /= 2;
	tdnPolygon::Rect(640 - x, 0, (int)(1280 * (90.0f / pMain->m_iMaxDegree)), 720, RS::COPY, 0xff002080);

	std::vector<WORD> *wSelectIndices = &pMain->m_SelectNoteIndices;

	if (wSelectIndices->empty())
	{
		tdnFont::RenderStringCentering("リストビューからノーツを選択して、どうぞ", 32, 640, 320);
		return;
	}

	MusicInfoVRShoot *refVRShoot = pMain->m_pDataMgr->GetMusicInfoVRShoot();
	tdn2DObj *refImage = pMain->m_pMap.get();
	refImage->SetScale(0.5f);

	//int iEndPos = min(wSelectIndex + iNumShowNote, refVRShoot->OMSInfo.wNumNotes);
	//for (int i = iEndPos - 1; i >= wSelectIndex; i--)

	// 一個だけ選択
	if (wSelectIndices->size() == 1)
	{
		int iStartPos = max(wSelectIndices->at(0) - NUM_SHOW_NOTE / 2, 0);
		int iEndPos = min(wSelectIndices->at(0) + NUM_SHOW_NOTE / 2, refVRShoot->OMSInfo.wNumNotes - 1);
		for (int i = iStartPos; i <= iEndPos; i++)
		{
			//NoteDataVRShoot *refNote = &refVRShoot->NoteDatas[i];

			int no = wSelectIndices->at(0) - i;

			// 透明度
			//float fAlphaRate = 1 - ((float)(no) / iEndPos);
			float fAlphaRate = 1/* - ((float)(no) / (wSelectIndex - iStartPos))*/;
			refImage->SetARGB((BYTE)(fAlphaRate * 255), (BYTE)128, (BYTE)128, (BYTE)128);
			//NoteRender(refVRShoot->NoteDatas, i, refImage);
			pMain->NoteRender(i);

			// 選択ノーツなら
			if (no == 0)
			{
				static float fAlphaAngle = 0;
				if ((fAlphaAngle += 0.1f) > PI * 2) fAlphaAngle -= PI * 2;
				refImage->SetAlpha(Math::Blend((sinf(fAlphaAngle) + 1) * 0.5f, 0.2f, 0.75f));	// -1～1→0～1
				pMain->NoteRender(i, RS::ADD);
				//NoteRender(refVRShoot->NoteDatas, i, refImage, RS::ADD);
				//tdnDebug::OutPutLog("%.1f, %.1f\n", NotePos.x, NotePos.y);
			}


			//refMesh->SetAngle(D3DXToRadian(refNote->fDegree));
			//refMesh->SetPos(refNote->GetPosition());
			//refMesh->Update();
			//refMesh->Render();
		}


		// テキスト最前面
		for (int i = iStartPos; i <= iEndPos; i++)
		{
			Vector2 vNotePos = refVRShoot->NoteDatas[i].GetScreenPosition(pMain->m_iMaxDegree);
			vNotePos -= Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);
			int no = wSelectIndices->at(0) - i;
			DWORD dwCol;

			// 選択ノーツの文字色
			if (no == 0) dwCol = 0xff00ffff;
			// 選択ノーツより後ろのノーツの文字色
			else if (no > 0) dwCol = 0xff0000ff;
			// 選択ノーツより前のノーツの文字色
			else if (no < 0) dwCol = 0xffffff00;
			tdnFont::RenderStringCentering(std::to_string(i).c_str(), 48, (int)vNotePos.x + 136, (int)vNotePos.y + 104, dwCol);

		}
	}

	// 複数選択
	else
	{
		for (auto it : *wSelectIndices)
		{
			//NoteDataVRShoot *refNote = &refVRShoot->NoteDatas[it];
			//NoteRender(refVRShoot->NoteDatas, it, refImage);
			pMain->NoteRender(it);
		}

		// テキスト最前面
		for (auto it : *wSelectIndices)
		{
			Vector2 vNotePos = refVRShoot->NoteDatas[it].GetScreenPosition(pMain->m_iMaxDegree);
			vNotePos -= Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);
			tdnFont::RenderStringCentering(std::to_string(it).c_str(), 48, (int)vNotePos.x + 136, (int)vNotePos.y + 104, 0xffff0000);
		}
	}

	refImage->SetScale(1);
	refImage->SetARGB(0xffffffff);

	// グリッド(縦)
	for (int x = 0; x < NUM_CELL_X; x++)
	{
		const float posX = tdnSystem::GetScreenSize().right * ((float)x / NUM_CELL_X);
		const float weight = (x % 2 == 0) ? 2.0f : 1.0f;
		tdnPolygon::DrawLine(posX, 0, posX, 720, 0xc0ffffff, weight);
	}

	// グリッド(横)
	for (int y = 0; y < NUM_CELL_Y; y++)
	{
		float posY = tdnSystem::GetScreenSize().bottom * ((float)y / NUM_CELL_Y);
		const float weight = (y % 2 == 0) ? 2.0f : 1.0f;
		tdnPolygon::DrawLine(0, posY, 1280, posY, 0xc0ffffff, weight);
	}

	// 右ドラッグ
	if (m_bRightClick)
	{
		Vector2 vec = m_vRightDragPosition - m_vRightClickPosition;

		// ￣
		tdnPolygon::DrawLine(m_vRightClickPosition.x, m_vRightClickPosition.y, m_vRightClickPosition.x + vec.x, m_vRightClickPosition.y, 0xffffff00, 3.0f);

		// |
		tdnPolygon::DrawLine(m_vRightClickPosition.x, m_vRightClickPosition.y, m_vRightClickPosition.x, m_vRightClickPosition.y + vec.y, 0xffffff00, 3.0f);
		
		//   |
		tdnPolygon::DrawLine(m_vRightClickPosition.x + vec.x, m_vRightClickPosition.y, m_vRightClickPosition.x + vec.x, m_vRightClickPosition.y + vec.y, 0xffffff00, 3.0f);
		
		// _
		tdnPolygon::DrawLine(m_vRightClickPosition.x, m_vRightClickPosition.y + vec.y, m_vRightClickPosition.x + vec.x, m_vRightClickPosition.y + vec.y, 0xffffff00, 3.0f);
	}
}

bool SceneMainState::EditVRShoot::OnMessage(SceneMain * pMain, const Message & msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PLAY_MUSIC:
		pMain->m_pState->ChangeState(SceneMainState::PlayingVRShoot::GetInstance());
		return true;

	case MESSAGE_TYPE::END_EDIT:
		// 元のステートに戻る
		pMain->m_pState->ChangeState(SceneMainState::ProjectLoaded::GetInstance());
		return true;

	case MESSAGE_TYPE::EDIT_SELECT_NOTE:
	{
		// 再生位置設定
		pMain->m_dwCurrentPosition = pMain->SetEditNoteVRShootString((LPCSTR)msg.ExtraInfo);
		pMain->m_dwCurrentPosition = max(0, pMain->m_dwCurrentPosition - MS_APPEAR_NOTE_TIME);
		//// カメラの位置を選択したノーツの位置に合わせてやる
		//pMain->m_pCamera->SetTarget(pMain->m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[wSelectIndex].GetPosition());
		return true;
	}

	case MESSAGE_TYPE::MOUSE_LEFT_DOWN:
	{
		std::vector<WORD> *wSelectIndices = &pMain->m_SelectNoteIndices;

		// 選択ノーツ保存
		pMain->SaveSelectPosition();

		Vector2 *vClickPos = (Vector2*)msg.ExtraInfo;

		pMain->ConvertCell(vClickPos);

		// クリック位置保存
		m_vLeftClickPosition.x = vClickPos->x;
		m_vLeftClickPosition.y = vClickPos->y;

		// クリックした地点にノーツ設定
		if(pMain->m_eDragFlag == SceneMain::DRAG_FLAG::LINE_PLOT)pMain->SetScreenPositionOnDegreeVRShoot(vClickPos, 0);
		return true;
	}


	case MESSAGE_TYPE::MOUSE_LEFT_DRAG:
	{
		std::vector<WORD> *wSelectIndices = &pMain->m_SelectNoteIndices;

		if (wSelectIndices->size() <= 0) return false;

		Vector2 *vDragPos = (Vector2*)msg.ExtraInfo;
		pMain->ConvertCell(vDragPos);
		MusicInfoVRShoot *refVRShoot = pMain->m_pDataMgr->GetMusicInfoVRShoot();

		// 1個だけの場合
		if (wSelectIndices->size() == 1)
		{
			// 0～1280 → -640～640
			vDragPos->x -= 640;

			// かける
			vDragPos->x *= 360.0f / pMain->m_iMaxDegree;

			// 戻す(-640～640 → 0～1280)
			vDragPos->x += 640;

			pMain->SetScreenPositionOnDegreeVRShoot(vDragPos, 0);
			return true;
		}

		// 複数選択の場合
		switch (pMain->m_eDragFlag)
		{
			case SceneMain::DRAG_FLAG::LINE_PLOT:
			{
				for (UINT i = 0; i < wSelectIndices->size(); i++)
				{
					//NoteDataVRShoot *refNote = &refVRShoot->NoteDatas[wSelectIndices->at(i)];
					float rate = (float)i / (wSelectIndices->size() - 1);
					Vector2 vNotePos;
					Math::Blend(&vNotePos, rate, m_vLeftClickPosition, *vDragPos);

					// 0～1280 → -640～640
					vNotePos.x -= 640;

					// かける
					vNotePos.x *= 360.0f / pMain->m_iMaxDegree;

					// 戻す(-640～640 → 0～1280)
					vNotePos.x += 640;

					pMain->SetScreenPositionOnDegreeVRShoot(&vNotePos, i);
				}
				break;
			}

			case SceneMain::DRAG_FLAG::X_SCALE:
			{
				// 移動量取得
				float x = ((vDragPos->x - m_vLeftClickPosition.x) * (360 / pMain->m_iMaxDegree)) + EDIT_VR_NOTE_SIZE;
				float fMove = (x + 640) / 640;

				//// 0～1280 → -640～640
				//pMain->SelectPositionToWorld();

				for (UINT i = 0; i < wSelectIndices->size(); i++)
				{
					Vector2 orgPosition = pMain->m_vSelectNotePositions[i];

					// 角度の値でXをいじる
					//orgPosition.x *= pMain->m_iMaxDegree / 360.0f;
					//
					//tdnDebug::OutPutLog("%.1f\n", orgPosition.x);

					// 0～1280 → -640～640
					orgPosition.x -= 640;

					// かける
					orgPosition.x *= fMove;

					// 戻す(-640～640 → 0～1280)
					orgPosition.x += 640;

					// いじったXを戻す
					//orgPosition.x *= fRate2;

					// セット
					pMain->SetScreenPositionOnDegreeVRShoot(&orgPosition, i);
				}

				//// 
				//pMain->SelectPositionToScreen();
				break;
			}

			case SceneMain::DRAG_FLAG::TRANSLATE:
			{
				// 移動量取得
				float x = ((vDragPos->x - m_vLeftClickPosition.x) * (360 / pMain->m_iMaxDegree));
				float y = vDragPos->y - m_vLeftClickPosition.y;

				for (UINT i = 0; i < wSelectIndices->size(); i++)
				{
					Vector2 orgPosition = pMain->m_vSelectNotePositions[i];
					//orgPosition.x = Math::Clamp(orgPosition.x + x, 0, 1280);
					orgPosition.x = orgPosition.x + x;
					orgPosition.y = Math::Clamp(orgPosition.y + y, 0, 720);
					pMain->SetScreenPositionOnDegreeVRShoot(&orgPosition, i);
				}
				break;
			}

		}
		return true;
	}

	case MESSAGE_TYPE::MOUSE_RIGHT_DOWN:
	{
		Vector2 *vClickPos = (Vector2*)msg.ExtraInfo;
		//pMain->ConvertCell(vClickPos);

		// クリック位置保存
		m_vRightClickPosition.x = m_vRightDragPosition.x = vClickPos->x;
		m_vRightClickPosition.y = m_vRightDragPosition.y = vClickPos->y;

		m_bRightClick = true;
		return true;
	}

	case MESSAGE_TYPE::MOUSE_RIGHT_UP:
		m_bRightClick = false;
		return true;

	case MESSAGE_TYPE::MOUSE_RIGHT_DRAG:
	{
		Vector2 *vDragPos = (Vector2*)msg.ExtraInfo;

		// ドラッグ位置保存
		m_vRightDragPosition.x = vDragPos->x;
		m_vRightDragPosition.y = vDragPos->y;

		return true;
	}

	case MESSAGE_TYPE::COPY_NOTE:
		pMain->CopyNote();
		return true;

	case MESSAGE_TYPE::PASTE_NOTE:
		pMain->PasteNote();
		return true;

	case MESSAGE_TYPE::REVERSE_X:
		pMain->ReverseX();
		return true;

	case MESSAGE_TYPE::REVERSE_Y:
		pMain->ReverseY();
		return true;
	}

	return false;

}


//*****************************************************************************************************************************
//
//		再生状態
//
//*****************************************************************************************************************************
//------------------------------------------------------
//	開始
//------------------------------------------------------
void SceneMainState::PlayingEP::Enter(SceneMain *pMain)
{
	// 再生位置の初期化
	//play_cursor->Initialize();

	// ソフランとノーツの情報をセッティング
	pMain->m_pSoflanMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());
	pMain->m_pNoteMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());
	pMain->m_pBeatMgr->Set(pMain->m_pDataMgr->GetMusicInfoEP());

	// スコアとコンボ数リセット
	pMain->m_pJudgeMgr->Reset();
	pMain->m_pBeatBar->Reset();

	// 再生
	pMain->m_pPlayCursor->SetShift(pMain->m_pDataMgr->GetShift());
	pMain->m_pPlayCursor->SetMusic(std::string(pMain->m_pDataMgr->GetProjectDirectory() + c_lpcPROJECT_OGG_NAME).c_str());
	pMain->m_pPlayCursor->Play();
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void SceneMainState::PlayingEP::Execute(SceneMain *pMain)
{
	DWORD dwCurrentPosition = pMain->m_pPlayCursor->GetMSecond();
	pMain->m_pBeatBar->Update(dwCurrentPosition);

	// ノーツ更新
	pMain->m_pNoteMgr->Update(dwCurrentPosition);

	// ソフラン更新(★BPMが切り替わったらtrueが返るので、rhythmバーを切り替えてあげる)
	if(pMain->m_pSoflanMgr->Update(dwCurrentPosition)) pMain->m_pBeatBar->SetBPM(pMain->m_pSoflanMgr->GetBPM());

	// 拍子更新(拍子が切り替わったらtrueが返る)
	if (pMain->m_pBeatMgr->Update(dwCurrentPosition)) {}

	// 演奏者更新
	//pMain->m_pPlayer->Update();

	// 判定更新
	pMain->m_pJudgeMgr->Update();

	if (pMain->m_pPlayCursor->isEnd())
	{
		Stop(pMain);
	}
}

void SceneMainState::PlayingEP::Stop(SceneMain *pMain)
{
	// 曲を止める
	pMain->m_pPlayCursor->Stop();

	// 前のステートに戻る
	pMain->m_pState->RevertToPreviousState();
}



//------------------------------------------------------
//	終了
//------------------------------------------------------
void SceneMainState::PlayingEP::Exit(SceneMain *pMain)
{

}

//------------------------------------------------------
//	描画
//------------------------------------------------------
void SceneMainState::PlayingEP::Render(SceneMain *pMain)
{
	// UI背景
	pMain->m_pUI->RenderBack();

	// UI描画
	pMain->m_pUI->Render();

	// 判定描画
	pMain->m_pJudgeMgr->Render();
}

bool SceneMainState::PlayingEP::OnMessage(SceneMain * pMain, const Message & msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::STOP_MUSIC:
		Stop(pMain);
		return true;
		break;
	}
	return false;
}



void SceneMainState::PlayingVRShoot::Enter(SceneMain * pMain)
{
	// ソフランとノーツの情報をセッティング
	pMain->m_pSoflanMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot());
	pMain->m_pNoteMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot(), pMain->m_dwCurrentPosition);
	pMain->m_pBeatMgr->Set(pMain->m_pDataMgr->GetMusicInfoVRShoot());

	// スコアとコンボ数リセット
	pMain->m_pJudgeMgr->Reset();
	pMain->m_pBeatBar->Reset();

	// 再生
	pMain->m_pPlayCursor->SetShift(pMain->m_pDataMgr->GetShift());
	pMain->m_pPlayCursor->SetMusic(std::string(pMain->m_pDataMgr->GetProjectDirectory() + c_lpcPROJECT_OGG_NAME).c_str());
	pMain->m_pPlayCursor->SetOffset(pMain->m_dwCurrentPosition);
	pMain->m_pPlayCursor->Play();
}

void SceneMainState::PlayingVRShoot::Execute(SceneMain * pMain)
{
	DWORD dwCurrentPosition = pMain->m_pPlayCursor->GetMSecond();
	pMain->m_pBeatBar->Update(dwCurrentPosition);

	// ノーツ更新
	pMain->m_pNoteMgr->Update(dwCurrentPosition);

	// ソフラン更新(★BPMが切り替わったらtrueが返るので、rhythmバーを切り替えてあげる)
	if (pMain->m_pSoflanMgr->Update(dwCurrentPosition)) pMain->m_pBeatBar->SetBPM(pMain->m_pSoflanMgr->GetBPM());

	// 拍子更新(拍子が切り替わったらtrueが返る)
	if (pMain->m_pBeatMgr->Update(dwCurrentPosition)) {}

	// 演奏者更新
	//pMain->m_pPlayer->Update();

	// 判定更新
	pMain->m_pJudgeMgr->Update();

	if (pMain->m_pPlayCursor->isEnd())
	{
		Stop(pMain);
	}
}

void SceneMainState::PlayingVRShoot::Stop(SceneMain *pMain)
{
	// 曲を止める
	pMain->m_pPlayCursor->Stop();

	// 前のステートに戻る
	pMain->m_pState->RevertToPreviousState();
}

void SceneMainState::PlayingVRShoot::Exit(SceneMain * pMain)
{
}

void SceneMainState::PlayingVRShoot::Render(SceneMain * pMain)
{
	// 背景色(度に応じてって感じで)
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xff400040);
	tdnPolygon::Rect(1280 / 4 * (pMain->m_iMaxDegree / 360), 0, (int)(1280 * (180.0f / pMain->m_iMaxDegree)), 720, RS::COPY, 0xff004040);
	int x = 1280 / 2;
	for (int i = 0; i < pMain->m_iMaxDegree / 180; i++) x /= 2;
	tdnPolygon::Rect(640 - x, 0, (int)(1280 * (90.0f / pMain->m_iMaxDegree)), 720, RS::COPY, 0xff002080);

	// グリッド(縦)
	for (int x = 0; x < NUM_CELL_X; x++)
	{
		const float posX = tdnSystem::GetScreenSize().right * ((float)x / NUM_CELL_X);
		const float weight = (x % 2 == 0) ? 2.0f : 1.0f;
		tdnPolygon::DrawLine(posX, 0, posX, 720, 0xc0ffffff, weight);
	}

	// グリッド(横)
	for (int y = 0; y < NUM_CELL_Y; y++)
	{
		float posY = tdnSystem::GetScreenSize().bottom * ((float)y / NUM_CELL_Y);
		const float weight = (y % 2 == 0) ? 2.0f : 1.0f;
		tdnPolygon::DrawLine(0, posY, 1280, posY, 0xc0ffffff, weight);
	}

	tdn2DObj *refImage = pMain->m_pMap.get();
	refImage->SetScale(0.5f);
	for (auto it : *pMain->m_pNoteMgr->GetList())
	{
		if (it->iPosition < -MS_APPEAR_NOTE_TIME) break;
		float alpha = ((float)abs(it->iPosition) / -MS_APPEAR_NOTE_TIME);

		pMain->PlayNoteRender(it->pNoteVRShoot, refImage, alpha, it->wIndex);
	}
	refImage->SetScale(1);
	refImage->SetAlpha(1.0f);

	// 判定描画
	pMain->m_pJudgeMgr->Render();
}

bool SceneMainState::PlayingVRShoot::OnMessage(SceneMain * pMain, const Message & msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::STOP_MUSIC:
		Stop(pMain);
		return true;
		break;
	}
	return false;
}
