/*****************************************************************
 * Trabalho 2 - Busca Informada (gulosa e A*)
 *
 * Problema: navegar pelo mapa da Romênia a partir de CIDADE_INICIAL
 * até o objetivo fixo, Lugoj
 *
 * Estado atual: busca gulosa (f(n) = h(n))
 *               busca A* (f(n) = g(n) + h(n))
 *****************************************************************/

#include <stdbool.h>
#include <stdio.h>

// parâmetros Principais
#define INF -1 // para marcar valores inválidos

#define CIDADE_INICIAL Sibiu // cidade de onde a busca parte
#define MAX_VIZINHOS 4 // tamanho de ListaAcoes.cidades (maior grau do mapa = 4)
#define MAX_HISTORICO QuantidadeCidades // tamanho do vetor historico[]
#define MAX_FRONTEIRA QuantidadeCidades // tamanho do vetor fronteira[]

bool memoria_estourou = false; // vira true se algum vetor encher

/*****************************************
 * código auxiliar
 *****************************************/
// cada cidade é um número inteiro, usado como índice dos vetores
// quantidadeCidades é o tamanho desses vetores
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

// converte o número da cidade em texto, para imprimir o caminho
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

// tabela com os valores da função heurística com Lugoj como objetivo
// h(n) = distância estimada (linha reta) da cidade n até Lugoj.
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

// matriz de adjacência: mapa_custo[A][B] = distância da estrada A-B.
// posições não escritas valem 0(não há estrada)
int mapa_custo[QuantidadeCidades][QuantidadeCidades] = {
    [Arad] = { [Zerind] = 75, [Timisoara] = 118, [Sibiu] = 140 },
    [Bucharest] = { [Giurgiu] = 90, [Pitesti] = 101, [Fagaras] = 211, [Urziceni] = 85 },
    [Craiova] = { [Drobeta] = 120, [RimnicuVilcea] = 146, [Pitesti] = 138 },
    [Drobeta] = { [Mehadia] = 75, [Craiova] = 120 },
    [Eforie] = { [Hirsova] = 86 },
    [Fagaras] = { [Sibiu] = 99, [Bucharest] = 211 },
    [Giurgiu] = { [Bucharest] = 90 },
    // CORREÇÃO: o código auxiliar só tinha Urziceni->Hirsova (98)
    // Urziceni-Hirsova com 98, então adicionamos Hirsova->Urziceni
    // Sem isso, partindo de Eforie ou Hirsova a busca não encontrava caminho
    [Hirsova] = { [Eforie] = 86, [Urziceni] = 98 },
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

// retorna o custo da estrada origem->destino:
//   0   se origem == destino;
//   > 0 se a estrada existe;
//   INF (-1) se não existe estrada.
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

// retorna true se a cidade é o objetivo (fixo em Lugoj)
bool chegou_objetivo(Cidade cidade)
{
    return cidade == Lugoj;
}

// navegação pelos nós vizinhos / escolha de ações
// lista das cidades para onde se pode ir a partir de uma cidadeee
typedef struct {
    Cidade cidades[MAX_VIZINHOS]; // qtd máxima de conexões em uma cidade
    int n_acoes; // quantas posições de cidades[] estão preenchidas
} ListaAcoes; // estrutura que representa a lista de possíveis ações (cidades aonde pode ir)

// percorre a linha de mapa_custo da cidade atual e devolve, por valor, a lista de vizinhas
ListaAcoes cidades_vizinhas(Cidade cidade_atual)
{
    ListaAcoes lista;
    lista.n_acoes = 0; // a struct local começa com lixo

    for (int i = 0; i < QuantidadeCidades; i++) {
        if (custo(cidade_atual, i) > 0) { // > 0 exclui INF (-1) e a própria cidade (0)
            if (lista.n_acoes >= MAX_VIZINHOS) {
                printf("Estouro de memória: %s tem mais vizinhos que MAX_VIZINHOS (%d)\n", nome_cidade[cidade_atual], MAX_VIZINHOS);
                memoria_estourou = true;
                break;
            }
            lista.cidades[lista.n_acoes++] = i;
        }
    }
    return lista;
}
/*****************************************
 * historico de cidades
 * evita expandir a mesma cidade duas vezes e entrar em ciclos
 *****************************************/

Cidade historico[MAX_HISTORICO];
int tamanho_historico = 0;

// busca linear: retorna true se a cidade já foi expandida
bool esta_no_historico(Cidade cidade)
{
    for (int i = 0; i < tamanho_historico; i++) {
        if (historico[i] == cidade) {
            return true;
        }
    }
    return false;
}
// insere a cidade no fim do histórico.
void inserir_no_historico(Cidade cidade)
{
    if (tamanho_historico >= MAX_HISTORICO) {
        printf("Estouro de memória: historico cheio (MAX_HISTORICO = %d)\n", MAX_HISTORICO);
        memoria_estourou = true;
        return;
    }
    historico[tamanho_historico++] = cidade;
}
/*****************************************
 * fronteira(nós gerados mas ainda não expandidos)
 *****************************************/

typedef struct {
    Cidade cidade; // cidade que este nó representa
    Cidade anterior; // cidade de onde se chegou (caminho)
    int custo_acumulado; // g(n): distância real percorrida desde CIDADE_INICIAL
    int heuristica; // h(n): estimativa até Lugoj (tabela_heuristica)
    int avaliacao; // f(n): prioridade na fronteira (gulosa: h; A*: g + h)
} No; // estrutura que representa um nó no grafo

No fronteira[MAX_FRONTEIRA];
int tamanho_fronteira = 0;

// insere um nó na fronteira da busca GULOSA.
// o próprio f(n) é calculado aqui, para que a main() só precise
// trocar qual função de inserção chama ao alternar entre gulosa e A*
void inserir_na_fronteira_gulosa(No nova_cidade)
{
    nova_cidade.avaliacao = nova_cidade.heuristica; // gulosa: f(n) = h(n)

    // 1. REPETIÇÃO: cidade já está na fronteira?
    // como h vem da tabela, as duas cópias têm a mesma avaliação e, portanto, a mesma posição na fila
    // mantemos apenas a de menor custo_acumulado (caminho mais barato até ela) e descartamos a outra
    for (int i = 0; i < tamanho_fronteira; i++) {
        if (fronteira[i].cidade == nova_cidade.cidade) {
            // mesmo h -> mesma posição; só fica com o caminho mais barato até ela
            if (nova_cidade.custo_acumulado < fronteira[i].custo_acumulado) {
                fronteira[i] = nova_cidade;
            }
            return;
        }
    }

    // 2. ESTOURO: fronteira cheia
    if (tamanho_fronteira >= MAX_FRONTEIRA) {
        printf("Estrouro de memória: fronteira cheia (MAX_FRONTEIRA = %d)\n", MAX_FRONTEIRA);
        memoria_estourou = true;
        return;
    }

    // 3. POSIÇÃO em ordem crescente de avaliação.
    //    DESEMPATE: '<=' coloca o nó novo DEPOIS dos que têm a mesma avaliação
    //    entre empatados sai primeiro quem chegou primeiro (FIFO)
    int pos = 0;
    while (pos < tamanho_fronteira && fronteira[pos].avaliacao <= nova_cidade.avaliacao) {
        pos++;
    }

    // 4. DESLOCA os elementos de pos em diante uma casa para a direita e insere.
    for (int i = tamanho_fronteira; i > pos; i--) {
        fronteira[i] = fronteira[i - 1];
    }
    fronteira[pos] = nova_cidade;
    tamanho_fronteira++;
}

void inserir_na_fronteira_a_estrela(No nova_cidade); // deve testar repetição/desempate aqui dentro; inserir em ordem de prioridade
// apenas pega o nó e maior prioridade na fronteira

// remove e devolve o nó de maior prioridade (fronteira[0])
// não toma nenhuma decisão: a ordem já foi definida na inserção
// pré-condição: fronteira não vazia (garantido pela main())
No proxima_cidade(void)
{
    No melhor = fronteira[0]; // menor avaliação = maior prioridade
    for (int i = 1; i < tamanho_fronteira; i++) {
        fronteira[i - 1] = fronteira[i];
    }
    tamanho_fronteira--;
    return melhor;
}

int main()
{
    ListaAcoes prox_cidades; // vizinhas da cidade sendo expandida
    No no_atual, no_gerado; // nó expandido agora / nó filho sendo criado
    Cidade pai[QuantidadeCidades]; // pai[c] = cidade de onde chegamos em c

    // nó inicial. O início é "pai de si mesmo": isso marca o ponto
    // de parada da reconstrução do caminho, sem precisar de sentinela
    no_atual.cidade = CIDADE_INICIAL;
    no_atual.anterior = CIDADE_INICIAL;
    no_atual.custo_acumulado = 0;
    no_atual.heuristica = tabela_heuristica[no_atual.cidade];
    no_atual.avaliacao = no_atual.custo_acumulado + no_atual.heuristica;

    while (!chegou_objetivo(no_atual.cidade)) {
        // 1. EXPANDE o nó atual: registra no histórico e guarda o pai
        //    Cada cidade é expandida uma única vez, então pai[] fica consistente
        inserir_no_historico(no_atual.cidade);
        pai[no_atual.cidade] = no_atual.anterior;

        // 2. GERA os vizinhos ainda não expandidos e insere na fronteira
        prox_cidades = cidades_vizinhas(no_atual.cidade);
        for (int i = 0; i < prox_cidades.n_acoes; i++) {
            Cidade vizinha = prox_cidades.cidades[i];
            if (esta_no_historico(vizinha)) {
                continue; // já expandida: ignorar evita ciclos
            }
            no_gerado.cidade = vizinha;
            no_gerado.anterior = no_atual.cidade;
            no_gerado.custo_acumulado = no_atual.custo_acumulado + custo(no_atual.cidade, vizinha); // g do filho
            no_gerado.heuristica = tabela_heuristica[vizinha]; // h do filho
            inserir_na_fronteira_gulosa(no_gerado); // calcula f e posiciona na fronteira
        }

        // 3. PARADAS DE SEGURANÇA
        if (memoria_estourou) { // algum vetor encheu: continuar poderia gerar laço infinito
            printf("Busca interrompida: um vetor encheu.\n");
            return 1;
        }
        if (tamanho_fronteira == 0) { // nada mais a explorar: não há caminho até Lugoj
            printf("Nao existe caminho de %s ate Lugoj.\n", nome_cidade[CIDADE_INICIAL]);
            return 1;
        }

        // 4. ESCOLHE o próximo nó: o de menor avaliação (na gulosa, menor h)
        no_atual = proxima_cidade();
    }

    // O objetivo saiu da fronteira mas não passou pelo passo 1,
    // então o pai dele ainda não foi registrado.
    pai[no_atual.cidade] = no_atual.anterior;

    // RECONSTRÓI o caminho de trás para frente: Lugoj -> ... -> início
    // O limite n < QuantidadeCidades protege contra laço infinito
    // caso pai[] esteja inconsistente por algum erro
    Cidade caminho[QuantidadeCidades];
    int n = 0;
    Cidade c = no_atual.cidade;
    while (c != CIDADE_INICIAL && n < QuantidadeCidades) {
        caminho[n++] = c;
        c = pai[c];
    }
    caminho[n++] = CIDADE_INICIAL;

    // IMPRIME o caminho na ordem certa (início -> Lugoj) e o custo total g
    printf("Caminho: ");
    for (int i = n - 1; i >= 0; i--) {
        printf("%s%s", nome_cidade[caminho[i]], i > 0 ? " -> " : "\n");
    }
    printf("Custo total: %d\n", no_atual.custo_acumulado);
    return 0;
}
