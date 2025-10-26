/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex10.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 11:08:14 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>

using namespace std;

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
	cout << map(0, 0) << endl;
	cout << map(65535, 65535) << endl;
	cout << map(65535, 0) << endl;
	cout << map(0, 65535) << endl;
	cout << map(65535, 32767) << endl;
	cout << map(12345, 54321) << endl;
	return (0);
}
