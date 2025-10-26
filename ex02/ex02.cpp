/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:38:21 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 18:05:12 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

uint32_t	gray_code( uint32_t n )
{
	return ((n ^ (n << 1)) >> 1);
}

int	main( int argc, char **argv )
{
	if (argc != 2)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (1);
	}
	try
	{
		long long a = std::stoll(argv[1]);
		if (a < 0)
			return (std::cerr << "Argument must be positive.", 1);
		a = static_cast<uint32_t>(a);
		std::cout << gray_code(a) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Arguments must be valid integers" << std::endl;
		return (1);
	}
	return (0);
}