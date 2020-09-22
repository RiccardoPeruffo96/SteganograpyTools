#pragma once
#ifndef DIFFIEHELLMAN_H_
#define DIFFIEHELLMAN_H_

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

namespace DfHl
{
	class DiffieHellman
	{
	private:

		//ENG
		///prime number max rappresentabile with 32 bit
		//ITA
		///numero primo massimo rappresentabile con 32 bit
		const unsigned int q = 0xFFFFFFFB;
		//ENG
		///coprime integers with q, so gcd(q, alpha) = 1
		//ITA
		///numero coprimo di q, quindi mcd(q, alpha) = 1
		const unsigned int alpha = 0x2;

		//ENG
		///auxiliary strings (std::string)
		//ITA
		///stringhe ausiliari (std::string)
		std::string X_str;
		std::string Y_str;
		std::string K_str;

		//ENG
		///auxiliary constants vars
		//ITA
		///varibili costanti ausiliari
		const int zero = 0;
		const int cicli_128bits = 4;
		const int cicli_256bits = 8;
		const char pc = '%';

		//ENG
		/**
		 * @author Riccardo Peruffo
		 * @brief compute (Y ^ X) % q with recursion, complexity: O(log(X)) with tail recursion
		 * @param unsigned int Y - base
		 * @param unsigned int X - exponent
		 * @param unsigned int q - mod
		 * @param std::string auxiliary - aux string used by tail recursion
		 * @return unsigned int - (Y ^ X) % q
		 */
		//ITA
		/**
		 * @author Riccardo Peruffo
		 * @brief calcola (Y ^ X) % q ricorsivamente, complessita': O(log(X)) con ricorsione in coda
		 * @param unsigned int Y - base
		 * @param unsigned int X - esponente
		 * @param unsigned int q - modulo
		 * @param std::string auxiliary - string ausiliria usata per la ricorsione in coda
		 * @return unsigned int - (Y ^ X) % q
		 */
		unsigned int espMod(unsigned int Y, unsigned int X, unsigned int q, std::string auxiliary);

		//ENG
		/**
		 * @author Riccardo Peruffo
		 * @brief extract a uint 32 bit from string: every string's char it's a hex number and input.length() mod 8 == 0.
		 * @param std::string input - input.length() mod 8 == 0. every char it's a hex number
		 * @param int index - offset starting to extract the number
		 * @return unsigned int - the number between 0 and 2^32-1 extracted starting from input[index * 8] and input[(index * 8) + 8]
		 */
		//ITA
		/**
		 * @author Riccardo Peruffo
		 * @brief estrae un uint di 32 bit da una stringa: ogni carattere e' un esadecimale e la stringa ha un numero di caratteri divisibile per 8.
		 * @param std::string input - input.length() mod 8 == 0, rappresenta un numero a 128 o 256 bit: ogni carattere e' un esadecimale
		 * @param int index - rappresenta l'ottetto che si vuole estrarre
		 * @return unsigned int - numero tra 0 e 2^32-1 estratto a tra input[index * 8] e input[(index * 8) + 8]
		 */
		unsigned int uint32bitFromString(std::string input, int index);

		//ENG
		/**
		 * @author Riccardo Peruffo
		 * @brief create a random number smallest then q
		 * @param unsigned int modificatore - random number that help to create another randoms numbers
		 * @return unsigned int - a random number between [1; (this->q - 1)]
		 */
		//ITA
		/**
		 * @author Riccardo Peruffo
		 * @brief create a random number smallest then q
		 * @param unsigned int modificatore - un numero random che aiuta a creare altri numeri random
		 * @return unsigned int - un valore randomico compreso tra [1; (this->q - 1)]
		 */
		unsigned int createX(unsigned int modificatore);

	public:

		/**
		 * @author Peruffo Riccardo
		 * 
		 * @brief 
		 * ENG: constructor to create first half key - gives empty values to the private vars: std::string X_str, std::string Y_str, std::string K_str
		 * ITA: costruttore per creare la prima mezza chiave - popola con valori nulli le variabili private: std::string X_str, std::string Y_str, std::string K_str
		 */
		DiffieHellman();
		
		/**
		 * @author Peruffo Riccardo
		 * 
		 * @brief 
		 * ENG: constructor to create second half key - gives empty value to the private vars: std::string K_str
		 * ITA: costruttore per creare mezze chiavi - popola con valori nulli la variabile privata: std::string K_str
		 *
		 * @param Xa_str ENG: value that will be padded in X_str ITA: valore che verra' inserito in X_str
		 * @param Yb_str ENG: value that will be padded in Y_str ITA: valore che verra' inserito in Y_str
		 */
		DiffieHellman(std::string Xa_str, std::string Yb_str);
		
		/**
		 * @author Peruffo Riccardo
		 *
		 * @brief 
		 * ENG: destructor - delete the private vars: std::string X_str, std::string Y_str, std::string K_str
		 * ITA: distruttore - cancella le variabili private: std::string X_str, std::string Y_str, std::string K_str
		 */
		~DiffieHellman();

		/**
		 * @author Riccardo Peruffo
		 *
		 * @brief 
		 * ENG: generate an 128 bits (32 hex chars), half-key with DiffieHellman algorithm: the secret half key will be stored in X_str, the public half key will be stored in Y_str
		 *	It's necessary use the constructor without params or there may be mistakes
		 * ITA: genera una mezza chiave a 128 bits (32 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta verra' salvata in X_str, la mezza chiave pubblica verra' salvata in Y_str
		 * E' necessario usare il construttore senza parametri per non avere errori
		 */
		void HalfKey_Generator_128bits();

		/**
		 * @author Riccardo Peruffo
		 *
		 * @brief 
		 * ENG: generate an 128 bits (32 hex chars), key with DiffieHellman algorithm: the secret need to be stored in X_str, the friend's public half key need to be stored in Y_str
		 * It's necessary use the constructor with params or there may be mistakes
		 * ITA: genera una chiave a 128 bits (32 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta dev'essere salvata in X_str, la mezza chiave pubblica del tuo amico dev'essere salvata in Y_str
		 * E' necessario usare il construttore con parametri per non avere errori
		 */
		void Key_Generator_from_HalfKey_128bits();

		/**
		 * @author Riccardo Peruffo
		 *
		 * @brief
		 * ENG: generate an 256 bits (64 hex chars), half-key with DiffieHellman algorithm: the secret half key will be stored in X_str, the public half key will be stored in Y_str
		 *	It's necessary use the constructor without params or there may be mistakes
		 * ITA: genera una mezza chiave a 256 bits (64 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta verra' salvata in X_str, la mezza chiave pubblica verra' salvata in Y_str
		 * E' necessario usare il construttore senza parametri per non avere errori
		 */
		void HalfKey_Generator_256bits();

		/**
		 * @author Riccardo Peruffo
		 * @brief 
		 * ENG: generate an 256 (64 hex chars), key with DiffieHellman algorithm: the secret need to be stored in X_str, the friend's public half key need to be stored in Y_str
		 * It's necessary use the constructor with params or there may be mistakes
		 * ITA: genera una chiave a 256 bits (64 caratteri esadecimali), usando l'algoritmo DiffieHellman: la mezza chiave segreta dev'essere salvata in X_str, la mezza chiave pubblica del tuo amico dev'essere salvata in Y_str
		 * E' necessario usare il construttore con parametri per non avere errori
		 */
		void Key_Generator_from_HalfKey_256bits();

		/**
		 * @author Peruffo Riccardo
		 *
		 * @brief 
		 * ENG: to obtain the var K_str
		 * ITA: restituisce la variabile K_str
		 *
		 * @return var K_str
		 */
		std::string getK();
		
		/**
		 * @author Peruffo Riccardo
		 *
		 * @brief
		 * ENG: to obtain the var X_str
		 * ITA: restituisce la variabile X_str
		 *
		 * @return var X_str
		 */
		std::string getX();
		
		/**
		 * @author Peruffo Riccardo
		 *
		 * @brief 
		 * ENG: to obtain the var Y_str
		 * ITA: restituisce la variabile Y_str
		 *
		 * @return var Y_str
		 */
		std::string getY();
	};
}

#endif //DIFFIEHELLMAN_H_
#pragma once