#include<windows.h>

typedef struct{
	HWND hconsole;
	HDC hdc;
	HDC hbuf;
	HDC hfill;
	HGDIOBJ hbufbitmap;
	HGDIOBJ hfillbitmap;
}console;

int psize=1;

void hidecursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   	CONSOLE_CURSOR_INFO info;
   	info.dwSize = 100;
   	info.bVisible = FALSE;
   	SetConsoleCursorInfo(consoleHandle, &info);
}

console init(int w,int h){
	console c;
	c.hconsole=GetConsoleWindow();
	c.hdc=GetDC(c.hconsole);
	c.hbuf=CreateCompatibleDC(c.hdc);
	c.hfill=CreateCompatibleDC(c.hdc);
	c.hbufbitmap=CreateCompatibleBitmap(c.hdc,w,h);
	c.hfillbitmap=CreateCompatibleBitmap(c.hbuf,w,h);
	
	SelectObject(c.hbuf,c.hbufbitmap);
	SelectObject(c.hfill,c.hfillbitmap);
	
	hidecursor();
	
	return c;
}

void putpixel(console c,int x,int y,COLORREF color){
	for(int i=x;i<x+psize;i++)
		for(int j=y;j<y+psize;j++) SetPixel(c.hbuf,i,j,color);
}

void setfill(console c,int x,int y,int cx,int cy,COLORREF bc){
	for(int i=x;i<cx;i++)
		for(int j=y;j<cy;j++) SetPixel(c.hfill,i,j,bc);
	
}

void flush(console c,int x,int y,int cx,int cy){
	BitBlt(c.hbuf,x,y,cx,cy,c.hfill,0,0,SRCCOPY);
}

void render(console c,int x,int y,int cx,int cy){
	BitBlt(c.hdc,x,y,cx,cy,c.hbuf,0,0,SRCCOPY);
}

void terminate(console c){
	DeleteDC(c.hdc);
	DeleteDC(c.hbuf);
	DeleteDC(c.hfill);
	ReleaseDC(c.hconsole,c.hdc);
}


int main(){
	console c=init(800,800);
	setfill(c,0,0,800,800,RGB(245,245,245));
	flush(c,0,0,800,800);
	render(c,0,0,800,800);
	getch();
	terminate(c);
}




