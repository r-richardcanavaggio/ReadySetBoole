/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:38:21 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/20 20:39:08 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "GrayCode.h"

int	main( int argc, char **argv )
{
	if (argc != 2)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (1);
	}
	try
	{
		int a = std::stoi(argv[1]);
		std::cout << GrayCode(a) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Arguments must be valid integers" << std::endl;
		return (1);
	}
	return (0);
}