#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

typedef struct NumBox {
  int x;
  int y;
  int num;
}NumBox;

NumBox *numbox_array;

int main()
{
  int i;
  char ch = 'A';

  cout << ++ch << endl;
  cout << endl;

  numbox_array = new NumBox[6];
  for( i=0; i<5; i++ ) {
    numbox_array[i].x = i;
    numbox_array[i].y = i;
    numbox_array[i].num = i;

  }
  for( i=0; i<5; i++ ) {
    cout << "[" << i << "] : " <<  numbox_array[i].x << ", "
	 <<  numbox_array[i].y << ", " <<  numbox_array[i].num << "\n";
  }
  delete(numbox_array);
  return 0;
}
