/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:52:15 by rrichard          #+#    #+#             */
/*   Updated: 2025/12/02 13:58:57 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

bool	evaluate( const std::string& formula )
{
	std::stack<bool>	_stack;

	for (const auto& c : formula)
	{
		if (c == '0' || c == '1')
			_stack.push(c == '0' ? false : true);
		else
		{
			if (c == '!')
			{
				if (_stack.empty())
					throw std::runtime_error("Error: missing operand for '!");
				bool operand = _stack.top();
				_stack.pop();
				_stack.push(!operand);
			}
			else
			{
				if (_stack.size() < 2)
					throw std::runtime_error("Error: missing operand for binary operator");
				bool right = _stack.top(); _stack.pop();
				bool left = _stack.top(); _stack.pop();
				if (c == '&')
					_stack.push(left && right);
				else if (c == '|')
					_stack.push(left || right);
				else if (c == '^')
					_stack.push(left ^ right);
				else if (c == '>')
					_stack.push(!left || right);
				else if (c == '=')
					_stack.push(left == right);
				else
					throw std::runtime_error("Error: operator not supported");
			}
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: wrong expression.");
	return (_stack.top());
}

std::string toInfix( const std::string& formula )
{
	std::stack<std::string>	_stack;

	for (const auto& c : formula)
	{
		if (c == '0' || c == '1' || (isalpha(c)))
			_stack.push(std::string(1, c));
		else if (c == '!')
		{
			if (_stack.empty())
				throw std::runtime_error("Error: missing operand for '!");
			std::string	operand = _stack.top();
			_stack.pop();
			std::string subExpr = "(!" + operand + ")";
			_stack.push(subExpr); 
		}
		else if (c == '&' || c == '|' || c == '^' || c == '>' || c == '=')
		{
			if (_stack.size() < 2)
				throw std::runtime_error("Error: missing operand for binary operator");
			std::string	operand2 = _stack.top();
			_stack.pop();
			std::string	operand1 = _stack.top();
			_stack.pop();
			std::string	opStr(1, c);
			std::string	subExpr = "(" + operand1 + " " + opStr + " " + operand2 + ")";
			_stack.push(subExpr);
		}
		else
			throw std::runtime_error("Error: operator not supported");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: wrong expression");
	return(_stack.top());
}
