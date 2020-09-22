#pragma once
#ifndef AES256_H_
#define AES256_H_

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

class aes256
{
public:

	// Each 256bit key is split in two 128 bits sub-keys
	// Ogni chiave a 256 bit viene divisa in due sotto chiavi
	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: constructor to encrypt or decrypt a byte's sequence
	 * This function add at the byte's sequence an 128 bit header, that rappresent the data_file dimension, also add a final random padding until byte's sequence length % 16 == 0 because AES standard accept only 128bit (16 bytes) datas to input every time
	 * ITA: costruttore per codificare o decodificare una sequenza di byte
	 * Questa funzione aggiunge alla sequenza di byte un header di 128 bit che rappresenta la dimensione del file, inoltre aggiunge dei bit randomici alla fine affinche' la sequenza di byte totale non sia divisibile per 16 (128 bit) perche' AES accetta in input solo 16 byte alla volta
	 *
	 * @param data_file ENG: byte's sequence ITA: sequenza di byte
	 * @param length_file ENG: data_file.length ITA: lunghezza di data_file
	 * @param key ENG: a 256 bits (64 hex chars) key ITA: chiave a 256 bit (64 caratteri esadecimali)
	 * @param do_you_want_encrypt ENG: if (var==true) ? encrypt : decrypt ITA: se posta a vero, indica di voler criptare, se falso si vuole decriptare
	 */
	aes256(char* data_file, int length_file, std::string key, bool do_you_want_encrypt);

	/**
	 * @author Peruffo Riccardo
	 * @brief 
	 * ENG: destructor - currently does nothing
	 * ITA: distruttore - attualmente non fa nulla
	 */
	~aes256();

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

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes an integer, return the first higher number that n mod 16 == 0
	 * @param int n - input
	 * @return int - little number higher than n that n % 16 == 0
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un numero in input restituisce il primo numero più grande divisibile per 16
	 * @param int n - input
	 * @return int - il primo numero più grande divisibile per 16 di n
	 */
	int Mod16Roof(int n);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief from a hex char return the byte of the hex value: charToHex('F') return a char filled with 00001111
	 * @param chat tmp - hex char
	 * @return unsigned char - the byte of the hex value
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief da un carattere esadecimale ritorna il valore in byte dell'esadecimale: charToHex('F') ritorna un byte contenente 00001111
	 * @param chat tmp - carattere esadecimale
	 * @return unsigned char - il byte del valore esadecimale
	 */
	unsigned char charToHex(char tmp);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a chart, it returns the lower 4 bits: Low8Bit(0xAF) = F
	 * @param unsigned char letter - input
	 * @return unsigned short int - lower 4 bits
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un char in input restituisce i 4 bit meno significativi: Low8Bit(0xAF) = F
	 * @param unsigned char letter - input
	 * @return unsigned short int - i 4 bit meno significativi
	 */
	unsigned short int Low8Bit(unsigned char letter);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a chart, it returns the higher 4 bits: Low8Bit(0xAF) = A
	 * @param unsigned char letter - input
	 * @return unsigned short int - higher 4 bits
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief preso un char in input restituisce i 4 bit piu' significativi: Low8Bit(0xAF) = A
	 * @param unsigned char letter - input
	 * @return unsigned short int - i 4 bit piu' significativi
	 */
	unsigned short int High8Bit(unsigned char letter);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a 4x4 unsigned char matrix and copy every value into another matrix
	 * @param unsigned char matrix[4][4] - write matrix
	 * @param unsigned char matrixBKP[4][4] - read matrix
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una matrice 4x4 e copia ogni valore in un'altra matrice
	 * @param unsigned char matrix[4][4] - matrice che verra' sovrascritta
	 * @param unsigned char matrixBKP[4][4] - matrice di lettura
	 */
	void Matrixd4Copy(unsigned char matrix[4][4], unsigned char matrixBKP[4][4]);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief auxiliar function: for i=0..3 ( for j=0..3 (State[i][j] ^= Key[i][j]))
	 * @param unsigned char State[4][4] - aes data before transformation 
	 * @param unsigned char Key[4][4] - aes 128 bits sub-key
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief funzione ausiliare: for i=0..3 ( for j=0..3 (State[i][j] ^= Key[i][j]))
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 * @param unsigned char Key[4][4] - prima o seconda mezza chiave della chiave a 256 bit
	 */
	void XORmul(unsigned char State[4][4], unsigned char Key[4][4]);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and encrypt with the 4x4 matrix (KeyA and KeyB) using AES256 standard
	 * @param char* dataToPrint - byte's sequence
	 * @param int dimension - dataToPrint.length
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo codifica con la matrice 4x4 (KeyA e KeyB) usando lo standard AES256
	 * @param char* dataToPrint - sequenza di byte
	 * @param int dimension - lunghezza di dataToPrint
	 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
	 */
	void crypt(char* dataToPrint, int dimension, unsigned char KeyA[4][4], unsigned char KeyB[4][4]);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and decrypt with the 4x4 matrix (KeyA and KeyB) using AES256 standard
	 * @param char* dataToPrint - byte's sequence
	 * @param int dimension_not_original - dataToPrint.length
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo decodifica con la matrice 4x4 (KeyA e KeyB) usando lo standard AES256
	 * @param char* dataToPrint - sequenza di byte
	 * @param int dimension_not_original - lunghezza di dataToPrint
	 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
	 */
	void decrypt(char* dataToPrint, int dimensionOriginal, unsigned char KeyA[4][4], unsigned char KeyB[4][4]);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief SubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation with SBox
	 * @param unsigned char SBox[16][16] - transformation matrix
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione SubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la SBox
	 * @param unsigned char SBox[16][16] - matrice di transformazione
	 */
	void SubBytes(unsigned char State[4][4], unsigned char SBox[16][16]);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief ShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione ShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void ShiftRows(unsigned char State[4][4]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief MixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione MixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void MixColumns(unsigned char State[4][4]);
	
	
	/// The function AddRoundKey it was split in two subfunction: XORmul and ExpandKey
	/// In this version AddRoundKey it doesn't used, but XORmul and ExpandKey implement the same functionality
	/// AddRoundKey is a function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	
	/// La funzione AddRoundKey e' stata divisa in due sottofunzioni: XORmul ed ExpandKey
	/// In questa versione AddRoundKey non e' usata, ma XORmul ed ExpandKey implementano le stesse funzionalita'
	/// AddRoundKey e' una funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf

	
	/**
	 * @author Peruffo Riccardo
	 * @brief ExpandKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 * @param unsigned char SBox[16][16] - matrix for expansion key
	 * @param int round - index of reiteration of the algorithm
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione ExpandKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
	 * @param unsigned char SBox[16][16] - matrice per l'espansione della chiave
	 * @param int round - indice di reiterazione dell'algoritmo
	 */
	void ExpandKey(unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char SBox[16][16], int round);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief  function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: it will use: SubBytes, ShiftRows, MixColumns, XORmul, ExpandKey
	 * @param unsigned char State[4][4] - data before encrypt, will be overwrite with encrypted data
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: usera': SubBytes, ShiftRows, MixColumns, XORmul, ExpandKey
	 * @param unsigned char State[4][4] - dati prima della codifica, verranno sovrascritti dai dati codificati
	 * @param unsigned char KeyA[4][4] - prima chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda chiave di aes256
	 */
	void AES256(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvSubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation with invSbox
	 * @param unsigned char invSbox[16][16] - transformation matrix
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvSubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la invSbox
	 * @param unsigned char invSbox[16][16] - matrice di transformazione
	 */
	void InvSubBytes(unsigned char State[4][4], unsigned char invSbox[16][16]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void InvShiftRows(unsigned char State[4][4]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvMixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvMixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 */
	void InvMixColumns(unsigned char State[4][4]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvAddRoundKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - aes data before transformation 
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 * @param int round - index of reiteration of the algorithm
	 * @param unsigned char OriginalKeyA[4][4] - first aes256 bits sub-key no expanded
	 * @param unsigned char OriginalKeyB[4][4] - start aes256 bits sub-key no expanded
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione InvAddRoundKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * @param unsigned char State[4][4] - dati di aes prima della transformazione
	 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
	 * @param int round - numero di reiterazioni dell'espansione della chiave
	 * @param unsigned char OriginalKeyA[4][4] - prima sotto chiave di aes256 data ad inizio esecuzione
	 * @param unsigned char OriginalKeyB[4][4] - seconda sotto chiave di aes256 data ad inizio esecuzione
	 */
	void InvAddRoundKey(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4], int round, unsigned char OriginalKeyA[4][4], unsigned char OriginalKeyB[4][4]);
	
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvExpandKey is the reverse of ExpandKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: for the reverse function, round will be transform into 9-round
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
	 * @param int round - index of reiteration of the algorithm
	 * @param unsigned char OriginalKeyA[4][4] - first aes256 bits sub-key no expanded
	 * @param unsigned char OriginalKeyB[4][4] - start aes256 bits sub-key no expanded
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief InvExpandKey e' l'inversa della funzione ExpandKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: per effettuare la funzione inversa, round viene trasformata in 9-round
	 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
	 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
	 * @param unsigned char OriginalKeyA[4][4] - prima sotto chiave di aes256 data ad inizio esecuzione
	 * @param unsigned char OriginalKeyB[4][4] - seconda sotto chiave di aes256 data ad inizio esecuzione
	 * @param int round - indice di reiterazione dell'algoritmo
	 */
	void InvExpandKey(unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char OriginalKeyA[4][4], unsigned char OriginalKeyB[4][4], int round);

	
	/**
	 * @author Peruffo Riccardo
	 * @brief  function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: it will use: InvSubBytes, InvShiftRows, InvMixColumns, InvAddRoundKey and InvExpandKey
	 * @param unsigned char State[4][4] - data before decrypt, will be overwrite with decrypted data
	 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key expanded
	 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key expanded
	 * @param unsigned char KeyOrigA[4][4] - first aes256 bits sub-key original
	 * @param unsigned char KeyOrigB[4][4] - second aes256 bits sub-key original
	 */
	
	/**
	 * @author Peruffo Riccardo
	 * @brief  funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
	 * PS: usera': InvSubBytes, InvShiftRows, InvMixColumns, InvAddRoundKey e InvExpandKey
	 * @param unsigned char State[4][4] - dati prima della decodifica, verranno sovrascritti dai dati decodificati
	 * @param unsigned char KeyA[4][4] - prima semi chiave aes256 bit espansa
	 * @param unsigned char KeyB[4][4] - seconda semi chiave aes256 bit espansa
	 * @param unsigned char KeyOrigA[4][4] - prima semi chiave aes256 bit originale
	 * @param unsigned char KeyOrigB[4][4] - seconda semi chiave aes256 bit originale
	 */
	void InvAES256(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char KeyOrigA[4][4], unsigned char KeyOrigB[4][4]);
};

#endif //AES256_H_
#pragma once