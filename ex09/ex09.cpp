/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex09.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/25 15:44:03 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <algorithm>
#include <sstream>

using namespace std;

vector<int>	eval_set( const string& formula, vector<vector<int>> sets )
{
	stack<vector<int>>					_stack;
	unordered_map<char, vector<int>>	_map;
	vector<int>							univers;

	for (auto& s : sets)
	{
		sort(s.begin(), s.end());
		s.erase(unique(s.begin(), s.end()), s.end());
	}
	for (size_t i = 0; i < sets.size(); i++)
		_map['A' + static_cast<char>(i)] = sets[i];
	for (auto& current : sets)
	{
		sort(univers.begin(), univers.end());
		vector<int>	temp;
		set_union(univers.begin(), univers.end(), current.begin(), current.end(), back_inserter(temp));
		univers = std::move(temp);
	}
	for (char c : formula)
	{
		if (c >= 'A' && c <= 'Z')
		{
			auto itv = _map.find(c);
			if (itv == _map.end())
				throw runtime_error("Error: variable out of range for provided sets");
			_stack.push(itv->second);
		}
		else
		{
			if (c == '!')
			{
				vector<int> res, operand = _stack.top(); _stack.pop();
				set_difference(univers.begin(), univers.end(), operand.begin(), operand.end(), back_inserter(res));
				_stack.push(res);
			}
			else
			{
				if (_stack.size() < 2)
					throw runtime_error("Error: not enough operands for operator.");
				vector<int> operand1 = _stack.top(); _stack.pop();
				vector<int> operand2 = _stack.top(); _stack.pop();
				if (c == '&')
				{
					vector<int>	res;

					set_intersection(operand1.begin(), operand1.end(), operand2.begin(), operand2.end(), back_inserter(res));
					_stack.push(res);
				}
				else if (c == '|')
				{
					vector<int>	res;

					set_union(operand1.begin(), operand1.end(), operand2.begin(), operand2.end(), back_inserter(res));
					_stack.push(res);
				}
				else if (c == '^')
				{
					vector<int>	res;

					set_symmetric_difference(operand1.begin(), operand1.end(), operand2.begin(), operand2.end(), back_inserter(res));
					_stack.push(res);
				}
				else if (c == '>')
				{
					vector<int> complement, res;
					
					set_difference(univers.begin(), univers.end(), operand2.begin(), operand2.end(), back_inserter(complement));
					set_union(operand1.begin(), operand1.end(), complement.begin(), complement.end(), back_inserter(res));
					_stack.push(res);
				}
				else if (c == '=')
				{
					vector<int> symdiff, res;
					
					set_symmetric_difference(operand1.begin(), operand1.end(), operand2.begin(), operand2.end(), back_inserter(symdiff));
					set_difference(univers.begin(), univers.end(), symdiff.begin(), symdiff.end(), back_inserter(res));
					_stack.push(res);
				}
				else
					throw runtime_error("Error: operator not supported");
			}
		}
	}
	if (_stack.size() != 1)
		throw runtime_error("Error: wrong expression.");
	return (_stack.top());
}

int main( void )
{
	vector<vector<int>> sets = {{1, 2, 3}, {2,4}, {0,3}};
	try
	{
		string	input;
		cout << "A=1 2 3\nB=2 4\nC=0 3\nEnter propositional formula: ";
		getline(cin, input);
		while (input != "EXIT")
		{
			vector<int>			results = eval_set(input, sets);
			cout << "[";
			for (vector<int>::iterator it = results.begin(); it < results.end(); it++)
			{
				if (it != results.end() - 1)
					cout << *it << ", ";
				else
					cout << *it;
			}
			cout << "]";
			cout << endl << "Enter propositional formula: ";
			getline(cin, input);
		}
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
	}
	return (0);
}
