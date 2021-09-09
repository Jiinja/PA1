/*****************************************************************************************
* Programmer: Josh Maloy                                                                 *
* Class: CptS 122, Fall 2021; Lab Section 1                                              *
* Programming Assignment: PA1                                                            *
* Date: September 1, 2021                                                                *
* Description: This program reads a file and filters/organizes it into an array of structs.
* The program calculates specific data points and outputs the filtered data in addition  *
* to the data points to a file called "Results.csv"                                      s*
******************************************************************************************/

#include "Header.h" //including header

int main(void)
{	
	FitbitData fitbitdata[1440] = { {"", "", 0, 0, 0, 0, 0, 0,} }; //initializing the struct
	FILE* infile = fopen("FitbitData.csv", "r"); //setting up file stream - "r" means read
	int lineNumber = 0; //to traverse through the document
	char line[75] = ""; //initializing string to use as line by line copier
	if (infile != NULL) //checking if the file was opened 
	{
		char patientNumber[15] = "";
		fgets(line, 75, infile);
		printf("File opened successfully\n");
		strtok(line, ",");                         //to get past "initial target:"
		strcpy(patientNumber, strtok(NULL, ","));
		fgets(line, 75, infile);
		int sleeper;
		while (fgets(line, 55, infile) != NULL) //while there is a next line, continue
		{
			strcpy(line,dupeCheck(line)); //dupe checker

			if (addToArray(&fitbitdata, line, lineNumber, patientNumber) == -1) { continue; } //adds the line to the array and returns -1 if the patient id is incorrect
	
			int dupeCheck = lineNumber - 1;               //sets dupecheck to the value before lineNumber
			while (dupeCheck >= 0)						//this while loop compares the current minute with all previous minute to check for dupes
			{
				if (strcmp(fitbitdata[lineNumber].minute, fitbitdata[dupeCheck].minute) == 0) //if the current minute is equal to a previous minute,
				{																			  //move the linecounter back and end the while loop
					lineNumber--;
					dupeCheck = -1;
				}
				dupeCheck--;
			}
			lineNumber++;
		}
	}
	if (fclose(infile) == 0) { printf("File closed Successfully\n"); }
	
	FILE* outfile = fopen("Results.csv", "w"); //opening results file
	if (outfile != NULL) //checking to see if the results file opened correctly
	{
		printf("File opened successfully\n");
		fputs("Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max steps,Sleep\n", outfile);
		char* totals = calcString(&fitbitdata);
		fprintf(outfile, "%s\n", totals);
		fputs("Patient,Minute,Calories,Distance,Floors,Heart,Steps,Sleep Level\n", outfile);

		printf("Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max steps,Sleep\n");
		printf("%s\n", calcString(&fitbitdata));
		printf("Patient,Minute,Calories,Distance,Floors,Heart,Steps,Sleep Level\n");
		dataPrinter(&fitbitdata, outfile);
	}
	if (fclose(outfile) == 0) { printf("File closed successfully"); }
	return 0; //obligatory
}