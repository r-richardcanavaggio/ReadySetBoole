/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:31:15 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/20 17:28:56 by rrichard         ###   ########.fr       */
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
