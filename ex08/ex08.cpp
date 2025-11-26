/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex08.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/26 15:37:26 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

std::vector<std::vector<int32_t>>	powerset( std::vector<int32_t> set )
{
	std::vector<std::vector<int32_t>>	res;
	size_t								n = set.size();

	for (size_t mask = 0; mask < (1 << n); mask++)
	{
		std::vector<int32_t>	newSet;
		for (size_t j = 0; j < n; j++)
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
		return (std::cerr << "Wrong number of arguments" << std::endl, 1);
	std::istringstream	iss(argv[1]);
	std::vector<int>	v;
	int32_t				i;
	
	while (iss >> i)
		v.push_back(i);
	std::vector<std::vector<int32_t>> pwr = powerset(v);
	for (auto& i : pwr)
	{
		for (auto& j : i)
			std::cout << j << " ";
		std::cout << std::endl;
	}
	return (0);
}
