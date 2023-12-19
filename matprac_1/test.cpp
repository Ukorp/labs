/* Для правильной работы матрица в файле должна быть записана как:

M N

a11 a12 a13 ... a1n
a21 a22 a23 ... a2n
...................
am1 am2 am3 ... amn

Где M - число строк, N - число столбцов, (a11, ... , amn) - данные матрицы
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void print_matrix(vector<vector<int>> &arr, int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
            cout << arr[i][j] << " ";
        cout << endl;
    }
}

int main()
{  
    int num, cols, rows, ans;
    string file;
    ans = 0;
    
    cout << "Название файла: ";
    cin >> file;
    ifstream in(file);
    if (!in.is_open()){
        cout << "Ошибка открытия файла" << endl;
        return 1;
    }
    in >> rows >> cols;
    vector<vector <int>> array(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
            in >> array[i][j];
    }
    print_matrix(array, rows, cols);
    
    cout << endl;

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if ((i - j) > 0)
                ans += array[i][j];
        }
    }

    cout << "Сумма чисел ниже главной диагонали: " << ans << endl;

}
