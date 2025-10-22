/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:48:18 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/22 15:32:22 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <memory>
#include "ASTNode.hpp"

using namespace std;

class RPN
{
	private:
		stack<bool>			_stack;
		unique_ptr<ASTNode>	ast;
		string				formula;
		bool				isInfix;

		RPN( const RPN& );
		RPN& operator=( const RPN& );
		unique_ptr<ASTNode>	parseFormula( string::iterator &it, string::iterator end );
		unique_ptr<ASTNode>	parseImplication( string::iterator &it, string::iterator end );
		unique_ptr<ASTNode>	parseDisjunction( string::iterator &it, string::iterator end );
		unique_ptr<ASTNode>	parseTerm( string::iterator &it, string::iterator end );
		unique_ptr<ASTNode>	parseFactor( string::iterator &it, string::iterator end );
		unique_ptr<ASTNode>	parsePrimary( string::iterator &it, string::iterator end );
		void				print2D( const ASTNode*, const string& prefix = "", bool isLeft = false, bool isRoot = true );
		unique_ptr<ASTNode>	transformASTtoNNF( unique_ptr<ASTNode>, bool neg = false );
		void				computeToRpn( ASTNode*, string& );
		void				toRPN( string& );
		void				toAST();
		void				toInfix();

	public:
		RPN( const string& );
		~RPN();

		bool				evaluate();
		void				printASTFormula();
		string				toNNF();
};

#endif //RPN_HPP