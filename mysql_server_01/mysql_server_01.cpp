#include <iostream>
#include <string>
#include "mysql.h"

//mysql object
MYSQL     *mysqlObj;
//mysql response
MYSQL_RES *mysqlRes;

//gets contents of result fields
void processMySQLOutput(MYSQL_RES *resProc)
{
	int nRows = resProc->row_count;
	int nCols = resProc->field_count;
	for (int i = 0; i < nRows; i++)
	{
		std::cout << i << ") ";
		MYSQL_ROW currRow = mysql_fetch_row(resProc);
		if (currRow)
		{
			for (int j = 0; j < nCols; j++)
			{
				std::cout << currRow[j] << "; ";
			}
			std::cout << "\n";
		}
	}
}

//realises query
void printOutDataByRequest(MYSQL *mObj, const char* queryText, std::string queryComment)
{
	int qRes = mysql_query(mysqlObj, queryText);
	if (qRes == 0) //qRes == 0 means query went well
	{
		std::cout << queryComment.c_str() << "\n";
		mysqlRes = mysql_store_result(mObj);
		processMySQLOutput(mysqlRes);
	}

}

int main()
{
    //1 - Initialization
	std::cout << "Initialising MySQL object...\n";
	mysqlObj = mysql_init(NULL);
	if (mysqlObj == NULL)
	{
		std::cout << "Error creating MySQL object.\n";
		return -1;
	}

	//2 - Connect to MySQL instance
	bool connect = mysql_real_connect(
		mysqlObj,
		"localhost",   //ip
		"root",        //username
		"DOOM",        //user password
		"test_schema", //database name
		NULL,          //port
		NULL,          //socket
		0              //client flag
	);

	if (!connect)
	{
		std::cout << "Error connecting to MySQL object.\n" <<
			mysql_error(mysqlObj) << "\n";
		return -1;
	}

	while (true)
	{
		std::string request = "";
		std::cout << "Input query or \"exit\": \n";
		std::getline(std::cin,request); //we got request text
		if (request == "exit") break;
		printOutDataByRequest(mysqlObj, request.c_str(), "By query: "+request);
	}

	//4 - close mysql connection
	mysql_close(mysqlObj);

	system("pause");
}