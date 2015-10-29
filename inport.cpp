#include<iostream>
#include<fstream>
#include<list>
#include<cstdio>
#include<cstdlib> // atoi
using namespace std;

typedef struct NumBox {
  int x;
  int y;
  int num;
} NumBox;
list<NumBox> num_list; // 空のリストを作成
list<NumBox>::iterator ite_num_list; // リストのイテレータ

int main()
{
  char *intext = "input.txt";
  char *outtext = "output.txt";

  ifstream fin(intext);
  ofstream fout(outtext);

  if( !fin ) {
    cerr << "input.txtが開けません\n";
    return -1;
  }

  if( !fout ) {
    cerr << "output.txtが開けません\n";
    return -1;
  }

  NumBox tmpNumBox;
  bool isOdd = true;
  bool isEven = false;
  string st;
  int i, j, M, N_odd, N_even, tmp_num;
  i = 0;

  while( !fin.eof() ) { // fin.eof() はEOFの次からfalseを返す
    fin >> st;
    if( !fin.eof() ) {
      i++; // 行数をカウント
      for( j = 0; j < (int)st.size(); j++ ) {
	if( '0' <= st[j] && st[j] <= '6' ) {
	  tmpNumBox.y = i;
	  tmpNumBox.x = j+1; // 列番号と要素番号のズレを修正
	  // 数字が連続した場合、連続した数字を1つの数字としてst[j]に保存されるので
	  // 最大桁にある数字をnumに代入する必要がある。その処理。
	  tmp_num = atoi(&st[j]);
	  while( tmp_num > 6 ) { tmp_num /= 10; }
	  tmpNumBox.num = tmp_num;

	  num_list.push_back(tmpNumBox);
	}
      }
      // fout << st << endl;
      // cout << st << endl;
    }
    if( isOdd ) { isOdd = false; isEven = true; N_odd = st.size(); }
    else if( isEven ) { isEven = false; N_even = st.size(); }
  }
  M = i;

  ite_num_list = num_list.begin();
  while ( ite_num_list != num_list.end() ) {
    cout << "(" << ite_num_list->y << ","
	 << ite_num_list->x << ") >> " << ite_num_list->num << endl;
    ite_num_list++;
  }

  cout << "M >> " << M << "\nN_odd >> " << N_odd
       << "\nN_even >> " << N_even << '\n';


  fin.close();
  fout.close();

  return 0;
}
