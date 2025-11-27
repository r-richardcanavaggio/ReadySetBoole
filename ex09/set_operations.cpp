/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_operations.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:05:16 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/27 13:28:32 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

std::vector<int32_t>	manual_intersection( const std::vector<int32_t>& v1, const std::vector<int32_t>& v2 )
{
	std::vector<int32_t>	res;
	size_t				i = 0, j = 0;

	while (i < v1.size() && j < v2.size())
	{
		if (v1[i] < v2[j])
			i++;
		else if (v1[i] > v2[j])
			j++;
		else
		{
			res.push_back(v1[i]);
			i++;
			j++;
		}
	}
	return (res);
}

std::vector<int32_t>	manual_union( const std::vector<int32_t>& v1, const std::vector<int32_t>& v2 )
{
	std::vector<int32_t>	res;
	size_t				i = 0, j = 0;

	while (i < v1.size() && j < v2.size())
	{
		if (v1[i] < v2[j])
		{
			res.push_back(v1[i]);
			i++;
		}
		else if (v1[i] > v2[j])
		{
			res.push_back(v2[j]);
			j++;
		}
		else
		{
			res.push_back(v1[i]);
			i++;
			j++;
		}
	}
	while (i < v1.size())
		res.push_back(v1[i++]);
	while (j < v2.size())
		res.push_back(v2[j++]);
	return (res);
}

/**
 * @brief Computes the set difference of two sorted integer vectors.
 *
 * Given two sorted vectors v1 and v2, returns a vector containing the elements
 * that are present in v1 but not in v2. The result is also sorted.
 *
 * @param v1 The first sorted vector of integers (minuend set).
 * @param v2 The second sorted vector of integers (subtrahend set).
 * @return std::vector<int32_t> A vector containing the set difference v1 \ v2.
 *
 * @note Both input vectors must be sorted in ascending order for correct results.
 */
std::vector<int32_t>	manual_difference( const std::vector<int32_t>& v1, const std::vector<int32_t>& v2 )
{
	std::vector<int32_t>	res;
	size_t				i = 0, j = 0;

	while (i < v1.size() && j < v2.size())
	{
		if (v1[i] < v2[j])
		{
			res.push_back(v1[i]);
			i++;
		}
		else if (v1[i] > v2[j])
			j++;
		else
		{
			i++;
			j++;		
		}
	}
	while (i < v1.size())
		res.push_back(v1[i++]);
	return (res);
}

std::vector<int32_t>	manual_symmetric_difference( const std::vector<int32_t>& v1, const std::vector<int32_t>& v2 )
{
	std::vector<int32_t>	res;
	size_t				i = 0, j = 0;

	while (i < v1.size() && j < v2.size())
	{
		if (v1[i] < v2[j])
		{
			res.push_back(v1[i]);
			i++;
		}
		else if (v1[i] > v2[j])
		{
			res.push_back(v2[j]);
			j++;
		}
		else
		{
			i++;
			j++;
		}
	}
	while (i < v1.size())
		res.push_back(v1[i++]);
	while (j < v2.size())
		res.push_back(v2[j++]);
	return (res);
}
