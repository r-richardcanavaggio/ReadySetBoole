/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex11.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 19:57:19 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 11:08:58 by rrichard         ###   ########.fr       */
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


std::pair<uint16_t, uint16_t>	inverse_map( double input )
{
	std::pair<uint16_t, uint16_t>	result = {0, 0};
	double							denom = (1ULL << 32) - 1;
	uint32_t						newInput = static_cast<uint32_t>(input * denom);

	for (int i = 0; i < 32; i++)
	{
		if (i % 2 == 0)
		{
			uint16_t bit_x = (newInput >> i) & 1u;
			result.first |= (bit_x << (i / 2));
		}
		else
		{
			uint16_t bit_y = (newInput >> i) & 1u;
			result.second |= (bit_y << (i / 2));
		}
	}
	return result;
}

int main( void )
{
	std::pair<uint16_t, uint16_t>	res;
	
	res = inverse_map(0);
	std::cout << res.first << " " << res.second << " " << map(res.first, res.second) << std::endl;
	res = inverse_map(1);
	std::cout << res.first << " " << res.second << " " << map(res.first, res.second) << std::endl;
	res = inverse_map(map(65535, 0));
	std::cout << res.first << " " << res.second << " " << map(res.first, res.second) << std::endl;
	res = inverse_map(0.5);
	std::cout << res.first << " " << res.second << " " << map(res.first, res.second) << std::endl;
	res = inverse_map(0.65283294107132427975);
	std::cout << res.first << " " << res.second << " " << map(res.first, res.second) << std::endl;
	return (0);
}
