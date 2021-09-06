
#include "tdnlib.h"
#include "DragAndDrop.h"

void DRAG_AND_DROP::Initialize(HWND hwnd)
{
	DragAcceptFiles(hwnd, true);
}

void DRAG_AND_DROP::Get_file_path(LPSTR out_buf, UINT buf_size, WPARAM param)
{
	DragQueryFile((HDROP)param, 0, out_buf, buf_size);
	DragFinish((HDROP)param);
}

void DRAG_AND_DROP::Get_some_file_path(LPSTR out_buf, UINT num_buf, UINT buf_size, WPARAM param)
{
	UINT num_files = DragQueryFile((HDROP)param, -1, NULL, NULL);

	for (UINT i = 0; i < num_buf; i++)
	{
		if (i >= num_files)
			out_buf[i * buf_size] = '\0';
		else
			DragQueryFile((HDROP)param, i, out_buf + (i * buf_size), buf_size);
	}

	DragFinish((HDROP)param);
}

UINT DRAG_AND_DROP::Get_all_file_path(LPSTR &out_buf, UINT buf_size, WPARAM param)
{
	UINT num_files = DragQueryFile((HDROP)param, -1, NULL, NULL);

	out_buf = new CHAR[num_files * buf_size]{};
	for (UINT i = 0; i < num_files; i++)
	{
		DragQueryFile((HDROP)param, i, (out_buf + buf_size * i), buf_size);
	}

	DragFinish((HDROP)param);

	return num_files;
}
