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
				javaCode[i][j - 2] = ' ';
				javaCode[i][j - 1] = ' ';
				javaCode[i][j] = ' ';
			}
		}
	}

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 1; j < javaCode[i].size(); j++){
			if (doubleOperators.find(javaCode[i].substr(j - 1, 2)) != doubleOperators.end()){
				javaCode[i][j-1] = ' ';
				javaCode[i][j] = ' ';
			}
		}
	}

	for (int i = 0; i < javaCode.size(); i++){
		for (int j = 0; j < javaCode[i].size(); j++){
			if (singleOperators.find(javaCode[i].substr(j, 1)) != singleOperators.end() && javaCode[i].substr(j, 1) != "="){
				javaCode[i][j] = ' ';
			}
		}
	}

}

bool contains(string str, set<string> toFind){
        for(set<string>::iterator it = toFind.begin(); it != toFind.end(); it++){
        if(str.find(*it)!=string::npos){
            return true;
        }
    }
    return false;
}


void parseCode(vector<string> &javaCode, map<string, int> &operandCount, set<string> &operandSet, int &functionCount,
                map<string, int> &operandCountChep,set<string> &operandSetInp , bool isChep)
{

	set<string> types;
	set<string> operators;
	set<string> operands;
	set<string> mods;
	set<string> control;
	bool isValid = false;
    int currentPosition = 0;
    int tempPos = 0;
    string current = "";


    mods.insert("public");
    mods.insert("private");
    mods.insert("protected");
    mods.insert("static");
    mods.insert("virtual");
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
	types.insert("true");
	types.insert("false");
	operators.insert("if");
	operators.insert("switch");
	operators.insert("for");
	operators.insert("do");
	operators.insert("while");
	operators.insert("break");
	operators.insert("continue");
	operators.insert("return");
	control.insert("while");
	control.insert("if");
	control.insert("for");
	control.insert("switch");

	for (int i = 0; i < javaCode.size(); i++){
        isValid = false;
        currentPosition = 0;
		for (int j = 0; j <= javaCode[i].size(); j++){
			if (isDelimeter(javaCode[i][j]) || j == javaCode[i].size()){
				current = javaCode[i].substr(currentPosition, j - currentPosition);
				tempPos = currentPosition;
				currentPosition = j+1;
				if (isValid){
					if (((current[0] >= 'a')&&
                        (current[0] <= 'z')||
                        (current[0] >= 'A')&&
                        (current[0] <= 'Z')||
                        (current[0] == '_'))&&
                        (types.find(current) == types.end())&&
                        (operators.find(current) == operators.end())){
                        if(current.find('.')!=string::npos)
                            continue;
                        bool isFunc = false;
                        for(set<string>::iterator it = mods.begin(); it != mods.end(); it++){
                            if(javaCode[i].find(*it)!=string::npos){
                                isFunc = true;
                                break;
                            }
                        }
                        if(isFunc)
                            continue;

                        operands.insert(current);
                        operandSet.insert(current);
                        operandCount[current]++;

                        if(javaCode[i].find(".next")!=string::npos || javaCode[i].find("System.out.print")!=string::npos){
                            operandSetInp.insert(current);
                        }


						if(isChep){
                            if(contains(javaCode[i], control))
                                operandCountChep[current] = 3;
						}
						for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
					}
				}
				else{
                    if (operands.find(current) != operands.end()){
                        operandSet.insert(current);
                        operandCount[current]++;
                        if(javaCode[i].find(".next")!=string::npos || javaCode[i].find("System.out.print")!=string::npos){
                            operandSetInp.insert(current);
                        }
                        if(isChep){
                            int temp = javaCode[i].find('=');
                            if(temp!=string::npos){
                                if(tempPos < temp && operandCountChep[current]!=3){
                                    operandCountChep[current] = 2;
                                    //cout<<"row: "<<i<<", modified: "<<current<<endl;
                                }
                            }
                            if(contains(javaCode[i], control))
                                operandCountChep[current] = 3;
						}
                        for (int k = j-current.size(); k <= j-1; k++)
                            javaCode[i][k] = ' ';
                    }
				}
                if (current[0] >= '0' && current[0] <= '9')
                {
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
	for(set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++)
        operandCount[*it]--;
}

void countSpen(set<string> operandSet, map<string, int> operandCount){
    int totalAmount=0;
    string temp = "";
    cout<<"-------------------------"<<endl;
    cout<<"*РАСЧЁТ СПЕНА ПРОГРАММЫ*"<<endl<<endl;
    cout<<"Идентификатор\tспен"<<endl;
	for (set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++)
	{
	    if(operandCount[*it]!=0){
            temp = *it;
            if(temp.size()>7){
                temp = temp.substr(0,7);
                temp[6] = '.';
                temp[5] = '.';
            }
            cout << temp << "\t\t" << operandCount[*it] << endl;
            totalAmount+=operandCount[*it];
        }
	}
	cout<<"Суммарный спен программы = "<< totalAmount<<endl;
	cout<<"-------------------------"<<endl;
}

void countChep(set<string> operandSet, map<string, int> operandCountChep, set<string> operandSetInp, bool isInput){
    int counter = 0;
    float chepinMetric = 0;
    float divider = 1;
    cout<<"\n\n\n-------------------------"<<endl;
    if(!isInput)
        cout<<"*РАСЧЁТ ПОЛНОЙ МЕТРИКИ ЧЕПИНА*"<<endl<<endl;
    else
        cout<<"*РАСЧЁТ МЕТРИКИ ЧЕПИНА ВВОДА/ВЫВОДА*"<<endl<<endl;
    for(int i = 1 ; i< 5 ; i++){
        cout<<"Группа ";
        switch(i){
            case 1:
                cout<<"P\n";
                break;
            case 2:
                cout<<"M\n";
                divider = 2;
                break;
            case 3:
                cout<<"C\n";
                divider = 3;
                break;
            case 4:
                cout<<"T\n";
                divider = 0.5;
        }
        for(set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++){
            if(operandCountChep[*it]==i){
                if(isInput){
                    if(operandSetInp.find(*it)==operandSetInp.end())
                        continue;
                }
                cout<<"\t"<<*it<<endl;
                counter++;
            }
        }
        if(counter==0)
            cout<<"\t-Переменные отсуствуют-\n";
        cout<<"Количество переменных в группе: "<<counter<<endl<<endl;
        chepinMetric+=(divider*counter);
        counter = 0;
    }
    cout<<"Метрика Чепина: Q = "<<chepinMetric<<endl;
    cout<<"-------------------------"<<endl;
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
	vector<string> javaCode, javaCodeChep, javaCodeChepInp;
	map<string, int> statementCount, operandCount, operandCountChep;
	set<string> operatorSet, operandSet, operandSetInp;
	int functionCount = 0;
	if (!readFile(javaCode, "Code.txt"))
		return 0;
	remCommAndChars(javaCode, operandCount, operandSet);
	//printCode(javaCode);
	deleteComments(javaCode);
	//printCode(javaCode);
	findOperands(javaCode, statementCount, operatorSet);
	javaCodeChep = javaCode;
	javaCodeChepInp = javaCode;
	parseCode(javaCode, operandCount, operandSet, functionCount, operandCountChep, operandSetInp, false);
	//printCode(javaCode);map<string, int> &operandCountChep
	countSpen(operandSet, operandCount);
	operandCountChep = operandCount;
    for(set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++){
        operandCountChep[*it] = 4;
        if(operandCount[*it]>0)
            operandCountChep[*it] = 1;
    }
    //printCode(javaCodeChep);
    parseCode(javaCodeChep, operandCount, operandSet, functionCount, operandCountChep, operandSetInp, true);
    countChep(operandSet, operandCountChep, operandSetInp, false);
    countChep(operandSet, operandCountChep, operandSetInp, true);
}
