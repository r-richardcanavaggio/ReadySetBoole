/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:48:18 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/24 13:02:37 by rrichard         ###   ########.fr       */
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

// Abstract Syntax Tree Recursive functions
unique_ptr<ASTNode>	parseFormula( string::iterator &it, string::iterator end );
unique_ptr<ASTNode>	parseImplication( string::iterator &it, string::iterator end );
unique_ptr<ASTNode>	parseDisjunction( string::iterator &it, string::iterator end );
unique_ptr<ASTNode>	parseTerm( string::iterator &it, string::iterator end );
unique_ptr<ASTNode>	parseFactor( string::iterator &it, string::iterator end );
unique_ptr<ASTNode>	parsePrimary( string::iterator &it, string::iterator end );

// AST Helpers
void				print2D( const ASTNode*, const string& prefix = "", bool isLeft = false, bool isRoot = true );
unique_ptr<ASTNode>	transformASTtoNNF( unique_ptr<ASTNode>, bool neg = false );
unique_ptr<ASTNode>	transformASTtoCNF( unique_ptr<ASTNode> node );
unique_ptr<ASTNode>	eliminateImplications( unique_ptr<ASTNode> node );
void				computeToRpn( ASTNode*, string& );
unique_ptr<ASTNode>	toAST( const string& );

// Formula functions
string				toInfix( const string& );
bool				evaluate( const string& );
void				printASTFormula( const string& );
void				printASTFormulaNNF( const string& );
void				printASTFormulaCNF( const string& );
string				negation_normal_form( const string& );
string				conjunctive_normal_form( const string& );

#endif //RPN_HPP