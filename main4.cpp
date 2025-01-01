#include<graphics.h>
#include<stdio.h>
#include<math.h>
#include <commdlg.h>
#define maxdinh 20
#define INPUT "scanline.inp"
#define OUTPUT "saved_polygon.txt"

// Cau truc nut nhan
struct button{
	int x1, y1, x2, y2;
};

// Cau truc chu
struct text{
	int x, y;
	char txt[255];
};

// Cau truc diem
struct point{
	int x, y;
};

point pt[maxdinh];
int n;
FILE *fp;
float m[maxdinh];
int ymin, ymax;
int color = YELLOW;
int oldcolor;
int fillspeed = 500;

// Nhap da giac tu ban phim
void keyboard(){
	printf("Nhap so dinh cua da giac: ");
	scanf("%d", &n);
	printf("CAC DINH CUA DA GIAC PHAI CO TOA DO: 15 < X < 600, 50 < Y < 520");
	printf("\nNhap cac dinh cua da giac: \n");
	for(int i=0; i<n; i++){
		printf("Nhap dinh [%d]: ", i);
		do{
			printf("\nNhap x: ");
			scanf("%d", &pt[i].x);
			printf("Nhap y: ");
			scanf("%d", &pt[i].y);
			if(pt[i].x <= 15 || pt[i].x >= 600 || pt[i].y <=50 || pt[i].y >= 520){
				printf("Toa do ban nhap khong nam trong man hinh. Vui long nhap lai!");
			}
		}while(pt[i].x <= 15 || pt[i].x >= 600 || pt[i].y <=50 || pt[i].y >= 520);
	}

	pt[n].x = pt[0].x;
	pt[n].y = pt[0].y;

	printf("So dinh cua da giac: %d", n);
	printf("\nToa do cac dinh cua da giac: \n");
	for(int i=0; i<n; i++){
		printf("%d", pt[i].x);
		printf("%5d", pt[i].y);
		printf("\n");
	}
}

// Nhap da giac tu file
void readfile(){
	fp = fopen(INPUT, "r"); // Mo tap tin de doc
	if(fp == NULL){
		printf("\nFile not found");
	}
	fscanf(fp, "%d", &n);
	printf("So dinh cua da giac: %d", n);

	// Doc cac dinh cua da giac
	for(int i=0; i<n; i++){
		fscanf(fp, "%d %d", &pt[i].x, &pt[i].y);
	}

	// Them 1 dinh thu 4 == dinh thu 0
	pt[n].x = pt[0].x; // x[n] = x[0]
	pt[n].y = pt[0].y; // y[n] = y[0]

	// In ra cac dinh cua da giac
	printf("\nToa do cac dinh cua da giac: \n");
	for(int i=0; i<n; i++){
		printf("%d", pt[i].x);
		printf("%5d", pt[i].y);
		printf("\n");
	}
	fclose(fp);
}

// Ve da giac
void drawpoly(){
	for(int i=0; i<n; i++){
		setcolor(0); // Thiet lap mau ve
		setlinestyle(1, 0, 3);
		line(pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
	}
}

//------------------------------------------------------------------------Scanline----------------------------------------------------------------------//
void hesogoc(){
	for(int i=0; i<n; i++){
		// Co 2 truong hop dac biet: dy = 0 va dx = 0
		// xsau = xtruoc + (y - ytruoc) / m
		if(pt[i+1].y == pt[i].y){ // dy = 0
			m[i] = 1;
		}
		else{
			if(pt[i+1].x == pt[i].x){ // dx = 0
				m[i] = 9999;
			}
			else{
				// m=dy/dx;
				m[i] = (float)(pt[i+1].y - pt[i].y) / (pt[i+1].x - pt[i].x); // (yi+1 - yi) / (xi+1 - xi)
			}
		}
	}
	printf("\nHe so goc cac canh cua da giac: ");
	for(int i=0; i<n; i++){
		printf("%.1f  ",m[i]);
	}
}

void minmax(){
	ymin = pt[0].y; // ymin = tung do dinh 0
	ymax = pt[0].y; // ymax = tung do dinh 0
	for(int i=0; i<n; i++){
		if(ymin > pt[i].y){
			ymin = pt[i].y;
		}
		if(ymax < pt[i].y){
			ymax = pt[i].y;
		}
	}
	printf("\nymin = %d", ymin);
	printf("\nymax = %d\n", ymax);
}

void scanline(){
	printf("-----------------------------------------------Scanline-----------------------------------------------");
	hesogoc();
	minmax();
	int xgd[maxdinh];
	for(int y=ymin; y<ymax; y++){
		// Xet xem co giao diem hay khong co giao diem tung canh cua da giac
		int count = 0; // Luu giu so giao diem cua moi y so voi cac canh cua da giac
		// Moi lan tang dong quet thi dem se lai ve 0 hay so giao diem = 0
		for(int i=0; i<n; i++){ // Xet tung canh cua da giac
			if((y < pt[i].y && y <= pt[i+1].y) || (y >= pt[i].y && y > pt[i+1].y)){ // Dong quet nam duoi hoac tren ca 2 tung do dinh
				printf("Khong co giao diem y = %d voi canh (%d,%d) - (%d,%d)\n", y, pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
			}
			else{
				printf("Ton tai giao diem giua y = %d voi canh cua da giac\n", y);
				// Xac dinh hoanh do giao diem
				xgd[count++] = round(pt[i].x + (y - pt[i].y) / m[i]);
			}
		}

		// In ra cac xgd cua y voi cac canh cua da giac
		printf("Cac hoanh do giao diem cua y = %d voi cac canh cua da giac: ",y);
		for(int i=0; i<count; i++){
			printf("(%d,%d)  ",xgd[i], y);
		}
		printf("\n");

		// Sap xep cac dinh theo hoanh do cua xgd
		for(int i=0; i<count-1; i++){
			for(int j=i+1; j<count; j++){
				if(xgd[i] > xgd[j]){
					int temp = xgd[i];
					xgd[i] = xgd[j];
					xgd[j] = temp;
				}
			}
		}

		// Noi cac dinh cua hoanh do giao diem lai voi nhau
		for(int i=0; i<count-1; i=i+2){
			setcolor(color);
			line(xgd[i], y, xgd[i+1], y); // Vi cung y
		}
		delay(10);
	}
	printf("-------------------------------------------------End--------------------------------------------------\n");
}
//------------------------------------------------------------------------End---------------------------------------------------------------------------//

//----------------------------------------------------------------------Floodfill-----------------------------------------------------------------------//
// To mau ben phai
void fillright(int xa, int ya)
{
	if(getpixel(xa, ya) == oldcolor){
		putpixel(xa, ya, color);

		// Toc do to mau
	  	fillspeed--;
		if(fillspeed == 0){
			delay(20);
			fillspeed = 500;
		}

		// To mau het ve phia ben phai
	  	fillright(++xa, ya);
	  	xa--;

		// To mau tu diem ke bien den tan cung ben duoi
		fillright(xa, ya + 1);

	  	// To mau tu diem ke bien den tan cung ben tren
	  	fillright(xa, ya - 1);
	}
}

// To mau ben trai
void fillleft(int xa, int ya)
{
	if(getpixel(xa, ya) == oldcolor){
	  	putpixel(xa, ya, color);

	  	// Toc do to mau
	  	fillspeed--;
		if(fillspeed == 0){
			delay(20);
			fillspeed = 500;
		}

		// To mau het ve phia ben trai
	  	fillleft(--xa, ya);
	  	xa++;

	  	// To mau tu diem ke bien den tan cung ben duoi
		fillleft(xa, ya + 1);

	  	// To mau tu diem ke bien den tan cung ben tren
	  	fillleft(xa, ya - 1);
	}
}

// To mau da giac
void floodfill(int x, int y)
{
  	fillright(x, y);
	fillleft(x-1, y);
}
//---------------------------------------------------------------------------End------------------------------------------------------------------------//

//-----------------------------------------------------------------------Boundaryfill-------------------------------------------------------------------//
// To mau
void boundaryright(int xa, int ya)
{
	if((getpixel(xa, ya) != 0) && (getpixel(xa, ya) != color))
  	{
  	  	putpixel(xa, ya, color);

  	  	// Toc do to mau
	  	fillspeed--;
		if(fillspeed == 0){
			delay(20);
			fillspeed = 500;
		}

		// To mau het ve phia ben phai
  	  	boundaryright(++xa, ya);

  	  	// Giam x - 1
  	  	xa--;

		// To mau tu diem ke bien den tan cung ben duoi
  	  	boundaryright(xa, ya+1);

  	  	// To mau tu diem ke bien den tan cung ben tren
  	  	boundaryright(xa, ya-1);
	}
}

// To nua ben trai
void boundaryleft(int xa, int ya)
{
  	if((getpixel(xa, ya) != 0) && (getpixel(xa, ya) != color))
  	{
  	  	putpixel(xa, ya, color);

  	  	// Toc do to mau
	  	fillspeed--;
		if(fillspeed == 0){
			delay(20);
			fillspeed = 500;
		}

		// To mau het ve phia ben phai
  	  	boundaryleft(--xa,ya);

  	  	// Tang x - 1
  	  	xa++;

		// To mau tu diem ke bien den tan cung ben duoi
  	  	boundaryleft(xa,ya+1);

  	  	// To mau tu diem ke bien den tan cung ben tren
  	  	boundaryleft(xa,ya-1);
	}
}

void boundaryfill(int x, int y)
{
	boundaryright(x, y);
	boundaryleft(x-1, y);
}
// Cac nut mau
void button_color(button btn, int color){
	// Ve vien màu den
    setcolor(BLACK);
    setlinestyle(0, 0, 3);
    rectangle(btn.x1, btn.y1, btn.x2, btn.y2);

    // Tô màu nút
    setfillstyle(1, color);
    floodfill(btn.x1 + 2, btn.y1 + 2, BLACK);
}

// Nut chuc nang
void button_exe(button btn, int color_btn, text txt, int color_txt){
	setcolor(color_btn);
	setlinestyle(0, 0, 3);
	rectangle(btn.x1, btn.y1, btn.x2, btn.y2);
	settextstyle(10, 0, 2.5);
	setcolor(color_txt);
	outtextxy(txt.x, txt.y, txt.txt);
}

// Kiem tra 1 diem co nam trong 1 nut khong
bool check_button(button btn, int x, int y){
	if(x >= btn.x1 && x <= btn.x2 && y >= btn.y1 && y <= btn.y2)
		return true;
	return false;
}

void savefile() {
    FILE *fp = fopen(OUTPUT, "w"); // Mo tap tin da ghi
    if (fp == NULL) {
        printf("\nKhong the mo file de ghi");
        return;
    }
    fprintf(fp, "%d\n", n); // Ghi so d?nh cua da giác
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d %d\n", pt[i].x, pt[i].y); // Ghi toa do các dinh
    }
    fclose(fp);
    printf("Da luu da giac vao file %s\n", OUTPUT);
}

void GUI_init(){
    gui:
    // Man hinh hien thi
    setcolor(BLACK);
    setlinestyle(0, 0, 3);
    rectangle(15, 50, 600, 520);
    button btn_ui = {15, 50, 600, 520};

    // Thong tin tac gia
    settextstyle(SMALL_FONT, 0, 5);
    setcolor(BLACK);
    char cr1[] = "MSSV: 64131995";
    char cr2[] = "Name: Truong Ngu Quyen";
    char cr3[] = "Class: 64.CNTT-3";
    outtextxy(620, 535, cr1);
    outtextxy(620, 555, cr2);
    outtextxy(620, 575, cr3);

    // Cac nut mau
    button btn_color[17];
    int x1 = 615, y1 = 50, x2 = 655, y2 = 90;
    int k = 0;
    for (int i = 0; i < 16; i++) {
        if (i == 11) {
            continue;
        }
        k++;
        btn_color[i] = {x1, y1, x2, y2};
        button_color(btn_color[i], i);
        x1 += 55; x2 += 55;
        if (k == 3) {
            y1 += 55;
            y2 += 55;
            x1 = 615;
            x2 = 655;
            k = 0;
        }
    }

    // Nut loadfile
    button btn_lf = {615, 340, 765, 390};
    text txt_lf = {637.5, 350, "Load file"};
    button_exe(btn_lf, 0, txt_lf, 0);

    // Nut keyboard
    button btn_kb = {615, 405, 765, 455};
    text txt_kb = {637.5, 417.5, "Keyboard"};
    button_exe(btn_kb, 0, txt_kb, 0);

    // Nut dung chuot
    button btn_m = {615, 470, 765, 520};
    text txt_m = {660, 482.5, "Mouse"};
    button_exe(btn_m, 0, txt_m, 0);

    // Nut savefile
    button btn_sf = {15, 12, 130, 45};
    text txt_sf = {18, 18, "Save file"};
    button_exe(btn_sf, 0, txt_sf, 0);

    // Nut scanline
    button btn_sl = {15, 535, 120, 590};
    text txt_sl = {18, 550, "Scanline"};
    button_exe(btn_sl, 0, txt_sl, 0);

    // Nut floodfill
    button btn_ff = {135, 535, 255, 590};
    text txt_ff = {140, 550, "Floodfill"};
    button_exe(btn_ff, 0, txt_ff, 0);

    // Nut boundaryfill
    button btn_bf = {270, 535, 430, 590};
    text txt_bf = {275, 550, "Boundaryfill"};
    button_exe(btn_bf, 0, txt_bf, 0);

    // Nut xoa
    setcolor(0);
    line(505, 545, 540, 580);
    line(505, 580, 540, 545);
    button btn_del = {445, 535, 600, 590};
    text txt_del = {410, 495, ""};
    button_exe(btn_del, 0, txt_del, 0);
    
    // Nut exit
    button btn_ex = {145, 12, 210, 45};
    text txt_ex = {155, 18, "Exit"};
    button_exe(btn_ex, 0, txt_ex, 0);

    // Xu ly dung chuot
    while (1) {
        delay(0.1);

        char s[100];
        int x, y;
        // Tao hieu ung hover nut
        if (ismouseclick(WM_MOUSEMOVE)) {
            getmouseclick(WM_MOUSEMOVE, x, y);

            // Hien toa do khi di chuyen chuot
            setcolor(CYAN);
            sprintf(s, "\t Toa Do (%d,%d) \t", x, y);
            outtextxy(325, 20, s);

            // Nut loadfile
            if (check_button(btn_lf, x, y)) {
                button_exe(btn_lf, 1, txt_lf, 1);
            } else if (!check_button(btn_lf, x, y)) {
                button_exe(btn_lf, 0, txt_lf, 0);
            }

            // Nut keyboard
            if (check_button(btn_kb, x, y)) {
                button_exe(btn_kb, 1, txt_kb, 1);
            } else if (!check_button(btn_kb, x, y)) {
                button_exe(btn_kb, 0, txt_kb, 0);
            }

            // Nut dung chuot
            if (check_button(btn_m, x, y)) {
                button_exe(btn_m, 1, txt_m, 1);
            } else if (!check_button(btn_m, x, y)) {
                button_exe(btn_m, 0, txt_m, 0);
            }

            // Nut savefile
            if (check_button(btn_sf, x, y)) {
                button_exe(btn_sf, 9, txt_sf, 9);
            } else if (!check_button(btn_sf, x, y)) {
                button_exe(btn_sf, 0, txt_sf, 0);
            }

            // Nut scanline
            if (check_button(btn_sl, x, y)) {
                button_exe(btn_sl, 2, txt_sl, 2);
            } else if (!check_button(btn_kb, x, y)) {
                button_exe(btn_sl, 0, txt_sl, 0);
            }

            // Nut floodfill
            if (check_button(btn_ff, x, y)) {
                button_exe(btn_ff, 2, txt_ff, 2);
            } else if (!check_button(btn_ff, x, y)) {
                button_exe(btn_ff, 0, txt_ff, 0);
            }

            // Nut boundaryfill
            if (check_button(btn_bf, x, y)) {
                button_exe(btn_bf, 2, txt_bf, 2);
            } else if (!check_button(btn_kb, x, y)) {
                button_exe(btn_bf, 0, txt_bf, 0);
            }

            // Nut xoa
            if (check_button(btn_del, x, y)) {
                button_exe(btn_del, 12, txt_del, 12);
                setcolor(12);
                line(505, 545, 540, 580);
                line(505, 580, 540, 545);
            } else if (!check_button(btn_del, x, y)) {
                button_exe(btn_del, 0, txt_del, 0);
                setcolor(0);
                line(505, 545, 540, 580);
                line(505, 580, 540, 545);
            }
            
            // Nut exit
            if (check_button(btn_ex, x, y)) {
                button_exe(btn_ex, 4, txt_ex, 4);
            } else if (!check_button(btn_ex, x, y)) {
                button_exe(btn_ex, 0, txt_ex, 0);
            }

            // Cac nut mau
            for (int i = 0; i < 16; i++) {
                if (i == 11) continue;
                if (check_button(btn_color[i], x, y)) {
                    button_color(btn_color[i], i);
                    setcolor(CYAN);
                    setlinestyle(0, 0, 3);
                    rectangle(btn_color[i].x1, btn_color[i].y1, btn_color[i].x2, btn_color[i].y2);
                } else {
                    button_color(btn_color[i], i);
                }
            }
        }

        // Tao su kien khi nhan nut
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);

            // Nut loadfile
            if (check_button(btn_lf, x, y)) {
                readfile();
                drawpoly();
            }

            // Nut keyboard
            if (check_button(btn_kb, x, y)) {
                keyboard();
                drawpoly();
            }

            // Nut dung chuot
            if (check_button(btn_m, x, y)) {
                n = 0;
                while (1) {
                    delay(0.1);

                    if (ismouseclick(WM_MOUSEMOVE)) {
                        getmouseclick(WM_MOUSEMOVE, x, y);
                        // Hien toa do khi di chuyen chuot
                        setcolor(CYAN);
                        sprintf(s, "\t Toa Do (%d,%d) \t", x, y);
                        outtextxy(325, 20, s);
                    }

                    // Nhap da giac bang chuot
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        getmouseclick(WM_LBUTTONDOWN, x, y);
                        clearmouseclick(WM_LBUTTONDOWN);
                        if (check_button(btn_ui, x, y)) {
                            setcolor(0);
                            circle(x, y, 2);
                            pt[n].x = x;
                            pt[n].y = y;
                            n++;
                        }
                    }
                    if (ismouseclick(WM_RBUTTONDOWN)) {
                        clearmouseclick(WM_RBUTTONDOWN);

                        // Them 1 dinh thu 4 == dinh thu 0
                        pt[n].x = pt[0].x;
                        pt[n].y = pt[0].y;
                        drawpoly();

                        // In ra cac dinh cua da giac
                        printf("So dinh cua da giac: %d", n);
                        printf("\nToa do cac dinh cua da giac: \n");
                        for (int i = 0; i < n; i++) {
                            printf("%d", pt[i].x);
                            printf("%5d", pt[i].y);
                            printf("\n");
                        }
                        break;
                    }
                }
            }

            // Nut savefile
            if (check_button(btn_sf, x, y)) {
                savefile();
            }
            
            // Nut exit
            if (check_button(btn_ex, x, y)) {
                exit(0);
            }

            // Cac nut mau
            for (int i = 0; i < 16; i++) {
                if (i == 11) continue;
                if (check_button(btn_color[i], x, y)) {
                    color = i;
                }
            }

            // Nut scanline
            if (check_button(btn_sl, x, y)) {
                scanline();
            }

            // Nut floodfill
            if (check_button(btn_ff, x, y)) {
                while (1) {
                    delay(0.1);
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        getmouseclick(WM_LBUTTONDOWN, x, y);
                        if (check_button(btn_ui, x, y)) {
                            oldcolor = getpixel(x, y);
                            floodfill(x, y);
                            break;
                        }
                    }
                }
            }

            // Nut boundaryfill
            if (check_button(btn_bf, x, y)) {
                while (1) {
                    delay(0.1);
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        getmouseclick(WM_LBUTTONDOWN, x, y);
                        if (check_button(btn_ui, x, y)) {
                            boundaryfill(x, y);
                            break;
                        }
                    }
                }
            }

            // Nut xoa
            if (check_button(btn_del, x, y)) {
                cleardevice();
                color = 15;
                goto gui;
            }
            
        }
    }
}

int main(){
    initwindow(800, 650);
    setbkcolor(WHITE);
    cleardevice();
    GUI_init();

    getch();
    closegraph();
    return 0;
}
