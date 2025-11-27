/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex10.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/27 13:29:56 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>

double	map( uint16_t x, uint16_t y )
{
	uint32_t	result = 0;

	for (int i = 0; i < 16; i++)
	{
		uint32_t	bit_x = (x >> i) & 1u;
		uint32_t	bit_y = (y >> i) & 1u;
		result |= (bit_x << (2 * i));
		result |= (bit_y << (2 * i + 1));
	}
	return (static_cast<double>(result) / static_cast<double>((1ULL << 32) - 1));
}

int main( void )
{
	std::cout << map(0, 0) << std::endl;
	std::cout << map(65535, 65535) << std::endl;
	std::cout << map(65535, 0) << std::endl;
	std::cout << map(0, 65535) << std::endl;
	std::cout << map(65535, 32767) << std::endl;
	std::cout << map(12345, 54321) << std::endl;
	return (0);
}
