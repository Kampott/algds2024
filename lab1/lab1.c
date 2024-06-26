#define __STDC_WANT_LIB_EXT1__ 1
#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <time.h>


typedef struct Grid {
int i, j;
struct Grid* up;
struct Grid* right;
struct Grid* down;
struct Grid* left;
int down_n, right_n;
}Grid;

void ASSERT_EQ(float a, float b){
	if(a!=b) printf("Invalid Execution\n");
	else printf("Correct Execution\n");
}

Grid** InitOrderedGrid(int h, int w){
	Grid** p = NULL;
	if ((h <2) || (w < 2)) {
		printf("Grid needs to be atleast 2x2!\n");
	}
	else
	{
	p = (Grid**)malloc( h * sizeof(Grid*));
	for(int i = 0; i<h;i++){
		p[i] = (Grid*)malloc(w * sizeof(Grid));
		if(p[i] == NULL){
			printf("Critical Error: P is a null pointer!\n");
			return 0;
		}
		for(int j = 0;j<w;j++){
			p[i][j].left = NULL;
			p[i][j].right = NULL;
			p[i][j].i = i;
			p[i][j].j = 0;
			p[i][j].down = NULL;
			p[i][j].left = NULL;
			p[i][j].up = NULL;
			p[i][j].right = NULL;
			if(i != 0){
				p[i][j].up = &p[i-1][j];
				p[i-1][j].down = &p[i][j];
			}
			if(j != 0){
				p[i][j-1].right = &p[i][j];
				p[i][j].left = &p[i][j-1];
			}
		}
		}
	}
	return p;
}

Grid** CreateOrderedGrid(int h, int w){
	Grid** p = NULL;
	if ((h <2) || (w < 2)) {
		printf("Grid needs to be atleast 2x2!\n");
	}
	else
	{
	p = (Grid**)malloc( h * sizeof(Grid*));
	for(int i = 0; i<h;i++){
		p[i] = (Grid*)malloc(w * sizeof(Grid));
		if(p[i] == NULL){
			printf("Critical Error: P is a null pointer!\n");
			return 0;
		}
		for(int j = 0;j<w;j++){
			p[i][j].left = NULL;
			p[i][j].right = NULL;
			p[i][j].i = i;
			p[i][j].j = 0;
			p[i][j].down = NULL;
			p[i][j].left = NULL;
			p[i][j].up = NULL;
			p[i][j].right = NULL;
			if(i != 0){
				p[i][j].up = &p[i-1][j];
				p[i-1][j].down = &p[i][j];
				p[i-1][j].down_n = rand() % 9 + 1;
			}
			if(j != 0){
				p[i][j-1].right = &p[i][j];
				p[i][j].left = &p[i][j-1];
				p[i][j-1].right_n = rand() % 9 + 1;
			}
		}
		}
	}
	return p;
}

void PrintGrid(Grid** p, int h, int w){
	if(p == NULL) printf("Critical Error: P is a null pointer!\n");
	else if(h < 2 || w < 2) printf("Critical Error: h or w less than 2!\n");
	else{
	for(int i = 0; i<h;i++){
		printf("(%d,0)",i,p[i][0].right_n);
		for(int j = 1;j<w;j++){
			printf("--%d--(%d,%d)",p[i][j-1].right_n,i,j);
		}
		printf("\n");
		if(i != h-1){
			for(int j = 0;j<w;j++){
				if(j != w-1){
				printf("-|%d|------",p[i][j].down_n);
				}
				else{
					printf("-|%d|",p[i][j].down_n);
				}
			}
			printf("\n");
		}
	}
	}
}

int** FindShortestPath(Grid** p, int* r, int h, int w){
	if(p == NULL || r == NULL) {printf("Critical Error: P or r is a null pointer!\n"); return NULL;}
	int** s = (int**)malloc(h*sizeof(int*));
	int** l = (int**)malloc(h*sizeof(int*));
	for(int i = h-1; i >= 0; i--){
		s[i] = (int*)malloc(w*sizeof(int));
		l[i] = (int*)malloc(w*sizeof(int));
		for(int j = 0; j<w; j++){
			if(i == h-1 && j == 0){s[i][0] = 0; l[i][0] = 2;}
			else if(i == h-1 && j>0){
				s[i][j] = s[i][j-1]+p[i][j-1].right_n;
				l[i][j] = 0;
			}
			else if(i != h-1 && j == 0){
				s[i][j] = s[i+1][j]+p[i][j].down_n;
				l[i][j] = 1;
			}
			else if(i != h-1 && j != 0){
				if(s[i][j-1]+p[i][j-1].right_n < s[i+1][j]+p[i][j].down_n){
					s[i][j] = s[i][j-1]+p[i][j-1].right_n;
					l[i][j] = 0;
				}
				else if(s[i][j-1]+p[i][j-1].right_n > s[i+1][j]+p[i][j].down_n){
					s[i][j] = s[i+1][j]+p[i][j].down_n;
					l[i][j] = 1;
				}
				else if(s[i][j-1]+p[i][j-1].right_n == s[i+1][j]+p[i][j].down_n){
					s[i][j] = s[i+1][j]+p[i][j].down_n;
					l[i][j] = 1;
				}
			}
		}
	}
	*r = s[0][w-1];
	for(int i = h-1; i>=0;i--){
		free(s[i]);
	}
	free(s);
	return l;
}

void printShortestPath(int** l, int h, int w){
	if(h < 2 || w < 2 || l == NULL) printf("Critical Error: L null pointer, or w<2 or h<2!\n");
	else{
	int i=0, j=w-1;
	printf("Printing Shortest Path:\n");
	printf("(%d,%d)",i,j);
	do{
		if(l[i][j] == 0){
			j--;
			printf("<---(%d,%d)",i,j);
		}
		else if(l[i][j] == 1){
			i++;
			printf("<---(%d,%d)",i,j);
		}
		else if(l[i][j] == 2){
			break;
		}
	}while(l[i][w] != 2);
	}
	printf("\n");
}

void ClearMemory(Grid** p, int** l, int h){
	if(l != NULL && p != NULL){
	for(int i = 0; i < h; i++){
		free(p[i]);
		free(l[i]);
	}
	free(l);
	free(p);
	}
}


//���� �����


void TestFindShortestPath_FixedPath_ReturnTwentyTwo(){
	//arrange
	int h=2, w=4;
	Grid** path = InitOrderedGrid(2,4);
	path[0][0].down_n = 15;
	path[0][0].right_n = 3;
	path[1][0].right_n = 10;
	path[0][1].right_n = 4;
	path[0][1].down_n = 3;
	path[0][2].right_n=11;
	path[0][2].down_n=14;
	path[0][3].down_n=5;
	path[1][1].right_n = 6;
	path[1][2].right_n=1;
	//act
	int r = 0;
	int** l = FindShortestPath(path,&r,h,w);

	//assrt
	printf("TestFindShortestPath_FixedPath_ReturnTwentyTwo(): ");
	ASSERT_EQ(r,22);
}
void TestFindShortestPath_FixedPath_ReturnFour(){
	//arrange
	int h=2, w=4;
	Grid** path = InitOrderedGrid(2,4);
	path[0][0].down_n = 1;
	path[0][0].right_n = 1;
	path[1][0].right_n = 1;
	path[0][1].right_n = 1;
	path[0][1].down_n = 1;
	path[0][2].right_n=1;
	path[0][2].down_n=1;
	path[0][3].down_n=1;
	path[1][1].right_n = 1;
	path[1][2].right_n=1;
	//act
	int r = 0;
	int** l = FindShortestPath(path,&r,h,w);

	//assrt
	printf("TestFindShortestPath_FixedPath_ReturnFour(): ");
	ASSERT_EQ(r, 4);
}
void TestFindShortestPath_FixedPath_ReturnFourtyNine(){
	//arrange
	int h=2, w=4;
	Grid** path = InitOrderedGrid(2,4);
	path[0][0].down_n = 10;
	path[0][0].right_n = 11;
	path[1][0].right_n = 12;
	path[0][1].right_n = 13;
	path[0][1].down_n = 14;
	path[0][2].right_n=15;
	path[0][2].down_n=16;
	path[0][3].down_n=17;
	path[1][1].right_n = 18;
	path[1][2].right_n=19;
	//act
	int r = 0;
	int** l = FindShortestPath(path,&r,h,w);

	//assrt
	printf("TestFindShortestPath_FixedPath_ReturnFourtyNine(): ");
	ASSERT_EQ(r, 49);
}
void TestFindShortestPath_FixedPath_ReturnFifty(){
	//arrange
	int h=2, w=4;
	Grid** path = InitOrderedGrid(2,4);
	path[0][0].down_n = 19;
	path[0][0].right_n = 18;
	path[1][0].right_n = 17;
	path[0][1].right_n = 16;
	path[0][1].down_n = 15;
	path[0][2].right_n=14;
	path[0][2].down_n=13;
	path[0][3].down_n=12;
	path[1][1].right_n = 11;
	path[1][2].right_n=10;
	//act
	int r = 0;
	int** l = FindShortestPath(path,&r,h,w);

	//assert
	printf("TestFindShortestPath_FixedPath_ReturnFifty(): ");
	ASSERT_EQ(r, 50);
}

int main(void) {
	setlocale(LC_CTYPE, "Russian");
	system("chcp 1251");
	int height = 10, width = 10;
	srand(time(NULL));
	Grid** path = NULL;
	path = CreateOrderedGrid(height,width);
	if(path != NULL){
	PrintGrid(path,height,width);
	int result = 0;
	int** trajectory = FindShortestPath(path,&result,height,width);
	printf("Shortest path took %d steps!\n",result);
	printShortestPath(trajectory,height,width);
	ClearMemory(path,trajectory,height);
	}

	//���� �����
	TestFindShortestPath_FixedPath_ReturnTwentyTwo();
	TestFindShortestPath_FixedPath_ReturnFour();
	TestFindShortestPath_FixedPath_ReturnFourtyNine();
	TestFindShortestPath_FixedPath_ReturnFifty();
	return 1;
}