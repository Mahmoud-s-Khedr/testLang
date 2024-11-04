#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

unordered_map<string, string> keywords = {
    //data types
    {"int", "int"},
    {"float", "float"},
    {"char", "char"},
    {"bool", "bool"},
    {"string", "string"},
    //flow control
    {"if", "if"},
    {"else", "else"},
    {"while", "while"},
    {"for", "for"},
    {"break", "break"},
    {"continue", "continue"},
    //values
    {"true", "true"},
    {"false", "false"},
    //return
    {"return", "return"},
    //operators
    {"+", "plus"},
    {"-", "minus"},
    {"*", "multiply"},
    {"/", "divide"},
    {"%", "modulus"},
    {"^", "exponent"},
    {"=", "assign"},
    {"<", "Less_than"},
    {">", "Greater_than"},
    {"!", "not"},
    {"&", "and"},
    {"|", "or"},
    //delimiters
    {"(", "open_bracket"},
    {")", "close_bracket"},
    {"{", "open_brace"},
    {"}", "close_brace"},
    {"[", "open_square_bracket"},
    {"]", "close_square_bracket"},
    {";", "semi_colon"},
    {",", "comma"},
    {".", "dot"}
};

void printTokens(vector<pair<string, string>> tokens){

    for(int i=0; i<tokens.size(); i++){
        cout<<tokens[i].first<<"\t"<<tokens[i].second<<endl;
    }
}
void tokenizer(string str, vector<pair<string, string>> &tokens){

    int i = 0;
    while (i<str.length())
    {
        if(isspace(str[i])){
            i++;
            continue;
        }
        //check for headers
        if(str[i]=='#'){
            string token = "";
            while(i<str.length()&&str[i]!='\n'){
                token += str[i];
                i++;
            }
            tokens.push_back({token, "preprocessor"});
            continue;
        }
        //check for identifiers and keywords
        if(isalpha(str[i])){
            string token = "";
            while(i<str.length()&&(isalpha(str[i])|| isdigit(str[i])|| str[i]=='_')){
                token += str[i];
                i++;
            }
            string type = "identifier";
            if(keywords.find(token) != keywords.end()){
                type = keywords[token];
            }
            tokens.push_back({token, type});
            continue;
        }
        //check for numbers
        if(isdigit(str[i])){
            string token = "";
            string type = "integer";
            while(i<str.length()&&(isdigit(str[i]) || str[i] == '.')){
                if (str[i] == '.')
                {
                   type = "float";
                }
                
                token += str[i];
                i++;
            }
            tokens.push_back({token, type});
            continue;
        }
        //check for relational operators and bi-operators
        if(str[i]=='=' && str[i+1]=='='){
            tokens.push_back({"==", "equality"});
            i+=2;
            continue;
        }

        if(str[i]=='<' && str[i+1]=='='){
            tokens.push_back({"<=", "Less_than_or_equal_to"});
            i+=2;
            continue;
        }

        if(str[i]=='>' && str[i+1]=='='){
            tokens.push_back({">=", "Greater_than_or_equal_to"});
            i+=2;
            continue;
        }

        if(str[i]=='!' && str[i+1]=='='){
            tokens.push_back({"!=", "Not_equal_to"});
            i+=2;
            continue;
        }

        if(str[i]=='+' && str[i+1]=='+'){
            tokens.push_back({"++", "increment"});
            i+=2;
            continue;
        }

        if (str[i]=='-' && str[i+1]=='-')
        {
            tokens.push_back({"--", "decrement"});
            i+=2;
            continue;
        }

        if(str[i]=='&' && str[i+1]=='&'){
            tokens.push_back({"&&", "Logical_and"});
            i+=2;
            continue;
        }

        if(str[i]=='|' && str[i+1]=='|'){
            tokens.push_back({"||", "Logical_or"});
            i+=2;
            continue;
        }
        //comment skip
        if(str[i]=='/' && str[i+1]=='/'){
            return;
        }
        //check for operators brackets (){}[] + - * / % ^ = < > & | ; 
        if (str[i] == '(' || str[i] == ')' || str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']' || str[i] == ':' ||
            str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%' || str[i] == '^' || str[i] == '.' ||
            str[i] == '=' || str[i] == '<' || str[i] == '>' || str[i] == '&' || str[i] == '|' || str[i] == ';' || str[i] == ',')
        {
            tokens.push_back({string(1, str[i]), keywords[string(1, str[i])]});
            i++;
            continue;
        }
        //check for double quotes "string"
        if(str[i]=='"'){
            string token = "";
            token += str[i];
            i++;
            while(i<str.length()&&str[i]!='"'){
                token += str[i];
                i++;
            }
            token += str[i];
            tokens.push_back({token, "string"});
            i++;
            continue;
        }
        //check for single quotes "character"
        if(str[i]=='\''){
            string token = "";
            token += str[i];
            i++;
            while(i<str.length()&&str[i]!='\''){
                token += str[i];
                i++;
            }
            token += str[i];
            tokens.push_back({token, "char"});
            i++;
            continue;
        }

        if(str[i]){
            cerr<<"ERROR : "<<str[i]<<" is not a valid character: "<<i<<endl;
            exit(1);
        }
        i++;
    }
}

int main( int argc, char* argv[] ){

    if(argc<2){
        cout<<"usage: ./lexer <filename>"<<endl;
        return 1;
    }

    string filename = argv[1];
    vector<pair<string, string>> tokens;

    ifstream infile;
    infile.open(filename);
    string str = "";
    while (getline(infile, str))
    {
        tokenizer(str, tokens);
    }
    printTokens(tokens);
    //save tokens in a file as binary
    ofstream outfile;
    outfile.open("tokens", ios::binary);
    for(int i=0; i<tokens.size(); i++){
        outfile.write((char*)&tokens[i].first, sizeof(tokens[i].first));
        outfile.write((char*)&tokens[i].second, sizeof(tokens[i].second));
    }

    outfile.close();

    infile.close();
    
    return 0;
}