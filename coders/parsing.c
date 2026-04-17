/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 16:36:05 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/16 18:24:27 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int is_valid_number(char *str)
{
	int i;

	i = 0;
	if(!str || str[0] == '\0')
		return (0);

	while(str[i] != '\0')
	{
		if(str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);    
}

int	is_valid_scheduler(char *str)
{
	if (!str || str[0] == '\0')
		return (0);
	
	if (str[0] == 'f' && str[1] == 'i' && str[2] == 'f' && str[3] == 'o' && str[4] == '\0')
		return (1);

	if (str[0] == 'e' && str[1] == 'd' && str[2] == 'f' && str[3] == '\0')
		return (1);

	return (0);
}
