/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplier.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:49:25 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/20 18:27:32 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "adder.cpp"

int	multiplier( int a, int b )
{
	int sum = a;

	for (int i = 0; i < b - 1; i++)
		sum = adder(sum, a);
	return (sum);
}

// int	main( int argc, char **argv )
// {
// 	(void)argc;
// 	std::cout << multiplier(std::atoi(argv[1]), std::atoi(argv[2])) << std::endl;
// 	return (0);
// }