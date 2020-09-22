#pragma once
#ifndef AES128_H_
#define AES128_H_

#include <ctime>
#include <fstream>
#include <stdio.h>  // for printf
#include <cstdlib>  // for _countof
#include <errno.h>  // for return values
#include <cstring>  // for wcscpy_s, wcscat_s
#include <stdlib.h> // for _countof
#include <string.h> // for strcpy_s, strcat_s
#include <iostream> // for cout, includes <cstdlib>, <cstring>
#include <iostream>

class aes128
{
public:

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: constructor to encrypt or decrypt a byte's sequence. 
	 * This function add at the byte's sequence an 128 bit header, that rappresent the data_file dimension, also add a final random padding until byte's sequence length % 16 == 0 because AES standard accept only 128bit (16 bytes) datas to input every time
	 * ITA: costruttore per codificare o decodificare una sequenza di byte.
	 * Questa funzione aggiunge alla sequenza di byte un header di 128 bit che rappresenta la dimensione del file, inoltre aggiunge dei bit randomici alla fine affinche' la sequenza di byte totale non sia divisibile per 16 (128 bit) perche' AES accetta in input solo 16 byte alla volta
	 * 
	 * @param data_file ENG: byte's sequence ITA: sequenza di byte
	 * @param length_file ENG: data_file.length ITA: lunghezza di data_file
	 * @param key ENG: a 128 bits (32 hex chars) key ITA: chiave a 128 bit (32 caratteri esadecimali)
	 * @param do_you_want_encrypt ENG: if (var==true) ? encrypt : decrypt ITA: se posta a vero, indica di voler criptare, se falso si vuole decriptare
	 */
	aes128(char * data_file, unsigned long long int length_file, std::string key, bool do_you_want_encrypt);

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief
	 * ENG: destructor - currently does nothing
	 * ITA: distruttore - attualmente non fa nulla
	 */
	~aes128();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief
	 * ENG: to obtain the var encrypt_data
	 * ITA: restituisce la variabile encrypt_data
	 *
	 * @return var encrypt_data
	 */
	char* getEncryptData();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: to obtain the var decrypt_data
	 * ITA: restituisce la variabile decrypt_data
	 *
	 * @return var decrypt_data
	 */
	char* getDecryptData();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: to obtain the var new_length
	 * ITA: restituisce la variabile new_length
	 *
	 * @return var new_length
	 */
	int getNewLength();

private:

	///char* byte's sequence empty before decrypt()
	///char* sequenza di byte vuota prima di decrypt()
	char* decrypt_data;
	
	///char* byte's sequence empty before encrypt()
	///char* sequenza di byte vuota prima di encrypt()
	char* encrypt_data;
	
	///length encrypt_data or decrypt_data after encrypt() or decrypt()
	///dimensione di encrypt_data o decrypt_data dopo encrypt() o decrypt()
	int new_length;

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes an integer, return the first higher number that n mod 16 == 0
	 * @param int n - input
	 * @return int - little number higher than n that n % 16 == 0
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un numero in input restituisce il primo numero più grande divisibile per 16
	 * @param int n - input
	 * @return int - il primo numero più grande divisibile per 16 di n
	 */
	int Mod16Roof(int n);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief from a hex char return the byte of the hex value: charToHex('F') return a char filled with 00001111
	 * @param chat tmp - hex char
	 * @return unsigned char - the byte of the hex value
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief da un carattere esadecimale ritorna il valore in byte dell'esadecimale: charToHex('F') ritorna un byte contenente 00001111
	 * @param chat tmp - carattere esadecimale
	 * @return unsigned char - il byte del valore esadecimale
	 */
	unsigned char charToHex(char tmp);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a chart, it returns the lower 4 bits: Low8Bit(0xAF) = F
	 * @param unsigned char letter - input
	 * @return unsigned short int - lower 4 bits
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un char in input restituisce i 4 bit meno significativi: Low8Bit(0xAF) = F
	 * @param unsigned char letter - input
	 * @return unsigned short int - i 4 bit meno significativi
	 */
	unsigned short int Low8Bit(unsigned char letter);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a chart, it returns the higher 4 bits: Low8Bit(0xAF) = A
	 * @param unsigned char letter - input
	 * @return unsigned short int - higher 4 bits
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un char in input restituisce i 4 bit piu' significativi: Low8Bit(0xAF) = A
	 * @param unsigned char letter - input
	 * @return unsigned short int - i 4 bit piu' significativi
	 */
	unsigned short int High8Bit(unsigned char letter);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a 4x4 unsigned char matrix and copy every value into another matrix
	 * @param unsigned char matrix[4][4] - write matrix
	 * @param unsigned char matrixBKP[4][4] - read matrix
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una matrice 4x4 e copia ogni valore in un'altra matrice
	 * @param unsigned char matrix[4][4] - matrice che verra' sovrascritta
	 * @param unsigned char matrixBKP[4][4] - matrice di lettura
	 */
	void Matrixd4Copy(unsigned char matrix[4][4], unsigned char matrixBKP[4][4]);

	/*
	//iter crypt:
	//passo la dimensione di data arrotondate per mod16 perchè è stato aggiunto padding
	//string s = "dato da crittografare";
	//int dim = Mod16Roof(s.length() + 1);
	//char* data = new char[dim];
	//for (int i = 0; i < dim; data[i++] = 0); //first padding
	//strcpy_s(data, dim, s.c_str()); //strasformo la stringa in un array di char

	//random padding
	srand(time(NULL));
	for (int i = s.length() + 1; i < dim; i++)
	{
		//cout << "rnd: " << rand() << endl;
		data[i] = (rand() % 0xFF);
		if((data[i] * data[i]) != 0)
			if(rand() % (data[i] * data[i]) != 0)
				srand(rand() % (data[i] * data[i]));
			else
				srand(time(NULL));
		else
			srand(time(NULL));
	}

	//crypt(data, dim, KeyA);
	*/
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and encrypt with the 4x4 matrix (key) using AES128 standard
	 * @param char* dataToPrint - byte's sequence
	 * @param int dimension - dataToPrint.length
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo codifica con la matrice 4x4 (key) usando lo standard AES128
	 * @param char* dataToPrint - sequenza di byte
	 * @param int dimension - lunghezza di dataToPrint
	 * @param unsigned char Key[4][4] - chiave di aes128
	 */
	void encrypt(char* dataToPrint, int dimension, unsigned char Key[4][4]);

	/*
	//creare data e s.length
	//decrypt(data, s.length() + 1, KeyA);
	*/
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and decrypt with the 4x4 matrix (key) using AES128 standard
	 * @param char* dataToPrint - byte's sequence
	 * @param int dimension_not_original - dataToPrint.length
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo decodifica con la matrice 4x4 (key) usando lo standard AES128
	 * @param char* dataToPrint - sequenza di byte
	 * @param int dimension_not_original - lunghezza di dataToPrint
	 * @param unsigned char Key[4][4] - chiave di aes128
	 */
	void decrypt(char* dataToPrint, int dimensionOriginal, unsigned char Key[4][4]);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief SubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation with SBox
	 * @param unsigned char SBox[16][16] - transformation matrix
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione SubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la SBox
	 * @param unsigned char SBox[16][16] - matrice di transformazione
	 */
	void SubBytes(unsigned char State[4][4], unsigned char SBox[16][16]);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief ShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione ShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void ShiftRows(unsigned char State[4][4]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief MixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione MixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void MixColumns(unsigned char State[4][4]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief AddRoundKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 * @param unsigned char SBox[16][16] - matrix for expansion key
	 * @param int round - index of reiteration of the algorithm
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione AddRoundKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 * @param unsigned char Key[4][4] - chiave aes 128 bit
	 * @param unsigned char SBox[16][16] - matrice per l'espansione della chiave
	 * @param int round - indice di reiterazione dell'algoritmo
	 */
	void AddRoundKey(unsigned char State[4][4], unsigned char Key[4][4], unsigned char SBox[16][16], int round);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief ExpandKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 * @param unsigned char SBox[16][16] - matrix for expansion key
	 * @param int round - index of reiteration of the algorithm
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione ExpandKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char Key[4][4] - chiave aes 128 bit
	 * @param unsigned char SBox[16][16] - matrice per l'espansione della chiave
	 * @param int round - indice di reiterazione dell'algoritmo
	 */
	void ExpandKey(unsigned char Key[4][4], unsigned char SBox[16][16], int round);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief  function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: it will use: SubBytes, ShiftRows, MixColumns, AddRoundKey, ExpandKey
	 * @param unsigned char State[4][4] - data before encrypt, will be overwrite with encrypted data
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: usera': SubBytes, ShiftRows, MixColumns, AddRoundKey, ExpandKey
	 * @param unsigned char State[4][4] - dati prima della codifica, verranno sovrascritti dai dati codificati
	 * @param unsigned char Key[4][4] - chiave aes 128 bit
	 */
	void AES128(unsigned char State[4][4], unsigned char Key[4][4]);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief InvSubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation with invSbox
	 * @param unsigned char invSbox[16][16] - transformation matrix
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvSubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la invSbox
	 * @param unsigned char invSbox[16][16] - matrice di transformazione
	 */
	void InvSubBytes(unsigned char State[4][4], unsigned char invSbox[16][16]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief InvShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void InvShiftRows(unsigned char State[4][4]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief InvMixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvMixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void InvMixColumns(unsigned char State[4][4]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief InvAddRoundKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 * @param unsigned char Key[4][4] - aes 128 bits key expanded round times
	 * @param int round - index of reiteration of the algorithm
	 * @param unsigned char OriginalKey[4][4] - start aes 128 bits key
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvAddRoundKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 * @param unsigned char Key[4][4] - chiave di 128 bit che e' stata espansa round volte
	 * @param int round - numero di reiterazioni dell'espansione della chiave
	 * @param unsigned char OriginalKey[4][4] - la chiave simmetrica data ad inizio esecuzione
	 */
	void InvAddRoundKey(unsigned char State[4][4], unsigned char Key[4][4], int round, unsigned char OriginalKey[4][4]);
	
	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief InvExpandKey is the reverse of ExpandKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: for the reverse function, round will be transform into 9-round
	 * @param unsigned char Key[4][4] - aes 128 bits key
	 * @param unsigned char SBox[16][16] - matrix for expansion key
	 * @param int round - index of reiteration of the algorithm
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  InvExpandKey e' l'inversa della funzione ExpandKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: per effettuare la funzione inversa, round viene trasformata in 9-round
	 * @param unsigned char Key[4][4] - chiave aes 128 bit
	 * @param unsigned char SBox[16][16] - matrice per l'espansione della chiave
	 * @param int round - indice di reiterazione dell'algoritmo
	 */
	void InvExpandKey(unsigned char Key[4][4], unsigned char OriginalKey[4][4], int round);

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief  function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: it will use: InvSubBytes, InvShiftRows, InvMixColumns, InvAddRoundKey and InvExpandKey
	 * @param unsigned char State[4][4] - data before decrypt, will be overwrite with decrypted data
	 * @param unsigned char Key[4][4] - aes 128 bits key expanded 9 times
	 * @param unsigned char KeyOrig[4][4] - aes 128 bits key original
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: usera': InvSubBytes, InvShiftRows, InvMixColumns, InvAddRoundKey e InvExpandKey
	 * @param unsigned char State[4][4] - dati prima della decodifica, verranno sovrascritti dai dati decodificati
	 * @param unsigned char Key[4][4] - chiave aes 128 bit espansa 9 volte
	 * @param unsigned char KeyOrig[4][4] - chiave aes 128 bit originale
	 */
	void InvAES128(unsigned char State[4][4], unsigned char Key[4][4], unsigned char KeyOrig[4][4]);
};

#endif //AES128_H_
#pragma once