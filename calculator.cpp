#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include "string_calculator.h"
#include "calculator.h"

using std::cout, std::endl, std::wstring, std::stack, std::wistringstream;

int precedence(wchar_t c) {
    if (c == L'^')
        return 4;
    else if (c == L'*' || c == L'/' || c == L'%')
        return 3;
    else if (c == L'+' || c == L'-')
        return 2;
    else
        return 1;
}

bool isOperator (wchar_t c) {
    if (c == L'/' || c == L'*' || c == L'+' || c == L'-' || c == L'%' || c == L'^')
        return true;
    return false;
}

bool validate (wstring s) {
    if (s == L"()")
        return false;
    stack<wchar_t> parenStack;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s.at(i) == L'(')
            parenStack.push(s.at(i));
        else if (s.at(i) == L')')
        {    
            if (parenStack.empty())
                return false;
            parenStack.pop();
        }
    }
    if (!parenStack.empty())
        return false;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (!isOperator(s.at(i)) && s.at(i) != ')' && s.at(i) != '(' && !isdigit(s.at(i)))
            return false;  
    }
    return true;
}

wstring postfix (wstring s) { // Implement error checking for only (
    stack<wchar_t> theStack;
    wstring answer;
    wchar_t c;
    size_t sentinel = s.length();
    size_t i = 0;
    while (i < sentinel)
    {
        c = s.at(i);
        if (c == L'(')
        {
            theStack.push(c);
            i++;
        }
        else if (c == L')')
        {
            while (theStack.top() != L'(')
            {
                answer.push_back(theStack.top());
                answer.push_back(L' ');
                theStack.pop();
            }
            theStack.pop();
            i++;
        }
        else if ( !isOperator(c) || 
        ( c == L'-' && i != 0 && s.at(i-1) == L'(') || 
        ( i == 0 && c == L'-' ) ||
        ( c == L'-' && i != 0 && isOperator(s.at(i-1))) )
        {    
            answer.push_back(s.at(i));
            i++;
            while (i < sentinel && s.at(i) != L')' && !isOperator(s.at(i)))
            {
                answer.push_back(s.at(i));
                i++;
            }
            answer.push_back(L' ');
        }
        else
        {
            while (!theStack.empty() && precedence(c) <= precedence(theStack.top()))
            {
                answer.push_back(theStack.top());
                answer.push_back(L' ');
                theStack.pop();
            }
            theStack.push(c);
            i++;
        }
    }
    while (!theStack.empty())
    {
        answer.push_back(theStack.top());
        answer.push_back(L' ');
        theStack.pop();
    }
    return answer;
}

wstring standardize (wstring s) // makes it so each infix has only 1 space in between
{
    // first, if it has spaces, make it not have spaces
    wstring noSpaceResult = L"";
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s.at(i) != L' ')
            noSpaceResult.push_back(s.at(i));
    }
    return noSpaceResult;
}

wstring calculate (wstring s) {
    wstring result = L"";
    wstring lhs;
    wstring rhs;
    wstring parse;
    wistringstream iss (s);
    stack<wstring> theStack;
    while (iss >> parse)
    {
        if (parse.length() != 1 || !isOperator(parse.at(0))) // is operand
        {
            theStack.push(parse);
        }
        else if (isOperator(parse.at(0)) && parse.length() == 1) // is one of the 4 functions
        {
            switch (parse.at(0))
			{
				case L'+': 
                    if (theStack.empty()) // occurs if there's not another operand
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					rhs = theStack.top(); 
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
					theStack.push(add(lhs,rhs));
					break;
				case L'-':
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					rhs = theStack.top(); 
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
					theStack.push(subtract(lhs,rhs));
					break;
				case L'*':
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					rhs = theStack.top(); 
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
					theStack.push(multiply(lhs,rhs));
					break;
				case L'/':
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					rhs = theStack.top();
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
					theStack.push(division(lhs,rhs));
					break;
                case L'%':
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					rhs = theStack.top(); 
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
					lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
					theStack.push(modulus(lhs,rhs));
					break;
                case L'^':
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
                    rhs = theStack.top();
                    theStack.pop();
                    if (theStack.empty())
                    {
                        throw std::invalid_argument("Invalid expression.");
                    }
                    lhs = theStack.top();
                    theStack.pop();
                    //cout << "lhs: " << lhs << endl;
                    //cout << "rhs: " << rhs << endl;
                    theStack.push(power(lhs, rhs));
                    break;
				default: 
					cout << "[ERROR] invalid operator: " << parse.at(0) << endl;
					break;
			}
        }
    }
    return theStack.top();
}

