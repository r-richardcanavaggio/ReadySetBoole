/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:44:31 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/24 16:12:16 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <stdexcept>
#include <iostream>
#include "ASTNode.hpp"
#include "RPN.hpp"

// Abstract Syntax Tree Recursive functions
std::unique_ptr<ASTNode>	parseFormula( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseImplication( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseDisjunction( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseTerm( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseFactor( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parsePrimary( std::string::iterator &it, std::string::iterator end );

// AST Helpers
void						print2D( const ASTNode*, const std::string& prefix = "", bool isLeft = false, bool isRoot = true );
std::unique_ptr<ASTNode>	transformASTtoNNF( std::unique_ptr<ASTNode>, bool neg = false );
std::unique_ptr<ASTNode>	eliminateImplications( std::unique_ptr<ASTNode> node );
void						computeToRpn( ASTNode*, std::string& );
std::unique_ptr<ASTNode>	toAST( const std::string& );
void						printASTFormula( const std::string& );
void						printASTFormulaNNF( const std::string& );
std::string					negation_normal_form( const std::string& );