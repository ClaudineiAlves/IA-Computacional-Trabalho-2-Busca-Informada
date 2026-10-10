# Tarefa 1 · Busca gulosa

Arquivo testado: `src/____.c` · compilado com `gcc -Wall -Wextra`
Rodado em: __/__/2026

**Como preencher:** escreva o **Esperado** *antes* de rodar e só depois o **Obtido**. Diferenças entre os dois são assunto para a arguição.
Saídas completas (opcional) vão em `saidas/` com o nome `t1_<cidade-inicial>.txt`, por exemplo:

```bash
./build/main > testes/saidas/t1_sibiu.txt
```

A busca gulosa ordena a fronteira só pela **heurística** (`tabela_heuristica`, distância em linha reta até Lugoj).

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
| 7 | heurística engana a busca | | | | | | | |
| 8 | empate na fronteira (desempate) | | | | | | | |
| 9 | cidade repetida na fronteira | | | | | | | |
| 10 | vetor `fronteira` menor que 20 | | | | | | | |
| 11 | varredura | todas as 20 | | | | | | |
| 12 | | | | | | | | |

Perguntas para a arguição:
- Qual critério de desempate o grupo escolheu, e por quê?
- O que acontece quando uma cidade que já está na fronteira é gerada de novo?
- A busca gulosa garante o caminho de menor custo? Mostrar um caso que responda.
