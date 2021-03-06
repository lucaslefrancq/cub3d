/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   y_ray.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 09:59:56 by llefranc          #+#    #+#             */
/*   Updated: 2020/04/01 14:23:30 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

/*
** Return the distance on 'x axe' before it meets the border of the actual
** square, depending on ray angle value.
*/
double	y_ray_x1_value(t_rcast *cam, double angle)
{
	double x1;
	double x_round;

	x1 = cam->x;
	x_round = round(x1); //if x1 < 3.5 x_round = 3.0 || x1 >= 3.5 x_round = 4.0
	if (x_round > x1) //x1 >= 3.5
	{
		if ((angle >= 0.0 && angle < 90.0) ||
			(angle > 270.0 && angle < 360.0))
			x1 = x_round - x1; //x1 pos, we're going right in the tab
		else if (angle > 90 && angle < 270)
			x1 = -1.0 + (x_round - x1); //x1 neg, we're going left in the tab
	}
	else if (x_round < x1) //x1 < 3.5
	{
		if ((angle >= 0.0 && angle < 90.0) ||
			(angle > 270.0 && angle < 360.0))
			x1 = 1.0 - (x1 - x_round);
		else if (angle > 90 && angle < 270)
			x1 = x_round - x1;
	}
	else //y1 = 3.0
		x1 = 0.0;
	return (x1);
}

/*
** Return the value of xa for v_ray equation depending on the ray angle.
** Return -1 for angle > 90 && angle < 360, 1 for angle > 90 and angle < 270,
** 0 for angle == 90 || angle == 270.
*/
double	y_ray_xa_value(double angle)
{
	if ((angle >= 0.0 && angle < 90.0) || //we're going rigth in the tab so xa is positive
		(angle > 270.0 && angle < 360.0))
		return (1.0);
	else if (angle > 90 && angle < 270) //we're going left in the tab so ya is negative
		return (-1.0);
	else //vertical ray will never cross 'x axe'
		return (0.0);
}

/*
** Return y1 value (len y before corresponding from our position to first
** intersection with 'x axe') depending on the angle. y1 will be neg if angle
** is between 0 and 180 degrees.
*/
double	y_ray_y1_value(double angle, double x1)
{
	double	y1;

	if (x1 < 0.0) //we need pos values for trigo calculation
		x1 *= -1.0;
	y1 = x1 * tan(angle_tri_rect(angle) * TO_RAD);
	if (angle >= 0.0 && angle < 180.0)
		y1 *= -1.0;
	return (y1);
}

/*
** Return ya value (len ya between y1 and the next intersection with 'x axe', if
** we move on 1 unity on 'x axe' and depending on the ray angle. ya will be neg
** for an angle between 90 and 270 degrees.
*/
double	y_ray_ya_value(double angle, double x1, double xa)
{
	double ya;

	if (x1 < 0.0) //we need pos values for trigo calculation
		x1 *= -1.0;
	if (xa < 0.0)
		xa *= -1.0;
	x1 += xa;
	ya = x1 * tan(angle_tri_rect(angle) * TO_RAD);
	if (angle >= 0.0 && angle < 180.0)
		ya *= -1.0;
	return (ya);
}

/*
** Return 0 if it finds a wall/door for a certain position in the map.
** Return -1 if it's out of the map. Return 2 if a sprite.
** Corrects the position in the map because we're evolving in a tab[y][x]:
** for angle > 90 && < 270 : tab[y][x - 1] (checking if there's a wall at our left).
*/
int		y_ray_find_wall(t_mlx *mlx, double angle, double x_len, double y_len)
{
	if ((x_len || y_len) && (angle > 90.0 && angle < 270.0)) //!x1 && !y1 when player is standing on a sprite' square
		x_len = (double)((int)(x_len)) - 1.0; //at the intersection with y axe, checking if wall at our left
	else if (mlx->cam->x + x_len - (double)((int)(mlx->cam->x + x_len)) > 0.99)
		x_len = (double)((int)(x_len)) + 1.0;
	if ((int)(mlx->cam->y + y_len) < 0 || (int)(mlx->cam->y + y_len) >= mlx->cam->nb_lines) //if we're out of the map (lines)
		return (-1);
	if ((int)(mlx->cam->x + x_len) < 0 || (int)(mlx->cam->x + x_len) >=
			mlx->cam->nb_rows[(int)(mlx->cam->y + y_len)]) //same for rows
		return (-1);
	if (mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] > 3 &&
			mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] < 10) //if a sprite
		return (2);
	else if (mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] == 0 || //if empty
			mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] == DOOR + 10 || //if door opened (we're not printing it)
			mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] == SECRETDOOR + 10)
		return (1);
	else if (mlx->cam->map[(int)(mlx->cam->y + y_len)][(int)(mlx->cam->x + x_len)] == 2) //if a door
		mlx->textu->doors_y = DOOR;
	return (0); //only if we find a wall or doors
}

/*
** Checking y values on 'y axe' each time the ray cross 'x axe' until it meets a
** a wall or exit the map. Return the len of the ray on 'y axe', depending on
** the angle. If angle == 90 or 270 degrees, nan will be returned (ray will 
** never cross 'x axe'). Also calls find_sprites func for sprites raycasting
** each time the ray is meeting a sprite square.
*/
double	y_ray_len(t_mlx *mlx, t_rcast *cam, double angle, t_texture *textu)
{
	int		ret;
	double	x1; //distance on 'x axe' when we first cross 'y axe' with a certain angle
	double	y1; //distance on 'y axe' when we first cross 'x axe' with a certain angle
	double	xa; //next crosses on y = x1 + nb * xa, with xa the distance between
				// each line of x (so 1 or -1 depending on the angle)
	double	ya; //next crosses on x = y1 + nb * ya, ya is the distance between x1 and
				//the next cross of 'y axe'

	if (angle == 90.0 || angle == 270.0) //we will never cross 'x axe'
		return (NAN);
	x1 = y_ray_x1_value(cam, angle); //cam->x + x1 => border of the actual square (x axe)
	xa = y_ray_xa_value(angle);
	y1 = y_ray_y1_value(angle, x1); //cam->y + y1 => border of the actual square (y axe)
	ya = y_ray_ya_value(angle, x1, xa) - y1;
	if (y_ray_find_wall(mlx, angle, 0, 0) == 2) //in the case player is standing on a sprite' square
		find_sprites(mlx, sprites_ptr_y_ray(mlx, angle, 0, 0), cam->x + xa, cam->y + ya, angle);	
	while ((ret = y_ray_find_wall(mlx, angle, x1, y1)) > 0) //until we find a wall/door or exit map
	{
		if (ret == 2) //we add xa and ya to go further than sprite square and be sure ray can cross sprite plan
			find_sprites(mlx, sprites_ptr_y_ray(mlx, angle, x1,
					y1), cam->x + x1 + xa, cam->y + y1 + ya, angle);
		x1 += xa; //moving of (+-)1 unity on 'x axe'
		y1 += ya; //next cross with 'x axe'
	}
	textu->y_xa = x1;	//saving coordonates of where the ray is touching the wall
	textu->y_ya = y1;	//usefull later for calculating textures rendering
	return (ray_len(cam->x, cam->y, cam->x + x1, cam->y + y1)); //len of ray when we cross 'y axe' and we meet a wall
}
