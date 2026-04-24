#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/graph.h"
#define MAX_VERTICES 10000
#define MAX_EDGES 10000

Graph* create_graph()
{
    Graph *g = malloc(sizeof(Graph));
    g->edges = malloc(MAX_EDGES * sizeof(Edge));
    g->vertices = malloc(MAX_VERTICES * sizeof(Vertex));
    g->number_of_edges=0;
    g->number_of_vertices=0;
    return g;
}
void free_graph(Graph *g)
{
    free(g->edges);
    free(g->vertices);
    free(g);
}
int new_edge(Graph *g, int id)
{
    for(int i=0;i<g->number_of_vertices;i++)
    {
        if(g->vertices[i].id == id) return i;
    }

    if (g->number_of_vertices >= MAX_VERTICES)
    {
        return -1;
    }

    g->vertices[g->number_of_vertices].id = id;
    g->vertices[g->number_of_vertices].x = (double)rand() /(double) RAND_MAX;
    g->vertices[g->number_of_vertices].y = (double)rand() / RAND_MAX;
    return g->number_of_vertices++;
}

int load_file(Graph *g, char *file, int *error_line)
{
    FILE *in = fopen(file, "r");
    if(!in)
    {
        return GRAPH_ERR_INPUT_MISSING;
    }

    char line[1024];
    int line_no = 0;
    int status = GRAPH_OK;

    while (fgets(line, sizeof(line), in) != NULL)
    {
        char name[256];
        int a, b;
        double weight;
        char tail;

        line_no++;

        if (sscanf(line, "%255s %d %d %lf %c", name, &a, &b, &weight, &tail) != 4)
        {
            status = GRAPH_ERR_FORMAT;
            break;
        }

        if (a <= 0 || b <= 0 || weight < 0.0)
        {
            status = GRAPH_ERR_FORMAT;
            break;
        }

        if (g->number_of_edges >= MAX_EDGES)
        {
            status = GRAPH_ERR_FORMAT;
            break;
        }

        int a_id = new_edge(g, a);
        int b_id = new_edge(g, b);
        if (a_id < 0 || b_id < 0)
        {
            status = GRAPH_ERR_FORMAT;
            break;
        }

        g->edges[g->number_of_edges].x = a_id;
        g->edges[g->number_of_edges].y = b_id;
        g->edges[g->number_of_edges++].weight = weight;
    }

    fclose(in);
    if (status == GRAPH_ERR_FORMAT && error_line)
    {
        *error_line = line_no;
    }

    return status;
}

int save_file(Graph *g, char *file, char *filetype)
{
    if(strcmp(filetype, "txt")==0)
    {
        FILE *out = fopen(file, "w");
        if (!out)
        {
            return GRAPH_ERR_SAVE;
        }

        for(int i=0;i< g->number_of_vertices;i++)
        {
            if (fprintf(out, "%d %.17g %.17g \n", g->vertices[i].id, g->vertices[i].x, g->vertices[i].y) < 0)
            {
                fclose(out);
                return GRAPH_ERR_SAVE;
            }
        }

        if (fclose(out) != 0)
        {
            return GRAPH_ERR_SAVE;
        }

        return GRAPH_OK;
    }
    else if (strcmp(filetype, "bin")==0)
    {
        FILE *out = fopen(file, "wb");
        if (!out)
        {
            return GRAPH_ERR_SAVE;
        }

        for(int i=0; i<g->number_of_vertices; i++)
        {
            if (fwrite(&g->vertices[i].id, sizeof(int), 1, out) != 1 ||
                fwrite(&g->vertices[i].x, sizeof(double), 1, out) != 1 ||
                fwrite(&g->vertices[i].y, sizeof(double), 1, out) != 1)
            {
                fclose(out);
                return GRAPH_ERR_SAVE;
            }
        }

        if (fclose(out) != 0)
        {
            return GRAPH_ERR_SAVE;
        }

        return GRAPH_OK;
    }

    return GRAPH_ERR_SAVE;
}

