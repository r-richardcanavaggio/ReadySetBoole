/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex04.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:03:28 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/24 15:37:03 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <iostream>
#include <string>
#include <vector>

bool	eval_formula( const std::string& formula )
{
	std::stack<bool>	stack;

	for (const auto& c : formula)
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
					throw std::runtime_error("Error: not enough operands for operator.");
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
					throw std::runtime_error("Error: operator not supported");
			}
		}
	}
	if (stack.size() != 1)
		throw std::runtime_error("Error: wrong expression.");
	return (stack.top());
}

std::vector<char> buildVarList( const std::string& input )
{
	std::vector<char>	res;
	bool				seen[26] = {false};

	for (const auto& c : input)
	{
		if (c >= 'A' && c <= 'Z')
		{
			int	index = c - 'A';
		
			if (!seen[index])
			{
				seen[index] = true;
				res.push_back(c);
			}
		}
	}
	if (res.size() < 2)
		throw std::runtime_error("Error: not enough variables");
	return (res);
}

void	print_truth_table( const std::string& input )
{
	std::vector<char>	vars = buildVarList(input);
	size_t				n = vars.size();
	size_t				limit = 1U << n;

	for (auto& c : vars)
		std::cout << "| " << c << " ";
	std::cout << "| = |" << std::endl;
	for (size_t i = 0; i < n + 1; i++)
		std::cout << "|---";
	std::cout << "|" << std::endl;

	for (size_t i = 0; i < limit; i++)
	{
		std::string expr = input;
		for (size_t j = 0; j < n; j++)
		{
			bool	isTrue = (i >> (n - 1 - j)) & 1U;
			char var = vars[j];
			char val = isTrue ? '1' : '0';
			for (size_t k = 0; k < expr.length(); k++)
			{
				if (expr[k] == var)
					expr[k] = val;
			}
			std::cout << "| " << (isTrue ? 1 : 0) << " ";
		}
		std::cout << "| " << (eval_formula(expr) ? '1' : '0') << " |" << std::endl;
	}
}

int main (void)
{
	try
	{
		std::string	input;

		std::cout << "Enter propositional RPN formula: ";
		getline(std::cin, input);
		print_truth_table(input);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}