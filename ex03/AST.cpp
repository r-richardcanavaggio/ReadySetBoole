/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:09:29 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/15 22:26:36 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

std::unique_ptr<ASTNode>	parseFormula( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseImplication(it, end);
	while (it != end && *it == "=")
	{
		it++;
		auto	right = parseImplication(it, end);
		left = std::make_unique<ASTNode>(NodeType::LOGICAL_EQUIVALENCE, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseImplication( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseDisjunction(it, end);
	while (it != end && *it == ">")
	{
		it++;
		auto	right = parseDisjunction(it, end);
		left = std::make_unique<ASTNode>(NodeType::MATERIAL_CONDITION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseDisjunction( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto left = parseTerm(it, end);
	while (it != end && (*it == "^" || *it == "|"))
	{
		std::string op = *it;
		it++;
		auto	right = parseTerm(it, end);
		if (op == "^")
			left = std::make_unique<ASTNode>(NodeType::EXCLUSIVE_DISJUNCTION, "", std::move(left), std::move(right));
		else // op == "|"
			left = std::make_unique<ASTNode>(NodeType::DISJUNCTION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseTerm( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseFactor(it, end);
	while (it != end && *it == "&")
	{
		it++;
		auto	right = parseFactor(it, end);
		left = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseFactor( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	if (it != end && *it == "!")
	{
		it++;
		auto operand = parseFactor(it, end);
		return (std::make_unique<ASTNode>(NodeType::NEGATION, "", std::move(operand), nullptr));
	}
	else
		return (parsePrimary(it, end));
}

std::unique_ptr<ASTNode>	parsePrimary( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	if (it == end)
		throw std::runtime_error("Unexpected end of input");
	if (*it == "(")
	{
		it++;
		auto expr = parseFormula(it, end);
		if (it == end || *it != ")")
			throw std::runtime_error("Expected ')'");
		it++;
		return (expr);
	}
	else if (*it == "0" || *it == "1")
	{
		NodeType	type = (*it == "0") ? NodeType::FALSE : NodeType::TRUE;
		std::string	value = *it;
		it++;
		return (std::make_unique<ASTNode>(type, std::move(value), nullptr, nullptr));
	}
	else if (it->size() == 1 && isalpha((*it)[0]))
	{
		std::string	value = *it;
		it++;
		return (std::make_unique<ASTNode>(NodeType::VARIABLE, std::move(value), nullptr, nullptr));
	}
	else
		throw std::runtime_error("Unexpected token: " + *it);
}

std::vector<std::string>	tokenize( const std::string& input )
{
	std::vector<std::string>	tokens;
	std::istringstream			iss(input);
	std::string					token;

	while (iss >> token)
		tokens.push_back(token);
	return (tokens);
}

std::unique_ptr<ASTNode>	parse( const std::string& input )
{
	std::vector<std::string>	tokens = tokenize(input);
	auto						it = tokens.begin();
	auto						ast = parseFormula(it, tokens.end());

	if (it != tokens.end())
		throw std::runtime_error("Unexpected token at end of input: " + *it);
	return (ast);
}

std::string	getNodeSymbol( const ASTNode* node )
{
	switch (node->type)
	{
		case NodeType::FALSE: return "0";
		case NodeType::TRUE: return "1";
		case NodeType::NEGATION: return "!";
		case NodeType::CONJUNCTION: return "&";
		case NodeType::DISJUNCTION: return "|";
		case NodeType::EXCLUSIVE_DISJUNCTION: return "^";
		case NodeType::MATERIAL_CONDITION: return ">";
		case NodeType::LOGICAL_EQUIVALENCE: return "=";
		case NodeType::VARIABLE: return node->value;
		default: return "?";
	}
}

static constexpr double HSEP = 2.0;

ASTNode*	getLeftSibling( ASTNode* node )
{
	if (!node || !node->parent)
	return (nullptr);
	ASTNode*	parent = node->parent;
	if (parent->right.get() == node && parent->left)
	return (parent->left.get());
	return (nullptr);
}

void	setParentsAndDepth( ASTNode* node, ASTNode* parent = nullptr, int depth = 0 )
{
	if (!node) return ;
	node->parent = parent;
	node->depth = depth;
	setParentsAndDepth(node->left.get(), node, depth + 1);
	setParentsAndDepth(node->right.get(), node, depth + 1);
}

void	fillContours( const ASTNode* node, double accMod, int d, std::unordered_map<int, double>& leftmost, std::unordered_map<int, double>& rightmost )
{
	if (!node) return;
	double x = node->prelim + accMod;
	if (!leftmost.count(d) || x < leftmost[d]) leftmost[d] = x;
	if (!rightmost.count(d) || x > rightmost[d]) rightmost[d] = x;
	fillContours(node->left.get(), accMod + node->mod, d + 1, leftmost, rightmost);
	fillContours(node->right.get(), accMod + node->mod, d + 1, leftmost, rightmost);
}

double	computeShift( const ASTNode* L, const ASTNode* R, double sep )
{
	if (!L || !R) return (0.0);
	std::unordered_map<int, double>	Lleft, Lright, Rleft, Rright;
	fillContours(L, L->mod, 0, Lleft, Lright);
	fillContours(R, R->mod, 0, Rleft, Rright);

	double	need = 0.0;
	int		maxDepth = std::min<int>(Lright.size(), Rleft.size());
	for (int d = 0; d < maxDepth; d++)
	{
		if (Lright.count(d) && Rleft.count(d))
		{
			double	overlap = (Lright[d] + sep) - Rleft[d];
			if (overlap > 0.0) need = std::max(need, overlap);
		}
	}
	return (need);
}

void	calculateInitialX( ASTNode* node )
{
	if (!node) return ;
	
	calculateInitialX(node->left.get());
	calculateInitialX(node->right.get());
	
	if (!node->left && !node->right)
	{
		if (ASTNode* prev = getLeftSibling(node))
			node->prelim = prev->prelim + HSEP;
		else
			node->prelim = 0.0;
		return ;
	}

	if (node->left && node->right)
	{
		double shift = computeShift(node->left.get(), node->right.get(), HSEP);
		if (shift > 0.0)
			node->right->mod += shift;
		double L = node->left->prelim + node->left->mod;
		double R = node->right->prelim + node->right->mod;	
		node->prelim = (L + R) / 2.0;
	}
	else if (node->left)
		node->prelim = node->left->prelim + node->left->mod;
	else
		node->prelim = node->right->prelim + node->right->mod;
}

void	secondWalk( ASTNode* node, double accMod )
{
	if (!node) return ;
	
	accMod += node->mod;
	double cur = node->prelim + accMod;
	node->x = static_cast<int>(std::round(cur));
	node->y = node->depth;
	
	secondWalk(node->left.get(), accMod);
	secondWalk(node->right.get(), accMod);
}

void dumpLayoutDFS(const ASTNode* n)
{
    if (!n) return;
    std::cout << std::string(n->depth * 2, ' ')
              << getNodeSymbol(n)
              << "  depth=" << n->depth
              << " prelim=" << n->prelim
              << " mod=" << n->mod
              << " -> x=" << n->x
              << " y=" << n->y
              << '\n';
    dumpLayoutDFS(n->left.get());
    dumpLayoutDFS(n->right.get());
}

void dumpXPreorder(const ASTNode* n) // if you want only x
{
    if (!n) return;
    std::cout << getNodeSymbol(n) << ", x = " << n->x << '\n';
    dumpXPreorder(n->left.get());
    dumpXPreorder(n->right.get());
}

static void collectExtents(const ASTNode* n, int& minX, int& maxX, int& maxD) {
    if (!n) return;
    minX = std::min(minX, n->x);
    maxX = std::max(maxX, n->x);
    maxD = std::max(maxD, n->depth);
    collectExtents(n->left.get(),  minX, maxX, maxD);
    collectExtents(n->right.get(), minX, maxX, maxD);
}

static void gatherByDepth(const ASTNode* root, std::vector<std::vector<const ASTNode*>>& levels) {
    if (!root) return;
    std::queue<const ASTNode*> q;
    q.push(root);
    while (!q.empty()) {
        auto n = q.front(); q.pop();
        if (n->depth >= (int)levels.size()) levels.resize(n->depth + 1);
        levels[n->depth].push_back(n);
        if (n->left)  q.push(n->left.get());
        if (n->right) q.push(n->right.get());
    }
	for (auto& lvl : levels)
        std::sort(lvl.begin(), lvl.end(), [](const ASTNode* a, const ASTNode* b){ return a->x < b->x; });
}

void printByXY(const ASTNode* root, int scale = 2) {
    if (!root) return;

    int minX = root->x, maxX = root->x, maxDepth = root->depth;
    collectExtents(root, minX, maxX, maxDepth);

    const int width = (maxX - minX) * scale + 1;
    std::vector<std::vector<const ASTNode*>> levels;
    gatherByDepth(root, levels);

    for (int d = 0; d < (int)levels.size(); ++d) {
        // Nodes line
        std::string line(width, ' ');
        for (const ASTNode* n : levels[d]) {
            const std::string label = getNodeSymbol(n);
            int cx = (n->x - minX) * scale;
            int start = std::max(0, cx - (int)label.size() / 2);
            for (int i = 0; i < (int)label.size(); ++i) {
                int pos = start + i;
                if (pos >= 0 && pos < width) line[pos] = label[i];
            }
        }
        std::cout << line << '\n';

        // Connectors line (between this level and the next)
        if (d + 1 < (int)levels.size()) {
            std::string conn(width, ' ');
            for (const ASTNode* n : levels[d]) {
                int pc = (n->x - minX) * scale;
                bool hasL = (n->left  != nullptr);
                bool hasR = (n->right != nullptr);
                if (!hasL && !hasR) continue;

                // 1) Draw child paths first (so the "/\" below can overwrite near the parent)
                if (hasL) {
                    int lc = (n->left->x - minX) * scale;
                    if (lc < pc) {
                        conn[std::max(0, lc)] = '/';
                        for (int k = lc + 1; k < pc; ++k) conn[k] = '_';
                    }
                }
                if (hasR) {
                    int rc = (n->right->x - minX) * scale;
                    if (rc > pc) {
                        conn[std::min(width - 1, rc)] = '\\';
                        for (int k = pc + 1; k < rc; ++k) conn[k] = '_';
                    }
                }

                // 2) Always draw a small "/\" under the parent for visual symmetry
                if (pc - 1 >= 0)           conn[pc - 1] = '/';
                if (pc + 1 < (int)width)   conn[pc + 1] = '\\';
            }
            std::cout << conn << '\n';
        }
    }
}

int	main( void )
{
	std::string	input;
	std::cout << "Enter a propositional logic formula: ";
	std::getline(std::cin, input);

	try
	{
		if (input == "manual")
		{
			std::unique_ptr<ASTNode> A = std::make_unique<ASTNode>(NodeType::VARIABLE, "A", nullptr, nullptr);
			std::unique_ptr<ASTNode> B = std::make_unique<ASTNode>(NodeType::VARIABLE, "B", nullptr, nullptr);
			std::unique_ptr<ASTNode> C = std::make_unique<ASTNode>(NodeType::VARIABLE, "C", nullptr, nullptr);
			std::unique_ptr<ASTNode> I = std::make_unique<ASTNode>(NodeType::VARIABLE, "I", nullptr, nullptr);
			std::unique_ptr<ASTNode> J = std::make_unique<ASTNode>(NodeType::VARIABLE, "J", nullptr, nullptr);
			std::unique_ptr<ASTNode> H = std::make_unique<ASTNode>(NodeType::VARIABLE, "H", nullptr, nullptr);
			std::unique_ptr<ASTNode> K = std::make_unique<ASTNode>(NodeType::VARIABLE, "K", nullptr, nullptr);
			std::unique_ptr<ASTNode> L = std::make_unique<ASTNode>(NodeType::VARIABLE, "L", nullptr, nullptr);
			std::unique_ptr<ASTNode> P = std::make_unique<ASTNode>(NodeType::VARIABLE, "P", nullptr, nullptr);
			
			std::unique_ptr<ASTNode> F = std::make_unique<ASTNode>(NodeType::VARIABLE, "F", std::move(H), std::move(P));
			std::unique_ptr<ASTNode> G = std::make_unique<ASTNode>(NodeType::VARIABLE, "G", std::move(F), nullptr);
			std::unique_ptr<ASTNode> D = std::make_unique<ASTNode>(NodeType::VARIABLE, "D", std::move(B), std::move(C));
			std::unique_ptr<ASTNode> E = std::make_unique<ASTNode>(NodeType::VARIABLE, "E", std::move(A), std::move(D));
			std::unique_ptr<ASTNode> M = std::make_unique<ASTNode>(NodeType::VARIABLE, "M", std::move(I), std::move(J));
			std::unique_ptr<ASTNode> N = std::make_unique<ASTNode>(NodeType::VARIABLE, "N", std::move(G), std::move(M));
	
			std::unique_ptr<ASTNode> root = std::make_unique<ASTNode>(NodeType::VARIABLE, "O", std::move(E), std::move(N));
			
			setParentsAndDepth(root.get());
			calculateInitialX(root.get());
			secondWalk(root.get(), 0.0);
			printByXY(root.get(), 6);
		}
		else
		{
			auto	ast = parse(input);
			setParentsAndDepth(ast.get());
			calculateInitialX(ast.get());
			secondWalk(ast.get(), 0.0);
			printByXY(ast.get(), 6);
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}