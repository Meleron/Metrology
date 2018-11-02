#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <stack>

using namespace std;

bool isDelimeter(char x)
{
    return (x < 'a' || x > 'z') && (x < 'A' || x > 'Z') && (x < '0' || x > '9') && x != '_' && x != '.';
}

bool areForwardSpacesAndX(string &str, int from, char X)
{
	int i = from;
	while (str[i] == ' ' && str[i] != X)
		i++;
	return str[i] == X;
}

bool readFile(vector<string> &javaCode, char* filePath)
{
	ifstream fin(filePath);
	if (!fin.is_open())
	{
		cout << "Ошибка при открытии файла" << endl;
		return false;
	}
	string temp;
	while(true){
        if(!getline(fin, temp))
            break;
        javaCode.push_back(temp);
	}
	fin.close();
	return true;
}

void remCommAndChars(vector<string> &javaCode, map<string, int> &operandDictMap, set<string> &operandSet)
{
    //Метод удаляет из кода  и добавляет в таблицу объявления строк и символов
    int quotes = 0;
    bool isString = false;
    string strComm = "";
    string strChar = "";

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 0; j < javaCode[i].length(); j++){

            //Удаление операндов-символов
            if (javaCode[i][j] == '\''){
				strChar = "\'";
				javaCode[i][j] = ' ';
				if (j + 1 < javaCode[i].size()){
					strChar += javaCode[i][j + 1];
					javaCode[i][j + 1] = ' ';
				}
				if (strChar.length() > 1 && strChar[1] != '\''){
					strChar += "\'";
					if (j + 2 < javaCode[i].size())
						javaCode[i][j + 2] = ' ';
				}
				operandSet.insert(strChar);
				operandDictMap[strChar]++;
			}

            //Удаление опреандов-строк
			if(javaCode[i][j] == '"'){
                quotes++;
                if(quotes % 2 == 1) {
                    strComm = "";
                    isString = true;
                } else {
					strComm += "\"";
					if (strComm != "\""){
						operandSet.insert(strComm);
						operandDictMap[strComm]++;
					}
					javaCode[i][j] = ' ';
					isString = false;
					continue;
				}
			}
			if (isString){
				strComm += javaCode[i][j];
				javaCode[i][j] = ' ';
				continue;
			}
			if ((j + 1 < javaCode[i].size()) &&
                (javaCode[i][j] == '/') &&
                (javaCode[i][j + 1] == '/') &&
                ((quotes % 2) != 1)){
                javaCode[i] = "";
				break;
			}
		}
	}
}

void deleteComments(vector<string> &javaCode)
{
    // Метод удаляет многострочные комментарии
	bool isComment = false;
	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 0; j < javaCode[i].length(); j++){
            if (j + 1 < javaCode[i].length()){
                if(javaCode[i][j + 1] == '*' && javaCode[i][j] == '/'){
                    javaCode[i][j] = ' ';
                    javaCode[i][j + 1] = ' ';
                    isComment = true;
                }
            } else {
                continue;
            }
			if (isComment){
				if (j + 1 < javaCode[i].length()){
                    if(javaCode[i][j + 1] == '/' && javaCode[i][j] == '*'){
                        javaCode[i][j] = ' ';
                        javaCode[i][j + 1] = ' ';
                        isComment = false;
                    }
				}
				else{
					javaCode[i][j] = ' ';
				}
			}
		}
	}
}

void findOperands(vector<string> &javaCode, map<string, int> &operandDictMap, set<string> &operandSet)
{
	set<string> singleOperators;
	set<string> doubleOperators;
	set<string> tripleOperators;
	singleOperators.insert("!");
	singleOperators.insert("~");
	singleOperators.insert("*");
	singleOperators.insert("/");
	singleOperators.insert("%");
	singleOperators.insert("+");
	singleOperators.insert("-");
	singleOperators.insert(">");
	singleOperators.insert("<");
	singleOperators.insert("=");
	singleOperators.insert("&");
	singleOperators.insert("^");
	singleOperators.insert("|");
	doubleOperators.insert("++");
	doubleOperators.insert("--");
	doubleOperators.insert(">>");
	doubleOperators.insert("<<");
	doubleOperators.insert(">=");
	doubleOperators.insert("<=");
	doubleOperators.insert("==");
	doubleOperators.insert("!=");
	doubleOperators.insert("&&");
	doubleOperators.insert("||");
	doubleOperators.insert("+=");
	doubleOperators.insert("-=");
	doubleOperators.insert("*=");
	doubleOperators.insert("/=");
	doubleOperators.insert("%=");
	doubleOperators.insert("&=");
	doubleOperators.insert("^=");
	doubleOperators.insert("|=");
	tripleOperators.insert(">>>");
	tripleOperators.insert("<<=");
	tripleOperators.insert(">>=");

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 2; j < javaCode[i].size(); j++){
			if (tripleOperators.find(javaCode[i].substr(j - 2, 3)) != tripleOperators.end()){
				operandDictMap[javaCode[i].substr(j - 2, 3)]++;
				operandSet.insert(javaCode[i].substr(j - 2, 3));
				javaCode[i][j - 2] = ' ';
				javaCode[i][j - 1] = ' ';
				javaCode[i][j] = ' ';
			}
		}
	}

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 1; j < javaCode[i].size(); j++){
			if (doubleOperators.find(javaCode[i].substr(j - 1, 2)) != doubleOperators.end()){
				operandDictMap[javaCode[i].substr(j - 1, 2)]++;
				operandSet.insert(javaCode[i].substr(j - 1, 2));
				javaCode[i][j-1] = ' ';
				javaCode[i][j] = ' ';
			}
		}
	}

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 0; j < javaCode[i].size(); j++){
			if (singleOperators.find(javaCode[i].substr(j, 1)) != singleOperators.end()){
				operandDictMap[javaCode[i].substr(j, 1)]++;
				operandSet.insert(javaCode[i].substr(j, 1));
				javaCode[i][j] = ' ';
			}
		}
	}

}

void parseCode(vector<string> &javaCode, map<string, int> &operandCount, map<string, int> &statementCount, set<string> &operandSet, set<string> &operatorSet, int &functionCount)
{

	set<string> types;
	set<string> operators;
	set<string> operands;
	bool isValid = false;
    int currentPosition = 0;
    string current = "";

	types.insert("char");
	types.insert("byte");
	types.insert("short");
    types.insert("int");
	types.insert("long");
	types.insert("float");
	types.insert("double");
	types.insert("boolean");
	types.insert("void");
	types.insert("Character");
	types.insert("Integer");
	types.insert("Long");
	types.insert("Float");
	types.insert("Double");
	types.insert("String");
	types.insert("Boolean");
	operators.insert("if");
	operators.insert("switch");
	operators.insert("for");
	operators.insert("do");
	operators.insert("while");
	operators.insert("break");
	operators.insert("continue");
	operators.insert("return");

	for (int i = 0; i < javaCode.size(); i++){
        isValid = false;
        currentPosition = 0;
		for (int j = 0; j <= javaCode[i].size(); j++){
			if (isDelimeter(javaCode[i][j]) || j == javaCode[i].size()){
				current = javaCode[i].substr(currentPosition, j - currentPosition);
				currentPosition = j+1;
				if (isValid){
					if (((current[0] >= 'a')&&
                        (current[0] <= 'z')||
                        (current[0] >= 'A')&&
                        (current[0] <= 'Z')||
                        (current[0] == '_'))&&
                        (types.find(current) == types.end())&&
                        (operators.find(current) == operators.end())){

						operands.insert(current);
						for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
					}
				}
				else{
                    if (operands.find(current) != operands.end()){
						operandSet.insert(current);
						operandCount[current]++;
						for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
					}
					if (operators.find(current) != operators.end()){
						operatorSet.insert(current);
						statementCount[current]++;
						for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
					}

				}
				if (current[0] >= '0' && current[0] <= '9')
				{
					operandSet.insert(current);
					operandCount[current]++;
					for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
				}
				if (types.find(current) != types.end() && !areForwardSpacesAndX(javaCode[i], j, ')'))
				{
					if (!isValid && javaCode[i].find(';') == javaCode[i].npos)
						functionCount++;
					isValid = true;
					for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
				}
				if (j < javaCode[i].size() && javaCode[i][j] == ';')
					isValid = false;
			}
		}
	}
}

void findFunctions(vector<string> &javaCode, map<string, int> &operatorCount, set<string> &operatorSet, int &amountOfFunctions)
{
    int endFlag = 0;
    int beginFlag = 0;
    string tmpStr = "";
	for (int i = 0; i < javaCode.size(); i++){
		if (javaCode[i].find(';') != string::npos){
			for (int j = 0; j < javaCode[i].length(); j++){
				if (javaCode[i][j] == '('){
					endFlag = j - 1;
					while (endFlag > 0 && javaCode[i][endFlag] == ' '){
						endFlag -= 1;
					}
					beginFlag = endFlag;
					while (beginFlag - 1 > 0 && !isDelimeter(javaCode[i][beginFlag - 1])){
						beginFlag--;
					}
					if (endFlag)
					{
						bool isValid = true;
						for (int k = beginFlag; k <= endFlag; k++)
							if (isDelimeter(javaCode[i][k]))
								isValid = false;
						if (isValid){
							amountOfFunctions++;
							tmpStr = javaCode[i].substr(beginFlag, endFlag - beginFlag + 1) + "()";
							operatorCount[tmpStr]++;
							operatorSet.insert(tmpStr);
						}

					}
				}
			}
		}
	}
}

void findBrackets(vector<string> &javaCode, map<string, int> &operatorCount, set<string> &operatorSet)
{
	stack<char> bracketsSet;
	for (int i = 0; i < javaCode.size(); i++)
	{
		for (int j = 0; j < javaCode[i].size(); j++)
		{
			if (javaCode[i][j] == '(' || javaCode[i][j] == '[' || javaCode[i][j] == '{'){
				bracketsSet.push(javaCode[i][j]);
				}
            if (javaCode[i][j] == '}' && bracketsSet.top() == '{'){
				operatorCount["{..}"]++;
				operatorSet.insert("{..}");
				bracketsSet.pop();
			}
			if (javaCode[i][j] == ')' && bracketsSet.top() == '(')
			{
				operatorCount["(..)"]++;
				operatorSet.insert("(..)");
				bracketsSet.pop();
			}
			if (javaCode[i][j] == ']' && bracketsSet.top() == '[')
			{
				operatorCount["[..]"]++;
				operatorSet.insert("[..]");
				bracketsSet.pop();
			}

		}
	}
}

/*void writeFile(vector<string> &javaCode, char* path)
{
	ofstream fout(path);
	if (!fout.is_open())
	{
		cout << "File was not opened somehow." << endl;
		return;
	}
	for (int i = 0; i < javaCode.size(); i++)
		fout << javaCode[i] << endl;
	fout.close();
}
*/
int main()
{
	setlocale(LC_ALL, "rus");
	vector<string> javaCode;
	map<string, int> statementCount, operandCount;
	set<string> operatorSet, operandSet;
	int functionCount = 0;
	if (!readFile(javaCode, "Code.txt"))
		return 0;
	remCommAndChars(javaCode, operandCount, operandSet);
	deleteComments(javaCode);
	findOperands(javaCode, statementCount, operatorSet);
	parseCode(javaCode, operandCount, statementCount, operandSet, operatorSet, functionCount);
	findFunctions(javaCode, statementCount, operatorSet, functionCount);
	findBrackets(javaCode, statementCount, operatorSet);
	int i = 0, mapStatementTotalCount = 0, mapOperandTotalCount = 0;
	cout << "Statement dictionary : " << operatorSet.size() << endl;
	for (set<string>::iterator it = operatorSet.begin(); it != operatorSet.end(); it++, i++)
	{
		if (*it == "(..)")
		{
			cout << i << ": " << *it << " " << statementCount[*it] - functionCount << endl;
			mapStatementTotalCount += statementCount[*it] - functionCount;
		}
		else
		{
			cout << i << ": " << *it << " " << statementCount[*it] << endl;
			mapStatementTotalCount += statementCount[*it];
		}
	}
	cout << "Total statements : " << mapStatementTotalCount << endl;
	i = 0;
	cout << "Operand dictionary(total count " << operandSet.size() << " ): " << endl;
	for (set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++, i++)
	{
		cout << i << ": " << *it << " " << operandCount[*it] << endl;
		mapOperandTotalCount += operandCount[*it];
	}
	cout << "Total operands : " << mapOperandTotalCount << endl << endl;
	cout << "Program dictionary : " << operatorSet.size() + operandSet.size() << endl;
	cout << "Program length : " << mapOperandTotalCount + mapStatementTotalCount << endl;
	cout << "Program volume : " << (mapOperandTotalCount + mapStatementTotalCount) * log2(operatorSet.size() + operandSet.size()) << endl;
	//writeFile(javaCode, "output.txt");
}
