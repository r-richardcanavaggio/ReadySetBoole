/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:07:28 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/20 20:24:36 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "multiplier.h"

int	main( int argc, char **argv )
{
	if (argc != 3)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (1);
	}
	try
	{
		int a = std::stoi(argv[1]);
		int b = std::stoi(argv[2]);
		std::cout << multiplier(a, b) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Arguments must be valid integers" << std::endl;
		return (1);
	}
	return (0);
}