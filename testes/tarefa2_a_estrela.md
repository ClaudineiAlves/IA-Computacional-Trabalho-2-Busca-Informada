# Tarefa 2 · Busca A\*

Arquivo testado: `src/____.c` · compilado com `gcc -Wall -Wextra`
Rodado em: __/__/2026

**Como preencher:** escreva o **Esperado** *antes* de rodar e só depois o **Obtido**.
Saídas completas (opcional) vão em `saidas/` com o nome `t2_<cidade-inicial>.txt`.

A busca A\* ordena a fronteira pela **avaliação** `f(n) = g(n) + h(n)`: `custo_acumulado` + `heuristica`.

**Constantes e vetores usados** (preencher com os nomes do código):

| Nome | Para que serve | Valor inicial |
|---|---|---|
| `QuantidadeCidades` | tamanho de `fronteira` e `historico` | 20 |
| `ListaAcoes.cidades[4]` | máximo de vizinhos por cidade | 4 |
| | | |

---

## Casos

| # | Caso | Cidade inicial | Esperado (caminho) | Obtido (caminho) | Custo total | Nós expandidos | Aviso de estouro? | OK? |
|---|---|---|---|---|---|---|---|---|
| 1 | caso do código auxiliar | Sibiu | | | | | | |
| 2 | estado inicial já é o objetivo | Lugoj | | | | | | |
| 3 | vizinho direto de Lugoj | Timisoara | | | | | | |
| 4 | vizinho direto de Lugoj | Mehadia | | | | | | |
| 5 | cidade distante | Eforie | | | | | | |
| 6 | cidade distante | Neamt | | | | | | |
| 7 | mesma cidade gerada com custo menor | | | | | | | |
| 8 | empate de `f(n)` na fronteira (desempate) | | | | | | | |
| 9 | vetor `fronteira` menor que 20 | | | | | | | |
| 10 | varredura | todas as 20 | | | | | | |
| 11 | | | | | | | | |

---

## Comparação com a busca gulosa (Tarefa 1)

| Cidade inicial | Caminho gulosa | Custo gulosa | Caminho A\* | Custo A\* | Iguais? | Por quê? |
|---|---|---|---|---|---|---|
| Sibiu | | | | | | |
| | | | | | | |
| | | | | | | |

Perguntas para a arguição:
- Quando uma cidade já está na fronteira com um custo maior, o que o código faz?
- A heurística do código é admissível? O que isso garante para a A\*?
- Por que a A\* pode expandir mais nós que a gulosa e ainda assim ser preferível?
