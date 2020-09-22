#pragma once
#ifndef STEGANOGRAPHY_H_
#define STEGANOGRAPHY_H_

#include <fstream>
#include <iostream>
#include "opencv2\opencv.hpp"

class steganography
{
public:

	/**
	 * @author Peruffo Riccardo
	 * 
	 * @brief
	 * ENG: constructor - currently does nothing
	 * ITA: costruttore - attualmente non fa nulla
	 */
	steganography();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: destructor - currently does nothing
	 * ITA: distruttore - attualmente non fa nulla
	 */
	~steganography();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief
	 * ENG: create a copy of the picture, with hide inside the file datas. Picture and file need to be in the same directory of the program.
	 * ITA: prendendo in input un'immagine ed un file, restituisce una copia dell'immagine con nascosta al suo interno il file. Immagine e file devono essere nella stessa cartella del programma
	 * 
	 * @param img_name_input ENG: input's picture's name ITA: nome dell'immagine in input
	 * @param file_name_input ENG: input's file's name to hide ITA: nome del file in input da nascondere
	 * @param bytes_data_file ENG: datas' file ITA: byte del file
	 * @param bytes_data_length ENG: bytes_data_file.length() ITA: lunghezza di bytes_data_file
	 * @param img_name_output ENG: output's picture's name ITA: nome dell'immagine in output
	 */
	void createMergeFile(std::string img_name_input, std::string file_name_input, char* bytes_data_file, int bytes_data_length, std::string img_name_output);

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief
	 * ENG: takes a img with hidden file inside as input, outputs the hidden file.
	 * ITA: dato in input un'immagine con un file nascosto al suo interno, restituisce il file. L'immagine dev'essere nella stessa cartella del file.
	 *
	 * @param img_nome_file ENG: input's picture's name ITA: nome dell'immagine in input
	 * @param name_hide_file ENG: empty string that will be padded with the filename ITA: stringa vuota che conterra' il nome del file nascosto
	 * @param data_file_length ENG: empty integer, will be padded with the return's dimension ITA: variabile vuota che conterra' la lunghezza di return
	 * @return ENG: hidden byte's file ITA: sequenza di byte del file
	 */
	char* DeMergeFile(std::string img_nome_file, std::string& name_hide_file, int& data_file_length);
};


#endif //STEGANOGRAPHY_H_
#pragma once