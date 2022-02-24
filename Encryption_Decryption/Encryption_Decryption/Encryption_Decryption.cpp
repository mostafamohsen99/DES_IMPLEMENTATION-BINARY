// Encryption_Decryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef __GNUC__
#define __rdtsc  __builtin_ia32_rdtsc
#else
#include<intrin.h>
#endif
#include <iostream>
#include<stdio.h>
#include<string>


#define F    4294967295 
#define F7   268435455
#define F77    72057593769492480
#define F0  18446744069414584320

#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(28-num)) )
using namespace std;

typedef unsigned long long u64;
typedef unsigned char byte;


u64 permutation_res(u64 permutation,short size_in,short size_out,bool a)
{
	int initial_permutation[64] = {
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7
	};
	int permutation_1[56] = {
57,49,41,33,25,17,9,
1,58,50,42,34,26,18,
10,2,59,51,43,35,27,
19,11,3,60,52,44,36,
63,55,47,39,31,23,15,
7,62,54,46,38,30,22,
14,6,61,53,45,37,29,
21,13,5,28,20,12,4
	};
	int permutation_2[48] = {
14,17,11,24,1,5,
3,28,15,6,21,10,
23,19,12,4,26,8,
16,7,27,20,13,2,
41,52,31,37,47,55,
30,40,51,45,33,48,
44,49,39,56,34,53,
46,42,50,36,29,32
	};
	int expansion_table[48] = {
32,1,2,3,4,5,4,5,
6,7,8,9,8,9,10,11,
12,13,12,13,14,15,16,17,
16,17,18,19,20,21,20,21,
22,23,24,25,24,25,26,27,
28,29,28,29,30,31,32,1
	};
	int permutation_tab[32] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
	};
	int inverse_permutation[64] = {
40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25
	};

	u64 detector, result=0;
	for (int i = 0; i < size_out; i++)
	{
		if(size_in==64&&size_out==56)
		   detector = permutation & (1ULL << size_in - permutation_1[i]);
		else if(size_in==56)
			detector = permutation & (1ULL << size_in - permutation_2[i]);
		else if (size_in == 64 && size_out == 64)
		{
			if(a==0)
			  detector = permutation & (1ULL << size_in - initial_permutation[i]);
			else 
				detector = permutation & (1ULL << size_in - inverse_permutation[i]);
		}
		else if (size_in == 32 && size_out == 48)
			detector = permutation & (1ULL << size_in - expansion_table[i]);
		else if(size_in==32&&size_out==32)
			detector = permutation & (1ULL << size_in - permutation_tab[i]);
		if (detector != 0)
		{
			result |= (1ULL << (size_out-1 - i));
			detector = 0;
		}
	}
	return result;
}

unsigned int S_box_Calc(u64 number)
{
	unsigned int num_32 = 0;
	unsigned int shifter = 0;
	int S_BOX_TABLE[8][64] = {

	{
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	}};
	for (int i = 0; i < 8; i++)
	{
		shifter =(number>> ((7 - i) * 6))& 63;
		shifter = shifter >> 1 & 15 | (shifter & 1) << 4 | shifter & 32;
		num_32 |= S_BOX_TABLE[i][shifter] << (7 - i) * 4;
	}
	return num_32;
}

u64 encrypt_or_decrypt(u64 plain_or_cipher,u64 key,bool op)
{
	u64 permutation_1_result, permutation_2_result[16] = { 0 }, initial_permutation_result, expansion_result, tab_result, final_round = 0, final_result = 0, key_comb = 0, xor_op;
	unsigned int left_new=0, x_box_op, right_new=0;
	bool a = 0;
	initial_permutation_result = permutation_res(plain_or_cipher, 64, 64,a);
	unsigned int right = (initial_permutation_result & (u64)F);
	unsigned int  left = initial_permutation_result  >> 32;
    permutation_1_result = permutation_res(key,64,56,a);
	unsigned int right_key = (permutation_1_result &(u64)F7);
	unsigned int left_key = (permutation_1_result &(u64)F77 ) >> 28;
	for (int i = 0; i < 16; i++)
	{
		if (i == 0 || i == 1 || i == 8 || i == 15)
		{
			ROL(left_key, 1);
			left_key &= F7;
			ROL(right_key, 1);
			right_key &= F7;
		}
		else
		{
			ROL(left_key, 2);
			left_key &= F7;
			ROL(right_key, 2);
			right_key &= F7;
		}
		key_comb = left_key;
		key_comb <<= 28;
		key_comb |= right_key;
		permutation_2_result[i] = permutation_res(key_comb, 56, 48, a);
	}
	long long t1 = __rdtsc();
	for (int i = 0; i < 16; i++)
	{
		if (i != 0)
		{
			left = left_new;
			right = right_new;
		}
		expansion_result= permutation_res(right, 32, 48,a);
		if(op==1)
		 xor_op = expansion_result ^ permutation_2_result[i];
		else if(op==0)
			 xor_op = expansion_result ^ permutation_2_result[15-i];
		x_box_op = S_box_Calc(xor_op);
		//cout << "expansion_result=" << expansion_result << endl;
		tab_result = permutation_res(x_box_op, 32, 32,a);
		right_new = tab_result ^ left;
		left_new = right;
	}
	final_round = right_new;
	final_round <<= 32;
	final_round |= left_new;
	a = 1;
	final_result = permutation_res(final_round, 64, 64, a);
	long long t2 = __rdtsc();
	printf("cycles=%lld \n", t2 - t1);
	return final_result;
}

u64 string_to_long(string a)
{
	u64  result = 0;
	for (int i = 0;i<a.length(); i++)
	{
		byte c= a[i];
		int temp = c - '0';
		if (!c||!((c>='0'&&c<='9')||(c>='A'&&c<='F')||(c>='a'&&c<='f')))
			break;
		result <<= 4;
		if (temp < 10)
			result |= c - '0';
		else 
			result |= (c & 0xDF)-'A'+10;
	}
	return result;
}
int main(int argc,char **argv)
{
	string key="", plain_or_cipher="", operation="";
	u64 key_long, plain_or_cipher_long, operation_long, plain_or_cipher_result;
	bool op;
	//operation = "encrypt";
	//plain_or_cipher = "0123456789ABCDEF";
	operation = argv[1];
	plain_or_cipher =argv[2];
    key=argv[3];
	//key = "133457799BBCDFF1";
	if (operation == "encrypt")
		op = 1;
	else if (operation == "decrypt")
		op = 0;
    plain_or_cipher_long=string_to_long(plain_or_cipher);
	key_long = string_to_long(key);
	plain_or_cipher_result = encrypt_or_decrypt(plain_or_cipher_long,key_long,op);
	if(op==1)
	printf("cipher:%016llX\n", plain_or_cipher_result);
	else if(op==0)
		printf("plain:%016llX\n", plain_or_cipher_result);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
