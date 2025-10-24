/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex07.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/24 17:34:19 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "RPN.hpp"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

bool	evaluate( const string& formula )
{
	stack<bool>	_stack;

	for (char c : formula)
	{
		if (c == '0' || c == '1')
			_stack.push(c == '0' ? false : true);
		else
		{
			if (c == '!')
			{
				bool operand = _stack.top();
				_stack.pop();
				_stack.push(!operand);
			}
			else
			{
				if (_stack.size() < 2)
					throw runtime_error("Error: wrong expression");
				bool operand1 = _stack.top();
				_stack.pop();
				bool operand2 = _stack.top();
				_stack.pop();
				if (c == '&')
					_stack.push(operand1 && operand2);
				else if (c == '|')
					_stack.push(operand1 || operand2);
				else if (c == '^')
					_stack.push(operand1 ^ operand2);
				else if (c == '>')
					_stack.push(!operand1 || operand2);
				else if (c == '=')
					_stack.push(operand1 == operand2);
				else
					throw runtime_error("Error: operator not supported");

			}
		}
	}
	if (_stack.size() != 1)
		throw runtime_error("Error: wrong expression.");
	return (_stack.top());
}

vector<vector<int>>	createTT( unsigned int n )
{
	vector<vector<int>>	output(n, vector<int>(1 << n));
	unsigned int		cols = 1U << n;
	unsigned int		num = 1U << (n - 1);

	for (unsigned int col = 0; col < n; col++, num >>= 1U)
	{
		for (unsigned int row = num; row < cols; row += (num * 2U))
			fill_n(&output[col][row], num, 1);
	}
	return (output);
}

vector<char> buildVarList( const string& input )
{
	vector<char>				res;

	for (char c : input)
		if (c >= 'A' && c <= 'Z')
			res.push_back(c);
	if (res.size() < 2)
		throw runtime_error("Error: not enough variables");
	return (res);
}

bool	sat( const string& formula )
{
	vector<char>		vars = buildVarList(formula);
	vector<vector<int>>	table = createTT(vars.size());
	size_t				n = vars.size();
	unsigned int		cols = 1u << n;

	for (unsigned int x = 0; x < cols; x++)
	{
		string expr = formula;
		for (unsigned int y = 0; y < n; y++)
		{
			char var = vars[y];
			char val = static_cast<char>('0' + table[y][x]);
			replace(expr.begin(), expr.end(), var, val);
		}
		if (evaluate(expr) == true)
			return (true);
	}
	return (false);
}

int main( int argc, char *argv[] )
{
	if (argc != 2)
		return (cerr << "Wrong number of arguments" << endl, 1);
	try 
	{
		string input = argv[1];
		cout << boolalpha << sat(input) << endl;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
	}
	return (0);
}
