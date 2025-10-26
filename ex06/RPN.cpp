/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:52:15 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 18:18:59 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

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

string toInfix( const string& formula )
{
	stack<string>	s;

	for (auto c : formula)
	{
		if (c == '0' || c == '1' || (isalpha(c)))
			s.push(string(1, c));
		else if (c == '!')
		{
			if (s.empty())
				throw runtime_error("Error: missing operand for '!");
			string	operand = s.top();
			s.pop();
			string subExpr = "(!" + operand + ")";
			s.push(subExpr); 
		}
		else if (c == '&' || c == '|' || c == '^' || c == '>' || c == '=')
		{
			if (s.size() < 2)
				throw runtime_error("Error: missing operand for binary operator");
			string	operand2 = s.top();
			s.pop();
			string	operand1 = s.top();
			s.pop();
			string	opStr(1, c);
			string	subExpr = "(" + operand1 + " " + opStr + " " + operand2 + ")";
			s.push(subExpr);
		}
		else
			throw runtime_error("Error: operator not supported");
	}
	if (s.size() != 1)
		throw runtime_error("Error: wrong expression");
	return(s.top());
}

unique_ptr<ASTNode>	parsePrimary( string::iterator &it, string::iterator end )
{
	if (it == end)
		throw runtime_error("Unexpected end of input");
	if (*it == '(')
	{
		it++;
		auto expr = parseFormula(it, end);
		if (it == end || *it != ')')
			throw runtime_error("Expected ')'");
		it++;
		return (expr);
	}
	else if (*it == '0' || *it == '1')
	{
		NodeType	type = (*it == '0') ? NodeType::FALSE : NodeType::TRUE;
		string	value(1, *it);
		it++;
		return (make_unique<ASTNode>(type, value, nullptr, nullptr));
	}
	else if (isalpha(*it))
	{
		string	value(1, *it);
		it++;
		return (make_unique<ASTNode>(NodeType::VARIABLE, value, nullptr, nullptr));
	}
	else
		throw runtime_error("Unexpected token: " + string(1, *it));
}

unique_ptr<ASTNode>	parseFactor( string::iterator &it, string::iterator end )
{
	if (it != end && *it == '!')
	{
		it++;
		auto operand = parseFactor(it, end);
		return (make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(operand), nullptr));
	}
	else
		return (parsePrimary(it, end));
}

unique_ptr<ASTNode>	parseTerm( string::iterator &it, string::iterator end )
{
	auto	left = parseFactor(it, end);
	while (it != end && *it == '&')
	{
		it++;
		auto	right = parseFactor(it, end);
		left = make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(left), std::move(right));
	}
	return (left);
}

unique_ptr<ASTNode>	parseDisjunction( string::iterator &it, string::iterator end )
{
	auto left = parseTerm(it, end);
	while (it != end && (*it == '^' || *it == '|'))
	{
		char op = *it;
		it++;
		auto	right = parseTerm(it, end);
		if (op == '^')
			left = make_unique<ASTNode>(NodeType::EXCLUSIVE_DISJUNCTION, "^", std::move(left), std::move(right));
		else // op == '|'
			left = make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(left), std::move(right));
	}
	return (left);
}

unique_ptr<ASTNode>	parseImplication( string::iterator &it, string::iterator end )
{
	auto	left = parseDisjunction(it, end);
	while (it != end && *it == '>')
	{
		it++;
		auto	right = parseDisjunction(it, end);
		left = make_unique<ASTNode>(NodeType::MATERIAL_CONDITION, ">", std::move(left), std::move(right));
	}
	return (left);
}

unique_ptr<ASTNode>	parseFormula( string::iterator &it, string::iterator end )
{
	auto	left = parseImplication(it, end);
	while (it != end && *it == '=')
	{
		it++;
		auto	right = parseImplication(it, end);
		left = make_unique<ASTNode>(NodeType::LOGICAL_EQUIVALENCE, "=", std::move(left), std::move(right));
	}
	return (left);
}

unique_ptr<ASTNode>	toAST( const string& formula )
{	
	string				input;
	string::iterator	it;
	unique_ptr<ASTNode>	ast;

	input = formula;
	input.erase(remove(input.begin(), input.end(), ' '), input.end());
	it = input.begin();
	ast = parseFormula(it, input.end());
	if (it != input.end())
		throw runtime_error(string("Unexpected token at end of input: ") + string(1, *it));
	return (ast);
}

unique_ptr<ASTNode>	eliminateImplications( unique_ptr<ASTNode> node )
{
	if (!node)
		return nullptr;
	node->left = eliminateImplications(std::move(node->left));
	node->right = eliminateImplications(std::move(node->right));

	switch (node->type)
	{
		case NodeType::MATERIAL_CONDITION:
			return (make_unique<ASTNode>(NodeType::DISJUNCTION, "|",
				make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(node->left), nullptr), std::move(node->right)));
		case NodeType::LOGICAL_EQUIVALENCE:
		{
			// (A & B) | (!A & !B))
			auto A = std::move(node->left);
			auto B = std::move(node->right);
			auto cpA = A->clone();
			auto cpB = B->clone();
			auto newLeft = make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(A), std::move(B));
			auto newRight = make_unique<ASTNode>(NodeType::CONJUNCTION, "&",
					make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpA), nullptr),
					make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpB), nullptr));
			return (make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(newLeft), std::move(newRight)));
		}
		case NodeType::EXCLUSIVE_DISJUNCTION:
		{
			// A ^ B  === (A & !B) | (!A & B)
			auto A = std::move(node->left);
			auto B = std::move(node->right);
			auto cpA = A->clone();
			auto cpB = B->clone();
			auto newLeft = make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(A),
				make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(B), nullptr));
			auto newRight = make_unique<ASTNode>(NodeType::CONJUNCTION, "&",
				make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpA), nullptr), std::move(cpB));
			return (make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(newLeft), std::move(newRight)));
		}
		default:
			return (node);
	}
	return (node);
}

unique_ptr<ASTNode>	transformASTtoNNF( unique_ptr<ASTNode> node, bool neg )
{
	if (!node)
		return (nullptr);
	
	if (node->type == NodeType::VARIABLE)
	{
		if (!neg)
			return (node);
		return (make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(node), nullptr));
	}
	if (node->type == NodeType::NEGATION)
		return (transformASTtoNNF(std::move(node->left), !neg));
	if (!neg)
	{
		node->left = transformASTtoNNF(std::move(node->left), false);
		node->right = transformASTtoNNF(std::move(node->right), false);
		return (node);
	}
	else
	{
		if (node->type == NodeType::CONJUNCTION)
		{
			auto L = transformASTtoNNF(std::move(node->left), true);
			auto R = transformASTtoNNF(std::move(node->right), true);
			return (make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(L), std::move(R)));
		}
		if (node->type == NodeType::DISJUNCTION)
		{
			auto L = transformASTtoNNF(std::move(node->left), true);
			auto R = transformASTtoNNF(std::move(node->right), true);
			return (make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(L), std::move(R)));
		}
	}
	return (node);
}

void	print2D( const ASTNode* node, const string& prefix, bool isLeft, bool isRoot )
{
	if (!node)
		return ;

	if (node->right)
		print2D(node->right.get(), prefix + (isRoot ? "" : (isLeft ? "│   " : "    ")), false, false);

	cout << prefix;
	if (!isRoot)
		cout << (isLeft ? "└── " : "┌── ");
	cout << node->value << endl;

	if (node->left)
		print2D(node->left.get(), prefix + (isRoot ? "" : (isLeft ? "    " : "│   ")), true, false);
}

void	printASTFormula( const string& formula )
{
	string				infix;
	unique_ptr<ASTNode>	ast;

	infix = toInfix(formula);
	ast = toAST(infix);
	print2D(ast.get());
}

void	printASTFormulaNNF( const string& formula )
{
	string				infix;
	unique_ptr<ASTNode>	ast;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	print2D(ast.get());
}

string	negation_normal_form( const string& formula )
{
	unique_ptr<ASTNode>	ast;
	string				infix;
	string				nnf;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	computeToRpn(ast.get(), nnf);
	return (nnf);
}

void	computeToRpn( ASTNode* node, string& str )
{
	if (!node)
		return ;
	computeToRpn(node->left.get(), str);
	computeToRpn(node->right.get(), str);
	str += node->value;
}
