#include <NTL/ZZ.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
NTL_CLIENT

long  tol;
ZZ ans, factor[100000];
void findfac(ZZ);
void sha1();
void init_R();
ZZ create_Message();
ZZ f1(ZZ, ZZ, ZZ);
ZZ f2(ZZ, ZZ, ZZ);
ZZ f3(ZZ, ZZ, ZZ);
ZZ f4(ZZ, ZZ, ZZ);

//消息
ZZ M = to_ZZ("0");
ZZ Array_M[16];
ZZ X[80];
ZZ A, B, C, D, E, AA, BB, CC, DD, EE, K1, K2, K3, K4;

int main()
{

	ZZ p, q, t;
	//确定p和q
	while (1) {
		p = GenPrime_ZZ(80);
		cout << "p= " << p << endl;

		tol = 0;
		findfac(p - 1);//对n分解质因子
		int i;
		for (i = 0; i<tol; i++)
		{
			cout << "factor" << i + 1 << ":" << factor[i] << endl;
			if (factor[i]>power2_ZZ(29) && factor[i] < power2_ZZ(30)) {
				cout << "we find it!" << endl;
				q = factor[i];
				cout << "q = " << q << endl;
				break;
			}
		}
		if (i < tol) break;
	}

	//寻找h和g
	ZZ h = RandomBnd(p - 1) + 1;
	ZZ g = PowerMod(h, (p - 1) / q, p);
	while (g <= 1) {
		h = RandomBnd(p - 1) + 1;
		g = PowerMod(h, (p - 1) / q, p);
	}

	//寻找x和y
	ZZ x = RandomBnd(q + 1);
	ZZ y = PowerMod(g, x, p);


	sha1();
	//	cout << M << endl;
	//签名
	ZZ k = RandomBnd(q + 1);
	ZZ r = AddMod(PowerMod(g, k, p), 0, q);
	cout << "***************  r  ***********" << endl;
	cout << r << endl;
	ZZ inverse_k = InvMod(k, q);//逆元
	ZZ hash_M = (A << (32 * 4)) + (B << (32 * 3)) + (C << (32 * 2)) + (D << (32 * 1)) + (E);
	ZZ s = MulMod(inverse_k, (hash_M + x*r), q);
	cout << "*****************  s  *************" << endl;
	cout << s << endl;

	system("pause");
	return 0;
}

bool Miller_Rabin(ZZ n)
{
	if (n<2)return false;
	if (n == 2 || n == 3 || n == 5 || n == 7)return true;
	if (n == 1 || (n % 2 == 0) || (n % 3 == 0) || (n % 5 == 0) || (n % 7 == 0)) return false;//偶数
	for (int i = 0; i<10; i++)
	{
		ZZ wit = RandomBnd(n - 1) + 1;
		if (MillerWitness(n, wit) == 1) {
			return false;
		}

	}
	return true;
}

ZZ Pollard_rho(ZZ x, ZZ c)
{
	ZZ i = to_ZZ("1");
	ZZ k = to_ZZ("2");
	//ZZ x0 = rand() % x;
	ZZ x0 = RandomBnd(x);
	ZZ y = x0;
	while (1)
	{
		i++;
		x0 = AddMod(MulMod(x0, x0, x), c, x);
		ZZ d = GCD(y - x0, x);
		if (d != 1 && d != x) return d;
		if (y == x0) return x;
		if (i == k)
		{
			y = x0;
			k += k;
		}
	}
}

void findfac(ZZ n)
{
	if (Miller_Rabin(n))//素数
	{
		factor[tol++] = n;
		return;
	}
	ZZ p = n;
	while (p >= n) {

		//		cout << "to pollar-----------------------" << endl;
		p = Pollard_rho(p, RandomBnd(n - 1) + 1);
	}
	findfac(p);//递归调用
	findfac(n / p);
}

void sha1() {
	init_R();
	//补全消息
	create_Message();

	for (int i = 0; i <= 0; i++) {

		int j;
		for (j = 0; j <= 15; j++)
			X[j] = Array_M[16 * i + j];
		for (j = 16; j <= 79; j++)
			X[j] = (X[j - 3] ^ X[j - 8] ^ X[j - 14] ^ X[j - 16]) << 1;
		AA = A;
		BB = B;
		CC = C;
		DD = D;
		EE = E;

		//ROUND1
		int k;
		ZZ temp_z;
		for (k = 0; k <= 19; k++) {
			temp_z = (A << 5) + f1(B, C, D) + E + X[k] + K1;
			E = D;
			D = C;
			C = (B << 30);
			B = A;
			A = temp_z;
		}

		//round2
		for (k = 20; k <= 39; k++) {
			temp_z = (A << 5) + f2(B, C, D) + E + X[k] + K2;
			E = D;
			D = C;
			C = (B << 30);
			B = A;
			A = temp_z;
		}

		//round3
		for (k = 40; k <= 59; k++) {
			temp_z = (A << 5) + f3(B, C, D) + E + X[k] + K3;
			E = D;
			D = C;
			C = (B << 30);
			B = A;
			A = temp_z;
		}

		//round4
		for (k = 60; k <= 79; k++) {
			temp_z = (A << 5) + f4(B, C, D) + E + X[k] + K4;
			E = D;
			D = C;
			C = (B << 30);
			B = A;
			A = temp_z;

		}

		A += AA;
		B += BB;
		C += CC;
		D += DD;
		E += EE;

	}
	cout << "**************  hash   digest  *************" << endl;
	cout << hex << A << " " << B << " " << C << " " << D << " " << E << endl;
}
void init_R() {
	unsigned long t;
	//t=
	t = 6 * pow(16, 7) + 7 * pow(16, 6) + 4 * pow(16, 5) + 5 * pow(16, 4) + 2 * pow(16, 3) + 3 * pow(16, 2) + 0 * pow(16, 1) + 1 * pow(16, 0);
	A = t;

	t = 14 * pow(16, 7) + 15 * pow(16, 6) + 12 * pow(16, 5) + 13 * pow(16, 4) + 10 * pow(16, 3) + 11 * pow(16, 2) + 8 * pow(16, 1) + 9 * pow(16, 0);
	B = t;

	t = 9 * pow(16, 7) + 8 * pow(16, 6) + 11 * pow(16, 5) + 10 * pow(16, 4) + 13 * pow(16, 3) + 12 * pow(16, 2) + 15 * pow(16, 1) + 14 * pow(16, 0);
	C = t;

	t = 1 * pow(16, 7) + 0 * pow(16, 6) + 3 * pow(16, 5) + 2 * pow(16, 4) + 5 * pow(16, 3) + 4 * pow(16, 2) + 7 * pow(16, 1) + 6 * pow(16, 0);
	D = t;

	t = 12 * pow(16, 7) + 3 * pow(16, 6) + 13 * pow(16, 5) + 2 * pow(16, 4) + 14 * pow(16, 3) + 1 * pow(16, 2) + 15 * pow(16, 1) + 0 * pow(16, 0);
	E = t;

	t = 5 * pow(16, 7) + 10 * pow(16, 6) + 8 * pow(16, 5) + 2 * pow(16, 4) + 7 * pow(16, 3) + 9 * pow(16, 2) + 9 * pow(16, 1) + 9 * pow(16, 0);
	K1 = t;

	t = 6 * pow(16, 7) + 14 * pow(16, 6) + 13 * pow(16, 5) + 9 * pow(16, 4) + 14 * pow(16, 3) + 11 * pow(16, 2) + 10 * pow(16, 1) + 1 * pow(16, 0);
	K2 = t;

	t = 8 * pow(16, 7) + 15 * pow(16, 6) + 1 * pow(16, 5) + 11 * pow(16, 4) + 11 * pow(16, 3) + 12 * pow(16, 2) + 13 * pow(16, 1) + 12 * pow(16, 0);
	K3 = t;

	t = 12 * pow(16, 7) + 10 * pow(16, 6) + 6 * pow(16, 5) + 2 * pow(16, 4) + 12 * pow(16, 3) + 1 * pow(16, 2) + 13 * pow(16, 1) + 6 * pow(16, 0);
	A = t;
}
ZZ create_Message() {
	char msg[20];
	int len;
	//获得输入,每个字符两个字节
	cout << "please input Message" << endl;
	cin.get(msg, 20);

	//构建M
	int i;
	for (i = 0; msg[i] != '\0'; i++) {
		M += (unsigned)msg[i];
		M <<= 8;
	}
	M >>= 8;
	M <<= 1;
	M += 1;
	M <<= (512 - i * 8 - 1);
	M += i * 8;

	//形成数组
	cout << "****************** Message *************" << endl;
	cout << M << endl;
	ZZ max_32_bit = to_ZZ("1");
	max_32_bit = (max_32_bit << 32) - 1;
	//转为二进制
	for (int j = 0; j < 16; j++) {
		Array_M[15 - j] = (M & (max_32_bit << (32 * j))) >> (32 * j);
	}
	return M;
}

ZZ f1(ZZ x, ZZ y, ZZ z) {
	return (x&y) | ((-x - 1) & z);
}

ZZ f2(ZZ x, ZZ y, ZZ z) {
	return x^y^z;
}

ZZ f3(ZZ x, ZZ y, ZZ z) {
	return (x&y) | (x&z) | (y&z);
}
ZZ f4(ZZ x, ZZ y, ZZ z) {
	return x^y^z;
}