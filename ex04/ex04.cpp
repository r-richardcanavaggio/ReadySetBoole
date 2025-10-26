/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex04.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:03:28 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 18:22:19 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <map>

using namespace std;

bool	eval_formula( const string& formula )
{
	stack<bool>	stack;

	for (char c : formula)
	{
		if (c == '0' || c == '1')
			stack.push(c == '0' ? false : true);
		else
		{
			if (c == '!')
			{
				if (stack.empty())
					throw std::runtime_error("Error: missing operand for '!");
				bool operand = stack.top();
				stack.pop();
				stack.push(!operand);
			}
			else
			{
				if (stack.size() < 2)
					throw runtime_error("Error: not enough operands for operator.");
				bool operand1 = stack.top();
				stack.pop();
				bool operand2 = stack.top();
				stack.pop();
				if (c == '&')
					stack.push(operand1 && operand2);
				else if (c == '|')
					stack.push(operand1 || operand2);
				else if (c == '^')
					stack.push(operand1 ^ operand2);
				else if (c == '>')
					stack.push(!operand1 || operand2);
				else if (c == '=')
					stack.push(operand1 == operand2);
				else
					throw runtime_error("Error: operator not supported");
			}
		}
	}
	if (stack.size() != 1)
		throw runtime_error("Error: wrong expression.");
	return (stack.top());
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

void	printTT( vector<vector<int>>& table, string formula, vector<char>& vars )
{
	size_t			n = vars.size();
	unsigned int	cols = 1u << n;

	for (char c : vars)
	{
		cout << "| " << c << " ";
	}
	cout << "| = |" << endl;
	for (unsigned int i = 0; i < n + 1; i++)
		cout << "|---";
	cout << "|" << endl;
	for (unsigned int x = 0; x < cols; x++)
	{
		string expr = formula;
		for (unsigned int y = 0; y < n; y++)
		{
			char var = vars[y];
			char val = static_cast<char>('0' + table[y][x]);
			replace(expr.begin(), expr.end(), var, val);
			cout << "| " << table[y][x] << " ";
		}
		cout << "| " << (eval_formula(expr) ? '1' : '0') << " |" << endl;
	}
}

vector<char> buildVarList( const string& input )
{
	vector<char>				res;
	unordered_map<char, int>	outIndex;

	for (char c : input)
	{
		if (c >= 'A' && c <= 'Z')
		{
			if (!outIndex.count(c))
			{
				int idx = static_cast<int>(res.size());
				res.push_back(c);
				outIndex[c] = idx;
			}
		}
	}
	if (res.size() < 2)
		throw runtime_error("Error: not enough variables");
	return (res);
}

void	print_truth_table( const string& input )
{
	vector<char>		vars;
	vector<vector<int>>	table;

	vars = buildVarList(input);
	table = createTT(vars.size());
	printTT(table, input, vars);
}

int main (void)
{
	string	input;
	cout << "Enter propositional RPN formula: ";
	getline(cin, input);

	try
	{
		print_truth_table(input);
	}
	catch (const exception &e)
	{
		cerr << e.what() << endl;
	}
	return (0);
}