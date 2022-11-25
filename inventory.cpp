
// Description:
//      This project involves designing and creating a C++ program that will utilize the InventoryItem class
//     The program must create an array or vector of 100 InventoryItem objects and support the following interactive commands:
//      (a) Add parts: increase the units value for an existing inventory item. (i) Input inventory data from a file.
//      (n) create a New inventory Item. (o) Output inventory data to a file. (r) Remove parts: reduce the units value for an existing inventory item.

#include <iostream>
using namespace std;
#include "InventoryItem.h"
#include <fstream>
#include <string>
#include <iomanip>

//Function prototype
void AddPart(InventoryItem tool[]);
void DisplayHelp();
void InputInventory(ifstream &inFile, string fieldArray[], InventoryItem itemArr[], int &size);
void PrintInventory(InventoryItem itemArr[], int &size);
void NewItem(InventoryItem tool[], int &size);
void OutPutInventory(ofstream &outFile, InventoryItem itemArr[], int &size);
void RemovePart(InventoryItem tool[]);
void splitLineToArray(string inputText, string delimiter, string fieldArray[], int maxFields);

//Global veriable
const int FIELD = 150; //size for string array
const int MAX = 100; //size for InventoryItem array


int main()
{
    char command;       //for user command
    ifstream inFile;    //will be used to read data from a file.
    ofstream outFile;   //Will be used to output data to a file.

    string fieldArray[FIELD]; //array will be used to temporary store data from a pipe-delimited” text file.
    InventoryItem tool[MAX];  //array that can hold upto 100 InventoryItem objects.
    int toolArraySize = 0;

    while (command != false)
    {
        cout << "\nCommand: ";
        cin >> command;
        cin.ignore();
        {
            switch(command)
            {
                case 'a': toolArraySize++;
                          AddPart(tool);
                          break;
                case 'h': DisplayHelp();
                          break;  
                case 'i': InputInventory(inFile, fieldArray, tool, toolArraySize);
                          break;
                case 'p': PrintInventory(tool, toolArraySize);
                          break; 
                case 'n': toolArraySize++; NewItem(tool, toolArraySize);
                          break;
                case 'o': OutPutInventory(outFile, tool, toolArraySize);
                          break;
                case 'r': RemovePart(tool);
                          break;
                case 'q': command = false;
                          break;
                default: cout << "Please enter a valid command. 'a', 'h', 'i', 'p', 'n', 'o', 'r', 'q' " << endl;
            }
        }
    }
    return 0;
}


//*****************************************
//Function will allow the user to add
//new units to an item.
//Task a InventoryItem array as an argument
//*****************************************

void AddPart(InventoryItem tool[])
{
    int ItemNum; // will be used to store item number
    int add; // Units to add to the item number. 
    int NewStock; // Will hold the current inventory + the units added by the user. 

    cout << "Choose a Item Number: "; //ask what item number and store it in num variable
    cin >> ItemNum;

    cout << "How many parts to add: "; //Ask how many units user would like to add. 
    cin >> add;

    NewStock = tool[ItemNum].getUnits() + add; //current inventory + the units added by the user. 

    //Stock can not exceed 30 units 
    if (NewStock > 30)
        cout << "You cannot hold more than 30 parts.";
    else
        tool[ItemNum].setUnits(NewStock); //Call the setter function to set the new stock # which includes current stock plus user added stock. 
}

//*****************
// Display commands 
//*****************

void DisplayHelp()
{
    cout << "\nSupported commands: \n"
    "a    Add parts. \n"
    "h    print this help text. \n"
    "i    Input inventory data from a file. \n"
    "p    Print inventory list. \n"
    "n    New inventory Item. \n"
    "o    Output inventory data to a file. \n"
    "q    Quit (end the program). \n"
    "r    Remove parts. \n";
}


//*****************************************
//Function will allow the user to open a txt file.
//It calls the splitLineToArray which stores everythhing in the fieldArray.
//We then call the setter functions to store data in the InventoryItem array
//*****************************************

void InputInventory(ifstream &inFile, string fieldArray[], InventoryItem tool[], int &size) 
{
   string filename; //Will be used to hold file name user wants to open
   string inputBuffer;
   int record = 0;  //store number of records added to InventoryItem array

   cout << "Enter name of input file: ";
   getline(cin, filename);

   inFile.open(filename); //use the file object to open the file. pass the file name user provided as an argument. 

   while (inFile.fail()) //Error checking. loop runs until a file is opened
   {
       cout << "File did not open. Please try again: ";
       getline(cin, filename);
       inFile.open(filename);
   }

   while (getline(inFile, inputBuffer)) 
   {

       splitLineToArray(inputBuffer, "|", fieldArray, MAX); //Call splitLineToArray function

       string description = fieldArray[1];

        //We need to convert the cost and unit strings stored in the string array to an int and double.
        //We can use the stod member function to convert the cost string into a double. and stoi to convert to an int. 
        //Passing both stod and stoi the current locaiton for either the cost or quantity location in the string array.
       double cost = stod(fieldArray[2]); 
       int quantity = stoi(fieldArray[3]);

        //After converting from strings call the setter functions and pass the cost,quantity. Will be saved in the InventoryItem array.
       tool[size].setDescription(description);
       tool[size].setCost(cost);
       tool[size].setUnits(quantity);
       size++;
       record++;
   }
   cout << record << " records loaded to array.";
   inFile.close();
}


//*****************************************
//Function displays the inventory 
//Uses a for loop to call the getter functions
//Task a InventoryItem array as an argument.
//*****************************************

void PrintInventory(InventoryItem itemArr[], int &size)
{
    cout << left << setw(15) << "Itemnum " << setw(40) << " Description " << setw(15) << " Cost " << setw(10) << " Quantity " << endl;
    cout << left << setw(15) << "-------" << setw(40) << "--------------" << setw(15) << "-----" << setw(10) << "---------" << endl;

    //for loop that will be used to call the getter functions to display description, cost and quantity.
    for (int i = 0; i < size; i++)
    {
        cout << setw(15) << i << setw(40) << itemArr[i].getDescription()
             << setw(15) << fixed << setprecision(2) << itemArr[i].getCost()
             << setw(10) << itemArr[i].getUnits() << endl;
    }
}


//*****************************************
//Function will allow the user to add a new
//Inventory item. Creates temp variable for 
//desc, cost and quantity. Takes an 
//InventoryItem array and array size as arguments 
//*****************************************

void NewItem(InventoryItem tool[], int &size) 
{
    //Variables for new inventory item
    string description; 
    double cost;
    int quantity;

    //ask the user to enter new inventory item data
    cout << "Enter description for new Item: ";
    cin >> description;
    tool[size - 1].setDescription(description); // Call the setter functions to add the new item

    cout << "Enter unit cost for new Item: ";
    cin >> cost;
    tool[size - 1].setCost(cost);

    cout << "Enter initial quantity for the new item: ";
    cin >> quantity;
    //Stock for item can not be more than 30 or less than 0
    if (quantity > 30 || quantity < 0)
        cout << "Error: Item stock can not be greater than 30 or less than 0: ";
    else
    {
        tool[size - 1].setUnits(quantity);
        cout << "Announcing a new inventory Item: " << description << endl;
        cout << "We now have " << size << " different inventory Items in stock! ";
    }
}


//*****************************************
//Function takes an ofstream object and an InventoryItem
//array as arguments. It uses the ofstream object to output
//data from the InventoryItem array
//*****************************************

void OutPutInventory(ofstream &outFile, InventoryItem itemArr[], int &size)
{
    string filename; //store file name.

    cout << "Enter name of output file: ";
    cin >> filename;

    outFile.open(filename); //use the outFile object to open the file.

    while(outFile.fail()) //make sure file was created.
    {
        cout << "Error. file not created. Try again: ";
        cin >> filename;
        outFile.open(filename);
    }

    for (int i = 0; i < size; i++)
    {
        outFile << i << '|' << itemArr[i].getDescription()
           << '|' << fixed << setprecision(2) << showpoint << itemArr[i].getCost()
           << '|' << itemArr[i].getUnits() << endl;
    }
    cout << size << " records written to file.";
    outFile.close(); //close the file
}


//*****************************************
//Function will allow the user to remove
//units from an item.
//Task a InventoryItem array as an argument.
//Function is simular to AddPart function
//*****************************************

void RemovePart(InventoryItem tool[])
{
    int ItemNum; //Item number in inventory list
    int remove; // Units user wants to remove
    int NewStock;

    cout << "Choose a Item Number: ";
    cin >> ItemNum;
    cout << "How many units do you want to remove: ";
    cin >> remove;

    NewStock = tool[ItemNum].getUnits() - remove;

    // stock can not be less than 0
    if (NewStock < 0)
        cout << "There can not be less than 0 units in stock.";
    else
        tool[ItemNum].setUnits(NewStock); //Call the setter function to set the new stock # which includes current stock minus user removed units. 
}




void splitLineToArray(string inputText, string delimiter, string fieldArray[], int maxFields) //Reuse function from Project3: File Format Conversion Program. 
{
	//	Local Variables:
	int nextFieldPosition = 0;		//	Start position of data field.
	int foundPosition;				//  Position where delimiter was found,
	int fieldCount = 0;		        //  Number of data fields found so far,
	bool endOfLine = false;			//	Flag -- end of input line detected,
	string dataField;				//  Copy of the data field.

	while (!endOfLine)
	{
		foundPosition = inputText.find(delimiter, nextFieldPosition);
		if (foundPosition == string::npos)
		{
			// (Here if the string 'find' function reached the end of the input string.)
			endOfLine = true;
			foundPosition = inputText.length();

			if (foundPosition > nextFieldPosition)
			{
				dataField = inputText.substr(nextFieldPosition,
					foundPosition - nextFieldPosition);
				fieldCount++;
			}
		}
		else
		{
			// (Here if 'inputDelim' was found in the input string.)
			// Copy the data field contents from the input string to the
			// 'dataField' local variable.
			dataField = inputText.substr(nextFieldPosition, foundPosition - nextFieldPosition);
			fieldCount++;

			// Adjust 'nextFieldPosition' as preparation for finding NEXT delimiter.
			nextFieldPosition = foundPosition + 1;
		}

		if (fieldCount > 0)
		{
			if (fieldCount > maxFields)
			{
				cout << "ERROR at line __LINE__: too many input fields, fieldCount="
					<< fieldCount << endl;
			}
			else
			{
				fieldArray[fieldCount - 1] = dataField;
			}
		}
	} // (end while)
} // (end function splitLineToArray )