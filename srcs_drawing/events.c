/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:44:38 by llefranc          #+#    #+#             */
/*   Updated: 2020/03/04 17:48:10 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

/*
** Allow to rotate when the player is moving the mouse.
*/
int		motionnotify(int x, int y, t_mlx *mlx)
{
	(void)y;
	if (mlx->cam->mouse_bool == 1) //at the moment the mouse is activated, to initiate it
	{
		mlx->cam->mouse_bool = 2;
		mlx->cam->mouse_x = x;	//we're saving original position of the mouse
	}
	if (mlx->cam->mouse_bool) 
	{
		if (x < mlx->cam->mouse_x) //allow to know if the mouse is moving left or right
			mlx->cam->angle = positive_angle(mlx->cam->angle + MOUSE_ROTA_SIZE);
		else if (x > mlx->cam->mouse_x)
			mlx->cam->angle = positive_angle(mlx->cam->angle - MOUSE_ROTA_SIZE);
		mlx->cam->mouse_x = x;
		raycasting(mlx);
		mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img->screen, 0, 0);
	}
	return (1);
}

/*
** Control player's movements. When a key is pressed, the boolean is set to 1
** and the function for this movement will start to be called until the player
** released the key.
*/
int		keypress(int keycode, t_mlx *mlx)
{
	// printf("up %d, do %d, le %d, ri %d\n", mlx->cam->m_up, mlx->cam->m_down, mlx->cam->m_left, mlx->cam->m_right);
	if (keycode == W_KEY)
		mlx->cam->m_up = 1;
	if (keycode == S_KEY)
		mlx->cam->m_down = 1;
	if (keycode == A_KEY)
		mlx->cam->m_left = 1;
	if (keycode == D_KEY)
		mlx->cam->m_right = 1;
	if (keycode == LEFT_KEY)
		mlx->cam->r_left = 1;
	if (keycode == RIGHT_KEY)
		mlx->cam->r_right = 1;
	if (keycode == M_KEY)
		mlx->cam->mouse_bool = (mlx->cam->mouse_bool == 0) ? 1 : 0;
	mlx->cam->m_up ? move_up_in_map(mlx) : 0;
	mlx->cam->m_down ? move_down_in_map(mlx) : 0;
	mlx->cam->m_left ? move_left_in_map(mlx) : 0;
	mlx->cam->m_right ? move_right_in_map(mlx) : 0;
	mlx->cam->r_left ? mlx->cam->angle = positive_angle(mlx->cam->angle + ROTA_SIZE) : 0;
	mlx->cam->r_right ? mlx->cam->angle = positive_angle(mlx->cam->angle - ROTA_SIZE) : 0;
	raycasting(mlx);
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img->screen, 0, 0); //ici ?
	return (1);
}

/*
** Control player's movements. When a key is released, the boolean is set to 0
** and the function for this movement will stop to be called.
*/
int		keyrelease(int keycode, t_mlx *mlx)
{
	// printf("up %d, do %d, le %d, ri %d\n", mlx->cam->m_up, mlx->cam->m_down, mlx->cam->m_left, mlx->cam->m_right);
	if (keycode == W_KEY)		//movements
		mlx->cam->m_up = 0;
	if (keycode == S_KEY)
		mlx->cam->m_down = 0;
	if (keycode == A_KEY)
		mlx->cam->m_left = 0;
	if (keycode == D_KEY)
		mlx->cam->m_right = 0;
	if (keycode == LEFT_KEY)	//rotation
		mlx->cam->r_left = 0;
	if (keycode == RIGHT_KEY)
		mlx->cam->r_right = 0;
	mlx->cam->m_up ? move_up_in_map(mlx) : 0;			//movements
	mlx->cam->m_down ? move_down_in_map(mlx) : 0;
	mlx->cam->m_left ? move_left_in_map(mlx) : 0;
	mlx->cam->m_right ? move_right_in_map(mlx) : 0;
	mlx->cam->r_left ? mlx->cam->angle = positive_angle(mlx->cam->angle + ROTA_SIZE) : 0; //rotation
	mlx->cam->r_right ? mlx->cam->angle = positive_angle(mlx->cam->angle - ROTA_SIZE) : 0;
	raycasting(mlx);
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img->screen, 0, 0); //ici ?
	return (1);
}

/*
** Allow to properly qui the program when the player is closing the window.
*/
int		destroynotify(t_pars *par)
{
	struct_free(par);
	exit(EXIT_SUCCESS);
}