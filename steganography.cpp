#include <fstream>
#include <iostream>
#include "steganography.h"
#include <opencv2\opencv.hpp>

//ENG DOC
/*
 * @author Peruffo Riccardo
 * @brief constructor - currently does nothing
 */
//ITA DOC
/*
 * @author Peruffo Riccardo
 * @brief costruttore - attualmente non fa nulla
 */
steganography::steganography()
{
}

//ENG DOC
/*
 * @author Peruffo Riccardo
 * @brief destructor - currently does nothing
 */
//ITA DOC
/*
 * @author Peruffo Riccardo
 * @brief costruttore - attualmente non fa nulla
 */
steganography::~steganography()
{
}

//ENG DOC
/*
 * @author Peruffo Riccardo
 * @brief create a copy of the picture, with hide inside the file datas. Picture and file need to be in the same directory of the program.
 * @param std::string img_name_input - input's picture's name
 * @param std::string file_name_input - input's file's name to hide
 * @param char* bytes_data_file - datas' file
 * @param int bytes_data_length - bytes_data_file.length()
 * @param std::string img_name_output - output's picture's name
 */
//ITA DOC
/*
 * @author Peruffo Riccardo
 * @brief prendendo in input un'immagine ed un file, restituisce una copia dell'immagine con nascosta al suo interno il file. Immagine e file devono essere nella stessa cartella del programma
 * @param std::string img_name_input - nome dell'immagine in input
 * @param std::string file_name_input - nome del file in input da nascondere
 * @param char* bytes_data_file - byte del file
 * @param int bytes_data_length - lunghezza di bytes_data_file
 * @param std::string img_name_output - nome dell'immagine in output
 */
void steganography::createMergeFile(std::string img_name_input, std::string file_name_input, char* bytes_data_file, int bytes_data_length, std::string img_name_output)
{
	//std::string img_name_inputconEst = img_name_input + ".jpg";
	cv::Mat img = cv::imread(img_name_input, cv::IMREAD_COLOR);

	unsigned long long int rows;
	unsigned long long int cols;

	int r;
	int g;
	int b;

	int arrLTotal = (sizeof(int) * 3) + bytes_data_length + file_name_input.length(); //è il numero di byte da nascondere
	//in ordine mettiamo:
	//1. la lunghezza di bytes_data_length
	//2. il contenuto di bytes_data_length
	//3. la lunghezza di file_name_input
	//4. il contenuto di file_name_input

	//porto tutte le info necessarie in un unico char*
	unsigned char* dataToPrint = new unsigned char[arrLTotal];

	int value;
	unsigned char atomicL;
	int index = 0;
	int i;

	//0. la lunghezza di totale
	value = arrLTotal;
	for (i = 0; i < sizeof(int); i++)
	{
		atomicL = value >> ((sizeof(int) - ((unsigned int)i + 1)) * 8);
		dataToPrint[index++] = atomicL;

		/*
		unsigned int prova;
		prova = atomicL; std::cout << "at: " << prova << std::endl;
		prova = dataToPrint[index - 1]; std::cout << "dtp: " << prova << std::endl; */
	}

	//1. la lunghezza di bytes_data_length
	//ci mettiamo i byte più significativi di bytes_data_length
	value = bytes_data_length;
	for (i = 0; i < sizeof(int); i++)
	{
		atomicL = value >> ((sizeof(int) - ((unsigned int)i + 1)) * 8);
		dataToPrint[index++] = atomicL;
		/*
		unsigned int prova;
		prova = atomicL; std::cout << "at: " << prova << std::endl;
		prova = dataToPrint[index - 1]; std::cout << "dtp: " << prova << std::endl;
		*/
	}

	//2. la contenuto di bytes_data_length
	//passo ogni cella di bytes_data_file
	for (i = 0; i < bytes_data_length; i++)
	{
		dataToPrint[index++] = bytes_data_file[i];
	}

	//3. la lunghezza di file_name_input
	//ci mettiamo i byte più significativi di file_name_input
	value = file_name_input.length();
	for (i = 0; i < sizeof(int); i++)
	{
		atomicL = value >> ((sizeof(int) - ((unsigned int)i + 1)) * 8);
		dataToPrint[index++] = atomicL;
	}

	//4. la contenuto di file_name_input
	//passo ogni char del nome
	for (i = 0; i < file_name_input.length(); i++)
	{
		dataToPrint[index++] = file_name_input[i];
	}

	//5. Cripto la std::stringa di char
	//cript(dataToPrint);

	//6. Inserisco i dati nell'immagine
	index = 0;
	char colorToChange = 0;
	char posix = 7;
	unsigned char unitBit;

	long int maxSpace = img.rows * img.cols * 3;
	long int spaceNeed = arrLTotal * 8;

	char Iuse3Color;

	/*
	ofstream fpixel("pixelOriginaliBit.txt");
	ofstream fpixelp("pixelModificatiBit.txt");
	ofstream fpixeln("pixelOriginaliNumeri.txt");
	ofstream fpixelpn("pixelModificatiNumeri.txt");
	ofstream ld("lastdata.txt");
	*/
	if (maxSpace > spaceNeed)
	{
		for (rows = 0; rows < img.rows; rows++)
		{
			for (cols = 0; cols < img.cols; cols++)
			{
				if (index < arrLTotal)
				{
					cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

					b = color[0];
					g = color[1];
					r = color[2];
					/*
					fpixel << "r: " << rows << "; c: " << cols << "; r: " << r % 2 << "; g: " << g % 2 << "; b: " << b % 2 << std::endl;
					fpixeln << "r: " << rows << "; c: " << cols << "; r: " << r << "; g: " << g << "; b: " << b << std::endl;
					*/
					Iuse3Color = 0;

					do
					{
						//let's start
						switch (colorToChange)
						{
						case 0:
							//ottengo il bit che voglio
							unitBit = dataToPrint[index] >> posix;
							/*
							ld << "Data: (unsigned char) " << (unsigned char)dataToPrint[index];
							ld << "; Data: (char) " << (char)dataToPrint[index] << "; Data: (bit) ";
							for (int ii = (sizeof(dataToPrint[index]) * 8) - 1; ii >= 0; ld << ((dataToPrint[index] >> ii) % 2), ii--);
							ld << "; Data: (unsigned int) " << (unsigned int)dataToPrint[index];
							ld << "; Data: (int) " << (int)dataToPrint[index];
							ld << "; value: " << (int) unitBit;
							ld << "; posix: " << (int) posix << std::endl;
							*/
							//confronto il bit di parità di r con quello che devo salvare
							if ((r % 2) != (unitBit % 2))
							{
								//se sono diversi
								//guardo se il bit di parità è a 0 e lo incremento, se invece è a 1 lo decremento
								(r % 2 == 0) ? r++ : r--;
							}

							//posix è la posizione del bit che ci interessa, se è a 0 non dobbiamo andare ancora più sotto
							if (posix == 0)
							{
								//ritorniamo a 7 in caso
								posix = 7;
								//e passiamo al blocco successivo
								index++;
							}
							else
							{
								//altrimenti passiamo al bit dopo
								posix--;
							}

							//e cambiamo colore per su cui stiparla
							colorToChange++;

							break;

						case 1:
							//ottengo il bit che voglio
							unitBit = dataToPrint[index] >> posix;
							/*
							ld << "Data: (unsigned char) " << (unsigned char)dataToPrint[index];
							ld << "; Data: (char) " << (char)dataToPrint[index] << "; Data: (bit) ";
							for (int ii = (sizeof(dataToPrint[index]) * 8) - 1; ii >= 0; ld << ((dataToPrint[index] >> ii) % 2), ii--);
							ld << "; Data: (unsigned int) " << (unsigned int)dataToPrint[index];
							ld << "; Data: (int) " << (int)dataToPrint[index];
							ld << "; value: " << unitBit;
							ld << "; posix: " << (int)posix << std::endl;
							*/
							//confronto il bit di parità di r con quello che devo salvare
							if ((g % 2) != (unitBit % 2))
							{
								//se sono diversi
								//guardo se il bit di parità è a 0 e lo incremento, se invece è a 1 lo decremento
								(g % 2 == 0) ? g++ : g--;
							}

							//posix è la posizione del bit che ci interessa, se è a 0 non dobbiamo andare ancora più sotto
							if (posix == 0)
							{
								//ritorniamo a 7 in caso
								posix = 7;
								//e passiamo al blocco successivo
								index++;
							}
							else
							{
								//altrimenti passiamo al bit dopo
								posix--;
							}

							//e cambiamo colore per su cui stiparla
							colorToChange++;

							break;

						case 2:
							//ottengo il bit che voglio
							unitBit = dataToPrint[index] >> posix;
							/*
							ld << "Data: (unsigned char) " << (unsigned char)dataToPrint[index];
							ld << "; Data: (char) " << (char)dataToPrint[index] << "; Data: (bit) ";
							for (int ii = (sizeof(dataToPrint[index]) * 8) - 1; ii >= 0; ld << ((dataToPrint[index] >> ii) % 2), ii--);
							ld << "; Data: (unsigned int) " << (unsigned int)dataToPrint[index];
							ld << "; Data: (int) " << (int)dataToPrint[index];
							ld << "; value: " << unitBit;
							ld << "; posix: " << (int)posix << std::endl;
							*/
							//confronto il bit di parità di r con quello che devo salvare
							if ((b % 2) != (unitBit % 2))
							{
								//se sono diversi
								//guardo se il bit di parità è a 0 e lo incremento, se invece è a 1 lo decremento
								(b % 2 == 0) ? b++ : b--;
							}

							//posix è la posizione del bit che ci interessa, se è a 0 non dobbiamo andare ancora più sotto
							if (posix == 0)
							{
								//ritorniamo a 7 in caso
								posix = 7;
								//e passiamo al blocco successivo
								index++;
							}
							else
							{
								//altrimenti passiamo al bit dopo
								posix--;
							}

							//e cambiamo colore per su cui stiparla
							colorToChange = 0;

							Iuse3Color++;

							break;

						default:
							std::cout << "something wrong" << std::endl;
							break;
						}
					} while (Iuse3Color == 0);
					/*
					fpixelp << "r: " << rows << "; c: " << cols << "; r: " << r % 2 << "; g: " << g % 2 << "; b: " << b % 2 << std::endl;
					fpixelpn << "r: " << rows << "; c: " << cols << "; r: " << r << "; g: " << g << "; b: " << b << std::endl;
					*/
					//salviamo il risultato
					color[0] = b;
					color[1] = g;
					color[2] = r;
				}
				else
				{
					//altrimenti terminiamo
					cols = img.cols;
					rows = img.rows;
				}
			}
		}
	}
	else
	{
		std::cout << "the img it's too little" << std::endl;
	}
	/*
	fpixel.close();
	fpixelp.close();
	fpixeln.close();
	fpixelpn.close();
	ld.close();*/
	delete[] dataToPrint;
	/*
	std::string img_name_output_plus_estension = "";
	img_name_output_plus_estension.append(img_name_output);
	img_name_output_plus_estension.append(".png");*/
	imwrite(img_name_output + ".png", img);
}

//ENG DOC
/*
 * @author Peruffo Riccardo
 * @brief takes a img with hidden file inside as input, outputs the hidden file.
 * @param std::string img_nome_file - input's picture's name
 * @param std::string& name_hide_file - empty string that will be padded with the filename
 * @param int& data_file_length - empty integer, will be padded with the return's dimension
 * @return char* - hidden byte's file
 */
//ITA DOC
/*
 * @author Peruffo Riccardo
 * @brief dato in input un'immagine con un file nascosto al suo interno, restituisce il file. L'immagine dev'essere nella stessa cartella del file.
 * @param std::string img_nome_file - nome dell'immagine in input
 * @param std::string& name_hide_file - stringa vuota che conterra' il nome del file nascosto
 * @param int& data_file_length - variabile vuota che conterra' la lunghezza di return
 * @return char* - sequenza di byte del file
 */
char* steganography::DeMergeFile(std::string img_nome_file, std::string& name_hide_file, int& data_file_length)
{
	cv::Mat img = cv::imread(img_nome_file, cv::IMREAD_COLOR);

	//decript(dataToPrint);

	unsigned long long int rows;
	unsigned long long int cols;

	int r;
	int g;
	int b;

	int arrLTotal = 0; //è il numero di byte nascosti
	data_file_length = 0;

	char* dataToPrint = new char[1]; //è il file
	char* nameFileArr = new char[1]; //nome del file

	int fileNameLength = 0;

	int index = 0;
	char colorToChange = 0;
	char posix = 0;
	char unitBit = 0;

	char Iuse3Color;
	char IhaveL = 0;
	char IhaveLD = 0;
	char IhaveMex = 0;
	char IhaveFileNameLength = 0;
	char IhaveFileName = 0;

	unsigned char byteLetto = 0;
	char checkLvl = 0;
	char checkLvl2 = 0;
	char checkLvl3 = 0;

	/*
	ofstream fnlb("imgBitLetti.txt");
	ofstream fnln("imgNumLetti.txt");
	*/
	for (rows = 0; rows < img.rows; rows++)
	{
		for (cols = 0; cols < img.cols; cols++)
		{
			if (IhaveL == 0) //se non ho ottenuto la lunghezza di arrLTotal
			{
				//isolo il pixel
				cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

				b = color[0];
				g = color[1];
				r = color[2];

				//ed identifico che devo utilizzare 3 colori
				Iuse3Color = 0;

				do
				{
					//let's start
					switch (colorToChange)
					{
					case 0:

						arrLTotal *= 2; //faccio posto
						arrLTotal += (r % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri del byte
						if (index == sizeof(int) * 8)
						{
							index = 0;
							IhaveL++;
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 1:

						arrLTotal *= 2; //faccio posto
						arrLTotal += (g % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri del byte
						if (index == sizeof(int) * 8)
						{
							index = 0;
							IhaveL++;
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 2:

						arrLTotal *= 2; //faccio posto
						arrLTotal += (b % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri del byte
						if (index == sizeof(int) * 8)
						{
							index = 0;
							IhaveL++;
						}

						//e cambiamo colore
						colorToChange = 0;

						Iuse3Color++;

						break;

					default:
						std::cout << "something wrong" << std::endl;
						break;
					}
				} while (Iuse3Color == 0 && IhaveL == 0);
			}
			else if (IhaveLD == 0) //se ho ottenuto la lunghezza di data_file_length
			{
				//questo pezzo di codice deve avverarsi solo una volta da quando entro in questo if:
				//Se la lettura dei dati nell'if precedente si ferma al bit rosso o verde,
				//entrando nel nuovo if a causa dell'iterazione del ciclo, leggeremo i pixel delle nuove coordinate,
				//perdendo di fatto il bit contenuto nel blu oppure nel verde e nel blu.
				//Per risolvere questo, questo pezzo di codice viene attivato quando:
				//Iuse3Color == 0 -> significa che nel do while precedente non avevamo completato la lettura di tutti e 3 i pixel
				//IhaveL == 1 -> significa che nel do while precedente avevamo comunque ottenuto ciò che ci serviva
				//checkLvl == 0 -> Se siamo qui per la prima volta
				if (Iuse3Color == 0 && IhaveL == 1 && checkLvl == 0)
				{
					//dobbiamo ottenere le ultime coordinate utilizzate
					//se cols > 0 ci basta indietreggiare con le colonne
					if (cols > 0)
					{
						cols--;
					}
					//altrimenti dobbiamo indietreggiare con le righe e portare la colonna all'ultima possibile
					else
					{
						rows--;
						cols = img.cols - 1;
					}
					checkLvl++;
				}
				else if (checkLvl == 0)
				{
					checkLvl++;
				}

				cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

				b = color[0];
				g = color[1];
				r = color[2];

				Iuse3Color = 0;

				do
				{
					//let's start
					switch (colorToChange)
					{
					case 0:

						data_file_length *= 2; //faccio posto
						data_file_length += (r % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveLD++;
							index = 0;
							delete[] dataToPrint;
							dataToPrint = new char[data_file_length];
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 1:

						data_file_length *= 2; //faccio posto
						data_file_length += (g % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveLD++;
							index = 0;
							delete[] dataToPrint;
							dataToPrint = new char[data_file_length];
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 2:

						data_file_length *= 2; //faccio posto
						data_file_length += (b % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveLD++;
							index = 0;
							delete[] dataToPrint;
							dataToPrint = new char[data_file_length];
						}

						//e cambiamo colore
						colorToChange = 0;

						Iuse3Color++;

						break;

					default:
						std::cout << "something wrong" << std::endl;
						break;
					}
				} while (Iuse3Color == 0 && IhaveLD == 0);
			}
			//arrivati qua si ricomincia salvando ogni byte
			else if (index < data_file_length && checkLvl2 != 2) //qua si parla di ogni byte dell'immagine
			{
				//questo pezzo di codice deve avverarsi solo una volta da quando entro in questo if:
				//Se la lettura dei dati nell'if precedente si ferma al bit rosso o verde,
				//entrando nel nuovo if a causa dell'iterazione del ciclo, leggeremo i pixel delle nuove coordinate,
				//perdendo di fatto il bit contenuto nel blu oppure nel verde e nel blu.
				//Per risolvere questo, questo pezzo di codice viene attivato quando:
				//Iuse3Color == 0 -> significa che nel do while precedente non avevamo completato la lettura di tutti e 3 i pixel
				//IhaveLD == 1 -> significa che nel do while precedente avevamo comunque ottenuto ciò che ci serviva
				//checkLvl2 == 0 -> Se siamo qui per la prima volta
				if (Iuse3Color == 0 && IhaveLD == 1 && checkLvl2 == 0)
				{
					//dobbiamo ottenere le ultime coordinate utilizzate
					//se cols > 0 ci basta indietreggiare con le colonne
					if (cols > 0)
					{
						cols--;
					}
					//altrimenti dobbiamo indietreggiare con le righe e portare la colonna all'ultima possibile
					else
					{
						rows--;
						cols = img.cols - 1;
					}
					checkLvl2++; //se checkLvl2 == 1 allora rimaniamo dentro questo blocco
				}
				else if (checkLvl2 == 0)
				{
					checkLvl2++;
				}

				cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

				b = color[0];
				g = color[1];
				r = color[2];

				Iuse3Color = 0;

				do
				{
					//let's start
					switch (colorToChange)
					{
					case 0:
						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += r % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							dataToPrint[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= data_file_length)
								IhaveMex++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange++;

						break;

					case 1:

						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += g % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							dataToPrint[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= data_file_length)
								IhaveMex++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange++;

						break;

					case 2:

						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += b % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							dataToPrint[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= data_file_length)
								IhaveMex++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange = 0;

						Iuse3Color++;

						break;

					default:
						std::cout << "something wrong" << std::endl;
						break;
					}
				} while (Iuse3Color == 0 && IhaveMex == 0);
			}
			//lettura numeri caratteri nome
			else if (IhaveFileNameLength == 0)
			{
				//questo pezzo di codice deve avverarsi solo una volta da quando entro in questo if:
				//Se la lettura dei dati nell'if precedente si ferma al bit rosso o verde,
				//entrando nel nuovo if a causa dell'iterazione del ciclo, leggeremo i pixel delle nuove coordinate,
				//perdendo di fatto il bit contenuto nel blu oppure nel verde e nel blu.
				//Per risolvere questo, questo pezzo di codice viene attivato quando:
				//Iuse3Color == 0 -> significa che nel do while precedente non avevamo completato la lettura di tutti e 3 i pixel
				//IhaveMex == 1 -> significa che nel do while precedente avevamo comunque ottenuto ciò che ci serviva
				//checkLvl2 != 2 -> Se siamo qui per la prima volta, significa che (index >= data_file_length) e che (checkLvl2 == 1)
				//	quindi sarà questo codice a fare checkLvl2 = 2
				if (Iuse3Color == 0 && IhaveMex == 1 && checkLvl2 != 2)
				{
					//dobbiamo ottenere le ultime coordinate utilizzate
					//se cols > 0 ci basta indietreggiare con le colonne
					if (cols > 0)
					{
						cols--;
					}
					//altrimenti dobbiamo indietreggiare con le righe e portare la colonna all'ultima possibile
					else
					{
						rows--;
						cols = img.cols - 1;
					}
					checkLvl2 = 2; //per non entrare più nel blocco di prima e poter riutilizzare index anche se index < data_file_length
					index = 0;
					//std::cout << "\nAlT: " << arrLTotal << "; Dl: " << data_file_length << "; firstByte: " << (int)dataToPrint[0] << std::endl;
				}
				else if (checkLvl2 != 2)
				{
					checkLvl2 = 2; //per non entrare più nel blocco di prima e poter riutilizzare index anche se index < data_file_length
					index = 0;
					//std::cout << "\nAlT: " << arrLTotal << "; Dl: " << data_file_length << "; firstByte: " << (int)dataToPrint[0] << std::endl;
				}

				cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

				b = color[0];
				g = color[1];
				r = color[2];

				Iuse3Color = 0;

				do
				{
					//let's start
					switch (colorToChange)
					{
					case 0:

						fileNameLength *= 2; //faccio posto
						fileNameLength += (r % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveFileNameLength++;
							index = 0;
							delete[] nameFileArr;
							nameFileArr = new char[fileNameLength];
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 1:

						fileNameLength *= 2; //faccio posto
						fileNameLength += (g % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveFileNameLength++;
							index = 0;
							delete[] nameFileArr;
							nameFileArr = new char[fileNameLength];
						}

						//e cambiamo colore
						colorToChange++;

						break;

					case 2:

						fileNameLength *= 2; //faccio posto
						fileNameLength += (b % 2); //salvo il bit meno significativo
						index++;

						//se abbiamo letto tutti i possibili caratteri dell'intero
						if (index == sizeof(int) * 8)
						{
							IhaveFileNameLength++;
							index = 0;
							delete[] nameFileArr;
							nameFileArr = new char[fileNameLength];
						}

						//e cambiamo colore
						colorToChange = 0;

						Iuse3Color++;

						break;

					default:
						std::cout << "something wrong" << std::endl;
						break;
					}
				} while (Iuse3Color == 0 && IhaveFileNameLength == 0);
			}
			//lettura del nome
			else if (IhaveFileName == 0 && index < fileNameLength)
			{
				//questo pezzo di codice deve avverarsi solo una volta da quando entro in questo if:
				//Se la lettura dei dati nell'if precedente si ferma al bit rosso o verde,
				//entrando nel nuovo if a causa dell'iterazione del ciclo, leggeremo i pixel delle nuove coordinate,
				//perdendo di fatto il bit contenuto nel blu oppure nel verde e nel blu.
				//Per risolvere questo, questo pezzo di codice viene attivato quando:
				//Iuse3Color == 0 -> significa che nel do while precedente non avevamo completato la lettura di tutti e 3 i pixel
				//IhaveFileNameLength == 1 -> significa che nel do while precedente avevamo comunque ottenuto ciò che ci serviva
				//checkLvl3 == 0 -> Significa che siamo qui per la prima volta
				if (Iuse3Color == 0 && IhaveFileNameLength == 1 && checkLvl3 == 0)
				{
					//dobbiamo ottenere le ultime coordinate utilizzate
					//se cols > 0 ci basta indietreggiare con le colonne
					if (cols > 0)
					{
						cols--;
					}
					//altrimenti dobbiamo indietreggiare con le righe e portare la colonna all'ultima possibile
					else
					{
						rows--;
						cols = img.cols - 1;
					}
					checkLvl3++;
					unitBit = 0;
					posix = 0;
					//std::cout << "lunghezza nome file: " << (int)fileNameLength << std::endl;
				}
				else if (checkLvl3 == 0)
				{
					checkLvl3++;
					unitBit = 0;
					posix = 0;
					//std::cout << "lunghezza nome file: " << (int)fileNameLength << std::endl;
				}

				cv::Vec3b& color = img.at<cv::Vec3b>(cv::Point(cols, rows));

				b = color[0];
				g = color[1];
				r = color[2];

				Iuse3Color = 0;

				do
				{
					//let's start
					switch (colorToChange)
					{
					case 0:
						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += r % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							nameFileArr[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= fileNameLength)
								IhaveFileName++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange++;

						break;

					case 1:

						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += g % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							nameFileArr[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= fileNameLength)
								IhaveFileName++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange++;

						break;

					case 2:

						//ottengo il bit che voglio
						unitBit *= 2; //faccio spazio
						unitBit += b % 2; //salvo il bit di disparità
						posix++; //conta il numero di pixel letti

						//abbiamo letto un byte
						if (posix == 8)
						{
							nameFileArr[index++] = unitBit; //assegno il byte alla sua posizione
							posix = 0;
							unitBit = 0;
							if (index >= fileNameLength)
								IhaveFileName++;
						}

						//e cambiamo colore per su cui stiparla
						colorToChange = 0;

						Iuse3Color++;

						break;

					default:
						std::cout << "something wrong" << std::endl;
						break;
					}
				} while (Iuse3Color == 0 && IhaveFileName == 0);
			}
			else
			{
				//altrimenti terminiamo
				cols = img.cols;
				rows = img.rows;
			}
		}
	}/*
	fnlb.close();
	fnln.close();*/

	/*
	ofstream d("datasLetti.txt");
	for (int i = 0; i < data_file_length; i++)
		d << i << ") " << (int) dataToPrint[i] << std::endl;
	d.close();*/

	for (int i = 0; i < fileNameLength; i++)
	{
		name_hide_file += nameFileArr[i];
	}

	delete[] nameFileArr;
	return dataToPrint;
}
