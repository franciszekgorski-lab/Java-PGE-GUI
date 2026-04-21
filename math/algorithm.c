#include "headers/algorithm.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

double vector_length(double a, double b)
{
    return sqrt((a*a) + (b*b));
}

void fruchterman(Graph *g, int count)
{
    double area = WIDTH * HEIGHT;
    double n = sqrt(area / g->number_of_vertices);
    double t = WIDTH / 10;
    for(int i =0; i < count;i++)
    {
        for(int j=0; j<g->number_of_vertices;j++)
        {
            Vertex *v = &g->vertices[j];
            v->dx = 0;
            v->dy = 0;
            for(int k=0;k< g->number_of_vertices;k++)
            {
                if(j !=k)
                {
                    Vertex *v2 = &g->vertices[k];
                    double dx = v->x - v2->x;
                    double dy = v->y - v2->y;
                    double distance = vector_length(dx, dy);
                    if(distance > 0)
                    {
                        double pom = (n*n) / distance;
                        v->dx +=(dx/distance) * pom;
                        v->dy +=(dy/distance) * pom;
                    }
                }
            }
        }
        for(int i=0;i<g->number_of_vertices;i++)
        {
            Vertex *x = &g->vertices[g->edges[i].x];
            Vertex *y = &g->vertices[g->edges[i].y];
            double dx = x->x - y->x;
            double dy = x->y - y->y;
            double distance = vector_length(dx, dy);
            if(distance > 0)
            {
                double pom = (distance * distance) / n;
                x->dx -= (dx / distance) * pom;
                x->dy -=(dy/distance) *pom;
                y->dx+=(dx/distance)*pom;
                y->dy+=(dy/distance) * pom;
            }
        }
        for(int i=0; i< g->number_of_edges;i++)
        {
            Vertex *v = &g->vertices[i];
            double distance = vector_length(v->dx, v->dy);
            if(distance > 0)
            {
                double limited = fmin(distance, t);
                v->x += (v->dx / distance) * limited;
                v->y += (v->dy / distance) * limited;
            }
            v->x = fmin(WIDTH, fmax(0.0, v->x));
            v->y = fmin(HEIGHT, fmax(0.0, v->y));
        }
        t*=0.95;
    }
}

void tutte(Graph *g, int iterations)
{
    if (!g || g->number_of_vertices <= 0)
    {
        return;
    }

    int vertex_count = g->number_of_vertices;
    if (vertex_count == 1)
    {
        g->vertices[0].x = WIDTH / 2.0;
        g->vertices[0].y = HEIGHT / 2.0;
        return;
    }

    int *is_fixed = calloc(vertex_count, sizeof(int));
    if (!is_fixed)
    {
        return;
    }

    // Arbitrary fixing the first three vertices in a triangle for simplicity
    int boundary_count = vertex_count < 3 ? vertex_count : 3;
    for (int i = 0; i < boundary_count; i++)
    {
        is_fixed[i] = 1;
    }

    double center_x = WIDTH / 2.0;
    double center_y = HEIGHT / 2.0;
    double radius = 0.45 * fmin(WIDTH, HEIGHT); // 5% margin from the edges

    for (int i = 0; i < boundary_count; i++)
    {
        double angle = 2.0 * PI * (double)i / (double)boundary_count - PI / 2.0;
        int v = i;
        g->vertices[v].x = center_x + radius * cos(angle);
        g->vertices[v].y = center_y + radius * sin(angle);
    }

    for (int i = 0; i < vertex_count; i++)
    {
        if (!is_fixed[i])
        {
            g->vertices[i].x = center_x;
            g->vertices[i].y = center_y;
        }
    }
    for (int it = 0; it < iterations; it++)
    {
        // Storing the graph as an adjacency list would be more efficient for this part
        for (int v = 0; v < vertex_count; v++)
        {
            if (is_fixed[v])
            {
                continue;
            }

            double sum_x = 0.0;
            double sum_y = 0.0;
            double sum_w = 0.0;

            for (int e = 0; e < g->number_of_edges; e++)
            {
                int a = g->edges[e].x;
                int b = g->edges[e].y;
                int u = -1;

                if (a == v)
                {
                    u = b;
                }
                else if (b == v)
                {
                    u = a;
                }

                if (u >= 0 && u < vertex_count)
                {
                    double w = g->edges[e].weight;
                    sum_x += w * g->vertices[u].x;
                    sum_y += w * g->vertices[u].y;
                    sum_w += w;
                }
            }

            if (sum_w > 0.0)
            {
                double new_x = sum_x / sum_w;
                double new_y = sum_y / sum_w;

                g->vertices[v].x = new_x;
                g->vertices[v].y = new_y;
            }
        }
    }

    free(is_fixed);
}