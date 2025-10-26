/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:07:28 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 18:00:52 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

uint32_t	adder( uint32_t a, uint32_t b )
{
	int	x = a;
	int	y = b;
	int	carry;
	
	while (y != 0)
	{
		carry = (x & y) << 1;
		x = x ^ y;
		y = carry;
	}
	return (x);
}

uint32_t	multiplier( uint32_t a, uint32_t b )
{
	uint32_t sum = 0;

	for (uint32_t i = 0; i < b; i++)
		sum = adder(sum, a);
	return (sum);
}

int	main( int argc, char **argv )
{
	if (argc != 3)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (1);
	}
	try
	{
		long long a = std::stoll(argv[1]);
		long long b = std::stoll(argv[2]);
		if (a < 0 || b < 0)
			return (std::cerr << "Arguments must be natural integers." << std::endl, 1);
		a = static_cast<uint32_t>(a); b = static_cast<uint32_t>(b);
		std::cout << multiplier(a, b) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Arguments must be valid integers" << std::endl;
		return (1);
	}
	return (0);
}