#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#define arg
using namespace std;

vector<int> initial_perm
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};
vector<int> PC1
{
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};
vector<int> shift_table
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};
vector<int> PC2
{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};
vector<int> E
{
	32, 1, 2, 3, 4, 5, 4, 5,
	6, 7, 8, 9, 8, 9, 10, 11,
	12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21,
	22, 23, 24, 25, 24, 25, 26, 27,
	28, 29, 28, 29, 30, 31, 32, 1
};
vector<int> P
{
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25
};
vector<int> final_perm
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};
vector<int> shift_left1
{
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 1
};
vector<int> shift_left2
{
	3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 1, 2
};

int S_BOX[8][4][16] = {
		{
		{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
		{ 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
		{ 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
		{ 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
	},
	{
		{ 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
		{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
		{ 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
		{ 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
	},
	{
		{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
		{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
		{ 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
		{ 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
	},
	{
		{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
		{ 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
		{ 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
		{ 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
	},
	{
		{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
		{ 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
		{ 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
		{ 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
	},
	{
		{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
		{ 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
		{ 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
		{ 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
	},
	{
		{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
		{ 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
		{ 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
		{ 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
	},
	{
		{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
		{ 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
		{ 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
		{ 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
	}
};

template <size_t N1, size_t N2 >
inline bitset <N1 + N2> bitset_concat(const bitset <N1>& b1, const bitset <N2>& b2) { return bitset <N1 + N2>(b1.to_string() + b2.to_string()); }

bitset<56> divider56(0b00000000000000000000000000001111111111111111111111111111);
bitset<64> divider64(0b0000000000000000000000000000000011111111111111111111111111111111);

template<int inSize,int outSize>
bitset<outSize> permutation(bitset<inSize> plaintext, vector<int> perm) {
	//table is reverse order, so reverse permutation table back
	reverse(perm.begin(), perm.end());
	bitset<outSize> result;
	for (int i = 0; i < outSize; i++) {
		//index in table is base on reverse order, so minus from input size to reverse back
		result[i] = plaintext[inSize - perm[i]];
	}
	return result;
}

bitset<32> F(bitset<32> R, bitset<48> round_key) {
	//expand function 32 bit -> 48 bit
	bitset<48> exR = permutation<32, 48>(R, E);
	//xor with round key
	bitset<48> B = exR ^ round_key;
	bitset<32> result;
	int output_index = 0;
	//loop through 8 S box
	for (int i = 0; i < 8; i++) {
		//base index on 6 bit input
		int base = i * 6;
		//calculate row col in S box
		int row = B[base + 0] + B[base + 5] * 2;
		int col = B[base + 1] + B[base + 2] * 2 + B[base + 3] * 4 + B[base + 4] * 8;
		//get 4 bit output
		bitset<4> out = bitset<4>(S_BOX[7 - i][row][col]);
		//assign to result
		for (int j = 0; j < 4; j++) {
			result[output_index++] = out[j];
		}
	}
	//P function 32 bit -> 32 bit
	result = permutation<32, 32>(result, P);
	return result;
}



#ifdef arg
int main(int argc, char* argv[]) {
	if (argc != 5) {
		cout << "parameter error!\n";
		return 0;
	}
#else
int main() {
	int argc = 5;
	const char* argv[7];
	argv[0] = "";
	argv[1] = "-i"; argv[2] = "0x456";
	argv[3] = "-k"; argv[4] = "0x123";
#endif // arg
	bitset<64> plaintext;
	bitset<64> key64;
	//read parameter to bitset
	for (int i = 1; i < 5; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			plaintext = bitset<64>(strtoull(argv[i + 1], NULL, 16));
		}
		else if (strcmp(argv[i], "-k") == 0) {
			key64 = bitset<64>(strtoull(argv[i + 1], NULL, 16));
		}
	}
	//Initial Permutation  64 bit -> 64 bit
	plaintext = permutation<64, 64>(plaintext, initial_perm);
	//PC1 key 64 bit -> 56 bit
	bitset<56> key56 = permutation<64, 56>(key64, PC1);
	//split plaintext 64 bit -> 2 * 32 bit
	bitset<32> L = bitset<32>(((plaintext >> 32) & divider64).to_ullong());
	bitset<32> R = bitset<32>((plaintext & divider64).to_ullong());
	//split key 56 bit -> 2 * 28 bit
	bitset<28> L_key = bitset<28>(((key56 >> 28) & divider56).to_ullong());
	bitset<28> R_key = bitset<28>((key56 & divider56).to_ullong());
	//16 round
	for (int i = 0; i < 16; i++) {
		//shift L R key
		if (shift_table[i] == 1) {
			L_key = permutation<28, 28>(L_key, shift_left1);
			R_key = permutation<28, 28>(R_key, shift_left1);
		}
		else if (shift_table[i] == 2) {
			L_key = permutation<28, 28>(L_key, shift_left2);
			R_key = permutation<28, 28>(R_key, shift_left2);
		}
		//combine L R key
		bitset<56> combine = bitset_concat(L_key, R_key);
		//PC2 key 56 bit -> 48 bit
		bitset<48> round_key = permutation<56, 48>(combine, PC2);
		//F function and xor with L plaintext
		L = L ^ F(R, round_key);
		//swap L and R
		swap(L, R);
	}
	swap(L, R); //dont swap on last round, so swap back
	//combine L R plaintext and Final Permutation  64 bit -> 64 bit
	bitset<64> ciphertext = permutation<64, 64>(bitset_concat(L, R), final_perm);
	//output
	cout << "0x" << hex << uppercase << ciphertext.to_ullong() << endl;
	return 0;
}