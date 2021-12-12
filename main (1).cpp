#include <iostream>
#include<cctype>
#include <bits/stdc++.h>
using namespace std;

/* References: 
https://www.geeksforgeeks.org/convert-infix-prefix-notation/
https://www.geeksforgeeks.org/prefix-infix-conversion/
https://www.geeksforgeeks.org/prefix-postfix-conversion/
https://www.geeksforgeeks.org/postfix-to-infix/
https://www.geeksforgeeks.org/postfix-prefix-conversion/
https://www.geeksforgeeks.org/expression-evaluation/
*/

bool isOperator(char);
int Precedence (char);
string removeSpace (string);

string infix_to_prefix (string);
string infix_to_postfix (string);
string prefix_to_infix (string);
string prefix_to_postfix (string);
string postfix_to_infix (string);
string postfix_to_prefix (string);

int Arithmetic(int, char, int);
int infix_evaluation(string);

int flag;
int main()
{
    string input;
    cout<<"Enter string: ";
    getline(cin, input);
    
    string INPUT = removeSpace(input);
    int length = INPUT.size();
    
    string input_type; 
    bool to_evaluate=false;
    
    // Check if the input contains integers
    if (any_of(INPUT.begin(), INPUT.end(), ::isdigit)) to_evaluate=true;
    
    for(int j=0; j<length; j++) {
        if (isOperator(INPUT[length-1]) && INPUT[length-1]!=')') {
            input_type = "postfix"; 
            break;
        }
        else if (INPUT[j]=='(') {
            continue;
        }
        else if (isOperator(INPUT[j])) {
            input_type = "prefix"; 
            break;
        }
        else {
            input_type = "infix"; 
            break;
        }
    }
    
    if (input_type=="infix") {
        if (to_evaluate==true) cout<< infix_evaluation(INPUT) << endl;
        else {
            flag=1;
            cout<<"Prefix: "<<infix_to_prefix(INPUT)<<endl;
            flag=0;
            cout<<"Potfix: "<<infix_to_postfix(INPUT)<<endl;
        }
    }
    else if (input_type=="prefix") {
        string temp = prefix_to_infix(INPUT);
        if (to_evaluate==true) cout<<infix_evaluation(temp)<<endl;
        else {
            cout <<"Infix: "<<prefix_to_infix(INPUT)<<endl;
            cout <<"Postfix: "<<prefix_to_postfix(INPUT)<<endl;
        }
    }
    else if (input_type=="postfix") {
        string temp = postfix_to_infix(INPUT);
        if (to_evaluate==true) cout<<infix_evaluation(temp)<<endl;
        else {
            cout <<"Infix: "<<postfix_to_infix(INPUT)<<endl;
            cout <<"Prefix: "<<postfix_to_prefix(INPUT)<<endl;
        }
    }
    else cout << "Will work on it\n";

    return 0;
}


string removeSpace(string input) {
    int i=0, j=0;
    int l=input.size();
    char noSpace[l];
    while (input[i]!='\0') {
        if (input[i]!=32) {
            noSpace[j]=input[i];
            j++;
        }
        i++;
    } noSpace[j]='\0';
    return noSpace;
}


bool isOperator(char x) {
    return (!isdigit(x) && !isalpha(x));
}


int Precedence(char x) {
    if (x=='+'||x=='-') return 1;
    else if (x=='*'||x=='/') return 2;
    else if (x=='^') return 3;
    return 0;
}


string infix_to_prefix (string infix) {

    int l = infix.size();
    reverse(infix.begin(), infix.end());
 
    // Replace ( with ) and vice versa
    for (int i = 0; i < l; i++) {
        if (infix[i] == '(') {
            infix[i] = ')';
            i++;
        }
        else if (infix[i] == ')') {
            infix[i] = '(';
            i++;
        }
    }
    // Get the postfix then reverse
    string prefix = infix_to_postfix(infix);
    reverse(prefix.begin(), prefix.end());
 
    return prefix;
}


string infix_to_postfix (string infix) {
    
    infix = '(' + infix + ')';
    int l = infix.size();
    stack <char> Stack;
    string result;
 
    for (int i = 0; i < l; i++) {
      
        if (isalpha(infix[i]) || isdigit(infix[i])) 
            result += infix[i];
            
        else if (infix[i] == '(') 
            Stack.push('(');
            
        else if (infix[i] == ')') {
            while (Stack.top() != '(') {
                result += Stack.top();
                Stack.pop();
            }
            // Remove '(' from the stack
            Stack.pop();
        }
        
        // If operator is found
        else {
            if (isOperator(Stack.top())) {
                if(infix[i] == '^') {
                    while (Precedence(infix[i]) <= Precedence(Stack.top())) {
                        result += Stack.top();
                        Stack.pop();
                    }
                }
                else {
                    /* flag=1 means the function infix_to_postfix is called
                       from the function infix_to_prefix while flag=0 means 
                       the function was called from the main */
                    switch(flag) {
                        case 0:
                            while (Precedence(infix[i]) <= Precedence(Stack.top())) {
                            result += Stack.top();
                            Stack.pop();
                            }
                            break;
                        case 1:
                            while (Precedence(infix[i]) < Precedence(Stack.top())) {
                            result += Stack.top();
                            Stack.pop();
                            }
                            break;
                    }
                }
                // Push current Operator on stack
                Stack.push(infix[i]);
            }
        }
    }
      while(!Stack.empty()){
         result += Stack.top();
        Stack.pop();
    }
    return result;
}


string prefix_to_infix(string prefix) {
    
    stack<string> Stack;
    int length = prefix.size();
 
    // Reading the prefix expression from right to left
    for (int i = length - 1; i >= 0; i--) {
 
    // If scanned character is an operator, pop two operands from the stack
        if (isOperator(prefix[i])) {
            string op1 = Stack.top();   Stack.pop();
            string op2 = Stack.top();   Stack.pop();
     
            // Concatenate the operands and operator
            string temp = "(" + op1 + prefix[i] + op2 + ")";
            
            // Push the concatenated string back to stack
            Stack.push(temp);
        }
     
        // If the scanned character is an operand, push it to the stack
        else {
            Stack.push(string(1, prefix[i]));
        }
    }
    // Infix expression is now contained in the stack
    return Stack.top();
}

string prefix_to_postfix (string prefix) {
    stack<string> Stack;
    int l=prefix.size();
    
    for (int i=l-1; i>=0; i--) {
        if (isOperator(prefix[i])) {
            string op1 = Stack.top(); Stack.pop();
            string op2 = Stack.top(); Stack.pop();
            
            string temp = op1 + op2 + prefix[i];
            Stack.push(temp);
        }
        else {
            Stack.push(string(1,prefix[i]));
        }
    }
    return Stack.top();
}


string postfix_to_infix(string postfix) {
    stack <string> Stack;
    int l = postfix.size();
    
    // Reading postfix expression from left to right
    for (int i=0; i<l; i++) {
        if (!isOperator(postfix[i])) {
            Stack.push(string(1, postfix[i]));
        }
        else {
            string op1 = Stack.top(); Stack.pop();
            string op2 = Stack.top(); Stack.pop();
            string temp = '(' + op2 + postfix[i] + op1 + ')';
            Stack.push(temp);
        }
    }
    return Stack.top();
}


string postfix_to_prefix (string postfix) {
    stack <string> Stack;
    int l = postfix.size();
    
    for (int i=0; i<l; i++) {
        if (!isOperator(postfix[i])) {
            Stack.push(string(1, postfix[i]));
        }
        else {
            string op1 = Stack.top(); Stack.pop();
            string op2 = Stack.top(); Stack.pop();
            string temp = postfix[i] + op2 + op1;
            Stack.push(temp);
        }
    }
    
    string prefix = "";
    while (!Stack.empty()) {
        prefix += Stack.top();
        Stack.pop();
    }
    return prefix;
}


int Arithmetic (int a, int b, char op) {
    switch (op) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return a/b;
        default: return 0;
    }
}


int infix_evaluation (string infix) {
    stack <char> Operators;
    stack <int> Values;
    
    for (int i=0; i<infix.length(); i++) {
        
        if (infix[i]==' ') continue;
        
        else if (infix[i]=='(') {
            Operators.push('(');
        }
        
        else if (isdigit(infix[i])) {
            int temp=0;
            while (i<infix.length() && isdigit(infix[i])) {
                temp = (temp*10) + infix[i]-'0';
                i++;
            }
            Values.push(temp);
            i--;
        }
        
        else if (infix[i]==')') {
            while (!Operators.empty() && Operators.top()!='(') {
                int op2 = Values.top(); Values.pop();
                int op1 = Values.top(); Values.pop();
                
                char op = Operators.top(); Operators.pop();
                int temp = Arithmetic (op1, op2, op);
                Values.push(temp);
            }
            if (!Operators.empty()) {
                Operators.pop();
            }
        }
        
        else {
            while(!Operators.empty()&&Precedence(Operators.top())>=Precedence(infix[i])){
                int val2 = Values.top();
                Values.pop();
                 
                int val1 = Values.top();
                Values.pop();
                 
                char op = Operators.top();
                Operators.pop();
                 
                Values.push(Arithmetic(val1, val2, op));
            }
             
            // Push current token to 'ops'.
            Operators.push(infix[i]);
        }
    }
     
    while(!Operators.empty()){
        int val2 = Values.top();
        Values.pop();
                 
        int val1 = Values.top();
        Values.pop();
                 
        char op = Operators.top();
        Operators.pop();
                 
        Values.push(Arithmetic(val1, val2, op));
    }
     
    return Values.top();
}






