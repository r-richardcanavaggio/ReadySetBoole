/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:31:15 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/20 16:44:41 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	adder( int a, int b )
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

// int	main( int argc, char **argv )
// {
// 	(void)argc;
// 	std::cout << adder(std::atoi(argv[1]), std::atoi(argv[2])) << std::endl;
// 	return (0);
// }