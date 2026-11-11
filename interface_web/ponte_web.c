/*
 * ponte_web.c
 * Ponte entre a pagina web (JavaScript) e o Sistema de Controle de Pousos.
 *
 * Assim como o menu de terminal e a janela GTK, esta e so mais uma CASCA:
 * ela chama as funcoes de sistema_controle.h do Modulo 6, sem alterar
 * nenhuma estrutura de dados. O codigo C e compilado para WebAssembly
 * (Emscripten) e roda dentro do navegador.
 *
 * O JavaScript nao enxerga structs de C. Por isso a ponte:
 *   - recebe e devolve apenas inteiros e strings;
 *   - entrega o estado inteiro do sistema como um texto JSON.
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <emscripten.h>
#include "sistema_controle.h"
#include "validacao.h"
#include "utils.h"

#define TAM_JSON 65536
#define MAX_HISTORICO 500

static SistemaControle sistema;
static bool iniciado = false;
static char json[TAM_JSON];

/* ---------- Montagem do JSON ---------- */

/* Acrescenta texto formatado ao fim de 'json', sem estourar o buffer. */
static void acrescentar(size_t *usado, const char *formato, ...)
{
    va_list args;

    if (*usado >= TAM_JSON - 1)
        return;
    va_start(args, formato);
    int n = vsnprintf(json + *usado, TAM_JSON - *usado, formato, args);
    va_end(args);
    if (n > 0)
        *usado += (size_t)n;
    if (*usado > TAM_JSON - 1)
        *usado = TAM_JSON - 1;
}

/* Escreve uma string entre aspas, escapando o que quebraria o JSON. */
static void acrescentar_texto(size_t *usado, const char *texto)
{
    acrescentar(usado, "\"");
    for (const unsigned char *c = (const unsigned char *)texto; *c != '\0'; c++) {
        if (*c == '"' || *c == '\\')
            acrescentar(usado, "\\%c", *c);
        else if (*c < 0x20)
            acrescentar(usado, "\\u%04x", *c);
        else
            acrescentar(usado, "%c", *c);
    }
    acrescentar(usado, "\"");
}

static void acrescentar_voo(size_t *usado, const Voo *voo)
{
    acrescentar(usado, "\"id\":%d,\"codigo\":", voo->id);
    acrescentar_texto(usado, voo->codigo);
    acrescentar(usado, ",\"nivel\":%d,\"prioridade\":\"%s\",\"chegada\":",
                (int)voo->prioridade, nome_prioridade(voo->prioridade));
    acrescentar_texto(usado, voo->hora_chegada);
}

/* ---------- Funcoes chamadas pelo JavaScript ---------- */

/* Liga o sistema (ou reinicia, liberando toda a memoria antes). */
EMSCRIPTEN_KEEPALIVE
void web_iniciar(void)
{
    if (iniciado)
        sistema_destruir(&sistema);
    sistema_iniciar(&sistema);
    resetar_contador_id();
    iniciado = true;
}

/* Devolve o id do voo criado, ou 0 se codigo/prioridade forem invalidos. */
EMSCRIPTEN_KEEPALIVE
int web_registrar(const char *codigo, const char *prioridade)
{
    Voo *voo = sistema_cadastrar_voo(&sistema, codigo, prioridade);
    return voo != NULL ? voo->id : 0;
}

/* Devolve o id do voo que pousou, ou 0 se a fila estiver vazia. */
EMSCRIPTEN_KEEPALIVE
int web_autorizar_pouso(void)
{
    Voo *voo = sistema_autorizar_pouso(&sistema);
    return voo != NULL ? voo->id : 0;
}

EMSCRIPTEN_KEEPALIVE
int web_reclassificar(int id, const char *nova_prioridade)
{
    return sistema_reclassificar_voo(&sistema, id, nova_prioridade) ? 1 : 0;
}

EMSCRIPTEN_KEEPALIVE
int web_desfazer(int id)
{
    return sistema_desfazer_reclassificacao(&sistema, id) ? 1 : 0;
}

EMSCRIPTEN_KEEPALIVE
int web_refazer(int id)
{
    return sistema_refazer_reclassificacao(&sistema, id) ? 1 : 0;
}

/*
 * Fotografia do sistema em JSON:
 *   niveis     - nome e descricao dos 5 niveis (TABELA_PRIORIDADES)
 *   contagem   - quantos voos aguardam em cada nivel
 *   fila       - voos em espera, ja na ordem real de pouso
 *   historico  - pousos, do mais recente para o mais antigo
 */
EMSCRIPTEN_KEEPALIVE
const char *web_estado(void)
{
    static Voo *pousos[MAX_HISTORICO];
    int contagem[NUM_PRIORIDADES];
    size_t usado = 0;
    int pos = 0;

    json[0] = '\0';
    acrescentar(&usado, "{\"niveis\":[");
    for (int i = 0; i < NUM_PRIORIDADES; i++) {
        acrescentar(&usado, "%s{\"nome\":\"%s\",\"descricao\":\"%s\"}", i > 0 ? "," : "",
                    TABELA_PRIORIDADES[i].nome, TABELA_PRIORIDADES[i].descricao);
    }

    sistema_voos_por_nivel(&sistema, contagem);
    acrescentar(&usado, "],\"aguardando\":%d,\"contagem\":[%d,%d,%d,%d,%d],\"fila\":[",
                sistema_voos_aguardando(&sistema),
                contagem[0], contagem[1], contagem[2], contagem[3], contagem[4]);

    /* Mesma varredura da janela GTK: nivel por nivel, na ordem de chegada. */
    for (int nivel = 1; nivel <= NUM_PRIORIDADES; nivel++) {
        for (RegistroEspera *r = sistema.espera_inicio; r != NULL; r = r->proximo) {
            if ((int)r->voo->prioridade != nivel)
                continue;
            acrescentar(&usado, "%s{\"posicao\":%d,", pos > 0 ? "," : "", pos);
            acrescentar_voo(&usado, r->voo);
            acrescentar(&usado, ",\"pode_desfazer\":%s,\"pode_refazer\":%s}",
                        gerenciador_pode_desfazer(&r->gerenciador) ? "true" : "false",
                        gerenciador_pode_refazer(&r->gerenciador) ? "true" : "false");
            pos++;
        }
    }

    acrescentar(&usado, "],\"historico\":[");
    int n = sistema_historico(&sistema, pousos, MAX_HISTORICO);
    for (int i = 0; i < n; i++) {
        acrescentar(&usado, "%s{", i > 0 ? "," : "");
        acrescentar_voo(&usado, pousos[i]);
        acrescentar(&usado, "}");
    }
    acrescentar(&usado, "]}");
    return json;
}
