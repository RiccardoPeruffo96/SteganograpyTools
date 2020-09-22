#pragma once
#ifndef IOFILE_H_
#define IOFILE_H_

#include <fstream>
#include <iostream>


class iofile
{
private:

	//ENG
	/// std::string fileName - file's name
	//ITA
	/// std::string fileName - nome del file
	std::string fileName;
	//ENG
	/// int sizeFile - data_file's length
	//ITA
	/// int sizeFile - lunghezza di data_file
	int sizeFile;
	//ENG
	/// char* data_file - file's bytes
	//ITA
	/// char* data_file - byte del file
	char* data_file;

	//ENG
	/**
	 * @author Peruffo Riccardo
	 * @brief gives value to data_file
	 * @return char* - file's bytes
	 */
	//ITA
	/**
	 * @author Peruffo Riccardo
	 * @brief rimempie il campo data_file
	 * @return char* - byte del file
	 */
	char* readFile();

public:

	/**
	 * @author Peruffo Riccardo
	 * 
	 * @brief 
	 * ENG: constructor to file reading - gives value to the private vars: std::string fileName, int sizeFile and char* data_file
	 * ITA: costruttore per solo lettura file - popola le variabili private: std::string fileName, int sizeFile and char* data_file
	 *
	 * @param fileName ENG: file's name in the program's directory ITA: nome del file nella directory del programma
	 */
	iofile(std::string fileName);

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief override
	 * ENG: constructor to write file - in the case the file doesn't exists and we want to create with our bytes.
	 * ITA: costruttore per scrittura su file - nel caso in cui il file non esiste e vogliamo crearlo con i nostri byte.
	 *
	 * @param fileName ENG: file's name in the program's directory ITA: nome del file nella directory del programma
	 * @param sizeFile ENG: bytes.length ITA: dimensione del file in byte
	 */
	//@override
	iofile(std::string fileName, int sizeFile);

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief override
	 * ENG: constructor delete - if we want delete the file and every trace of the content to the disk
	 * ITA: costruttore per cancellazione - nel caso volessimo eliminare completamente il file ed il suo contenuto dal disco fisso
	 *
	 * @param fileName ENG: file's name in the program's directory ITA: nome del file nella directory del programma
	 * @param if_there_is_this_param_you_want_cancel_the_file ENG: if this param exists, the file will be deleted ITA: indica la volota' di usare questa funzione
	 */
	//@override
	iofile(std::string fileName, bool if_there_is_this_param_you_want_cancel_the_file);

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: destructor - currently does nothing
	 * ITA: distruttore - attualmente non fa nulla
	 */
	~iofile();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief 
	 * ENG: takes a char sequence to input, outputs a file with name saved on fileName, in the same program's directory
	 * ITA: dato una sequenza di byte, crea un file con nome salvato in fileName, nella stessa cartella del programma
	 *
	 * @param bytesData ENG: bytes sequence, bytesData.length() == sizeFile ITA: sequenza di byte che verranno salvati su disco di lunghezza sizeFile
	 */
	void writeFile(char* bytesData);

	/**
	 * @author Peruffo Riccardo
	 * 
	 * @brief
	 * ENG: to obtain the var fileName
	 * ITA: restituisce la variabile fileName
	 *
	 * @return var fileName
	 */
	std::string getFileName();

	/**
	 * @author Peruffo Riccardo
	 *
	 * @brief
	 * ENG: to obtain the var sizeFile
	 * ITA: restituisce la variabile sizeFile
	 *
	 * @return var fileName
	 */
	int getSizeFile();

	/**
	 * @author Peruffo Riccardo
	 * 
	 * @brief 
	 * ENG: to obtain the var data_file
	 * ITA: restituisce la variabile data_file
	 *
	 * @return var data_file
	 */
	char* getData();
};

#endif //IOFILE_H_