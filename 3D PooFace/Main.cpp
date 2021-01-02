#include <Windows.h>
#include "Jogo.h"

static Jogo* pJogo = NULL;

static TecladoServidor tServer;
static MouseServer mServ;

UINT ALTURA, LARGURA;

LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	const wchar_t* nomeClasse = L"PooFacee";
	WNDCLASS wc = {};
	wc.hInstance = hInst;
	wc.lpfnWndProc = MsgProc;
	wc.lpszClassName = nomeClasse;

	RegisterClass(&wc);

	HWND hJanela = CreateWindow(nomeClasse, L"Pooface 3D",
		WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	RECT ret;
	GetClientRect(hJanela, &ret);
	ALTURA = ret.bottom - ret.top;
	LARGURA = ret.right - ret.left;

	TecladoCliente tCliente(tServer);
	MouseClient mCliente(mServ);

	ControleXBServer joystickServer(0);
	ControleXBCliente joystickCliente(joystickServer);

	pJogo = new Jogo(hJanela, tCliente, mCliente, joystickCliente);

	UpdateWindow(hJanela);
	ShowWindow(hJanela, SW_SHOW);
	ShowCursor(FALSE);

	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			joystickServer.Checar();
			pJogo->Vai();
		}
	}

	delete pJogo;
	pJogo = NULL;

	return 0;
}

LRESULT CALLBACK MsgProc(HWND hJanela, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}break;
	case WM_SIZE:
	{
		pJogo->MudouTamanhoJanela();

		RECT ret;
		GetClientRect(hJanela, &ret);
		ALTURA = ret.bottom - ret.top;
		LARGURA = ret.right - ret.left;
	}break;

	case WM_KEYDOWN:
		if (!(lparam & 0x40000000)) // no thank you on the autorepeat
		{
			tServer.BotaoFoiApertado((unsigned char)wparam);
			if (wparam == VK_ESCAPE)
				PostQuitMessage(0);
		}
		break;
	case WM_KEYUP:
		tServer.BotaoFoiSolto((unsigned char)wparam);
		break;
	case WM_CHAR:
		tServer.AddFilaChars((unsigned char)wparam);
		break;
		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		unsigned int x = (short)LOWORD(lparam);
		unsigned int y = (short)HIWORD(lparam);
		if (x > 0 && x < LARGURA && y > 0 && y < ALTURA)
		{
			mServ.OnMouseMove(x, y);
			if (!mServ.IsInWindow())
			{
				SetCapture(hJanela);
				mServ.OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = max(0, x);
				x = min(LARGURA - 1, x);
				y = max(0, y);
				y = min(ALTURA - 1, y);
				mServ.OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mServ.OnMouseLeave();
				mServ.OnLeftReleased(x, y);
				mServ.OnRightReleased(x, y);
			}
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			mServ.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			mServ.OnWheelDown(x, y);
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mServ.OnLeftPressed(x, y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mServ.OnRightPressed(x, y);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mServ.OnLeftReleased(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mServ.OnRightReleased(x, y);
		break;
	}

	// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc(hJanela, msg, wparam, lparam);
}