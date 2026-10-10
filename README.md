# IAC · Trabalho 2: Busca informada

> Inteligência Artificial Computacional, Prof. Alexandre Seidy Ioshisaqui, 2026.
> Início: 09/10/2026 · Prazo final (código no Canvas **e** apresentação em aula prática): **06/11/2026**.
> Atraso desconta 0,5 pt por semana. Grupos de até 3 pessoas, registrados no Canvas.

Navegação no grafo do mapa da Romênia com **objetivo fixo em Lugoj**, implementando a **busca gulosa** e a **busca A\***.

## Estrutura

```
IA-Computacional-Trabalho-2-Busca-Informada/
├── .vscode/
│   ├── tasks.json              → compilar / compilar e executar (gcc)
│   ├── launch.json             → depurar com gdb (F5)
│   └── c_cpp_properties.json   → IntelliSense apontando para o gcc do sistema
├── src/           → código-fonte .c (parte do código auxiliar do professor)
├── testes/        → casos de teste e resultados observados
├── apresentacao/  → enunciado, roteiro e anotações para a apresentação e a arguição
├── build/         → executáveis gerados (criada ao compilar; ignorada pelo git)
├── README.md      → este arquivo
├── guide_github.md → como baixar, trabalhar e enviar alterações pelo GitHub Desktop
└── .gitignore
```

### `src/`
`busca-informada-codigo-auxiliar.c` é o código de referência do professor, sem alterações. Ele já traz:
- o `enum Cidade` e os nomes das 20 cidades;
- a `tabela_heuristica` (distância em linha reta até Lugoj);
- a matriz `mapa_custo` e a função `custo()`;
- os protótipos que faltam implementar: `chegou_objetivo()`, `cidades_vizinhas()`, `esta_no_historico()`, `inserir_no_historico()`, `inserir_na_fronteira_gulosa()`, `inserir_na_fronteira_a_estrela()` e `proxima_cidade()`;
- uma `main()` que parte de Sibiu, com o laço principal vazio.

Seguir o código de referência não é obrigatório, mas o professor recomenda fortemente.

### `testes/`
Casos para conferir o programa e mostrar na apresentação. Aguentar mudanças de parâmetros sem erro vale 2 pts. Casos que vale cobrir:
- estados iniciais diferentes (Sibiu, o próprio Lugoj, vizinhos de Lugoj, cidades distantes como Eforie e Neamt);
- diferentes tamanhos de vetor (`fronteira`, `historico`, `ListaAcoes.cidades`);
- empates de avaliação na fronteira (critério de desempate);
- cidades repetidas na fronteira e no histórico;
- comparação gulosa × A\*: caminho, custo total e nós expandidos.

Anote o que cada caso produziu, incluindo os avisos de estouro, se houver.

### `apresentacao/`
Enunciado (`trabalho-IA-02.pdf`) e material de apoio para a apresentação, que vale 6 pts: roteiro, pontos que cada integrante precisa saber explicar e a lista de parâmetros que vocês vão alterar ao vivo.

## Avaliação

| Item | Pontos |
|---|---|
| Implementação do código (Canvas) | 2 |
| Documentação, comentários e organização (Canvas) | 2 |
| Apresentar o código base funcionando | 2 |
| Atender a variações de parâmetros sem erros durante a apresentação | 2 |
| Completude, profundidade e clareza da apresentação e das explicações | 2 |

O trabalho só conta como entregue quando o grupo **já entregou o código no Canvas e já apresentou em sala**.

## Planejamento
A organização e as tarefas ficam no Notion.
