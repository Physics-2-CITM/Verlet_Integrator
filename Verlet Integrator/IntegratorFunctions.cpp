#include "IntegratorFunctions.h"
#include"ModuleVerlet.h"

VerletIntegrator::VerletIntegrator()
{}

VerletIntegrator::VerletIntegrator(Application* app)
{
	App = app;
}

void VerletIntegrator::InitPoint(Point* p, vector2 pos)
{
	p->x = pos.x;
	p->y = pos.y;
	p->old_x = pos.x;
	p->old_y = pos.y;
	p->vx =	0.f;
	p->vy = 0.f;
	p->dt = (1.f/ 60.f);


	//p->color.r = (Uint8)rand() % 255;
	//p->color.g = (Uint8)rand() % 255;
	//p->color.b = (Uint8)rand() % 255;
	p->color.r = 255;
	p->color.a = 80;

	p->selector_rect = { (int)pos.x, (int)pos.y, p->radius * 2, p->radius * 2};
}

void VerletIntegrator::AddForce(Point* p, vector2 force) 
{
	p->vx += force.x;
	p->vy += force.y;
}

void VerletIntegrator::updatePoints(Point* p)
{

	double incrementX = fabs(p->x);
	double incrementY = fabs(p->y);

	//Update particle position
	p->x = p->x + (p->vx * p->dt) + (0.5f * 0.f * (p->dt * p->dt));
	p->y = p->y + (p->vy * p->dt) + (0.5f * gravity * (p->dt * p->dt));

	incrementX -= fabs(p->x);
	incrementY -= fabs(p->y);



	//BOTTOM LIMIT
	if (p->y > (float)floor_Limit_Y)
	{
		p->y = (float)floor_Limit_Y;
		p->vy *= -1.f * bounce;
	}
	else if(p->y <= (float)p->radius)
	{
		p->y = (float)p->radius;
		p->vy *= -1 * bounce;
	}

	if (p->x >= (float)floor_Limit_X) 
	{
		p->x = (float)floor_Limit_X;
		p->vx *= -1 * bounce;
	}
	else if(p->x <= (float)p->radius)
	{
		p->x = (float)p->radius;
		p->vx *= -1 * bounce;
	}

	//Collision detection between particles (not lines)
	for (unsigned int j = 0; j < App->verlet->world_points.count(); j++)
	{
		Point* check_Point = App->verlet->world_points[j];
		if (App->verlet->world_points[j] != p)
		{
			if (pow((check_Point->x - p->x), 2) + pow((p->y - check_Point->y), 2) <= pow((p->radius + check_Point->radius), 2))
			{
				p->vx *= -1 * bounce;
				p->vy *= -1 * bounce;

				//Ugly way to do collision correction, needs an update
				p->x += incrementX;
				p->y += incrementY;

				//if (p->x > check_Point->x) 
				//{
				//	p->x = check_Point->x + check_Point->radius + check_Point->radius;
				//}
				//else if (p->x < check_Point->x)
				//{
				//	p->x = check_Point->x - check_Point->radius - check_Point->radius;
				//}



				check_Point->vx = p->vx;
				check_Point->vx *= -1 * bounce;

				check_Point->vy = p->vy;
				check_Point->vy *= -1 * bounce;
			}
		}
	}

	//Velocity update for the next frame (and next position calculation)
	if (p->vx > 0) 
	{
		p->vx = p->vx + (-friction * p->dt);
	}
	else if(p->vx < 0)
	{
		p->vx = p->vx + (friction * p->dt);
	}
	if (p->vx == 0) {
		p->vx = p->vx + (0.f * p->dt);
	}
		p->vy = p->vy + (gravity * p->dt);

	//LOG("%f, %f", incrementX, incrementY);


}

//void VerletIntegrator::OnCollision(vector2 point, Point* p) 
//{
//
//}

