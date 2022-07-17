#include <iostream>
#include <windows.h>  
#include <stdio.h>  
#include <time.h>  
#include <vector>



using namespace std;

//gets sets cursor to {x, y}
void gotoxy(int x, int y) {
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

int main() {

	//setting default parameters...
	string seq1 = "-CAGCAACTCTGA", seq2 = "-CGTAAATGG";
	int PARAM_SAME_SIMB = 3;
	int PARAM_SAME_GRP = 1;
	int PARAM_DIF_GRP = -2;
	int PARAM_DEL = -5;
	char opt;

	//asking for secuences...
	cout << "U Whana introduce own seqs?(Y/N)"; cin >> opt;
	if (opt == 'Y' or opt == 'y') {
		cout << "Insert sequence 1:"; cin >> seq1;
		cout << "Insert sequence 2:"; cin >> seq2;
		cout << endl;
		seq1.insert(0, "-"); seq2.insert(0, "-");
	}

	//asking for parameter changes
	cout << "U Whana modify parameters uwu????(Y/N)"; cin >> opt;
	if (opt == 'Y' or opt == 'y') {
		cout << "Value given equal symbol:"; cin >> PARAM_SAME_SIMB;
		cout << "Value when equal group:"; cin >> PARAM_SAME_GRP;
		cout << "Value different group:"; cin >> PARAM_DIF_GRP;
		cout << "Value of gap penalty:"; cin >> PARAM_DEL;
		cout << endl;
	}
	system("CLS");

	//setting more parameters...
	const int SEQ_SIZE = seq1.size();
	int **matr = new int*[seq1.size()];
	for (int i = 0; i < seq1.size(); i++)
		matr[i] = new int[seq2.size()];
	const int S1_LEN = seq1.size();
	const int S2_LEN = seq2.size();
	const int MAX_LEN = (S1_LEN >= S2_LEN) * S1_LEN + (S1_LEN < S2_LEN) * S2_LEN;
	const int MIN_LEN = (S1_LEN < S2_LEN) * S1_LEN + (S1_LEN >= S2_LEN) * S2_LEN;
	
	//printing secuences...
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	cout << "  ";
	for (int i = 0; i < S1_LEN; i++)
		cout << seq1[i] << "   ";
	cout << endl;
	for (int i = 0; i < S2_LEN; i++)
		cout << seq2[i] << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << endl;
	
	//starting algorithm...
	for (int i = 0; i < MAX_LEN; i++) {
		for (int j = 0; j <= i; j++) {
			
			//stating wether we should each (diagonal'\)half of the matrix has to continue...
			bool stillIn[2] = { (j < MIN_LEN and i < S1_LEN), (j < MIN_LEN and i < S2_LEN) };
			
			//setting '-' rows&columns to 0...
			if (i == 0 or j == 0) {
				if (stillIn[0])
					matr[i][j] = 0;
				if (stillIn[1])
					matr[j][i] = 0;
			}
			else {
				int S[2][4];
				for (int k = 0; k < 4; k++) {

					//calculating each possible option...
					switch (k)
					{
					case 0:
						if(stillIn[0])
							S[0][k] = 0; 
						if (stillIn[1])
						S[1][k] = 0;
						break;
					case 1:{
						if (stillIn[0]) {
							if (seq1[i] == seq2[j])
								S[0][k] = PARAM_SAME_SIMB;
							else if (((seq1[i] == 'T') and (seq2[j] == 'C')) or ((seq1[i] == 'C') and (seq2[j] == 'T')) or 
								     ((seq1[i] == 'A') and (seq2[j] == 'G')) or ((seq1[i] == 'G') and (seq2[j] == 'A'))) {
								S[0][k] = PARAM_SAME_GRP;
							}
							else {
								S[0][k] = PARAM_DIF_GRP;
							}
							S[0][k] += matr[i - 1][j - 1];
						}
						if (stillIn[1]) {
							if (seq1[j] == seq2[i])
								S[1][k] = PARAM_SAME_SIMB;
							else if (((seq1[j] == 'T') and (seq2[i] == 'C')) or ((seq1[j] == 'C') and (seq2[i] == 'T')) or 
							   	     ((seq1[j] == 'A') and (seq2[i] == 'G')) or ((seq1[j] == 'G') and (seq2[i] == 'A'))) {
								S[1][k] = PARAM_SAME_GRP;
							}
							else
								S[1][k] = PARAM_DIF_GRP;
							S[1][k] += matr[j - 1][i - 1];
						}
					}
						  break;
					case 2: {
						if (stillIn[0])
							S[0][k] = matr[i - 1][j] + PARAM_DEL;
						if (stillIn[1])
							S[1][k] = matr[j - 1][i] + PARAM_DEL;
					}
						  break;
					case 3: {
						if (stillIn[0])
							S[0][k] = matr[i][j - 1] + PARAM_DEL;
						if (stillIn[1])
							S[1][k] = matr[j][i - 1] + PARAM_DEL;
					}
						  break;
					}
				}
				
				//selecting maximum value calculated...
				if(stillIn[0])
					matr[i][j] = -999;
				if (stillIn[1])
					matr[j][i] = -999;
				for (int k = 0; k < 4; k++) {
					if (stillIn[1] and S[1][k] > matr[j][i])
						matr[j][i] = S[1][k];
					if (stillIn[0] and S[0][k] > matr[i][j])
						matr[i][j] = S[0][k];
				}
			}

			//printing the calculated value...
			if (stillIn[0]) {
				gotoxy(2 + (4 * i), (1 + j));
				cout << matr[i][j];
			}
			if (stillIn[1]) {
				gotoxy((2 + 4 * j), (1 + i));
				cout << matr[j][i];
			}
			Sleep(10);
		}
	}

	//selecting and re-preinting the maximum value of the matrix
	vector<int> max, pos;
	max.push_back(-999); pos.push_back(0);
	for (int i = 0; i < S1_LEN * S2_LEN; i++) {
		if (matr[(i % S1_LEN)][(int)((i) / S1_LEN)] > max[0])
			max.clear(), pos.clear(),
			pos.push_back(i), max.push_back(matr[i % S1_LEN][(int)((i) / S1_LEN)]);
		if (matr[(i % S1_LEN)][(int)((i) / S1_LEN)] == max[0])
			pos.push_back(i), max.push_back(matr[i % S1_LEN][(int)((i) / S1_LEN)]);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
	for (int i = 0; i < pos.size(); i++)
		gotoxy(2 + (4 * (pos[i] % S1_LEN)), (1 + (int)((pos[i]) / S1_LEN))),
		cout << max[i];
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(0, S2_LEN + 2);

	for (int i = 0; i < seq1.size(); i++) 
		delete matr[i];
	delete[]matr;
	system("pause");

}