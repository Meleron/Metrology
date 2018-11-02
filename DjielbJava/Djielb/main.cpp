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

/*bool areForwardSpacesAndX(string &str, int from, char X)
{
	int i = from;
	while (str[i] == ' ' && str[i] != X)
		i++;
	return str[i] == X;
}*/

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
/*
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
}*/

/*void deleteComments(vector<string> &javaCode)
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
                    } else{
                        javaCode[i][j] = ' ';
                    }
				} else{
				    cout<<"here "<<javaCode[i][j]<<endl;
                        javaCode[i][j] = ' ';
                    }
			}
		}
	}
}*/
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
                }else if(javaCode[i][j + 1] == '/' && javaCode[i][j] == '*'){
                    javaCode[i][j] = ' ';
                    javaCode[i][j + 1] = ' ';
                    isComment = false;
                } else if(isComment){
                    javaCode[i][j] = ' ';
                } else continue;
            } else if(isComment)
                javaCode[i][j] = ' ';
		}
	}

	for (int i = 0; i < javaCode.size(); i++){
        isComment = false;
		for (int j = 0; j < javaCode[i].length(); j++){
		    if(isComment)
                javaCode[i][j] = ' ';
            else if (j + 1 < javaCode[i].length()){
                if(javaCode[i][j + 1] == '/' && javaCode[i][j] == '/'){
                    javaCode[i][j] = ' ';
                    javaCode[i][j + 1] = ' ';
                    isComment = true;
                }
            }
		}
	}
}

void removeStrings(vector<string> &javaCode){
    bool isString = false;
    for(int i = 0 ; i< javaCode.size() ; i++){
            isString = false;
        for(int j = 0 ; j<javaCode[i].size() ; j++){
            if(javaCode[i][j] == '"'){
                    isString = !isString;
                    continue;
            }else if(isString)
                javaCode[i][j] = ' ';
        }
    }
}

/*
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

}*/

void parseCode(vector<string> &javaCode, int &operatorsCount)
{

	vector<string> operators;

	operators.push_back("if");
	operators.push_back("switch");
	operators.push_back("for");
	operators.push_back("do");
	operators.push_back("while");
	operators.push_back("break");
	operators.push_back("continue");
	operators.push_back("return");

	for(int i = 0 ; i<javaCode.size() ; i++){
        for(int j = 0 ; j< operators.size() ; j++){
            if(javaCode[i].find(operators[j])<1000){
                operatorsCount++;
            }
        }
	}
}

void findFunctions(vector<string> &javaCode, int &amountOfFunctions)
{
    int endFlag = 0;
    int beginFlag = 0;
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
						}

					}
				}
			}
		}
	}
}

void findBrackets(vector<string> &javaCode, int &operatorsCount)
{
	stack<char> bracketsSet;
	for (int i = 0; i < javaCode.size(); i++)
	{
		for (int j = 0; j < javaCode[i].size(); j++)
		{
			if (javaCode[i][j] == '(' || javaCode[i][j] == '[' || javaCode[i][j] == '{'){
				bracketsSet.push(javaCode[i][j]);
				}
            if ((javaCode[i][j] == '}' && bracketsSet.top() == '{')||(javaCode[i][j] == ')' && bracketsSet.top() == '(')||(javaCode[i][j] == ']' && bracketsSet.top() == '[')){
				operatorsCount++;
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

void findLogicExpressions(vector<string> &javaCode, int &logicExpressionCount){
    /*for(int i = 0 ; i< javaCode.size() ; i++){
        for(int j = 0 ; j< javaCode[i].size() ; j++){
            cout<<javaCode[i];
            if(javaCode[i].find("asd")){
                cout<<"asd";
            }
            cout<<endl;
        }
    }*/
    for(int i = 0 ; i< javaCode.size() ; i++){
        //cout<<javaCode[i];
            if(javaCode[i].find("if")!=string::npos)
                logicExpressionCount++;
        //cout<<endl;
    }

    for(int i = 0 ; i< javaCode.size() ; i++){
        //cout<<javaCode[i];
            if(javaCode[i].find("while")!=string::npos)
                logicExpressionCount++;
        //cout<<endl;
    }

    for(int i = 0 ; i< javaCode.size() ; i++){
        //cout<<javaCode[i];
            if(javaCode[i].find("for")!=string::npos)
                logicExpressionCount++;
        //cout<<endl;
    }

    for(int i = 0 ; i< javaCode.size() ; i++){
        //cout<<javaCode[i];
            if(javaCode[i].find("switch")!=string::npos)
                logicExpressionCount++;
        //cout<<endl;
    }

}

bool containsLogicExpression(string str){
    if(str.find("if")!=string::npos||str.find("while")!=string::npos||str.find("for")!=string::npos||str.find("switch")!=string::npos||str.find("case")!=string::npos)
        return true;
    return false;
}

void countCLI(vector<string> &javaCode, int &CLI){
    int bracketCounter = 0;
    int nesting = 0;
    int maxNesting = 0;
    int maxBracketCounter = 0;
    int switchCounter = 0;
    int caseCounter = 0;
    int elseCounter = 0;
    bool isClosed = true;
    bool isValid = false;
    for(int i = 0 ; i< javaCode.size() ; i++){
        //if(maxBracketCounter<bracketCounter)
        //        maxBracketCounter = bracketCounter;
        if(isClosed){
            if(containsLogicExpression(javaCode[i])){
                /*if(javaCode[i].find("switch")!=string::npos){
                    switchCounter++;
                }*/
                //cout<<"here1"<<endl;
                //nesting++;
                //bracketCounter++;
                isClosed = false;
                //cout<<"isClosed = false"<<endl;
                i--;
                /*bracketCounter++;
                cout<<"bracketCounter = "<<bracketCounter<<"; "<<javaCode[i]<<endl;
                continue;*/
            }
        }else { //if(bracketCounter!=0){
            if(i+1<javaCode.size()-1)
                if(javaCode[i+1][javaCode[i+1].size()-1]=='{' && javaCode[i+1].size()==1){
                    isValid = true;
                    i++;
                }
            if(/*containsLogicExpression(javaCode[i])&&*/javaCode[i][javaCode[i].size()-1]=='{'||isValid){
                //cout<<i<<" bracket opens "<<javaCode[i]<<endl;
                isValid = false;
            //cout<<"here1"<<endl;
                if(javaCode[i].find("switch")!=string::npos){
                    switchCounter++;
                    //cout<<"bracketCounter = "<<bracketCounter<<"; "<<javaCode[i]<<endl;
                }

                if(javaCode[i].find("case")!=string::npos/*||javaCode[i].find("default")!=string::npos*/){
                    caseCounter++;
                }

                bracketCounter++;
                //cout<<"bracketCounter = "<<bracketCounter<<"; "<<javaCode[i]<<endl;
                if(maxNesting<bracketCounter)
                    maxNesting = bracketCounter;//-1;
            }
            if(/*javaCode[i].find("}")!=string::npos*/javaCode[i][javaCode[i].size()-1]=='}' /*|| (javaCode[i].size()>1&&javaCode[i][0]=='}')*/){
                //cout<<i+1<<" "<<javaCode[i]<<endl;
                if(bracketCounter-1==0&&javaCode[i+1].find("else")!=string::npos){
                    elseCounter++;
                    continue;
                }
                bracketCounter--;
            }
            if(bracketCounter==0){
                //cout<<"switchCounter = "<<switchCounter<<"; caseCounter = "<<caseCounter<<"; maxNesting = "<< maxNesting<<endl;
                maxNesting-=switchCounter;
                //cout<<"switchCounter = "<<switchCounter<<"; caseCounter = "<<caseCounter<<"; maxNesting = "<< maxNesting<<endl;
                maxNesting+=caseCounter;
                //cout<<"switchCounter = "<<switchCounter<<"; caseCounter = "<<caseCounter<<"; maxNesting = "<< maxNesting<<endl;
                if(caseCounter>0)
                    maxNesting--;
                switchCounter = 0;
                caseCounter = 0;
                isClosed = true;
            }
        }
    }
    //cout<<"maxNesting = "<< maxNesting<<endl;
    /*if(maxBracketCounter!=0)
        maxBracketCounter--;
    CLI = maxBracketCounter;*/
    CLI = maxNesting;
    CLI-=elseCounter;
    if(CLI!=0)
        CLI--;
}

void printCode(vector<string> javaCode){
    for(int i = 0 ; i< javaCode.size() ; i++){
        cout<<javaCode[i]<<endl;
    }
    cout<<"-------------------------"<<endl;
}

int main()
{
	setlocale(LC_ALL, "rus");
	vector<string> javaCode;
	int logicExpressionCount = 0;
	int functionCount = 0;
	int operatorsCount = 0;
	int CLI = 0;
	if (!readFile(javaCode, "Code.txt"))
		return 0;
    //printCode(javaCode);
    deleteComments(javaCode);
    //printCode(javaCode);
    removeStrings(javaCode);
    //printCode(javaCode);
    findLogicExpressions(javaCode, logicExpressionCount);
    parseCode(javaCode, operatorsCount);
    findBrackets(javaCode, operatorsCount);
    findFunctions(javaCode, operatorsCount);
    countCLI(javaCode, CLI);
    //if(javaCode[0][0]=='{')
    //    cout<<"true"<<endl;
    //    else cout<<"false"<<endl;
    cout<<"CL = "<<logicExpressionCount<<endl;
    cout<<"cl = "<<(double)logicExpressionCount/operatorsCount<<" (total operators = "<<operatorsCount<<")"<<endl;
    cout<<"CLI = "<<CLI<<endl;
    //cout<<"Total operators = "<<operatorsCount<<endl;

}
