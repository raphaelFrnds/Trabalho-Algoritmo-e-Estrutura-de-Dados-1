/*
 * interface_gtk.c
 * Interface grafica (GTK 3) do Sistema de Controle de Pousos.
 *
 * E so uma CASCA em cima do mesmo SistemaControle do Modulo 6: as
 * estruturas de dados (fila de prioridade, lista encadeada e pilha)
 * sao exatamente as mesmas usadas pelo menu de terminal. Trocar a
 * interface sem tocar nas estruturas e a vantagem de ter separado
 * os TADs em arquivos proprios.
 */
#include <gtk/gtk.h>
#include "sistema_controle.h"
#include "validacao.h"

#define MAX_LISTA 500

/* Tudo que a interface precisa guardar enquanto roda. */
typedef struct {
    SistemaControle sistema;
    GtkWidget *entrada_codigo;
    GtkWidget *combo_prioridade;      /* prioridade do cadastro */
    GtkWidget *combo_reclassificar;   /* prioridade da reclassificacao */
    GtkWidget *rotulo_status;
    GtkWidget *rotulo_contagem;
    GtkListStore *loja_fila;
    GtkListStore *loja_historico;
    GtkTreeView *tabela_fila;
} App;

/* Colunas das tabelas. */
enum { COL_POSICAO, COL_ID, COL_CODIGO, COL_PRIORIDADE, COL_HORA, NUM_COLUNAS };

/* ---------- Funcoes auxiliares da interface ---------- */

static void mostrar_status(App *app, const char *texto, gboolean erro)
{
    char marcado[512];
    g_snprintf(marcado, sizeof marcado, "<span foreground=\"%s\">%s</span>",
               erro ? "#b3261e" : "#1b5e20", texto);
    gtk_label_set_markup(GTK_LABEL(app->rotulo_status), marcado);
}

/* Le a prioridade escolhida num combo ("mayday", "regular"...). */
static const char *prioridade_do_combo(GtkWidget *combo)
{
    return gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo));
}

/* Redesenha as duas tabelas e a contagem por nivel a partir do sistema. */
static void atualizar_telas(App *app)
{
    Voo *voos[MAX_LISTA];
    GtkTreeIter iter;
    int contagem[NUM_PRIORIDADES];
    char resumo[256];
    int pos = 0;

    /* --- fila de espera, na ordem real de pouso --- */
    gtk_list_store_clear(app->loja_fila);
    for (int nivel = 1; nivel <= NUM_PRIORIDADES; nivel++) {
        for (RegistroEspera *r = app->sistema.espera_inicio; r != NULL; r = r->proximo) {
            if ((int)r->voo->prioridade != nivel)
                continue;
            gtk_list_store_append(app->loja_fila, &iter);
            gtk_list_store_set(app->loja_fila, &iter,
                               COL_POSICAO, pos++,
                               COL_ID, r->voo->id,
                               COL_CODIGO, r->voo->codigo,
                               COL_PRIORIDADE, nome_prioridade(r->voo->prioridade),
                               COL_HORA, r->voo->hora_chegada, -1);
        }
    }

    /* --- historico, do mais recente para o mais antigo --- */
    gtk_list_store_clear(app->loja_historico);
    int n = sistema_historico(&app->sistema, voos, MAX_LISTA);
    for (int i = 0; i < n; i++) {
        gtk_list_store_append(app->loja_historico, &iter);
        gtk_list_store_set(app->loja_historico, &iter,
                           COL_POSICAO, i + 1,
                           COL_ID, voos[i]->id,
                           COL_CODIGO, voos[i]->codigo,
                           COL_PRIORIDADE, nome_prioridade(voos[i]->prioridade),
                           COL_HORA, voos[i]->hora_chegada, -1);
    }

    /* --- contagem por nivel --- */
    sistema_voos_por_nivel(&app->sistema, contagem);
    g_snprintf(resumo, sizeof resumo,
               "Aguardando: %d   |   mayday %d · panpan %d · combustivel %d · regular %d · treinamento %d",
               sistema_voos_aguardando(&app->sistema),
               contagem[0], contagem[1], contagem[2], contagem[3], contagem[4]);
    gtk_label_set_text(GTK_LABEL(app->rotulo_contagem), resumo);
}

/* Devolve o id do voo selecionado na tabela da fila, ou -1. */
static int id_selecionado(App *app)
{
    GtkTreeSelection *selecao = gtk_tree_view_get_selection(app->tabela_fila);
    GtkTreeModel *modelo;
    GtkTreeIter iter;
    int id = -1;

    if (gtk_tree_selection_get_selected(selecao, &modelo, &iter))
        gtk_tree_model_get(modelo, &iter, COL_ID, &id, -1);
    return id;
}

/* ---------- Acoes dos botoes ---------- */

static void ao_registrar(GtkButton *botao, gpointer dados)
{
    App *app = dados;
    const char *codigo = gtk_entry_get_text(GTK_ENTRY(app->entrada_codigo));
    const char *prioridade = prioridade_do_combo(app->combo_prioridade);
    char msg[256];
    (void)botao;

    Voo *voo = sistema_cadastrar_voo(&app->sistema, codigo, prioridade);
    if (voo == NULL) {
        mostrar_status(app, "Codigo invalido (1 a 11 caracteres) ou prioridade nao escolhida.", TRUE);
        return;
    }

    g_snprintf(msg, sizeof msg, "Voo %s registrado como %s (id %d).",
               voo->codigo, nome_prioridade(voo->prioridade), voo->id);
    mostrar_status(app, msg, FALSE);
    gtk_entry_set_text(GTK_ENTRY(app->entrada_codigo), "");
    atualizar_telas(app);
}

static void ao_autorizar_pouso(GtkButton *botao, gpointer dados)
{
    App *app = dados;
    char msg[256];
    (void)botao;

    Voo *voo = sistema_autorizar_pouso(&app->sistema);
    if (voo == NULL) {
        mostrar_status(app, "Nao ha voos aguardando.", TRUE);
        return;
    }
    g_snprintf(msg, sizeof msg, "Pouso autorizado: %s (%s).",
               voo->codigo, nome_prioridade(voo->prioridade));
    mostrar_status(app, msg, FALSE);
    atualizar_telas(app);
}

static void ao_reclassificar(GtkButton *botao, gpointer dados)
{
    App *app = dados;
    int id = id_selecionado(app);
    const char *nova = prioridade_do_combo(app->combo_reclassificar);
    char msg[256];
    (void)botao;

    if (id == -1) {
        mostrar_status(app, "Selecione um voo na fila de espera primeiro.", TRUE);
        return;
    }
    if (!sistema_reclassificar_voo(&app->sistema, id, nova)) {
        mostrar_status(app, "Nao foi possivel reclassificar esse voo.", TRUE);
        return;
    }
    g_snprintf(msg, sizeof msg, "Voo id %d reclassificado para %s.", id, nova);
    mostrar_status(app, msg, FALSE);
    atualizar_telas(app);
}

static void ao_desfazer(GtkButton *botao, gpointer dados)
{
    App *app = dados;
    int id = id_selecionado(app);
    (void)botao;

    if (id == -1) {
        mostrar_status(app, "Selecione um voo na fila de espera primeiro.", TRUE);
        return;
    }
    if (!sistema_desfazer_reclassificacao(&app->sistema, id)) {
        mostrar_status(app, "Esse voo nao tem reclassificacao para desfazer.", TRUE);
        return;
    }
    mostrar_status(app, "Ultima reclassificacao desfeita.", FALSE);
    atualizar_telas(app);
}

static void ao_refazer(GtkButton *botao, gpointer dados)
{
    App *app = dados;
    int id = id_selecionado(app);
    (void)botao;

    if (id == -1) {
        mostrar_status(app, "Selecione um voo na fila de espera primeiro.", TRUE);
        return;
    }
    if (!sistema_refazer_reclassificacao(&app->sistema, id)) {
        mostrar_status(app, "Esse voo nao tem reclassificacao para refazer.", TRUE);
        return;
    }
    mostrar_status(app, "Reclassificacao refeita.", FALSE);
    atualizar_telas(app);
}

/* Libera toda a memoria do sistema ao fechar a janela. */
static void ao_fechar(GtkWidget *janela, gpointer dados)
{
    App *app = dados;
    (void)janela;
    sistema_destruir(&app->sistema);
    gtk_main_quit();
}

/* ---------- Montagem da janela ---------- */

/* Cria um combo ja preenchido com os 5 niveis de prioridade. */
static GtkWidget *criar_combo_prioridades(void)
{
    GtkWidget *combo = gtk_combo_box_text_new();
    for (int i = 0; i < NUM_PRIORIDADES; i++)
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo),
                                  TABELA_PRIORIDADES[i].nome,
                                  TABELA_PRIORIDADES[i].descricao);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 3);   /* regular */
    return combo;
}

/* Cria uma tabela (TreeView) ligada a um GtkListStore. */
static GtkWidget *criar_tabela(GtkListStore **loja, const char *titulo_primeira)
{
    const char *titulos[NUM_COLUNAS] = {titulo_primeira, "ID", "Codigo", "Prioridade", "Chegada"};

    *loja = gtk_list_store_new(NUM_COLUNAS, G_TYPE_INT, G_TYPE_INT,
                               G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkWidget *tabela = gtk_tree_view_new_with_model(GTK_TREE_MODEL(*loja));

    for (int i = 0; i < NUM_COLUNAS; i++) {
        GtkCellRenderer *celula = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *coluna =
            gtk_tree_view_column_new_with_attributes(titulos[i], celula, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tabela), coluna);
    }
    return tabela;
}

static GtkWidget *criar_botao(const char *texto, GCallback acao, App *app)
{
    GtkWidget *b = gtk_button_new_with_label(texto);
    g_signal_connect(b, "clicked", acao, app);
    return b;
}

static void montar_janela(App *app)
{
    GtkWidget *janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janela), "Torre de Controle - Sequencia de Pousos");
    gtk_window_set_default_size(GTK_WINDOW(janela), 980, 620);
    gtk_container_set_border_width(GTK_CONTAINER(janela), 12);
    g_signal_connect(janela, "destroy", G_CALLBACK(ao_fechar), app);

    GtkWidget *coluna = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(janela), coluna);

    /* --- linha 1: cadastro --- */
    GtkWidget *linha_cadastro = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    app->entrada_codigo = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->entrada_codigo), "Codigo do voo (ex.: AZU4521)");
    gtk_entry_set_max_length(GTK_ENTRY(app->entrada_codigo), TAM_CODIGO - 1);
    app->combo_prioridade = criar_combo_prioridades();

    gtk_box_pack_start(GTK_BOX(linha_cadastro), gtk_label_new("Novo voo:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_cadastro), app->entrada_codigo, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(linha_cadastro), app->combo_prioridade, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_cadastro),
                       criar_botao("Registrar voo", G_CALLBACK(ao_registrar), app), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(coluna), linha_cadastro, FALSE, FALSE, 0);

    /* --- linha 2: contagem por nivel --- */
    app->rotulo_contagem = gtk_label_new("");
    gtk_widget_set_halign(app->rotulo_contagem, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(coluna), app->rotulo_contagem, FALSE, FALSE, 0);

    /* --- linha 3: as duas tabelas, lado a lado --- */
    GtkWidget *painel = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(coluna), painel, TRUE, TRUE, 0);

    GtkWidget *caixa_fila = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_box_pack_start(GTK_BOX(caixa_fila),
                       gtk_label_new("Fila de espera (0 = proximo a pousar)"), FALSE, FALSE, 0);
    GtkWidget *tabela_fila = criar_tabela(&app->loja_fila, "Posicao");
    app->tabela_fila = GTK_TREE_VIEW(tabela_fila);
    GtkWidget *rolagem_fila = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(rolagem_fila), tabela_fila);
    gtk_box_pack_start(GTK_BOX(caixa_fila), rolagem_fila, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(painel), caixa_fila, TRUE, TRUE, 0);

    GtkWidget *caixa_hist = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_box_pack_start(GTK_BOX(caixa_hist),
                       gtk_label_new("Historico de pousos (mais recente primeiro)"), FALSE, FALSE, 0);
    GtkWidget *tabela_hist = criar_tabela(&app->loja_historico, "#");
    GtkWidget *rolagem_hist = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(rolagem_hist), tabela_hist);
    gtk_box_pack_start(GTK_BOX(caixa_hist), rolagem_hist, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(painel), caixa_hist, TRUE, TRUE, 0);

    /* --- linha 4: acoes sobre o voo selecionado --- */
    GtkWidget *linha_acoes = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    app->combo_reclassificar = criar_combo_prioridades();
    gtk_box_pack_start(GTK_BOX(linha_acoes),
                       gtk_label_new("Voo selecionado na fila:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_acoes), app->combo_reclassificar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_acoes),
                       criar_botao("Reclassificar", G_CALLBACK(ao_reclassificar), app), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_acoes),
                       criar_botao("Desfazer", G_CALLBACK(ao_desfazer), app), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(linha_acoes),
                       criar_botao("Refazer", G_CALLBACK(ao_refazer), app), FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(linha_acoes),
                     criar_botao("Autorizar proximo pouso", G_CALLBACK(ao_autorizar_pouso), app),
                     FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(coluna), linha_acoes, FALSE, FALSE, 0);

    /* --- linha 5: mensagens --- */
    app->rotulo_status = gtk_label_new("");
    gtk_widget_set_halign(app->rotulo_status, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(coluna), app->rotulo_status, FALSE, FALSE, 0);

    gtk_widget_show_all(janela);
}

int main(int argc, char *argv[])
{
    App app = {0};

    gtk_init(&argc, &argv);
    sistema_iniciar(&app.sistema);
    montar_janela(&app);
    atualizar_telas(&app);
    mostrar_status(&app, "Pronto. Registre um voo para comecar.", FALSE);
    gtk_main();
    return 0;
}
