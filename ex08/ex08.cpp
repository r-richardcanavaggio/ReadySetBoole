/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex08.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/24 22:08:27 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

vector<vector<int>>	powerset( vector<int> set )
{
	vector<vector<int>>	res;
	int				n = static_cast<int>(set.size());

	for (int mask = 0; mask < (1 << n); mask++)
	{
		vector<int>	newSet;
		for (int j = 0; j < n; j++)
		{
			if (mask & (1 << j))
				newSet.push_back(set[j]);
		}
		res.push_back(newSet);
	}
	return (res);
}

int main( int argc, char *argv[] )
{
	if (argc != 2)
		return (cerr << "Wrong number of arguments" << endl, 1);
	string			input = argv[1];
	istringstream	iss(input);
	vector<int>		v;
	int				i;
	
	while (iss >> i)
		v.push_back(i);
	vector<vector<int>> pwr = powerset(v);
	for (auto& i : pwr)
	{
		for (auto& j : i)
			cout << j << " ";
		cout << endl;
	}
	return (0);
}
