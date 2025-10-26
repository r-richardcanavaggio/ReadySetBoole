/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex00.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:53:25 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 17:49:48 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

uint32_t	adder( uint32_t a, uint32_t b )
{
	uint32_t	x = a;
	uint32_t	y = b;
	uint32_t	carry;
	
	while (y != 0)
	{
		carry = (x & y) << 1;
		x = x ^ y;
		y = carry;
	}
	return (x);
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
		std::cout << adder(a, b) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Arguments must be valid integers" << std::endl;
		return (1);
	}
	return (0);
}
