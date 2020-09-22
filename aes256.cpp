#include <ctime>
#include "aes256.h"
#include <fstream>
#include <stdio.h>  // for printf
#include <cstdlib>  // for _countof
#include <errno.h>  // for return values
#include <cstring>  // for wcscpy_s, wcscat_s
#include <stdlib.h> // for _countof
#include <string.h> // for strcpy_s, strcat_s
#include <iostream> // for cout, includes <cstdlib>, <cstring>
#define bytes_for_128bits 16
#define bits_for_byte 8


// Each 256bit key is split in two 128 bits sub-key
// Ogni chiave a 256 bit viene divisa in due sotto chiavi da 128 bit l'una
/*
 * @author Peruffo Riccardo
 * @brief ENG DOC: constructor to encrypt or decrypt a byte's sequence
 * This function add at the byte's sequence an 128 bit header, that rappresent the data_file dimension, also add a final random padding until byte's sequence length % 16 == 0 because AES standard accept only 128bit (16 bytes) datas to input every time
 * @param char* data_file - byte's sequence
 * @param unsigned long long int length_file - data_file.length
 * @param std::string key - a 256 bits (64 hex chars) key
 * @param bool do_you_want_encrypt - if (var==true) ? encrypt : decrypt
 *
 * @brief ITA DOC: costruttore per codificare o decodificare una sequenza di byte
 * Questa funzione aggiunge alla sequenza di byte un header di 128 bit che rappresenta la dimensione del file, inoltre aggiunge dei bit randomici alla fine affinche' la sequenza di byte totale non sia divisibile per 16 (128 bit) perche' AES accetta in input solo 16 byte alla volta
 * @param char* data_file - sequenza di byte
 * @param unsigned long long int length_file - lunghezza di data_file
 * @param std::string key - chiave a 256 bit (64 caratteri esadecimali)
 * @param bool do_you_want_encrypt - se posta a vero, indica di voler criptare, se falso si vuole decriptare
 */
aes256::aes256(char* data_file, int length_file, std::string key, bool do_you_want_encrypt)
{
	//preparo la chiave A
	unsigned char KeyA[4][4];
	unsigned char KeyB[4][4];

	//counter 
	int idx, jdx, kdx;
	//variabile char temporaneo
	unsigned char tmp = 0;
	//per ogni riga, fino a 4
	for (idx = 0, kdx = 0; idx < 4; idx++)
	{
		//faccio ogni colonna, fino a 4
		for (jdx = 0; jdx < 4; jdx++)
		{
			//pulisco la cella
			KeyA[jdx][idx] = 0;

			//salvo il char della chiave e lo converto in hex, anche se lo stipo in un char
			tmp = charToHex(key[kdx]);
			//metto il carattere nella cella
			KeyA[jdx][idx] |= tmp;

			//passo alla lettera successiva della stringa chiave
			kdx++;
			//preparo lo spazio per il prossimo hex all'interno della cella
			KeyA[jdx][idx] <<= 4;
			//salvo il char della chiave convertita in hex
			tmp = charToHex(key[kdx]);
			//faccio un OR dentro il restante della cella
			KeyA[jdx][idx] |= tmp;
			//passo alla lettera successiva della stringa chiave
			kdx++;
		}
	}
	//preparo la chiave B
	//per ogni riga, fino a 4
	for (idx = 0/*, kdx = 0*/; idx < 4; idx++)
	{
		//faccio ogni colonna, fino a 4
		for (jdx = 0; jdx < 4; jdx++)
		{
			//pulisco la cella
			KeyB[jdx][idx] = 0;

			//salvo il char della chiave e lo converto in hex, anche se lo stipo in un char
			tmp = charToHex(key[kdx]);
			//metto il carattere nella cella
			KeyB[jdx][idx] |= tmp;

			//passo alla lettera successiva della stringa chiave
			kdx++;
			//preparo lo spazio per il prossimo hex all'interno della cella
			KeyB[jdx][idx] <<= 4;
			//salvo il char della chiave convertita in hex
			tmp = charToHex(key[kdx]);
			//faccio un OR dentro il restante della cella
			KeyB[jdx][idx] |= tmp;
			//passo alla lettera successiva della stringa chiave
			kdx++;
		}
	}
	if (do_you_want_encrypt)
	{
		/*
		//iter crypt:
		//passo la dimensione di data arrotondate per mod16 perche' e' stato aggiunto padding
		//string s = "dato da crittografare";
		//int dim = Mod16Roof(s.length() + 1); //il +1 e' per il carattere speciale '\0' credo
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
		//preparo i dati
		int dim = 16 + Mod16Roof(length_file); //struct head + lenght_file + padding

		//create struct
		char* data = new char[dim];
		for (int i = 0; i < dim; data[i] ^= data[i], i++);

		//push head
		//int bytes_for_128bits = 16;
		//int bits_for_byte = 8;
		unsigned long long int length_file_divided_128 = length_file / bytes_for_128bits; //length_file e' in byte e non in bit
		unsigned long long int length_file_mod_128 = length_file % bytes_for_128bits;

		int i, j;

		//length_file_divided_128 = 0xf0dcba987654321e; //test value
		//length_file_mod_128 = 0xfedcb0987654321a; //test value
		unsigned long long int tmp_alpha;
		unsigned long long int and_value;
		unsigned char tmp_omega = 0;

		//start to 0, first 64 bits (8 bytes) are dedicated for the cardinality of 128 bits' input rows - 1
		for (i = 0, tmp_alpha = length_file_divided_128, and_value = 0xFF00000000000000; i < 8; i++)
		{
			tmp_omega ^= tmp_omega;
			//prendo length_file, ne estraggo il byte piu' significativo //(tmp_alpha & and_value)
			//prendo il byte piu' significativo, lo sparo sui primi 8 bit //>> ((7 - i) * bits_for_byte)
			//essendo tmp_alpha di 8 byte, ed avendo bisogno di ogni volta del byte piu' grande
			//7 quindi rappresenta il numero di byte da togliere
			//- i il byte che ci serve a questa iterazione
			//bits_for_byte = 8, cioe' la vera dimensione del byte
			tmp_omega |= (unsigned char)((tmp_alpha & and_value) >> ((7 - i) * bits_for_byte));

			data[i] = tmp_omega;

			and_value >>= bits_for_byte; //abbasso i bit di and di 1 byte
		}

		for (j = 0, tmp_alpha = length_file_mod_128, and_value = 0xFF00000000000000; i < 16; i++, j++)
		{
			tmp_omega ^= tmp_omega;
			//prendo length_file, ne estraggo il byte piu' significativo //(tmp_alpha & and_value)
			//prendo il byte piu' significativo, lo sparo sui primi 8 bit //>> ((7 - i) * bits_for_byte)
			//essendo tmp_alpha di 8 byte, ed avendo bisogno di ogni volta del byte piu' grande
			//7 quindi rappresenta il numero di byte da togliere
			//- i il byte che ci serve a questa iterazione
			//bits_for_byte = 8, cioe' la vera dimensione del byte
			//sostituisco i con j perche' j riparte da 0
			tmp_omega |= (unsigned char)((tmp_alpha & and_value) >> ((7 - j) * bits_for_byte));

			data[i] = tmp_omega;

			and_value >>= bits_for_byte; //abbasso i bit di and di 1 byte
		}

		/*
		std::cout << "length payload: " << length_file << std::endl;
		std::cout << "Mod16Roof(length payload): " << Mod16Roof(length_file) << std::endl;
		std::cout << "dim: " << dim << std::endl;
		std::cout << "length_file_divided_128: " << length_file_divided_128 << std::endl;
		std::cout << "length_file_mod_128: " << length_file_mod_128 << std::endl;

		std::cout << std::hex << "PAYLOAD 1:\t";
		for (int n = 0; n < length_file; n++)
		{
			unsigned char c = data_file[n];
			std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;*/

		//std::cout << std::hex << "PADDING: ";
		srand(time(NULL));
		for (i = 16 /*head struct*/; i < dim; i++) //padding tail
		{
			//16 == head struct ; length_file == payload
			if (i < length_file + 16)
				data[i] = data_file[i - 16];
			else
			{	//Padding
				data[i] = (rand() % 0xFF);

				//unsigned char c = data[i];
				//std::cout << (int)c;

				if ((data[i] * data[i]) != 0)
					if (rand() % (data[i] * data[i]) != 0)
						srand(rand() % (data[i] * data[i]));
					else
						srand(time(NULL));
				else
					srand(time(NULL));
			}
		}
		
		//std::cout << std::dec << std::endl;
		/*
		std::cout << std::hex << "HEAD: ";
		for (int n = 0; n < 16; n++)
		{
			unsigned char c = data[n];
			std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;
		std::cout << std::hex << "FILE: ";
		for (int n = 0; n < dim; n++)
		{
			unsigned char c = data[n];
			if(c < 0xf)
				std::cout << '0' << (int) c;
			else
				std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;
		*/
		/*
		//preparo i dati
		int dim = Mod16Roof(length_file);

		char* data = new char[dim];
		for (int i = 0; i < dim; i++) //padding
		{
			if (i < length_file)
				data[i] = data_file[i];
			else
				data[i] = 0xFF;
		}
		*/
		/*
		//padding
		srand(time(NULL));
		for (int i = length_file + 1; i < dim; i++)
		{
			//cout << "rnd: " << rand() << endl;
			data[i] = (rand() % 0xFF);
			if ((data[i] * data[i]) != 0)
				if (rand() % (data[i] * data[i]) != 0)
					srand(rand() % (data[i] * data[i]));
				else
					srand(time(NULL));
			else
				srand(time(NULL));
		}
		*/

		this->crypt(data, dim, KeyA, KeyB);

		this->encrypt_data = data;
		this->new_length = dim;
	}
	else
	{
		//il file letto e' crittografato
		char* data = new char[length_file];
		for (int i = 0; i < length_file; i++)
		{
			data[i] = data_file[i];
		}

		//lo codifichiamo
		this->decrypt(data, length_file, KeyA, KeyB);

		//in this moment, data has = head + data + random padding
		int dimension_file_without_padding = 0;

		char check;

		//search for first 128 bits: //firsts 64 are payload cardinality (number of 128 bits blocks), //seconds 64 are padding's bytes
		int i;
		unsigned long long int length_rows = 0;
		for (i = 0; i < 8; i++) //firsts 64 bits
		{
			unsigned char c = data[i];
			length_rows <<= bits_for_byte; //makes space
			length_rows |= c; //sizeof(data[i]) = bits_for_byte
		}
		unsigned long long int padding_bytes = 0;
		for (; i < 16; i++) //seconds 64 bits
		{
			unsigned char c = data[i];
			padding_bytes <<= bits_for_byte; //makes space
			padding_bytes |= c; //sizeof(data[i]) = bits_for_byte
		}

		/*
		//output di tutto il file
		std::cout << std::hex << "FILE: ";
		for (int in = 0; in < length_file; in++)
		{
			unsigned char c = data[in];
			if(c < 0xf)
				std::cout << '0' << (int) c;
			else
				std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;
		//output di solo l'head struct
		std::cout << std::hex << "HEAD: ";
		for (int in = 0; in < 16; in++)
		{
			unsigned char c = data[in];
			if (c < 0xf)
				std::cout << '0' << (int)c;
			else
				std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;*/
		//output di solo il payload 
		/*
		int x = 0;
		std::cout << std::hex << "PAYLOAD: ";
		for (int in = 16; in < (((length_rows+1) * 16) + padding_bytes); x++, in++)
		{
			unsigned char c = data[in];
			if (c < 0xF)
				std::cout << '0' << (int)c;
			else
				std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;*//*
		//output di solo il padding
		std::cout << std::hex << "PADDING: ";
		for (int in = (((length_rows + 1) * 16) + padding_bytes); in < length_file; in++)
		{
			unsigned char c = data[in];
			if (c < 0xF)
				std::cout << '0' << (int)c;
			else
				std::cout << (int)c;
		}
		std::cout << std::dec << std::endl;
		*/

		//16 = struct head; (length_rows * 16) = padding without last 128 bits; padding_bytes = bytes of 128 that aren't padding
		dimension_file_without_padding = (length_rows * 16) + padding_bytes;
		
		char* data_without_padding = new char[dimension_file_without_padding];
		for (i = 0; i < dimension_file_without_padding; i++)
		{
			data_without_padding[i] = data[i + 16];
		}

		this->decrypt_data = data_without_padding;
		this->new_length = dimension_file_without_padding;

		/*
		//il file letto e' crittografato
		char* data = new char[length_file];
		for (int i = 0; i < length_file; i++)
		{
			data[i] = data_file[i];
		}

		this->decrypt(data, length_file, KeyA, KeyB);

		//in this moment, data has the data + 0xFF padding
		int dimension_file_without_padding = 0;

		char check;
		for (int i = length_file - 1; i >= 0 && dimension_file_without_padding == 0; i--)
		{
			check = data[i] ^ 0xFF;
			if (check != 0)
				dimension_file_without_padding = i;
		}

		dimension_file_without_padding++;
		char* data_without_padding = new char[dimension_file_without_padding];
		for (int i = 0; i < dimension_file_without_padding; i++)
		{
			data_without_padding[i] = data[i];
		}

		this->decrypt_data = data_without_padding;
		this->new_length = dimension_file_without_padding;
		*/
	}
}

/*
 * @author Peruffo Riccardo
 * @brief ENG DOC: destructor - currently does nothing
 *
 * @brief ITA DOC: distruttore - attualmente non fa nulla
 */
aes256::~aes256()
{

}

/*
 * @author Peruffo Riccardo
 * @brief ENG DOC: to obtain the var encrypt_data
 * @return char* - var encrypt_data
 *
 * @brief ITA DOC: restituisce la variabile encrypt_data
 * @return char* - var encrypt_data
 */
char* aes256::getEncryptData()
{
	return this->encrypt_data;
}

/*
 * @author Peruffo Riccardo
 * @brief ENG DOC: to obtain the var decrypt_data
 * @return char* - var decrypt_data
 *
 * @brief ITA DOC: restituisce la variabile decrypt_data
 * @return char* - var decrypt_data
 */
char* aes256::getDecryptData()
{
	return this->decrypt_data;
}

/*
 * @author Peruffo Riccardo
 * @brief ENG DOC: to obtain the var new_length
 * @return int - var new_length
 *
 * @brief ITA DOC: restituisce la variabile new_length
 * @return int - var new_length
 */
int aes256::getNewLength()
{
	return this->new_length;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes an integer, return the first higher number that n mod 16 == 0
 * @param int n - input
 * @return int - little number higher than n that n % 16 == 0
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief preso un numero in input restituisce il primo numero più grande divisibile per 16
 * @param int n - input
 * @return int - il primo numero più grande divisibile per 16 di n
 */
int aes256::Mod16Roof(int n)
{
	if (n % 16 != 0)
		n += (16 - (n % 16));
	return n;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief from a hex char return the byte of the hex value: charToHex('F') return a char filled with 00001111
 * @param chat tmp - hex char
 * @return unsigned char - the byte of the hex value
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief da un carattere esadecimale ritorna il valore in byte dell'esadecimale: charToHex('F') ritorna un byte contenente 00001111
 * @param chat tmp - carattere esadecimale
 * @return unsigned char - il byte del valore esadecimale
 */
unsigned char aes256::charToHex(char tmp)
{
	unsigned char Y = 0;
	switch (tmp)
	{
	case '0': 		//0000
		break;

	case '1': 		//0001
		Y |= 0x1;
		break;

	case '2': 		//0010
		Y |= 0x2;
		break;

	case '3': 		//0011
		Y |= 0x3;
		break;

	case '4':		//0100
		Y |= 0x4;
		break;

	case '5':		//0101
		Y |= 0x5;
		break;

	case '6':		//0110
		Y |= 0x6;
		break;

	case '7':		//0111
		Y |= 0x7;
		break;

	case '8':		//1000
		Y |= 0x8;
		break;

	case '9':		//1001
		Y |= 0x9;
		break;

	case 'a':		//1010
	case 'A':
		Y |= 0xa;
		break;

	case 'b':		//1011
	case 'B':
		Y |= 0xb;
		break;

	case 'c':		//1100
	case 'C':
		Y |= 0xc;
		break;

	case 'd':		//1101
	case 'D':
		Y |= 0xd;
		break;

	case 'e':		//1110
	case 'E':
		Y |= 0xe;
		break;

	case 'f':		//1111
	case 'F':
		Y |= 0xf;
		break;

	default:
		std::cout << "Aes256::charToHex 1: caso inesistente" << std::endl;
		break;
	}
	return Y;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes a chart, it returns the lower 4 bits: Low8Bit(0xAF) = F
 * @param unsigned char letter - input
 * @return unsigned short int - lower 4 bits
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief preso un char in input restituisce i 4 bit meno significativi: Low8Bit(0xAF) = F
 * @param unsigned char letter - input
 * @return unsigned short int - i 4 bit meno significativi
 */
unsigned short int aes256::Low8Bit(unsigned char letter)
{
	return letter & 0x0f;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes a chart, it returns the higher 4 bits: Low8Bit(0xAF) = A
 * @param unsigned char letter - input
 * @return unsigned short int - higher 4 bits
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief preso un char in input restituisce i 4 bit piu' significativi: Low8Bit(0xAF) = A
 * @param unsigned char letter - input
 * @return unsigned short int - i 4 bit piu' significativi
 */
unsigned short int aes256::High8Bit(unsigned char letter)
{
	letter &= 0xf0;
	return letter >>= 4;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes a 4x4 unsigned char matrix and copy every value into another matrix
 * @param unsigned char matrix[4][4] - write matrix
 * @param unsigned char matrixBKP[4][4] - read matrix
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief prende una matrice 4x4 e copia ogni valore in un'altra matrice
 * @param unsigned char matrix[4][4] - matrice che verra' sovrascritta
 * @param unsigned char matrixBKP[4][4] - matrice di lettura
 */
void aes256::Matrixd4Copy(unsigned char matrix[4][4], unsigned char matrixBKP[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i][j] = matrixBKP[i][j];
		}
	}
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief auxiliar function: for i=0..3 ( for j=0..3 (State[i][j] ^= Key[i][j]))
 * @param unsigned char State[4][4] - aes data before transformation 
 * @param unsigned char Key[4][4] - aes 128 bits sub-key
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief funzione ausiliare: for i=0..3 ( for j=0..3 (State[i][j] ^= Key[i][j]))
 * @param unsigned char State[4][4] - dati di aes prima della transformazione
 * @param unsigned char Key[4][4] - prima o seconda mezza chiave della chiave a 256 bit
 */
void aes256::XORmul(unsigned char State[4][4], unsigned char Key[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			State[i][j] ^= Key[i][j];
		}
	}
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and encrypt with the 4x4 matrix (KeyA and KeyB) using AES256 standard
 * @param char* dataToPrint - byte's sequence
 * @param int dimension - dataToPrint.length
 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo codifica con la matrice 4x4 (KeyA e KeyB) usando lo standard AES256
 * @param char* dataToPrint - sequenza di byte
 * @param int dimension - lunghezza di dataToPrint
 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
 */
void aes256::crypt(char* dataToPrint, int dimension, unsigned char KeyA[4][4], unsigned char KeyB[4][4])
{
	//stato
	unsigned char State[4][4];

	//chiave di test
	unsigned char KeyBKP_A[4][4];
	unsigned char KeyBKP_B[4][4];
	Matrixd4Copy(KeyBKP_A, KeyA);
	Matrixd4Copy(KeyBKP_B, KeyB);

	//dati criptati
	char* newData = new char[dimension];
	for (int i = 0; i < dimension; newData[i++] = 0);

	//numero di volte che devo chiamare AES256
	int BlocchiDaCriptare = dimension / 16;

	//cicla per ogni chiamata ad AES256, cioe' ad ogni blocco di 128 bit di plaintext
	for (int block = 0; block < BlocchiDaCriptare; block++)
	{
		//inserisce i valori dall'array plaintext alla matrice plaintext
		for (int bytes = 0; bytes < 16; bytes++)
		{
			if ((bytes + (16 * block)) < dimension)
			{
				State[bytes % 4][bytes / 4] = dataToPrint[bytes + (16 * block)];
			}
			else
			{
				State[bytes % 4][bytes / 4] = 0x00;
				std::cout << "NON CI DEVO ENTRARE QUI PORCAMADONNA - funzione crypt - inserimento dei valori Data in State" << std::endl;
			}
		}

		//ripristino le chiavi affinche' possano riessere utilizzate per il nuovo plaintext
		Matrixd4Copy(KeyA, KeyBKP_A); //la chiave dev'essere ripristinata ad ogni lancio di AES256
		Matrixd4Copy(KeyB, KeyBKP_B);
		AES256(State, KeyA, KeyB);	//Si passa lo stato e la chiave

		//copio il nuovo cyphertext da matrice ad array
		for (int bytes = 0; bytes < 16; bytes++) //
		{
			newData[bytes + (16 * block)] = State[bytes % 4][bytes / 4];
		}
	}

	//travaso il nuovo array della funzione temporanea all'array passato come parametro (definitivo)
	for (int i = 0; i < dimension; i++)
		dataToPrint[i] = newData[i];

	delete[] newData;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief takes a byte's sequence, split into 128 bits 4x4 matrix (state) and decrypt with the 4x4 matrix (KeyA and KeyB) using AES256 standard
 * @param char* dataToPrint - byte's sequence
 * @param int dimension_not_original - dataToPrint.length
 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief prende una sequenza di byte, la divide in matrici 4x4 di 128 bit (state) e lo decodifica con la matrice 4x4 (KeyA e KeyB) usando lo standard AES256
 * @param char* dataToPrint - sequenza di byte
 * @param int dimension_not_original - lunghezza di dataToPrint
 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
 */
void aes256::decrypt(char* dataToPrint, int dimension_not_original, unsigned char KeyA[4][4], unsigned char KeyB[4][4])
{
	unsigned char SBox[16][16] =
	{	//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
		{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, //0
		{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}, //1
		{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15}, //2
		{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75}, //3
		{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84}, //4
		{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf}, //5
		{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8}, //6
		{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2}, //7
		{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73}, //8
		{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb}, //9
		{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79}, //A
		{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08}, //B
		{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a}, //C
		{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e}, //D
		{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf}, //E
		{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}, //F
	};

	int dimensionmod16_padding = dimension_not_original;//Mod16Roof(dimensionOriginal); //dimensione mod16

	char* decryptdatapluspadding = new char[dimensionmod16_padding]; //new data
	for (int i = 0; i < dimensionmod16_padding; decryptdatapluspadding[i++] = 0);

	unsigned char State[4][4]; //stato
	
	unsigned char KeyBKP_A[4][4]; //backup della chiave iniziale
	unsigned char KeyBKP_B[4][4]; //backup della chiave iniziale
	Matrixd4Copy(KeyBKP_A, KeyA); //funzione per popolare il backup 
	Matrixd4Copy(KeyBKP_B, KeyB); //funzione per popolare il backup 

	unsigned char KeyExpA[4][4]; //chiave espansa per iniziare
	unsigned char KeyExpB[4][4]; //chiave espansa per iniziare

	unsigned char tmp_m[4][4];

	unsigned char KeyOrigA[4][4]; //copia backup di sicurezza
	unsigned char KeyOrigB[4][4]; //copia backup di sicurezza
	Matrixd4Copy(KeyOrigA, KeyBKP_A);
	Matrixd4Copy(KeyOrigB, KeyBKP_B);

	for (int round = 0; round < 14; round++) //espandiamo la chiave per fare l'inversa
	{
		if (round % 2 == 0)
		{
		}
		else
		{
			int r = round / 2;
			ExpandKey(KeyA, KeyB, SBox, r);

			if(round == 11)
				Matrixd4Copy(tmp_m, KeyB);

			if(round > 11)
				Matrixd4Copy(KeyB, tmp_m);
		}
	}
	Matrixd4Copy(KeyExpA, KeyA); //copiamo la chiave espansa
	Matrixd4Copy(KeyExpB, KeyB); //copiamo la chiave espansa

	//cicliamo per il numero di blocchi
	//for (int block = 0; block < 1; block++)
	for (int block = 0; block < dimensionmod16_padding / 16; block++) //cicla per ogni blocco da 16 byte = la capienza di State
	{
		for (int bytes = 0; bytes < 16; bytes++)
		{
			if ((bytes + (16 * block)) < dimensionmod16_padding)
			{
				State[bytes % 4][bytes / 4] = dataToPrint[bytes + (16 * block)];
			}
			else
			{
				State[bytes % 4][bytes / 4] = 0x00;
				std::cout << "NON CI DEVO ENTRARE QUI PORCAMADONNA - funzione decrypt - inserimento dei valori Data in State" << std::endl;
			}
		}

		Matrixd4Copy(KeyA, KeyExpA);
		Matrixd4Copy(KeyB, KeyExpB);
		Matrixd4Copy(KeyOrigA, KeyBKP_A);
		Matrixd4Copy(KeyOrigB, KeyBKP_B);
		InvAES256(State, KeyA, KeyB, KeyOrigA, KeyOrigB);

		for (int bytes = 0; bytes < 16; bytes++) //
		{
			decryptdatapluspadding[bytes + (16 * block)] = State[bytes % 4][bytes / 4];
		}
	}

	for (int n = 0; n < dimensionmod16_padding; dataToPrint[n] = decryptdatapluspadding[n], n++);

	delete[] decryptdatapluspadding;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief SubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation with SBox
 * @param unsigned char SBox[16][16] - transformation matrix
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione SubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la SBox
 * @param unsigned char SBox[16][16] - matrice di transformazione
 */
void aes256::SubBytes(unsigned char State[4][4], unsigned char SBox[16][16])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			State[i][j] = SBox[High8Bit(State[i][j])][Low8Bit(State[i][j])];
		}
	}
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief ShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation 
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione ShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione
 */
void aes256::ShiftRows(unsigned char State[4][4])
{
	unsigned char tmp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			tmp = State[i][0];
			State[i][0] = State[i][1];
			State[i][1] = State[i][2];
			State[i][2] = State[i][3];
			State[i][3] = tmp;
		}
	}
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief MixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation 
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione MixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione
 */
void aes256::MixColumns(unsigned char State[4][4])
{
	unsigned char mul2[16][16] =
	{
		{0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e},
		{0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e},
		{0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e},
		{0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e},
		{0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e},
		{0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe},
		{0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde},
		{0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe},
		{0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05},
		{0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25},
		{0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45},
		{0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65},
		{0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, 0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85},
		{0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5},
		{0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5},
		{0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5}
	};

	unsigned char mul3[16][16] =
	{
		{0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, 0x18, 0x1b, 0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11},
		{0x30, 0x33, 0x36, 0x35, 0x3c, 0x3f, 0x3a, 0x39, 0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27, 0x22, 0x21},
		{0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69, 0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71},
		{0x50, 0x53, 0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, 0x48, 0x4b, 0x4e, 0x4d, 0x44, 0x47, 0x42, 0x41},
		{0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf, 0xca, 0xc9, 0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1},
		{0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9, 0xe8, 0xeb, 0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1},
		{0xa0, 0xa3, 0xa6, 0xa5, 0xac, 0xaf, 0xaa, 0xa9, 0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7, 0xb2, 0xb1},
		{0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99, 0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81},
		{0x9b, 0x98, 0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86, 0x8f, 0x8c, 0x89, 0x8a},
		{0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4, 0xa1, 0xa2, 0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba},
		{0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2, 0xe3, 0xe0, 0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea},
		{0xcb, 0xc8, 0xcd, 0xce, 0xc7, 0xc4, 0xc1, 0xc2, 0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc, 0xd9, 0xda},
		{0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52, 0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a},
		{0x6b, 0x68, 0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62, 0x73, 0x70, 0x75, 0x76, 0x7f, 0x7c, 0x79, 0x7a},
		{0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34, 0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a},
		{0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10, 0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a}
	};

	unsigned char S0, S1, S2, S3;

	for (int i = 0; i < 4; i++)
	{
		S0 = (unsigned char)(mul2[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ mul3[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ State[2][i] ^ State[3][i]);
		S1 = (unsigned char)(State[0][i] ^ mul2[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ mul3[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ State[3][i]);
		S2 = (unsigned char)(State[0][i] ^ State[1][i] ^ mul2[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ mul3[High8Bit(State[3][i])][Low8Bit(State[3][i])]);
		S3 = (unsigned char)(mul3[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ State[1][i] ^ State[2][i] ^ mul2[High8Bit(State[3][i])][Low8Bit(State[3][i])]);
		State[0][i] = S0;
		State[1][i] = S1;
		State[2][i] = S2;
		State[3][i] = S3;
	}
}

//ENG DOC
/// The function AddRoundKey it was split in two subfunction: XORmul and ExpandKey
/// In this version AddRoundKey it doesn't used, but XORmul and ExpandKey implement the same functionality
/// AddRoundKey is a function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
//ITA DOC
/// La funzione AddRoundKey e' stata divisa in due sottofunzioni: XORmul ed ExpandKey
/// In questa versione AddRoundKey non e' usata, ma XORmul ed ExpandKey implementano le stesse funzionalita'
/// AddRoundKey e' una funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief ExpandKey function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
 * @param unsigned char SBox[16][16] - matrix for expansion key
 * @param int round - index of reiteration of the algorithm
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione ExpandKey che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char KeyA[4][4] - prima sotto chiave di aes256
 * @param unsigned char KeyB[4][4] - seconda sotto chiave di aes256
 * @param unsigned char SBox[16][16] - matrice per l'espansione della chiave
 * @param int round - indice di reiterazione dell'algoritmo
 */
void aes256::ExpandKey(unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char SBox[16][16], int round)
{
	unsigned char Rcon[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

	unsigned char newKeyA[4][4];
	unsigned char newKeyB[4][4];

	for (int col = 0; col < 4; col++)
	{
		unsigned char S0, S1, S2, S3;
		if (col == 0)
		{
			//rotate
			S0 = KeyB[1][3];
			S1 = KeyB[2][3];
			S2 = KeyB[3][3];
			S3 = KeyB[0][3];
			//subbytes
			S0 = SBox[High8Bit(S0)][Low8Bit(S0)];
			S1 = SBox[High8Bit(S1)][Low8Bit(S1)];
			S2 = SBox[High8Bit(S2)][Low8Bit(S2)];
			S3 = SBox[High8Bit(S3)][Low8Bit(S3)];

			//first col transf
			newKeyA[0][col] = KeyA[0][col] ^ Rcon[round] ^ S0;
			newKeyA[1][col] = KeyA[1][col] ^ S1;
			newKeyA[2][col] = KeyA[2][col] ^ S2;
			newKeyA[3][col] = KeyA[3][col] ^ S3;
		}
		else
		{
			S0 = newKeyA[0][col - 1];
			S1 = newKeyA[1][col - 1];
			S2 = newKeyA[2][col - 1];
			S3 = newKeyA[3][col - 1];

			//first col transf
			newKeyA[0][col] = KeyA[0][col] ^ S0;
			newKeyA[1][col] = KeyA[1][col] ^ S1;
			newKeyA[2][col] = KeyA[2][col] ^ S2;
			newKeyA[3][col] = KeyA[3][col] ^ S3;
		}
	}
	for (int col = 0; col < 4; col++)
	{
		unsigned char S0, S1, S2, S3;
		if (col == 0)
		{
			//prendo l'ultima colonna di KeyA
			S0 = newKeyA[0][3];
			S1 = newKeyA[1][3];
			S2 = newKeyA[2][3];
			S3 = newKeyA[3][3];

			//subbytes
			S0 = SBox[High8Bit(S0)][Low8Bit(S0)];
			S1 = SBox[High8Bit(S1)][Low8Bit(S1)];
			S2 = SBox[High8Bit(S2)][Low8Bit(S2)];
			S3 = SBox[High8Bit(S3)][Low8Bit(S3)];

			//first col transf
			newKeyB[0][col] = KeyB[0][col] ^ S0;
			newKeyB[1][col] = KeyB[1][col] ^ S1;
			newKeyB[2][col] = KeyB[2][col] ^ S2;
			newKeyB[3][col] = KeyB[3][col] ^ S3;
		}
		else
		{
			S0 = newKeyB[0][col - 1];
			S1 = newKeyB[1][col - 1];
			S2 = newKeyB[2][col - 1];
			S3 = newKeyB[3][col - 1];

			//first col transf
			newKeyB[0][col] = KeyB[0][col] ^ S0;
			newKeyB[1][col] = KeyB[1][col] ^ S1;
			newKeyB[2][col] = KeyB[2][col] ^ S2;
			newKeyB[3][col] = KeyB[3][col] ^ S3;
		}
	}
	Matrixd4Copy(KeyA, newKeyA);
	Matrixd4Copy(KeyB, newKeyB);
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief  function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * PS: it will use: SubBytes, ShiftRows, MixColumns, XORmul, ExpandKey
 * @param unsigned char State[4][4] - data before encrypt, will be overwrite with encrypted data
 * @param unsigned char KeyA[4][4] - first aes256 bits sub-key
 * @param unsigned char KeyB[4][4] - second aes256 bits sub-key
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * PS: usera': SubBytes, ShiftRows, MixColumns, XORmul, ExpandKey
 * @param unsigned char State[4][4] - dati prima della codifica, verranno sovrascritti dai dati codificati
 * @param unsigned char KeyA[4][4] - prima chiave di aes256
 * @param unsigned char KeyB[4][4] - seconda chiave di aes256
 */
void aes256::AES256(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4])
{
	unsigned char SBox[16][16] =
	{	//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
		{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, //0
		{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}, //1
		{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15}, //2
		{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75}, //3
		{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84}, //4
		{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf}, //5
		{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8}, //6
		{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2}, //7
		{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73}, //8
		{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb}, //9
		{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79}, //A
		{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08}, //B
		{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a}, //C
		{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e}, //D
		{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf}, //E
		{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}, //F
	};

	int round = 0;

	//std::cout << "round["; outdoubleN(round); std::cout << "].input\t"; outMatrix4x4(State); std::cout << std::endl;
	//std::cout << "round["; outdoubleN(round); std::cout << "].k_sch\t"; outMatrix4x4(KeyA); std::cout << std::endl;
	
	//AddRoundKey(State, KeyA, SBox, round);
	XORmul(State, KeyA);

	while (round < 14)
	{
		++round;

		//std::cout << "round["; outdoubleN(round); std::cout << "].start\t"; outMatrix4x4(State); std::cout << std::endl;

		SubBytes(State, SBox);
		//std::cout << "round["; outdoubleN(round); std::cout << "].s_box\t"; outMatrix4x4(State); std::cout << std::endl;
		ShiftRows(State);
		//std::cout << "round["; outdoubleN(round); std::cout << "].s_row\t"; outMatrix4x4(State); std::cout << std::endl;
		if (round < 14)
		{
			MixColumns(State);
			//std::cout << "round["; outdoubleN(round); std::cout << "].m_col\t"; outMatrix4x4(State); std::cout << std::endl;
		}

		if(round % 2 == 0)
		{
			//std::cout << "round["; outdoubleN(round); std::cout << "].k_sch\t"; outMatrix4x4(KeyA); std::cout << std::endl;
			XORmul(State, KeyA);
			//AddRoundKey(State, KeyA, SBox, r);
		}
		else
		{
			int r = round / 2;
			//std::cout << "round["; outdoubleN(round); std::cout << "].k_sch\t"; outMatrix4x4(KeyB); std::cout << std::endl;
			XORmul(State, KeyB);
			//AddRoundKey(State, KeyB, SBox, r);
			ExpandKey(KeyA, KeyB, SBox, r);
		}
	}
	//std::cout << "round["; outdoubleN(round); std::cout << "].output\t"; outMatrix4x4(State); std::cout << std::endl;
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief InvSubBytes function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation with invSbox
 * @param unsigned char invSbox[16][16] - transformation matrix
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione InvSubBytes che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione con la invSbox
 * @param unsigned char invSbox[16][16] - matrice di transformazione
 */
void aes256::InvSubBytes(unsigned char State[4][4], unsigned char invSbox[16][16])
{

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			State[i][j] = invSbox[High8Bit(State[i][j])][Low8Bit(State[i][j])];
		}
	}

}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief InvShiftRows function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation 
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione InvShiftRows che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione
 */
void aes256::InvShiftRows(unsigned char State[4][4])
{
	unsigned char tmp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			tmp = State[i][3];
			State[i][3] = State[i][2];
			State[i][2] = State[i][1];
			State[i][1] = State[i][0];
			State[i][0] = tmp;
		}
	}
}

//ENG DOC
/**
 * @author Peruffo Riccardo
 * @brief InvMixColumns function that follow the standard set on FIPS's document: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - aes data before transformation 
 */
//ITA DOC
/**
 * @author Peruffo Riccardo
 * @brief  funzione InvMixColumns che segue lo standard indicato nel documento redatto dalla FIPS: https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 * @param unsigned char State[4][4] - dati di aes prima della transformazione
 */
void aes256::InvMixColumns(unsigned char State[4][4])
{
	unsigned char mul9[16][16] =
	{
		{0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, 0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77},
		{0x90, 0x99, 0x82, 0x8b, 0xb4, 0xbd, 0xa6, 0xaf, 0xd8, 0xd1, 0xca, 0xc3, 0xfc, 0xf5, 0xee, 0xe7},
		{0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04, 0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c},
		{0xab, 0xa2, 0xb9, 0xb0, 0x8f, 0x86, 0x9d, 0x94, 0xe3, 0xea, 0xf1, 0xf8, 0xc7, 0xce, 0xd5, 0xdc},
		{0x76, 0x7f, 0x64, 0x6d, 0x52, 0x5b, 0x40, 0x49, 0x3e, 0x37, 0x2c, 0x25, 0x1a, 0x13, 0x08, 0x01},
		{0xe6, 0xef, 0xf4, 0xfd, 0xc2, 0xcb, 0xd0, 0xd9, 0xae, 0xa7, 0xbc, 0xb5, 0x8a, 0x83, 0x98, 0x91},
		{0x4d, 0x44, 0x5f, 0x56, 0x69, 0x60, 0x7b, 0x72, 0x05, 0x0c, 0x17, 0x1e, 0x21, 0x28, 0x33, 0x3a},
		{0xdd, 0xd4, 0xcf, 0xc6, 0xf9, 0xf0, 0xeb, 0xe2, 0x95, 0x9c, 0x87, 0x8e, 0xb1, 0xb8, 0xa3, 0xaa},
		{0xec, 0xe5, 0xfe, 0xf7, 0xc8, 0xc1, 0xda, 0xd3, 0xa4, 0xad, 0xb6, 0xbf, 0x80, 0x89, 0x92, 0x9b},
		{0x7c, 0x75, 0x6e, 0x67, 0x58, 0x51, 0x4a, 0x43, 0x34, 0x3d, 0x26, 0x2f, 0x10, 0x19, 0x02, 0x0b},
		{0xd7, 0xde, 0xc5, 0xcc, 0xf3, 0xfa, 0xe1, 0xe8, 0x9f, 0x96, 0x8d, 0x84, 0xbb, 0xb2, 0xa9, 0xa0},
		{0x47, 0x4e, 0x55, 0x5c, 0x63, 0x6a, 0x71, 0x78, 0x0f, 0x06, 0x1d, 0x14, 0x2b, 0x22, 0x39, 0x30},
		{0x9a, 0x93, 0x88, 0x81, 0xbe, 0xb7, 0xac, 0xa5, 0xd2, 0xdb, 0xc0, 0xc9, 0xf6, 0xff, 0xe4, 0xed},
		{0x0a, 0x03, 0x18, 0x11, 0x2e, 0x27, 0x3c, 0x35, 0x42, 0x4b, 0x50, 0x59, 0x66, 0x6f, 0x74, 0x7d},
		{0xa1, 0xa8, 0xb3, 0xba, 0x85, 0x8c, 0x97, 0x9e, 0xe9, 0xe0, 0xfb, 0xf2, 0xcd, 0xc4, 0xdf, 0xd6},
		{0x31, 0x38, 0x23, 0x2a, 0x15, 0x1c, 0x07, 0x0e, 0x79, 0x70, 0x6b, 0x62, 0x5d, 0x54, 0x4f, 0x46}
	};

	unsigned char mul11[16][16] =
	{
		{0x00, 0x0b, 0x16, 0x1d, 0x2c, 0x27, 0x3a, 0x31, 0x58, 0x53, 0x4e, 0x45, 0x74, 0x7f, 0x62, 0x69},
		{0xb0, 0xbb, 0xa6, 0xad, 0x9c, 0x97, 0x8a, 0x81, 0xe8, 0xe3, 0xfe, 0xf5, 0xc4, 0xcf, 0xd2, 0xd9},
		{0x7b, 0x70, 0x6d, 0x66, 0x57, 0x5c, 0x41, 0x4a, 0x23, 0x28, 0x35, 0x3e, 0x0f, 0x04, 0x19, 0x12},
		{0xcb, 0xc0, 0xdd, 0xd6, 0xe7, 0xec, 0xf1, 0xfa, 0x93, 0x98, 0x85, 0x8e, 0xbf, 0xb4, 0xa9, 0xa2},
		{0xf6, 0xfd, 0xe0, 0xeb, 0xda, 0xd1, 0xcc, 0xc7, 0xae, 0xa5, 0xb8, 0xb3, 0x82, 0x89, 0x94, 0x9f},
		{0x46, 0x4d, 0x50, 0x5b, 0x6a, 0x61, 0x7c, 0x77, 0x1e, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2f},
		{0x8d, 0x86, 0x9b, 0x90, 0xa1, 0xaa, 0xb7, 0xbc, 0xd5, 0xde, 0xc3, 0xc8, 0xf9, 0xf2, 0xef, 0xe4},
		{0x3d, 0x36, 0x2b, 0x20, 0x11, 0x1a, 0x07, 0x0c, 0x65, 0x6e, 0x73, 0x78, 0x49, 0x42, 0x5f, 0x54},
		{0xf7, 0xfc, 0xe1, 0xea, 0xdb, 0xd0, 0xcd, 0xc6, 0xaf, 0xa4, 0xb9, 0xb2, 0x83, 0x88, 0x95, 0x9e},
		{0x47, 0x4c, 0x51, 0x5a, 0x6b, 0x60, 0x7d, 0x76, 0x1f, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2e},
		{0x8c, 0x87, 0x9a, 0x91, 0xa0, 0xab, 0xb6, 0xbd, 0xd4, 0xdf, 0xc2, 0xc9, 0xf8, 0xf3, 0xee, 0xe5},
		{0x3c, 0x37, 0x2a, 0x21, 0x10, 0x1b, 0x06, 0x0d, 0x64, 0x6f, 0x72, 0x79, 0x48, 0x43, 0x5e, 0x55},
		{0x01, 0x0a, 0x17, 0x1c, 0x2d, 0x26, 0x3b, 0x30, 0x59, 0x52, 0x4f, 0x44, 0x75, 0x7e, 0x63, 0x68},
		{0xb1, 0xba, 0xa7, 0xac, 0x9d, 0x96, 0x8b, 0x80, 0xe9, 0xe2, 0xff, 0xf4, 0xc5, 0xce, 0xd3, 0xd8},
		{0x7a, 0x71, 0x6c, 0x67, 0x56, 0x5d, 0x40, 0x4b, 0x22, 0x29, 0x34, 0x3f, 0x0e, 0x05, 0x18, 0x13},
		{0xca, 0xc1, 0xdc, 0xd7, 0xe6, 0xed, 0xf0, 0xfb, 0x92, 0x99, 0x84, 0x8f, 0xbe, 0xb5, 0xa8, 0xa3}
	};

	unsigned char mul13[16][16] =
	{
		{0x00, 0x0d, 0x1a, 0x17, 0x34, 0x39, 0x2e, 0x23, 0x68, 0x65, 0x72, 0x7f, 0x5c, 0x51, 0x46, 0x4b},
		{0xd0, 0xdd, 0xca, 0xc7, 0xe4, 0xe9, 0xfe, 0xf3, 0xb8, 0xb5, 0xa2, 0xaf, 0x8c, 0x81, 0x96, 0x9b},
		{0xbb, 0xb6, 0xa1, 0xac, 0x8f, 0x82, 0x95, 0x98, 0xd3, 0xde, 0xc9, 0xc4, 0xe7, 0xea, 0xfd, 0xf0},
		{0x6b, 0x66, 0x71, 0x7c, 0x5f, 0x52, 0x45, 0x48, 0x03, 0x0e, 0x19, 0x14, 0x37, 0x3a, 0x2d, 0x20},
		{0x6d, 0x60, 0x77, 0x7a, 0x59, 0x54, 0x43, 0x4e, 0x05, 0x08, 0x1f, 0x12, 0x31, 0x3c, 0x2b, 0x26},
		{0xbd, 0xb0, 0xa7, 0xaa, 0x89, 0x84, 0x93, 0x9e, 0xd5, 0xd8, 0xcf, 0xc2, 0xe1, 0xec, 0xfb, 0xf6},
		{0xd6, 0xdb, 0xcc, 0xc1, 0xe2, 0xef, 0xf8, 0xf5, 0xbe, 0xb3, 0xa4, 0xa9, 0x8a, 0x87, 0x90, 0x9d},
		{0x06, 0x0b, 0x1c, 0x11, 0x32, 0x3f, 0x28, 0x25, 0x6e, 0x63, 0x74, 0x79, 0x5a, 0x57, 0x40, 0x4d},
		{0xda, 0xd7, 0xc0, 0xcd, 0xee, 0xe3, 0xf4, 0xf9, 0xb2, 0xbf, 0xa8, 0xa5, 0x86, 0x8b, 0x9c, 0x91},
		{0x0a, 0x07, 0x10, 0x1d, 0x3e, 0x33, 0x24, 0x29, 0x62, 0x6f, 0x78, 0x75, 0x56, 0x5b, 0x4c, 0x41},
		{0x61, 0x6c, 0x7b, 0x76, 0x55, 0x58, 0x4f, 0x42, 0x09, 0x04, 0x13, 0x1e, 0x3d, 0x30, 0x27, 0x2a},
		{0xb1, 0xbc, 0xab, 0xa6, 0x85, 0x88, 0x9f, 0x92, 0xd9, 0xd4, 0xc3, 0xce, 0xed, 0xe0, 0xf7, 0xfa},
		{0xb7, 0xba, 0xad, 0xa0, 0x83, 0x8e, 0x99, 0x94, 0xdf, 0xd2, 0xc5, 0xc8, 0xeb, 0xe6, 0xf1, 0xfc},
		{0x67, 0x6a, 0x7d, 0x70, 0x53, 0x5e, 0x49, 0x44, 0x0f, 0x02, 0x15, 0x18, 0x3b, 0x36, 0x21, 0x2c},
		{0x0c, 0x01, 0x16, 0x1b, 0x38, 0x35, 0x22, 0x2f, 0x64, 0x69, 0x7e, 0x73, 0x50, 0x5d, 0x4a, 0x47},
		{0xdc, 0xd1, 0xc6, 0xcb, 0xe8, 0xe5, 0xf2, 0xff, 0xb4, 0xb9, 0xae, 0xa3, 0x80, 0x8d, 0x9a, 0x97}
	};

	unsigned char mul14[16][16] =
	{
		{0x00, 0x0e, 0x1c, 0x12, 0x38, 0x36, 0x24, 0x2a, 0x70, 0x7e, 0x6c, 0x62, 0x48, 0x46, 0x54, 0x5a},
		{0xe0, 0xee, 0xfc, 0xf2, 0xd8, 0xd6, 0xc4, 0xca, 0x90, 0x9e, 0x8c, 0x82, 0xa8, 0xa6, 0xb4, 0xba},
		{0xdb, 0xd5, 0xc7, 0xc9, 0xe3, 0xed, 0xff, 0xf1, 0xab, 0xa5, 0xb7, 0xb9, 0x93, 0x9d, 0x8f, 0x81},
		{0x3b, 0x35, 0x27, 0x29, 0x03, 0x0d, 0x1f, 0x11, 0x4b, 0x45, 0x57, 0x59, 0x73, 0x7d, 0x6f, 0x61},
		{0xad, 0xa3, 0xb1, 0xbf, 0x95, 0x9b, 0x89, 0x87, 0xdd, 0xd3, 0xc1, 0xcf, 0xe5, 0xeb, 0xf9, 0xf7},
		{0x4d, 0x43, 0x51, 0x5f, 0x75, 0x7b, 0x69, 0x67, 0x3d, 0x33, 0x21, 0x2f, 0x05, 0x0b, 0x19, 0x17},
		{0x76, 0x78, 0x6a, 0x64, 0x4e, 0x40, 0x52, 0x5c, 0x06, 0x08, 0x1a, 0x14, 0x3e, 0x30, 0x22, 0x2c},
		{0x96, 0x98, 0x8a, 0x84, 0xae, 0xa0, 0xb2, 0xbc, 0xe6, 0xe8, 0xfa, 0xf4, 0xde, 0xd0, 0xc2, 0xcc},
		{0x41, 0x4f, 0x5d, 0x53, 0x79, 0x77, 0x65, 0x6b, 0x31, 0x3f, 0x2d, 0x23, 0x09, 0x07, 0x15, 0x1b},
		{0xa1, 0xaf, 0xbd, 0xb3, 0x99, 0x97, 0x85, 0x8b, 0xd1, 0xdf, 0xcd, 0xc3, 0xe9, 0xe7, 0xf5, 0xfb},
		{0x9a, 0x94, 0x86, 0x88, 0xa2, 0xac, 0xbe, 0xb0, 0xea, 0xe4, 0xf6, 0xf8, 0xd2, 0xdc, 0xce, 0xc0},
		{0x7a, 0x74, 0x66, 0x68, 0x42, 0x4c, 0x5e, 0x50, 0x0a, 0x04, 0x16, 0x18, 0x32, 0x3c, 0x2e, 0x20},
		{0xec, 0xe2, 0xf0, 0xfe, 0xd4, 0xda, 0xc8, 0xc6, 0x9c, 0x92, 0x80, 0x8e, 0xa4, 0xaa, 0xb8, 0xb6},
		{0x0c, 0x02, 0x10, 0x1e, 0x34, 0x3a, 0x28, 0x26, 0x7c, 0x72, 0x60, 0x6e, 0x44, 0x4a, 0x58, 0x56},
		{0x37, 0x39, 0x2b, 0x25, 0x0f, 0x01, 0x13, 0x1d, 0x47, 0x49, 0x5b, 0x55, 0x7f, 0x71, 0x63, 0x6d},
		{0xd7, 0xd9, 0xcb, 0xc5, 0xef, 0xe1, 0xf3, 0xfd, 0xa7, 0xa9, 0xbb, 0xb5, 0x9f, 0x91, 0x83, 0x8d}
	};

	unsigned char S0, S1, S2, S3;

	for (int i = 0; i < 4; i++)
	{
		S0 = (unsigned char)mul14[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ mul11[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ mul13[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ mul9[High8Bit(State[3][i])][Low8Bit(State[3][i])];
		S1 = (unsigned char)mul9[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ mul14[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ mul11[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ mul13[High8Bit(State[3][i])][Low8Bit(State[3][i])];
		S2 = (unsigned char)mul13[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ mul9[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ mul14[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ mul11[High8Bit(State[3][i])][Low8Bit(State[3][i])];
		S3 = (unsigned char)mul11[High8Bit(State[0][i])][Low8Bit(State[0][i])] ^ mul13[High8Bit(State[1][i])][Low8Bit(State[1][i])] ^ mul9[High8Bit(State[2][i])][Low8Bit(State[2][i])] ^ mul14[High8Bit(State[3][i])][Low8Bit(State[3][i])];
		State[0][i] = S0;
		State[1][i] = S1;
		State[2][i] = S2;
		State[3][i] = S3;
	}
}

//ENG DOC
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
//ITA DOC
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
void aes256::InvAddRoundKey(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4], int round, unsigned char OriginalKeyA[4][4], unsigned char OriginalKeyB[4][4])
{
	if (round < 14)
	{
		InvExpandKey(KeyA, KeyB, OriginalKeyA, OriginalKeyB, round);
	}
}

//ENG DOC
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
//ITA DOC
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
void aes256::InvExpandKey(unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char OriginalKeyA[4][4], unsigned char OriginalKeyB[4][4], int round)
{
	unsigned char SBox[16][16] =
	{	//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
		{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, //0
		{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}, //1
		{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15}, //2
		{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75}, //3
		{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84}, //4
		{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf}, //5
		{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8}, //6
		{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2}, //7
		{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73}, //8
		{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb}, //9
		{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79}, //A
		{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08}, //B
		{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a}, //C
		{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e}, //D
		{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf}, //E
		{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}, //F
	};

	//prendo la chiave originale
	Matrixd4Copy(KeyA, OriginalKeyA);
	Matrixd4Copy(KeyB, OriginalKeyB);

	unsigned char tmp_m[4][4]; //tmp matrix

	//la espando 14-round volte e diventa la nuova espansione della chiave

	for (int roundExp = 0; roundExp < 6 - round; roundExp++)
	{
		if (roundExp + 1 == 6 - round)
		{
			Matrixd4Copy(tmp_m, KeyB);
		}

		ExpandKey(KeyA, KeyB, SBox, roundExp); //espando la chiave

		if (roundExp + 1 == 6 - round)
		{
			Matrixd4Copy(KeyB, tmp_m);
		}
	}

}

//ENG DOC
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
//ITA DOC
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
void aes256::InvAES256(unsigned char State[4][4], unsigned char KeyA[4][4], unsigned char KeyB[4][4], unsigned char KeyOrigA[4][4], unsigned char KeyOrigB[4][4])
{
	unsigned char KeyOriginalA[4][4];
	unsigned char KeyOriginalB[4][4];
	Matrixd4Copy(KeyOriginalA, KeyOrigA);
	Matrixd4Copy(KeyOriginalB, KeyOrigB);

	unsigned char invSBox[16][16] =
	{
		{0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
		{0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
		{0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
		{0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
		{0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
		{0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
		{0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
		{0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
		{0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
		{0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
		{0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
		{0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
		{0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
		{0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
		{0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
		{0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
	};

	//output
	int round = 0;

	//std::cout << "round["; outdoubleN(round); std::cout << "].iinput\t"; outMatrix4x4(State); std::cout << std::endl;
	//std::cout << "round["; outdoubleN(round); std::cout << "].ik_sch\t"; outMatrix4x4(KeyA); std::cout << std::endl;

	//InvAddRoundKey(State, Key, round, KeyOriginal);
	XORmul(State, KeyA);

	while (round < 14)
	{
		++round;
		//std::cout << "round["; outdoubleN(round); std::cout << "].istart\t"; outMatrix4x4(State); std::cout << std::endl;

		InvShiftRows(State);
		//std::cout << "round["; outdoubleN(round); std::cout << "].is_row\t"; outMatrix4x4(State); std::cout << std::endl;

		InvSubBytes(State, invSBox);
		//std::cout << "round["; outdoubleN(round); std::cout << "].is_box\t"; outMatrix4x4(State); std::cout << std::endl;

		if (round % 2 == 0)
		{
			//std::cout << "round["; outdoubleN(round); std::cout << "].ik_sch\t"; outMatrix4x4(KeyA); std::cout << std::endl;
			XORmul(State, KeyA);
		}
		else
		{
			int r = round / 2;
			//std::cout << "round["; outdoubleN(round); std::cout << "].ik_sch\t"; outMatrix4x4(KeyB); std::cout << std::endl;
			XORmul(State, KeyB);
			InvAddRoundKey(State, KeyA, KeyB, r, KeyOriginalA, KeyOriginalB);
		}

		if (round < 14)
		{
			//std::cout << "round["; outdoubleN(round); std::cout << "].ik_add\t"; outMatrix4x4(State); std::cout << std::endl;
			InvMixColumns(State);
		}
	}
	//std::cout << "round["; outdoubleN(round); std::cout << "].ioutput\t"; outMatrix4x4(State); std::cout << std::endl;
}