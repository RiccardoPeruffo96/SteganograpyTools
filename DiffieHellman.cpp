#include <list>
#include <ctime>
#include <math.h>
#include <thread>
#include <sstream>
#include <fstream>
#include <stdio.h>  // for printf
#include <cstdlib>  // for _countof
#include <errno.h>  // for return values
#include <cstring>  // for wcscpy_s, wcscat_s
#include <stdlib.h> // for _countof
#include <string.h> // for strcpy_s, strcat_s
#include <iostream> // for cout, includes <cstdlib>, <cstring>
#include "DiffieHellman.h"

namespace DfHl
{
	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief compute (Y ^ X) % q with recursion, complexity: O(log(X)) with tail recursion
	 * @param unsigned int Y - base
	 * @param unsigned int X - exponent
	 * @param unsigned int q - mod
	 * @param std::string auxiliary - aux string used by tail recursion
	 * @return unsigned int - (Y ^ X) % q
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief calcola (Y ^ X) % q ricorsivamente, complessita': O(log(X)) con ricorsione in coda
	 * @param unsigned int Y - base
	 * @param unsigned int X - esponente
	 * @param unsigned int q - modulo
	 * @param std::string auxiliary - string ausiliria usata per la ricorsione in coda
	 * @return unsigned int - (Y ^ X) % q
	 */
	unsigned int DiffieHellman::espMod(unsigned int Y, unsigned int X, unsigned int q, std::string auxiliary)
	{
		/*
		*
		* Considerato (a = b % n) dove es: a = 55, n = 10 -> b = 5
		* (a ^ k = (b ^ k) % n) con es k = 2
		* a ^ k = 55 ^ 2 = 3025
		* b ^ k = 5 ^ 2 = 25
		* 3025 % 10 = 5
		*
		* Quindi se devo fare 3 ^ 9 % 12 = 19683 % 12 = 3
		* posso fare:
		* 1) 3 * 3 % 12 = 9 = 3 * 3 % 12
		* 2) 9 * 3 % 12 = 3 = 9 * 3 % 12
		* 3) 3 * 3 % 12 = 9 = 27 * 3 % 12
		* 4) 9 * 3 % 12 = 3 = 81 * 3 % 12
		* 5) 3 * 3 % 12 = 9 = 243 * 3 % 12
		* 6) 9 * 3 % 12 = 3 = 729 * 3 % 12
		* 7) 3 * 3 % 12 = 9 = 2187 * 3 % 12
		* 8) 9 * 3 % 12 = 3 = 6561 * 3 % 12
		*/
		
		if (X == 0)
		{
			unsigned long long int temp = 1;
		
			//reverse string
			std::reverse(std::begin(auxiliary), std::end(auxiliary));

			//and makes operation
			for (int i = 0; i < auxiliary.length(); i++)
			{
				if(auxiliary[i] == '0')
				{
					temp *= temp;
					temp %= q;
				}
				else
				{
					temp *= temp;
					temp %= q;
					temp *= Y;
					temp %= q;
				}
			}
			return temp;
		}
		if (X % 2 == 0)
		{
			auxiliary.append("0");
		}
		else
		{
			auxiliary.append("1");
		}
		return espMod(Y, X / 2, q, auxiliary);
	}
	
	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief extract a uint 32 bit from string: every string's char it's a hex number and input.length() mod 8 == 0.
	 * @param std::string input - input.length() mod 8 == 0. every char it's a hex number
	 * @param int index - offset starting to extract the number
	 * @return unsigned int - the number between 0 and 2^32-1 extracted starting from input[index * 8] and input[(index * 8) + 8]
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief estrae un uint di 32 bit da una stringa: ogni carattere e' un esadecimale e la stringa ha un numero di caratteri divisibile per 8.
	 * @param std::string input - input.length() mod 8 == 0, rappresenta un numero a 128 o 256 bit: ogni carattere e' un esadecimale
	 * @param int index - rappresenta l'ottetto che si vuole estrarre
	 * @return unsigned int - numero tra 0 e 2^32-1 estratto a tra input[index * 8] e input[(index * 8) + 8]
	 */
	unsigned int DiffieHellman::uint32bitFromString(std::string output_omega, int index)
	{
		//valore da restituire
		unsigned int Y = 0;

		//read 8 hex char
		for (int i = index * 8; i < (index * 8) + 8; i++)
		{
			Y <<= 4;

			//transform char to number Y
			switch (output_omega[i])
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
				std::cout << "DiffieHellman::uint32bitFromString 1: caso inesistente" << std::endl;
				break;
			}
		}

		return Y;
	}

	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief create a random number smallest then q
	 * @param unsigned int modificatore - random number that help to create another randoms numbers
	 * @return unsigned int - a random number between [1; (this->q - 1)]
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief create a random number smallest then q
	 * @param unsigned int modificatore - un numero random che aiuta a creare altri numeri random
	 * @return unsigned int - un valore randomico compreso tra [1; (this->q - 1)]
	 */
	unsigned int DiffieHellman::createX(unsigned int modificatore)
	{
		//rand() = [0x0, 0x7fff]
		if (modificatore == 0)
		{
			unsigned int final1, final2;

			srand((time(NULL) * this->alpha) % this->q);

			unsigned int n0 = rand();

			srand(time(NULL));

			unsigned int n1 = rand();

			srand(this->q);

			unsigned int n2 = rand();

			srand(this->alpha);

			unsigned int n3 = rand();

			if ((n0 ^ n1) % 2 == 0)
			{
				n1 <<= 16;
				n1 |= n0;
				final1 = n1;
			}
			else
			{
				n0 <<= 16;
				n0 |= n1;
				final1 = n0;
			}

			if ((n2 ^ n3) % 2 == 0)
			{
				n3 <<= 16;
				n3 |= n2;
				final2 = n3;
			}
			else
			{
				n2 <<= 16;
				n2 |= n3;
				final2 = n2;
			}

			unsigned int n_end = final1 ^ final2;

			n_end %= this->q;

			return n_end;
		}

		unsigned int max = 0xFFFFFFFF;
		unsigned long long int var_uint64;
		unsigned int var_uint32;

		unsigned int final1, final2;

		var_uint64 = (time(NULL) * this->alpha) % modificatore;
		var_uint64 *= this->q;
		var_uint32 = var_uint64 % 0xFFFFFFFF;

		srand(var_uint32 % this->q);

		unsigned int n0 = rand();

		srand(time(NULL));

		unsigned int n1 = rand();

		if ((n0 ^ n1) % 2 == 0)
		{
			n1 <<= 16;
			n1 |= n0;
			final1 = n1;
		}
		else
		{
			n0 <<= 16;
			n0 |= n1;
			final1 = n0;
		}

		var_uint64 = ((unsigned long long int) modificatore) * ((unsigned long long int) time(NULL));
		var_uint32 = var_uint64 % max;
		std::string aux01("");
		srand(espMod(this->q, this->alpha, var_uint32, aux01));
		aux01.~basic_string();

		unsigned int n2 = rand();

		var_uint64 = ((unsigned long long int) modificatore) * ((unsigned long long int) time(NULL));
		var_uint32 = var_uint64 % max;
		std::string aux02("");
		srand(espMod(var_uint32, this->alpha, max, aux02));
		aux02.~basic_string();

		unsigned int n3 = rand();

		if ((n2 ^ n3) % 2 == 0)
		{
			n3 <<= 16;
			n3 |= n2;
			final2 = n3;
		}
		else
		{
			n2 <<= 16;
			n2 |= n3;
			final2 = n2;
		}
		

		unsigned int n_end = final1 ^ final2;

		n_end %= this->q;

		return n_end;

	}

	//costruttore generazione mezza chiave
	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief constructor to create first half key - gives empty values to the private vars: std::string X_str, std::string Y_str, std::string K_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief costruttore per creare la prima mezza chiave - popola con valori nulli le variabili private: std::string X_str, std::string Y_str, std::string K_str
	 */
	DiffieHellman::DiffieHellman()
	{
		this->X_str = std::string("");
		this->Y_str = std::string("");
		this->K_str = std::string("");
	}

	//costruttore generazione chiave intera
	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @override
	 * @brief constructor to create second half key - gives empty value to the private vars: std::string K_str
	 * @param std::string Xa_str - value that will be padded in X_str
	 * @param std::string Yb_str - value that will be padded in Y_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @override
	 * @brief costruttore per creare mezze chiavi - popola con valori nulli la variabile privata: std::string K_str
	 * @param std::string Xa_str - valore che verra' inserito in X_str
	 * @param std::string Yb_str - valore che verra' inserito in Y_str
	 */
	DiffieHellman::DiffieHellman(std::string Xa_str, std::string Yb_str)
	{
		this->X_str = Xa_str;
		this->Y_str = Yb_str;
		this->K_str = std::string("");
	}

	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief destructor - delete the private vars: std::string X_str, std::string Y_str, std::string K_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief distruttore - cancella le variabili private: std::string X_str, std::string Y_str, std::string K_str
	 */
	DiffieHellman::~DiffieHellman()
	{
		X_str.~basic_string();
		Y_str.~basic_string();
		K_str.~basic_string();
	}

	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief generate an 128 bits (32 hex chars), half-key with DiffieHellman algorithm: the secret half key will be stored in X_str, the public half key will be stored in Y_str
	 *	PS: it's necessary use the constructor without params or there may be mistakes
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief genera una mezza chiave a 128 bits (32 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta verra' salvata in X_str, la mezza chiave pubblica verra' salvata in Y_str
	 * PS: e' necessario usare il construttore senza parametri per non avere errori
	 */
	void DiffieHellman::HalfKey_Generator_128bits()
	{
		//stream per salvataggio su stringa
		std::stringstream streamY;
		std::stringstream streamX;

		unsigned int tmpX = this->q << this->alpha;
		unsigned int tmpY = this->q >> this->alpha;

		std::cout << "\nGenerazione semichiave:\n\tCompletato:" << std::endl;

		//devo ciclare 4 volte, per ogni ciclo creo una stringa da 32 bit
		for (int i = 0; i < this->cicli_128bits; i++)
		{
			std::cout << '\r';
			std::cout << "\t" << i * 25 << this->pc;

			//genero X < q
			unsigned int Xa = createX(tmpX ^ tmpY);
			std::string aux("");
			unsigned int Ya = espMod(this->alpha, Xa, this->q, aux);
			aux.~basic_string();
			
			unsigned int YSize = Ya;
			tmpY = Ya;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (YSize < 0x10000000)
			{
				YSize <<= 4;
				streamY << std::hex << this->zero;
				tmpY ^= YSize;
			}

			streamY << std::hex << Ya;

			unsigned int XSize = Xa;
			tmpX = Xa;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (XSize < 0x10000000)
			{
				XSize <<= 4;
				streamX << std::hex << this->zero;
				tmpX ^= XSize;
			}
			streamX << std::hex << Xa;

		}

		this->X_str.append(streamX.str());
		this->Y_str.append(streamY.str());

		std::cout << std::dec;

		std::cout << '\r';
		std::cout << "\t100% !!!\n\n";
	}

	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief generate an 128 bits (32 hex chars), key with DiffieHellman algorithm: the secret need to be stored in X_str, the friend's public half key need to be stored in Y_str
	 * PS: it's necessary use the constructor with params or there may be mistakes
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief genera una chiave a 128 bits (32 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta dev'essere salvata in X_str, la mezza chiave pubblica del tuo amico dev'essere salvata in Y_str
	 * PS: e' necessario usare il construttore con parametri per non avere errori
	 */
	void DiffieHellman::Key_Generator_from_HalfKey_128bits()
	{
		//stream per salvataggio su stringa
		std::stringstream streamK;

		std::cout << "\nGenerazione chiave:\n\tCompletato:" << std::endl;

		for (int i = 0; i < this->cicli_128bits; i++)
		{
			std::cout << '\r';
			std::cout << "\t" << i * 25 << this->pc;

			unsigned int Xa = 0;
			unsigned int Yb = 0;
			unsigned int Ka = 0;

			Yb = uint32bitFromString(this->Y_str, i);
			Xa = uint32bitFromString(this->X_str, i);

			std::string aux("");
			Ka = espMod(Yb, Xa, this->q, aux);
			aux.~basic_string();

			unsigned KaSize = Ka;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (KaSize < 0x10000000)
			{
				KaSize <<= 4;
				streamK << std::hex << this->zero;
			}

			streamK << std::hex << Ka;

			std::cout << std::dec;
		}

		this->K_str.append(streamK.str());

		std::cout << std::dec;
		std::cout << '\r';
		std::cout << "\t100% !!!\n\n";
	}

	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief generate an 256 bits (64 hex chars), half-key with DiffieHellman algorithm: the secret half key will be stored in X_str, the public half key will be stored in Y_str
	 *	PS: it's necessary use the constructor without params or there may be mistakes
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief genera una mezza chiave a 256 bits (64 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta verra' salvata in X_str, la mezza chiave pubblica verra' salvata in Y_str
	 * PS: e' necessario usare il construttore senza parametri per non avere errori
	 */
	void DiffieHellman::HalfKey_Generator_256bits()
	{
		// dichiarazioni
		std::time_t t1, t2;

		//memorizzo in "t1" il tempo finale
		t1 = time(0);

		//stream per salvataggio su stringa
		std::stringstream streamY;
		std::stringstream streamX;

		unsigned int tmpX = this->q << this->alpha;
		unsigned int tmpY = this->q >> this->alpha;

		std::cout << "\nGenerazione semichiave:\n\tCompletato:" << std::endl;

		//devo ciclare 4 volte, per ogni ciclo creo una stringa da 32 bit
		for (int i = 0; i < this->cicli_256bits; i++)
		{
			std::cout << '\r';
			std::cout << "\t" << i * 12.5 << this->pc;

			//genero X < q
			unsigned int Xa = createX(tmpX ^ tmpY);
			std::string aux("");
			unsigned int Ya = espMod(this->alpha, Xa, this->q, aux);
			aux.~basic_string();

			unsigned int YSize = Ya;
			tmpY = Ya;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (YSize < 0x10000000)
			{
				YSize <<= 4;
				streamY << std::hex << this->zero;
				tmpY ^= YSize;
			}

			streamY << std::hex << Ya;

			unsigned int XSize = Xa;
			tmpX = Xa;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (XSize < 0x10000000)
			{
				XSize <<= 4;
				streamX << std::hex << this->zero;
				tmpX ^= XSize;
			}
			streamX << std::hex << Xa;

		}

		this->X_str.append(streamX.str());
		this->Y_str.append(streamY.str());

		std::cout << std::dec;

		std::cout << '\r';
		std::cout << "\t100% !!!\n\n";

		//memorizzo in "t2" il tempo finale
		t2 = time(0);

		// e stampa a video dello stesso
		std::cout << "\nTempo trascorso in secondi = " << t2 - t1 << std::endl;
	}

	//ENG DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief generate an 256 (64 hex chars), key with DiffieHellman algorithm: the secret need to be stored in X_str, the friend's public half key need to be stored in Y_str
	 * PS: it's necessary use the constructor with params or there may be mistakes
	 */
	//ITA DOC
	/*
	 * @author Riccardo Peruffo
	 * @brief genera una chiave a 256 bits (64 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta dev'essere salvata in X_str, la mezza chiave pubblica del tuo amico dev'essere salvata in Y_str
	 * PS: e' necessario usare il construttore con parametri per non avere errori
	 */
	void DiffieHellman::Key_Generator_from_HalfKey_256bits()
	{
		//stream per salvataggio su stringa
		std::stringstream streamK;

		std::cout << "\nGenerazione chiave:\n\tCompletato:" << std::endl;

		for (int i = 0; i < this->cicli_256bits; i++)
		{
			std::cout << '\r';
			std::cout << "\t" << i * 12.5 << this->pc;

			unsigned int Xa = 0;
			unsigned int Yb = 0;
			unsigned int Ka = 0;

			Yb = uint32bitFromString(this->Y_str, i);
			Xa = uint32bitFromString(this->X_str, i);

			std::string aux("");
			Ka = espMod(Yb, Xa, this->q, aux);
			aux.~basic_string();

			unsigned KaSize = Ka;

			//in caso in cui il numero sia più piccolo di 32 bit
			while (KaSize < 0x10000000)
			{
				KaSize <<= 4;
				streamK << std::hex << this->zero;
			}

			streamK << std::hex << Ka;

			std::cout << std::dec;
		}

		this->K_str.append(streamK.str());

		std::cout << std::dec;
		std::cout << '\r';
		std::cout << "\t100% !!!\n\n";
	}

	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief to obtain the var K_str
	 * @return std::string - var K_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief restituisce la variabile K_str
	 * @return std::string - var K_str
	 */
	std::string DiffieHellman::getK()
	{
		return this->K_str;
	}
	
	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief to obtain the var X_str
	 * @return std::string - var X_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief restituisce la variabile X_str
	 * @return std::string - var X_str
	 */
	std::string DiffieHellman::getX()
	{
		return this->X_str;
	}
	
	//ENG DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief to obtain the var Y_str
	 * @return std::string - var Y_str
	 */
	//ITA DOC
	/*
	 * @author Peruffo Riccardo
	 * @brief restituisce la variabile Y_str
	 * @return std::string - var Y0_str
	 */
	std::string DiffieHellman::getY()
	{
		return this->Y_str;
	}
}