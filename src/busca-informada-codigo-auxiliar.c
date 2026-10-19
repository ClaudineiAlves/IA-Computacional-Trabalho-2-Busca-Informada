#include <stdbool.h>
#include <stdio.h>

#define INF -1 // para marcar valores inválidos

typedef enum {
    Arad,
    Bucharest,
    Craiova,
    Drobeta,
    Eforie,
    Fagaras,
    Giurgiu,
    Hirsova,
    Iasi,
    Lugoj,
    Mehadia,
    Neamt,
    Oradea,
    Pitesti,
    RimnicuVilcea,
    Sibiu,
    Timisoara,
    Urziceni,
    Vaslui,
    Zerind,
    QuantidadeCidades
} Cidade;

char* nome_cidade[] = {
    [Arad] = "Arad",
    [Bucharest] = "Bucharest",
    [Craiova] = "Craiova",
    [Drobeta] = "Drobeta",
    [Eforie] = "Eforie",
    [Fagaras] = "Fagaras",
    [Giurgiu] = "Giurgiu",
    [Hirsova] = "Hirsova",
    [Iasi] = "Iasi",
    [Lugoj] = "Lugoj",
    [Mehadia] = "Mehadia",
    [Neamt] = "Neamt",
    [Oradea] = "Oradea",
    [Pitesti] = "Pitesti",
    [RimnicuVilcea] = "Rimnicu Vilcea",
    [Sibiu] = "Sibiu",
    [Timisoara] = "Timisoara",
    [Urziceni] = "Urziceni",
    [Vaslui] = "Vaslui",
    [Zerind] = "Zerind",
};

// Tabela com os valores da função heurística com Lugoj como objetivo
int tabela_heuristica[QuantidadeCidades] = {
    [Arad] = 135,
    [Bucharest] = 240,
    [Craiova] = 126,
    [Drobeta] = 80,
    [Eforie] = 406,
    [Fagaras] = 156,
    [Giurgiu] = 236,
    [Hirsova] = 370,
    [Iasi] = 333,
    [Lugoj] = 0,
    [Mehadia] = 40,
    [Neamt] = 288,
    [Oradea] = 194,
    [Pitesti] = 155,
    [RimnicuVilcea] = 74,
    [Sibiu] = 88,
    [Timisoara] = 77,
    [Urziceni] = 292,
    [Vaslui] = 350,
    [Zerind] = 162,
};

int mapa_custo[QuantidadeCidades][QuantidadeCidades] = {
    [Arad] = { [Zerind] = 75, [Timisoara] = 118, [Sibiu] = 140 },
    [Bucharest] = { [Giurgiu] = 90, [Pitesti] = 101, [Fagaras] = 211, [Urziceni] = 85 },
    [Craiova] = { [Drobeta] = 120, [RimnicuVilcea] = 146, [Pitesti] = 138 },
    [Drobeta] = { [Mehadia] = 75, [Craiova] = 120 },
    [Eforie] = { [Hirsova] = 86 },
    [Fagaras] = { [Sibiu] = 99, [Bucharest] = 211 },
    [Giurgiu] = { [Bucharest] = 90 },
    [Hirsova] = { [Eforie] = 86 },
    [Iasi] = { [Neamt] = 87, [Vaslui] = 92 },
    [Lugoj] = { [Timisoara] = 111, [Mehadia] = 70 },
    [Mehadia] = { [Drobeta] = 75, [Lugoj] = 70 },
    [Neamt] = { [Iasi] = 87 },
    [Oradea] = { [Zerind] = 71, [Sibiu] = 151 },
    [Pitesti] = { [RimnicuVilcea] = 97, [Craiova] = 138, [Bucharest] = 101 },
    [RimnicuVilcea] = { [Pitesti] = 97, [Sibiu] = 80, [Craiova] = 146 },
    [Sibiu] = { [Fagaras] = 99, [Oradea] = 151, [RimnicuVilcea] = 80, [Arad] = 140 },
    [Timisoara] = { [Arad] = 118, [Lugoj] = 111 },
    [Urziceni] = { [Bucharest] = 85, [Vaslui] = 142, [Hirsova] = 98 },
    [Vaslui] = { [Iasi] = 92, [Urziceni] = 142 },
    [Zerind] = { [Arad] = 75, [Oradea] = 71 },
};

int custo(Cidade origem, Cidade destino)
{
    // pode ajudar a encontrar o objetivo
    if (origem == destino) {
        return 0;
    }

    // testa se existe caminho entre origem e destino
    int valor = mapa_custo[origem][destino];
    if (valor > 0) {
        return valor; // caminho existente
    } else {
        return INF; // caminho inexistente
    }
}

/*****************************************************************
 * Vocês precisam implementar todas as funções daqui para baixo. *
 *****************************************************************/

bool chegou_objetivo(Cidade cidade);

// navegação pelos nós vizinhos / escolha de ações
typedef struct {
    Cidade cidades[4]; // qtd máxima de conexões em uma cidade
    int n_acoes;
} ListaAcoes; // estrutura que representa a lista de possíveis ações (cidades aonde pode ir)
ListaAcoes cidades_vizinhas(Cidade cidade_atual);

// historico
Cidade historico[QuantidadeCidades];
int tamanho_historico = 0;
bool esta_no_historico(Cidade cidade);
void inserir_no_historico(Cidade cidade);

// geração de novos nós na fronteira
typedef struct {
    Cidade cidade;
    // Cidade anterior;
    int custo_acumulado;
    int heuristica;
    int avaliacao;
} No; // estrutura que representa um nó no grafo
No fronteira[QuantidadeCidades];
int tamanho_fronteira = 0;
void inserir_na_fronteira_gulosa(No nova_cidade); // deve testar repetição/desempate aqui dentro; inserir em ordem de prioridade
void inserir_na_fronteira_a_estrela(No nova_cidade); // deve testar repetição/desempate aqui dentro; inserir em ordem de prioridade
No proxima_cidade(void); // apenas pega o nó e maior prioridade na fronteira

int main()
{
    ListaAcoes prox_cidades;
    No no_atual, no_gerado;
    no_atual.cidade = Sibiu;
    no_atual.custo_acumulado = 0;
    no_atual.heuristica = tabela_heuristica[no_atual.cidade];
    no_atual.avaliacao = no_atual.custo_acumulado + no_atual.heuristica;

    while (!chegou_objetivo(no_atual.cidade)) {
        /* implementação da lógica principal */
    }
}
