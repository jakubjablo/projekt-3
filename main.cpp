#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <gdiplus.h>
#include <vector>
#include <cmath>

#define ID_BUTTON1				201
#define ID_BUTTON2				202
#define ID_BUTTON3				203
#define ID_BUTTON4				204
#define ID_BUTTON7				207
#define ID_BUTTON8				208


using namespace Gdiplus;

LPSTR WindowClass = "Projekt 3";
HINSTANCE hInst;

HWND hwndButton;


LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

void draw(HDC hdc);
void FillData();
void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, const std::vector<Point> &vect1,const std::vector<Point> &vect2,const std::vector<Point> &vect3,int x);
int OnCreate(HWND window);

std::vector<double> dataFile;
std::vector<Point> acc;
std::vector<Point> vel;
std::vector<Point> disp;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow){
    GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	MyRegisterClass(hInstance);

    MSG msg;

    if (!InitInstance(hInstance, nCmdShow))
	{
        return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){

    HWND hWnd;
	hInst = hInstance;

	hWnd = CreateWindow(WindowClass, "Projekt 3", WS_OVERLAPPEDWINDOW |WS_HSCROLL|WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,1500, 1000, NULL, NULL, hInstance, NULL);

    hwndButton = CreateWindow(TEXT("button"), TEXT("Przys"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 450, 100, 50,
		hWnd,(HMENU)ID_BUTTON1, hInstance, NULL);

    hwndButton = CreateWindow(TEXT("button"), TEXT("Predkosc"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 550, 100, 50,
		hWnd,(HMENU)ID_BUTTON2,hInstance,NULL);

    hwndButton = CreateWindow(TEXT("button"),TEXT("Droga"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 450,100, 50,
		hWnd,(HMENU)ID_BUTTON3,hInstance,NULL);

    hwndButton = CreateWindow(TEXT("button"),TEXT("Wszystko"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 550,100, 50,
        hWnd,(HMENU)ID_BUTTON4,hInstance,NULL);


    hwndButton = CreateWindow(TEXT("button"),TEXT("CLEAR"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 550,100, 50,
		hWnd,(HMENU)ID_BUTTON7,hInstance,NULL);

    hwndButton = CreateWindow(TEXT("button"),TEXT("Usun 100"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 450,100, 50,
		hWnd,(HMENU)ID_BUTTON8,hInstance,NULL);

    OnCreate(hWnd);
	if (!hWnd)
	{
		return FALSE;
	}

    ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


ATOM MyRegisterClass(HINSTANCE hInstance){

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WindowClass;

    return RegisterClassEx(&wc);
}

void FillData(){
    std::ifstream plik;
    plik.open( "outputRobotForwardB01.log", std::ios::in);
    double cur;
    while (plik >> cur) {
        dataFile.push_back(cur);
    }
    plik.close();

    double AxValue,velocity,displacement;
    for(int i = 3; i < dataFile.size(); i = i+12){
        AxValue = abs(dataFile[i]);
        acc.push_back(Point(i/3,50+200*AxValue));
    }

    velocity = acc[0].Y;
    vel.push_back(Point(1,velocity));

    for(int i = 1; i <= acc.size(); i++){
        velocity = (vel[i-1].Y + acc[i].Y);
        vel.push_back(Point(250*i,velocity));
    }
      displacement = vel[0].Y  + 1/2*acc[0].Y;
      disp.push_back(Point(1,displacement));
    for(int i = 1; i <= acc.size();i++){
        displacement = (disp[i-1].Y + vel[i].Y + 0.5*acc[i].Y);
        disp.push_back(Point(250*i, displacement));
    }
}

void draw(HDC hdc,const std::vector<Point> &vect) {
	Graphics graphics(hdc);
	Pen pen(Color(255, 255, 0, 0));
	for (int i = 1; i < acc.size(); i++){
        graphics.DrawLine(&pen,vect[i-1].X,vect[i-1].Y,vect[i].X,vect[i].Y);
	}
}
void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps,const std::vector<Point> &vect1,const std::vector<Point> &vect2,const std::vector<Point> &vect3,int x)
{
	InvalidateRect(hWnd, NULL, TRUE);
    if(x == 1){
        hdc = BeginPaint(hWnd, &ps);
        draw(hdc,vect1);
        EndPaint(hWnd, &ps);
    }
    else if(x == 2){
        hdc = BeginPaint(hWnd, &ps);
        draw(hdc,vect2);
        EndPaint(hWnd, &ps);
    }
    else if(x == 3){
        hdc = BeginPaint(hWnd, &ps);
        draw(hdc,vect3);
        EndPaint(hWnd, &ps);
    }
    else if(x == 4){
        hdc = BeginPaint(hWnd, &ps);
        draw(hdc,vect1);
        draw(hdc,vect2);
        draw(hdc,vect3);
        EndPaint(hWnd, &ps);
    }
}

int OnCreate(HWND window){
   FillData();
   return 0;
}

LRESULT CALLBACK	WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    int wmId, wmEvent;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	    case WM_COMMAND:
            wmId = LOWORD(wp);
            wmEvent = HIWORD(wp);
            switch (wmId){
            case ID_BUTTON1:
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,1);
                break;

            case ID_BUTTON2:
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,2);
                break;

            case ID_BUTTON3:
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,3);
                break;

            case ID_BUTTON4:
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,4);
                break;

            case ID_BUTTON7:
                acc.clear();
                vel.clear();
                disp.clear();
                FillData();
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,4);
                break;

            case ID_BUTTON8:
                acc.erase(acc.begin() , acc.begin() + 100);
                vel.erase(vel.begin() , vel.begin() + 100);
                disp.erase(disp.begin() , disp.begin() + 100);
                repaintWindow(hWnd, hdc, ps,acc,vel,disp,4);
                break;

            default:
                return DefWindowProc(hWnd, msg, wp, lp);
            }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
