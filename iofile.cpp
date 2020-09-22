#include <fstream>
#include <stdio.h>
#include <iostream>
#include "iofile.h"

/*
 * @author Peruffo Riccardo
 * @brief gives value to data_file
 * @return char* - file's bytes
 *
 * @brief rimempie il campo data_file
 * @return char* - byte del file
 */
char* iofile::readFile()
{

	std::ifstream fileRead(this->fileName, std::ios::binary | std::ios::in);

	if (!fileRead.is_open())
	{
		std::cout << "iofile.cpp - n1 : Il file non è stato aperto" << std::endl;
		return 0x00;
	}

	/*
	puntatore g per lettura e p per scrittura:
	seekg e tellg modificano g;
	seekp e tellp modificano p;
	*/

	//porto il puntatore alla fine del file
	fileRead.seekg(0, std::ios::end);
	//ritorna la mia posizione nel file
	this->sizeFile = fileRead.tellg();

	char* bytesData = new char[this->sizeFile];

	//riporto il puntatore all'inizio del file
	fileRead.seekg(0, std::ios::beg);

	//leggo ogni bit
	for (int i = 0; fileRead.get(bytesData[i]); i++);

	char byteCheck;
	if (fileRead.get(byteCheck))
		return 0x00;

	fileRead.close();

	return bytesData;
}

/*
 * @author Peruffo Riccardo
 * @brief constructor to file reading - gives value to the private vars: std::string fileName, int sizeFile and char* data_file
 * @param std::string fileName - file's name in the program's directory
 *
 * @brief costruttore per solo lettura file - popola le variabili private: std::string fileName, int sizeFile and char* data_file
 * @param std::string fileName - nome del file nella directory del programma
 */
iofile::iofile(std::string fileName)
{
	this->fileName = fileName;
	this->sizeFile = 0;
	this->data_file = this->readFile();
}

/*
 * @author Peruffo Riccardo
 * @override
 * @brief constructor to write file - in the case the file doesn't exists and we want to create with our bytes.
 * @param std::string fileName - file's name in the program's directory
 * @param int sizeFile - bytes.length
 *
 * @brief costruttore per scrittura su file - nel caso in cui il file non esiste e vogliamo crearlo con i nostri byte.
 * @param std::string fileName - nome del file nella directory del programma
 * @param int sizeFile - dimensione del file in byte
 */
iofile::iofile(std::string fileName, int sizeFile)
{
	this->fileName = fileName;
	this->sizeFile = sizeFile;
}

/*
 * @author Peruffo Riccardo
 * @override
 * @brief constructor delete - if we want delete the file and every trace of the content to the disk
 * @param std::string fileName - file's name in the program's directory
 * @param bool if_there_is_this_param_you_want_cancel_the_file - if this param exists, the file will be deleted
 *
 * @brief costruttore per cancellazione - nel caso volessimo eliminare completamente il file ed il suo contenuto dal disco fisso
 * @param std::string fileName - nome del file nella directory del programma
 * @param bool if_there_is_this_param_you_want_cancel_the_file - indica la volota' di usare questa funzione
 */
iofile::iofile(std::string fileName, bool if_there_is_this_param_you_want_cancel_the_file)
{
	this->fileName = fileName;
	this->sizeFile = 0;
	this->data_file = this->readFile(); //needs to obtain this->sizeFile value
	for (int i = 0; i < this->sizeFile; this->data_file[i] ^= this->data_file[i], i++);
	this->writeFile(this->data_file);
	delete[] this->data_file; //pulizia

	
	char* fileName_char_arr = new char[this->fileName.length() + 1];
	strcpy_s(fileName_char_arr, this->fileName.length() + 1, fileName.c_str());

	if (remove(fileName_char_arr) != 0)
		std::cout << "\nIl file per qualche motivo non e' stato cancellato!" << std::endl;
	else
		std::cout << "\nIl file e' stato correttamente sovrascritto e cancellato!" << std::endl;
	
	delete[] fileName_char_arr; //pulizia
	
}

/*
 * @author Peruffo Riccardo
 * @brief destructor - currently does nothing
 *
 * @brief costruttore - attualmente non fa nulla
 */
iofile::~iofile()
{
}

/*
 * @author Peruffo Riccardo
 * @brief takes a char sequence to input, outputs a file with name saved on fileName, in the same program's directory
 * @param char * bytesData - bytes sequence, bytesData.length() == sizeFile
 *
 * @brief dato una sequenza di byte, crea un file con nome salvato in fileName, nella stessa cartella del programma
 * @param char * bytesData - sequenza di byte che verranno salvati su disco di lunghezza sizeFile
 */
void iofile::writeFile(char* bytesData)
{
	std::ofstream outfile(this->fileName, std::ios::binary | std::ios::out);
	outfile.write(bytesData, this->sizeFile);
	outfile.close();
}

/*
 * @author Peruffo Riccardo
 * @brief to obtain the var fileName
 * @return std::string - var fileName
 *
 * @brief restituisce la variabile fileName
 * @return std::string - var fileName
 */
std::string iofile::getFileName()
{
	return this->fileName;
}

/*
 * @author Peruffo Riccardo
 * @brief to obtain the var sizeFile
 * @return int - var sizeFile
 *
 * @brief restituisce la variabile sizeFile
 * @return int - var fileName
 */
int iofile::getSizeFile()
{
	return this->sizeFile;
}

/*
 * @author Peruffo Riccardo
 * @brief to obtain the var data_file
 * @return char* - var data_file
 *
 * @author Peruffo Riccardo
 * @brief restituisce la variabile data_file
 * @return char* - var data_file
 */
char* iofile::getData()
{
	return this->data_file;
}