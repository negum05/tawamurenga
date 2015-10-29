#include<iostream>
#include<fstream>
#include<list>
#include<cstdio>
#include<cstdlib> // atoi
using namespace std;

// 数字の入ったマスを記録
typedef struct NumBox {
  int x; // 数字の入ったマスの行番号
  int y; // 数字の入ったマスの列番号
  int num; // マスの中の数字
} NumBox;
list<NumBox> num_list; // 空のリストを作成
list<NumBox>::iterator ite_num_list; // リストのイテレータ

char *intext = "input.txt"; // 各格子の数字が入ったテキストファイルへのポインタ
char *outtext = "output.txt"; // 出力用ファイルへのポインタ

ifstream fin(intext);
ofstream fout(outtext);

int M, N_odd, N_even; // 行数、列数(奇数行)、列数(偶数行)

// 数字マスのリストを作成する
void genNumList()
{
	NumBox tmpNumBox;
	bool isOdd = true;
	bool isEven = false;
	string st;
	int i, j, tmp_num;
	i = 0;

	while (!fin.eof()) { // fin.eof() はEOFの次からfalseを返す
		fin >> st;
		if (!fin.eof()) {
			i++; // 行数をカウント
			for (j = 0; j < (int)st.size(); j++) {
				if ('0' <= st[j] && st[j] <= '6') {
					tmpNumBox.y = i;
					tmpNumBox.x = j + 1; // 列番号と要素番号のズレを修正
					// 数字が連続した場合、連続した数字を1つの数字としてst[j]に保存されるので
					// 最大桁にある数字をnumに代入する必要がある。6は最大値。
					tmp_num = atoi(&st[j]);
					while (tmp_num > 6) { tmp_num /= 10; }
					tmpNumBox.num = tmp_num;

					num_list.push_back(tmpNumBox);
				}
			}
		}
		if (isOdd) { isOdd = false; isEven = true; N_odd = st.size(); }
		else if (isEven) { isEven = false; N_even = st.size(); }
	}
	M = i;

	ite_num_list = num_list.begin();
	while (ite_num_list != num_list.end()) {
		cout << "(" << ite_num_list->y << ","
			<< ite_num_list->x << ") >> " << ite_num_list->num << endl;
		ite_num_list++;
	}
}

// 左端から始まる行の数字マス周りの黒マスの判定
void checkAroundNumBoxLonger(int method)
{
	fout << "C:" << method++ << " B_" << (*ite_num_list).y - 1 << "_" << (*ite_num_list).x - 1
		<< " + " << " B_" << (*ite_num_list).y << "_" << (*ite_num_list).x - 1
		<< " + " << " B_" << (*ite_num_list).y + 1 << "_" << (*ite_num_list).x - 1
		<< " + " << " B_" << (*ite_num_list).y + 1 << "_" << (*ite_num_list).x
		<< " + " << " B_" << (*ite_num_list).y + 1 << "_" << (*ite_num_list).x + 1
		<< " + " << " B_" << (*ite_num_list).y - 1 << "_" << (*ite_num_list).x
		<< " = " << (*ite_num_list).num << endl;
}

// 左端から半マスずれて始まる行の数字マス周りの黒マスの判定
void checkAroundNumBox(int method)
{
	fout << "C:" << method++ << " B_" << (*ite_num_list).y - 1 << "_" << (*ite_num_list).x
		<< " + " << " B_" << (*ite_num_list).y << "_" << (*ite_num_list).x - 1
		<< " + " << " B_" << (*ite_num_list).y + 1 << "_" << (*ite_num_list).x
		<< " + " << " B_" << (*ite_num_list).y + 1 << "_" << (*ite_num_list).x + 1
		<< " + " << " B_" << (*ite_num_list).y << "_" << (*ite_num_list).x + 1
		<< " + " << " B_" << (*ite_num_list).y - 1 << "_" << (*ite_num_list).x + 1
		<< " = " << (*ite_num_list).num << endl;
}

// 目的関数を書き込む
void printObj()
{
	fout << "minimize\nX\n";
}

// 制約式を書き込む
void printST()
{
	int method = 1; // method:制約式の番号
	int lenN; // 注目行の列数
	int i, j;

	fout << "\nsubject to\n";

	/**************************************
	 **
	 **	マスは黒又は白である
	 **
	 **************************************/
	// B_i_j + W_i_j = 1
	for (i = 1; i <= M; i++) {
		lenN = (i % 2 == 1) ? N_odd : N_even; // 列数の調整
		for (j = 1; j <= lenN; j++) {
			fout << "A:" << method++ << " B_" << i << "_" << j
				<< " + W_" << i << '_' << j << " = 1\n";
		}
	}

	/**************************************
	 **
	 **   数字マスは白である
	 **
	 **************************************/
	// W_i_j = 1  (数字マス)
	method = 1;
	ite_num_list = num_list.begin();
	while (ite_num_list != num_list.end()) {
		fout << "B:" << method++ << " W_" << (*ite_num_list).y << "_"
			<< (*ite_num_list).x << " = 1" << endl;
		ite_num_list++;
	}

	/**************************************
	**
	**   数字マスの周りの黒マスの数
	**
	**************************************/
	method = 1;
	bool isOddLonger = (N_odd > N_even) ? true : false;

	// 左端から始まる行
	// B_i-1_j + B_i_j-1 + B_i+1_j-1 + B_i+1_j + B_i+1_j+1 + B_i-1_j = F(k)[マスの数字]
	// 左端から半マスずれて始まる行
	// B_i-1_j + B_i_j-1 + B_i+1_j + B_i+1_j+1 + B_i_j+1 + B_i-1_j+1 = F(k)[マスの数字]
	ite_num_list = num_list.begin();
	while (ite_num_list != num_list.end()) {
		if ( (*ite_num_list).y % 2 == 1 ) {
			if (isOddLonger) { checkAroundNumBoxLonger(method); }
			else {	checkAroundNumBox(method); }
		}
		else {
			if (!isOddLonger) {	checkAroundNumBox(method); }
			else { checkAroundNumBoxLonger(method); }
		}
		ite_num_list++;
	}

}

void printEnd()
{
  fout << "\nend\n";
}

int main()
{
  if( !fin ) {
    cerr << "input.txtが開けません\n";
    return -1;
  }

  if( !fout ) {
    cerr << "output.txtが開けません\n";
    return -1;
  }

  genNumList();
  printObj();
  printST();
  // printBounds();
  // printBinary();
  // printGeneral();
  // printEnd();

  fin.close();
  fout.close();
  return 0;
}
