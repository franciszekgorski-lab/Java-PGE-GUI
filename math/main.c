#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/algorithm.h"
#include "headers/graph.h"
#include <time.h>

void print_help() {
    const char *help_text = 
        "Uzycie: graph -n <sciezka> -a <numer> [opcje]\n\n"
        "Opis:\n"
        "  Aplikacja wyznacza wspolrzedne wierzcholkow grafow planarnych.\n"
        "  Wykorzystuje algorytm Fruchtermana-Reingolda (1) oraz Twierdzenie Tutte'a (2).\n\n"
        "Opcje wymagane:\n"
        "  -n <sciezka>  Sciezka do pliku wejsciowego z lista krawedzi\n"
        "  -a <numer>    Wybor algorytmu: \n"
        "                  1 = Algorytm Fruchtermana-Reingolda\n"
        "                  2 = Twierdzenie Tutte'a o sprezynach\n\n"
        "Opcje dodatkowe:\n"
        "  -o <nazwa>    Nazwa pliku wyjsciowego (domyslnie: \"out\")\n"
        "  -i <liczba>   Liczba iteracji algorytmu (domyslnie: 500)\n"
        "  -f <format>   Format pliku wyjsciowego: \"txt\" lub \"bin\" (domyslnie: \"txt\")\n"
        "  -s <seed>     Ziarno generatora liczb losowych (dla powtarzalnosci wynikow)\n"
        "  -h            Wyswietla dokumentacje\n\n"
        "Format pliku wejsciowego (txt):\n"
        "  <nazwa_krawedzi> <wierzcholek_A> <wierzcholek_B> <waga_krawedzi>\n\n"
        "Przyklady:\n"
        "  ./graph -n dane.csv -a 1 -o out_file -f txt\n"
        "  ./graph -n dane.csv -a 2 -s 42\n\n"
        "Kody wyjscia:\n"
        "  0 - Sukces\n"
        "  1 - Brak/blad pliku wejsciowego\n"
        "  2 - Blad formatu danych\n"
        "  3 - Blad zapisu pliku wyjsciowego\n"
        "  4 - Nieprawidlowy/nieznany algorytm\n";

    printf("%s", help_text);
}

int main(int argc, char *argv[]) 
{
    char *input = NULL;
    char *output = "out";
    char *format = "txt";
    int number = 0;
    int seed = 0;
    int has_seed = 0;
    int iterations = 500;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            print_help();
            return 0;
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) 
        {
            input = argv[++i];
        } 
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) 
        {
            number = atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            output = argv[++i];
        } 
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) 
        {
            format = argv[++i];
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
        {
            seed = atoi(argv[++i]);
            has_seed = 1;
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            iterations = atoi(argv[++i]);
        }
    }

    if (input == NULL)
    {
        fprintf(stderr, "Error: Nie znaleziono pliku: <ścieżka>\n");
        return GRAPH_ERR_INPUT_MISSING;
    }

    if (number != 1 && number != 2)
    {
        fprintf(stderr, "Error: Nieznany algorytm: %d. Dostępne: 1,2\n", number);
        return GRAPH_ERR_UNKNOWN_ALGORITHM;
    }

    if (strcmp(format, "txt") != 0 && strcmp(format, "bin") != 0)
    {
        fprintf(stderr, "Error: Nie można zapisać pliku: %s\n", output);
        return GRAPH_ERR_SAVE;
    }

    if (has_seed)
    {
        srand(seed);
    }
    else
    {
        srand(time(NULL));
    }

    Graph *g = create_graph();

    int error_line = 0;
    int load_status = load_file(g, input, &error_line);
    if (load_status == GRAPH_ERR_INPUT_MISSING)
    {
        fprintf(stderr, "Error: Nie znaleziono pliku: %s\n", input);
        free_graph(g);
        return GRAPH_ERR_INPUT_MISSING;
    }
    if (load_status == GRAPH_ERR_FORMAT)
    {
        fprintf(stderr, "Error: Nieprawidłowy format w linii %d\n", error_line);
        free_graph(g);
        return GRAPH_ERR_FORMAT;
    }

    if (number == 1) 
    {
        fruchterman(g, iterations);
    }
    else 
    {
        tutte(g, iterations);
    }

    if (save_file(g, output, format) != GRAPH_OK)
    {
        fprintf(stderr, "Error: Nie można zapisać pliku: %s\n", output);
        free_graph(g);
        return GRAPH_ERR_SAVE;
    }

    free_graph(g);
    return GRAPH_OK;
}